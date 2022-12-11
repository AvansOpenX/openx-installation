#include "MoistureSensor.h"
#include "Reservoir.h"
#include "Battery.h"
#include "Screen.h"
#include "Button.h"
#include "WaterValve.h"
#include "PlantLamp.h"
#include "Plant.h"
#include "DHT.h"
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

const byte MOISTURE_SENSOR_PINS[] {32, 35, 34, 39, 36};
const byte WATER_VALVE_PINS[] {12, 14, 27, 26, 25};
const byte LAMP_PINS[] {8, 9, 10, 11, 12}; // mcp2
const byte GAME_BUTTON_PINS[] {0, 13, 1, 12, 2, 11, 3, 10, 4, 9, 5, 8}; // mcp1
const byte RESERVOIR_SENSOR_PINS[] {6, 14, 7, 15}; // mcp2

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

void setup() {
  Serial.begin(9600);
  dht.begin();
  mcp1.begin_I2C(MCP_1_ADDRESS);
  mcp2.begin_I2C(MCP_2_ADDRESS);

  // Default values
  // prefs.begin("app", false);
  // prefs.putShort("gameDuration", 30); // Value in seconds
  // prefs.putShort("tInterval", 5); // Value in minutes
  // prefs.putShort("mInterval", 5); // Value in seconds
  // prefs.putShort("highscore", 862); // Highest score from playing the button game
  // prefs.putShort("rValveFlow", 20); // Percentage that the reservoir valve opens at an interval
  // prefs.putShort("rPumpFlow", 20); // Percentage (of duration) that the pump is on at an interval
  // for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
  //   prefs.putShort("moistureLimit" + i, 300 + i); // Values at which each plant should get water
  //   prefs.putShort("valveFlow" + i, 20 + i); // Percentages that the water valves open while watering
  // }
  // prefs.end();
  
  // IO Expanders need to be initialized before the reservoir
  reservoir = new Reservoir(RESERVOIR_SENSOR_PINS, RESERVOIR_VALVE_PIN, RESERVOIR_PUMP_PIN, RESERVOIR_LED_PIN, mcp2);

  // EEPROM needs to be initialized before the moisture sensors and valves
  for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
    // Create plant dependencies
    PlantLamp *plantLamp = new PlantLamp(LAMP_PINS[i], mcp2);
    WaterValve *waterValve = new WaterValve(WATER_VALVE_PINS[i]);
    moistureSensors[i] = new MoistureSensor(MOISTURE_SENSOR_PINS[i]);
    // Create the plant object and save it in the plants array
    plants[i] = new Plant(i, moistureSensors[i], waterValve, plantLamp);
  }

  // IO Expanders need to be initialized before the buttons
  startButton = new Button(START_BUTTON_PIN, mcp1);
  modeButton = new Button(MODE_BUTTON_PIN, mcp1);
  for (byte i = 0; i < NUMBER_OF_BUTTONS; i++) {
    gameButtons[i] = new Button(GAME_BUTTON_PINS[i], mcp1);
  }
}

void loop() {
  prefs.begin("app", false);
  Serial.println(prefs.getShort("gameDuration"));
  Serial.println(prefs.getShort("tInterval"));
  Serial.println(prefs.getShort("mInterval"));
  Serial.println(prefs.getShort("highscore"));
  Serial.println(prefs.getShort("rValveFlow"));
  Serial.println(prefs.getShort("rPumpFlow"));
  for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
    Serial.println(prefs.getShort("moistureLimit" + i));
    Serial.println(prefs.getShort("valveFlow" + i));
  }
  prefs.end();

  Serial.println("");
  delay(3000);
}

void shareData() {
  unsigned static long previousBroadcast = millis();
  if (millis() - previousBroadcast >= 5 * 60000) {
    previousBroadcast = millis();
    // Gather data to be shared
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    // TODO: measure light intensity
    int soilMoistureOffsets[NUMBER_OF_PLANTS];
    for (byte i = 0; i < NUMBER_OF_PLANTS; i++) {
      // soilMoistureOffsets[i] = moistureSensors[i].getOffset();
    }
    // TODO: Send the above values paired with the highscore and the playcount to UDP
  }
}