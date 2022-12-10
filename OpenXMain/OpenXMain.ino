#include "MoistureSensor.h"
#include "Reservoir.h"
#include "Battery.h"
#include "Screen.h"
#include "Button.h"
#include "WaterValve.h"
#include "PlantLamp.h"
#include "Plant.h"
#include "DHT.h"
#include <EEPROM.h>
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
// EEPROM addresses
#define EEPROM_SIZE 23  // Number of necessary bytes
// TODO: Write and read settings from EEPROM
// #define GAME_DURATION_ADDRESS 0 // Value stored in seconds
// #define BROADCAST_INTERVAL_ADDRESS 1 // Value stored in minutes, minimum of 2 seconds due to DHT22 sampling period limits
// #define MEASUREMENT_INTERVAL_ADDRESS 2 // Value stored in seconds
// #define HIGHSCORE_ADDRESS 3
// byte moistureAddresses[NUMBER_OF_PLANTS][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {9, 10, 11}, {12, 13, 14}};
// byte valveAddresses[NUMBER_OF_PLANTS] = {15, 16, 17, 18, 19};

const byte MOISTURE_SENSOR_PINS[] {32, 35, 34, 39, 36};
const byte WATER_VALVE_PINS[] {12, 14, 27, 26, 25};
const byte LAMP_PINS[] {8, 9, 10, 11, 12}; // mcp2
const byte GAME_BUTTON_PINS[] {0, 13, 1, 12, 2, 11, 3, 10, 4, 9, 5, 8}; // mcp1
const byte RESERVOIR_SENSOR_PINS[] {6, 14, 7, 15}; // mcp2

// Global objects
DHT dht(DHTPIN, DHTTYPE);
Adafruit_MCP23X17 mcp1;
Adafruit_MCP23X17 mcp2;

// TODO: Initialize battery and screen objects
// Battery battery;
// Screen screen;

Reservoir *reservoir;
Button *startButton;
Button *modeButton;
Button *gameButtons[NUMBER_OF_BUTTONS];
Plant *plants[NUMBER_OF_PLANTS];
MoistureSensor *moistureSensors[NUMBER_OF_PLANTS];

void setup() {
  Serial.begin(9600);
  dht.begin();
  EEPROM.begin(EEPROM_SIZE);
  mcp1.begin_I2C(MCP_1_ADDRESS);
  mcp2.begin_I2C(MCP_2_ADDRESS);

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