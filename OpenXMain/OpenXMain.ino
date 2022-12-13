#include "MoistureSensor.h"
#include "Reservoir.h"
#include "Battery.h"
#include "Screen.h"
#include "Button.h"
#include "WaterValve.h"
#include "PlantLamp.h"
#include "Plant.h"
#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>
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

const byte MOISTURE_SENSOR_PINS[] {32, 35, 34, 39, 36};
const byte WATER_VALVE_PINS[] {12, 14, 27, 26, 25};
const byte LAMP_PINS[] {8, 9, 10, 11, 12}; // mcp2
const byte GAME_BUTTON_PINS[] {0, 13, 1, 12, 2, 11, 3, 10, 4, 9, 5, 8}; // mcp1
const byte RESERVOIR_SENSOR_PINS[] {6, 14, 7, 15}; // mcp2

// TODO: Store SSID and password in prefs
// const char* ssid = "The Promised LAN";
// const char* password =  "password";
const char* ssid = "G11-R1";
const char* password =  "trombone";

// Global objects
Preferences prefs;
DHT dht(DHTPIN, DHTTYPE);
Adafruit_MCP23X17 mcp1;
Adafruit_MCP23X17 mcp2;
// TODO: Initialize battery and screen objects
Reservoir *reservoir;
Button *startButton;
Button *modeButton;
Button *gameButtons[NUMBER_OF_BUTTONS];
Plant *plants[NUMBER_OF_PLANTS];
MoistureSensor *moistureSensors[NUMBER_OF_PLANTS];
PlantLamp *plantLamps[NUMBER_OF_PLANTS];

// Should be set to false when doing any action (running game, watering plants, filling reservoir, etc.)
bool idle = false;

void setup() {
  Serial.begin(9600);
  dht.begin();
  mcp1.begin_I2C(MCP_1_ADDRESS);
  mcp2.begin_I2C(MCP_2_ADDRESS);
  prefs.begin("app", false);
  WiFi.begin(ssid, password);
  // WiFi needs a small delay before it works
  delay(2000);
  createUDPSensors();

  // Set default values, enable this code block if the sketch is flashed to a new board
  // prefs.putShort("gameDuration", 30); // Value in seconds
  // prefs.putShort("tInterval", 5); // Value in minutes
  // prefs.putShort("mInterval", 5); // Value in seconds
  // prefs.putShort("highscore", 862); // Highest score from playing the button game
  // prefs.putShort("rValveFlow", 20); // Percentage that the reservoir valve opens at an interval
  // for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
  //   prefs.putShort("moistureLimit" + i, 300 + i); // Values at which each plant should get water
  //   prefs.putShort("valveFlow" + i, 20 + i); // Percentages that the water valves open while watering
  // }
  
  // IO Expanders need to be initialized before the reservoir
  reservoir = new Reservoir(RESERVOIR_SENSOR_PINS, RESERVOIR_VALVE_PIN, RESERVOIR_PUMP_PIN, RESERVOIR_LED_PIN, mcp2, prefs);

  // EEPROM needs to be initialized before the moisture sensors and valves
  for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
    // Create plant dependencies
    plantLamps[i] = new PlantLamp(LAMP_PINS[i], mcp2);
    WaterValve *waterValve = new WaterValve(WATER_VALVE_PINS[i], prefs);
    moistureSensors[i] = new MoistureSensor(MOISTURE_SENSOR_PINS[i], prefs);
    // Create the plant object and save it in the plants array
    plants[i] = new Plant(i, moistureSensors[i], waterValve, plantLamps[i]);
  }

  // IO Expanders need to be initialized before the buttons
  startButton = new Button(START_BUTTON_PIN, mcp1);
  modeButton = new Button(MODE_BUTTON_PIN, mcp1);
  for (byte i = 0; i < NUMBER_OF_BUTTONS; i++) {
    gameButtons[i] = new Button(GAME_BUTTON_PINS[i], mcp1);
  }

  idle = true;
}

void loop() {
  // Serial.println(prefs.getShort("gameDuration"));
  // Serial.println(prefs.getShort("mInterval"));
  // Serial.println(prefs.getShort("highscore"));

  delay(3000);
  shareData();
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
    "\"id\":\""UDP_NAME"\","
    "\"type\":\"MultiSensor\","
    "\"humidity\":{\"type\":\"Integer\"},"
    "\"temperature\":{\"type\":\"Float\"}"
  "}");
  // Execute post request for the game 'sensor'
  http.POST("{"
    "\"id\":\""UDP_G_NAME"\","
    "\"type\":\"MultiSensor\","
    "\"score\":{\"type\":\"Integer\"},"
    "\"highscore\":{\"type\":\"Integer\"}"
  "}");
  // Execute post requests for the individual plants
  for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
    http.POST("{"
      "\"id\":\""UDP_P_NAME + String(i) + "\","
      "\"type\":\"MultiSensor\","
      "\"moisture\":{\"type\":\"Integer\"},"
      "\"light\":{\"type\":\"Boolean\"}"
    "}");
  }
  http.end();
}

void shareData() {
  unsigned static long previousBroadcast = millis();
  // Only transmit measurement data if the system is idle 
  if (millis() - previousBroadcast >= prefs.getShort("tInterval") * 60000 && idle) {
    // Exit the function if a WiFi connection is not established
    if (WiFi.status() != WL_CONNECTED) return;
    previousBroadcast = millis();
    // TODO: measure and add light intensity
    HTTPClient http;
    // Transmit installation data
    http.begin("http://20.16.84.167:1026/v2/entities/"UDP_NAME"/attrs");
    http.addHeader("Content-Type", "application/json");
    http.POST("{"
      "\"humidity\":{\"type\":\"Integer\",\"value\":" + String(dht.readHumidity()) + "},"
      "\"temperature\":{\"type\":\"Integer\",\"value\":" + String(dht.readTemperature()) + "}"
    "}");
    http.end();
    // Transmit data for each plant
    for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
      http.begin("http://20.16.84.167:1026/v2/entities/"UDP_P_NAME + String(i) + "/attrs");
      http.addHeader("Content-Type", "application/json");
      http.POST("{"
        "\"moisture\":{\"type\":\"Integer\",\"value\":" + String(moistureSensors[i]->getLevel()) + "},"
        "\"light\":{\"type\":\"Boolean\",\"value\":" + String(plantLamps[i]->state) + "}"
      "}");
      http.end();
    }
  }
}
