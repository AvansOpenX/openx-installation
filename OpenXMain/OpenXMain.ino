#include "MoistureSensor.h"
#include "Reservoir.h"
#include "Battery.h"
#include "Screen.h"
#include "Button.h"
#include "WaterValve.h"
#include "PlantLamp.h"
#include "Plant.h"
#include "DHT.h"
// Libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <ArduinoJson.h>
// Potential long-term issue https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
#include <Adafruit_MCP23X17.h>

// Buttons (mcp1)
#define START_RING_PIN 15
#define BUTTON_LED_PIN 4
#define START_BUTTON_PIN 6
#define MODE_BUTTON_PIN 14
// Reservoir
#define RESERVOIR_LED_PIN 2
#define RESERVOIR_PUMP_PIN 13
#define RESERVOIR_VALVE_PIN 33
// Other
#define MCP_1_ADDRESS 0x20
#define MCP_2_ADDRESS 0x21
#define NUMBER_OF_PLANTS 5
#define NUMBER_OF_BUTTONS 12
#define DHTPIN 5
#define DHTTYPE DHT22
#define UDP_NAME "OpenXMain"
#define UDP_P_NAME "OpenXPlant"
#define UDP_G_NAME "OpenXGame"

const byte MOISTURE_SENSOR_PINS[] { 32, 35, 34, 39, 36 };
const byte WATER_VALVE_PINS[] { 12, 14, 27, 26, 25 };
const byte LAMP_PINS[] { 8, 9, 10, 11, 12 }; // mcp2
const byte GAME_BUTTON_PINS[] { 0, 13, 1, 12, 2, 11, 3, 10, 4, 9, 5, 8 }; // mcp1
const byte RESERVOIR_SENSOR_PINS[] { 6, 14, 7, 15 }; // mcp2

// BLE variables
String BLEPin = "999999";
BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
#define SERVICE_UUID "06cd0a01-f2af-4739-83ac-2be012508cd6"
#define CHARACTERISTIC_UUID "4a59aa02-2178-427b-926a-ff86cfb87571"

// NTP server variables
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// Global objects
Preferences prefs;
DHT dht(DHTPIN, DHTTYPE);
Adafruit_MCP23X17 mcp1;
Adafruit_MCP23X17 mcp2;
// TODO: Initialize screen object
Battery *battery;
Reservoir *reservoir;

Button *startButton;
Button *modeButton;
Button *gameButtons[NUMBER_OF_BUTTONS];
Adafruit_NeoPixel buttonLeds(NUMBER_OF_BUTTONS, BUTTON_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledRing(32, START_RING_PIN, NEO_GRB + NEO_KHZ800);

Plant *plants[NUMBER_OF_PLANTS];
MoistureSensor *moistureSensors[NUMBER_OF_PLANTS];
PlantLamp *plantLamps[NUMBER_OF_PLANTS];

bool multiplayer = false;
// Should be set to false when doing any action (running game, watering plants, filling reservoir, etc.)
bool idle = false;

void setup() {
  Serial.begin(9600);
  dht.begin();
  mcp1.begin_I2C(MCP_1_ADDRESS);
  mcp2.begin_I2C(MCP_2_ADDRESS);
  prefs.begin("app", false);
  initBLE();

  // IO Expanders need to be initialized before the reservoir
  reservoir = new Reservoir(RESERVOIR_SENSOR_PINS, RESERVOIR_VALVE_PIN, RESERVOIR_PUMP_PIN, RESERVOIR_LED_PIN, mcp2, prefs);
  battery = new Battery();

  // EEPROM needs to be initialized before the moisture sensors and valves
  for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
    // Create plant dependencies
    plantLamps[i] = new PlantLamp(LAMP_PINS[i], mcp2);
    WaterValve *waterValve = new WaterValve(WATER_VALVE_PINS[i], prefs);
    moistureSensors[i] = new MoistureSensor(MOISTURE_SENSOR_PINS[i], prefs);
    // Create the plant object and save it in the plants array
    plants[i] = new Plant(i, moistureSensors[i], waterValve, plantLamps[i], prefs);
  }

  ledRing.begin();
  buttonLeds.begin();
  // IO Expanders need to be initialized before the buttons
  startButton = new Button(START_BUTTON_PIN, mcp1, buttonLeds, 0);
  modeButton = new Button(MODE_BUTTON_PIN, mcp1, buttonLeds, 1);
  for (byte i = 0; i < NUMBER_OF_BUTTONS; i++) {
    gameButtons[i] = new Button(GAME_BUTTON_PINS[i], mcp1, buttonLeds, i + 2);
  }

  createUDPSensors();
  // Share sensor measurements with UDP
  xTaskCreate(udpTransmit, "udpTransmit", 3000, NULL, 0, NULL);
  // Measure moisture values and control the waterflow
  xTaskCreate(measureValues, "measureValues", 2048, NULL, 2, NULL);
  // Drain the battery and control the lamps
  xTaskCreate(batDrain, "batDrain", 2048, NULL, 1, NULL);

  // Connect to the internet
  char ssidBuffer[32];
  char passBuffer[32];
  String ssid = prefs.getString("ssid");
  String pass = prefs.getString("pass");
  ssid.toCharArray(ssidBuffer, ssid.length() + 1);
  pass.toCharArray(passBuffer, pass.length() + 1);
  WiFi.begin(ssidBuffer, passBuffer);
  // Only continue when an internet connection has been established
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to the internet...");
    delay(250);
  }
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  idle = true;
}

void loop() {
  if (startButton->isPressed()) {
    idle = false;
    // Delay of 3 seconds
    startGame();
    // Create a task to visualize the progress of the game
    xTaskCreate(gameCountdown, "gameCountdown", 2048, NULL, 2, NULL);
    // Start the game, passed parameter is hardcoded due to the input being binary, runGame itself is scalable
    runGame(multiplayer ? 2 : 1);
    idle = true;
  } else if (modeButton->isPressed()) {
    // Toggle multiplayer mode when the modeButton is pressed
    multiplayer = !multiplayer;
    modeButton->toggle();
  } else {
    // TODO: Idle animation
    delay(30);
  }
}

void startGame() {
  // Start with the led ring green
  ledRing.fill(ledRing.Color(0, 255, 0));
  for (byte i = 0; i < 3; i++) {
    for (byte j = 0; j < 32; j++) {
      // Iteratively turn leds orange, red, and off
      if (i == 0) ledRing.setPixelColor(j, ledRing.Color(255, 125, 0));
      else if (i == 1) ledRing.setPixelColor(j, ledRing.Color(255, 0, 0));
      else if (i == 2) ledRing.setPixelColor(j, ledRing.Color(0, 0, 0));
      ledRing.show();
      delay(32);
    }
  }
}

void runGame(byte playerCount) {
  int scores[playerCount];
  int activeButtons[playerCount];
  // Fill the activeButtons array with random numbers and turn them on
  for (byte i = 0; i < playerCount; i++) {
    activeButtons[i] = getRandomIntBetween(-1, NUMBER_OF_BUTTONS / playerCount * i, NUMBER_OF_BUTTONS / playerCount * (i + 1));
    gameButtons[activeButtons[i]]->on();
  }
  // Calculate when the game should end
  unsigned long endTime = millis() + prefs.getShort("gameDuration", 30) * 1000;
  // Keep looping until the current time is greater than the endTime
  while (millis() < endTime) {
    for (byte i = 0; i < playerCount; i++) {
      // Turn the button off if it's pressed
      if (gameButtons[activeButtons[i]]->isPressed()) {
        gameButtons[activeButtons[i]]->off();
        // Get a new random button, and turn it on
        activeButtons[i] = getRandomIntBetween(activeButtons[i], NUMBER_OF_BUTTONS / playerCount * i, NUMBER_OF_BUTTONS / playerCount * (i + 1));
        gameButtons[activeButtons[i]]->on();
        scores[i]++;
        // TODO: Update score on the led matrix
      }
    }
  }
  // Turn all active buttons off and return the scores
  buttonLeds.clear();
  buttonLeds.show();
  // Get the winner
  byte winner;
  for (byte i = 0; i < playerCount; i++) {
    if (scores[i] > winner) winner = i;
  }
  // Flash the winner's buttons green and the loser's buttons red for 3 seconds
  for (byte i = 0; i < 6; i++) {
    // Set all buttons to red
    buttonLeds.fill(buttonLeds.Color(255, 0, 0));
    // Turn the winner's buttons green
    for (byte i = NUMBER_OF_BUTTONS / playerCount * i; i < NUMBER_OF_BUTTONS / playerCount * (i + 1); i++) {
      buttonLeds.setPixelColor(i, buttonLeds.Color(0, 255, 0));
    }
    delay(250);
    buttonLeds.clear();
    buttonLeds.show();
    delay(250);
  }
  // Check whether either of the scores has surpassed the highscore
  for (byte i = 0; i < playerCount; i++) {
    if (scores[i] > prefs.getShort("highscore")) {
      prefs.putShort("highscore", scores[i]);
      // TODO: Display highscore on the led matrix
      // Flash all buttons yellow for 5 seconds
      for (byte i = 0; i < 10; i++) {
        buttonLeds.fill(buttonLeds.Color(255, 255, 0));
        buttonLeds.show();
        delay(250);
        buttonLeds.clear();
        buttonLeds.show();
        delay(250);
      }
    }
  }
  // Share the scores
  for (byte i = 0; i < playerCount; i++) {
    shareScore(scores[i]);
  }
}

void gameCountdown(void *params) {
  // Fill the ring and calculate the interval at which it should drain
  ledRing.fill(ledRing.Color(0, 255, 0));
  ledRing.show();
  int interval = prefs.getShort("gameDuration", 30) * 1000 / 32;
  for (byte i = 0; i < 32; i++) {
    // Turn the next led off and wait
    ledRing.setPixelColor(i, ledRing.Color(0, 0, 0));
    ledRing.show();
    vTaskDelay(pdMS_TO_TICKS(interval));
  }
}

// Use max + 1 or min - 1 as the old value if there is no old value
int getRandomIntBetween(int old, int min, int max) {
  while (true) {
    int rnd = random(max - min) + min;
    // Return the int if it's not the same as the old value, otherwise keep looping
    if (rnd != old) {
      return rnd;
    }
  }
}

class ServerCallback : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    // Calculated using https://arduinojson.org/v6/assistant/#/step1
    StaticJsonDocument<512 + 64 * NUMBER_OF_PLANTS> doc;
    doc["pin"] = "";
    // Add WiFi settings
    doc["ssid"] = prefs.getString("ssid");
    doc["pass"] = "";
    // Add general settings
    doc["gameDuration"] = prefs.getShort("gameDuration", 30);
    doc["tInterval"] = prefs.getShort("tInterval", 5);
    doc["mInterval"] = prefs.getShort("mInterval", 5);
    doc["highscore"] = prefs.getShort("highscore");
    doc["rValveFlow"] = prefs.getShort("rValveFlow", 20);
    doc["activeStart"] = prefs.getShort("activeStart", 7);
    doc["activeEnd"] = prefs.getShort("activeEnd", 21);
    doc["sunHours"] = prefs.getShort("sunHours", 8);
    doc["battDrain"] = prefs.getShort("batDrain", 30);
    // Add individual plant settings
    JsonArray plants = doc.createNestedArray("plants");
    for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
      JsonObject plant = plants.createNestedObject();
      plant["moistureLimit"] = prefs.getShort("moistureLimit" + i, 300);
      plant["valveFlow"] = prefs.getShort("valveFlow" + i, 20);
      plant["moistureValue"] = moistureSensors[i]->getLevel();
    }
    // Serialize the JSON data and set the characteristic's value
    char json_string[512 + 64 * NUMBER_OF_PLANTS];
    serializeJson(doc, json_string);
    pCharacteristic->setValue(json_string);
  };

  void onDisconnect(BLEServer *pServer) {
    BLEDevice::startAdvertising();
  }
};

class CCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String json = pCharacteristic->getValue().c_str();
    // Calculated using https://arduinojson.org/v6/assistant/#/step1
    StaticJsonDocument<512 + 64 * NUMBER_OF_PLANTS> doc;
    DeserializationError error = deserializeJson(doc, json);
    if (!error) {
      String pin = doc["pin"];
      if (pin == BLEPin) {
        // Store all general settings
        prefs.putShort("gameDuration", doc["gameDuration"]);
        prefs.putShort("tInterval", doc["tInterval"]);
        prefs.putShort("mInterval", doc["mInterval"]);
        prefs.putShort("highscore", doc["highscore"]);
        prefs.putShort("rValveFlow", doc["rValveFlow"]);
        prefs.putShort("activeStart", doc["activeStart"]);
        prefs.putShort("activeEnd", doc["activeEnd"]);
        prefs.putShort("sunHours", doc["sunHours"]);
        prefs.putShort("batDrain", doc["batDrain"]);
        // Store all plant specific settings
        for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
          prefs.putShort("moistureLimit" + i, doc["plants"][i]["moistureLimit"]);
          prefs.putShort("valveFlow" + i, doc["plants"][i]["valveFlow"]);
        }
        // If either the ssid or password has been changed, save the new values and reboot
        if (doc["ssid"] != prefs.getString("ssid") || doc["pass"] != "") {
          String ssid = doc["ssid"];
          String pass = doc["pass"];
          prefs.putString("ssid", ssid);
          prefs.putString("pass", pass);
          ESP.restart();
        }
      }
    }
  };
};

void initBLE() {
  BLEDevice::init("OpenX-ETP-Inst");
  // Create server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallback());
  // Create service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | 
    BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->setCallbacks(new CCallbacks());
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
}

void createUDPSensors() {
  // Exit the function if a WiFi connection is not established
  if (WiFi.status() != WL_CONNECTED) return;
  // Set http destination and headers
  HTTPClient http;
  http.begin("http://20.16.84.167:1026/v2/entities");
  http.addHeader("Content-Type", "application/json");
  // Execute post request for the installation
  http.POST("{"
    "\"id\":\"" UDP_NAME "\","
    "\"type\":\"MultiSensor\","
    "\"humidity\":{\"type\":\"Integer\"},"
    "\"temperature\":{\"type\":\"Float\"}"
  "}");
  // Execute post request for the game 'sensor'
  http.POST("{"
    "\"id\":\"" UDP_G_NAME "\","
    "\"type\":\"MultiSensor\","
    "\"score\":{\"type\":\"Integer\"},"
    "\"highscore\":{\"type\":\"Integer\"}"
  "}");
  // Execute post requests for the individual plants
  for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
    http.POST("{"
      "\"id\":\"" UDP_P_NAME + String(i) + "\","
      "\"type\":\"MultiSensor\","
      "\"moisture\":{\"type\":\"Integer\"},"
      "\"light\":{\"type\":\"Boolean\"}"
    "}");
  }
  http.end();
}

void shareScore(int score) {
  // Exit the function if a WiFi connection is not established
  if (WiFi.status() != WL_CONNECTED) return;
  // Create the JSON to be sent
  StaticJsonDocument<64> game;
  game["score"] = score;
  game["highscore"] = score;
  char game_json[64];
  serializeJson(game, game_json);
  // Set http destination and headers
  HTTPClient http;
  http.begin("http://20.16.84.167:1026/v2/entities/" UDP_G_NAME "/attrs");
  http.addHeader("Content-Type", "application/json");
  // Execute post request for the game data
  http.POST(game_json);
  http.end();
}

void udpTransmit(void *params) {
  for (;;) {
    // Don't try to send data if a WiFi connection is not established or when the system isn't in idle mode
    if (WiFi.status() == WL_CONNECTED && idle) {
      HTTPClient http;
      // Create the JSON to be sent
      StaticJsonDocument<128> installation;
      // TODO: measure and add light intensity
      JsonObject humidity = installation.createNestedObject("humidity");
      humidity["type"] = "Integer";
      humidity["value"] = dht.readHumidity();
      JsonObject temperature = installation.createNestedObject("temperature");
      temperature["type"] = "Integer";
      temperature["value"] = dht.readTemperature();
      char installation_json[128];
      serializeJson(installation, installation_json);
      // Transmit installation data
      http.begin("http://20.16.84.167:1026/v2/entities/" UDP_NAME "/attrs");
      http.addHeader("Content-Type", "application/json");
      http.POST(installation_json);
      http.end();
      // Transmit data for each plant
      for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
        StaticJsonDocument<128> plant;
        JsonObject moisture = plant.createNestedObject("moisture");
        moisture["type"] = "Integer";
        moisture["value"] = moistureSensors[i]->getLevel();
        JsonObject light = plant.createNestedObject("light");
        light["type"] = "Boolean";
        light["value"] = plantLamps[i]->state;
        char plant_json[128];
        serializeJson(plant, plant_json);
        http.begin("http://20.16.84.167:1026/v2/entities/" UDP_P_NAME + String(i) + "/attrs");
        http.addHeader("Content-Type", "application/json");
        http.POST(plant_json);
        http.end();
      }
    }
    vTaskDelay(pdMS_TO_TICKS(prefs.getShort("tInterval") * 60000));
  }
}

void measureValues(void *params) {
  for (;;) {
    if (idle) {
      for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
        if (plants[i]->needsWater()) {
          // TODO: Open the plant's water valve and add a delay for said plant to let the water transfer into the soil
        }
      }
      struct tm timeinfo;
      if (!getLocalTime(&timeinfo)) return;
      // Turn the lamps on after closing time if the specified number of sun hours hasn't been reached during the day
      if (timeinfo.tm_hour > prefs.getShort("activeEnd") || timeinfo.tm_hour < prefs.getShort("activeStart")) {
        // Check for each plant whether they need light
        for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
          // If the plant needs light, turn the lamp on, otherwise turn the lamp off, if the lamp is already off, do nothing
          if (plants[i]->needsLight()) {
            plants[i]->plantLamp->on();
          } else {
            plants[i]->plantLamp->off();
          }
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(prefs.getShort("mInterval") * 1000));
  }
}

void batDrain(void *params) {
  for (;;) {
    if (idle) {
      // Drain battery by one percent
      battery->drain();
      // Turn all lamps off if the battery is empty
      if (!battery->getLevel()) {
        for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
          plantLamps[i]->off();
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(prefs.getShort("batDrain") / 100 * 60000));
  }
}
