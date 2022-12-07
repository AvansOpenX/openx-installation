#include "MoistureSensor.h"
#include "Reservoir.h"
#include "Battery.h"
#include "Screen.h"
#include "Button.h"
#include "WaterValve.h"
#include "PlantLamp.h"
// Potential long-term issue https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
#include <Adafruit_MCP23X17.h>

// Plants
#define MOISTURE_SENSOR_1_PIN 32
#define MOISTURE_SENSOR_2_PIN 35
#define MOISTURE_SENSOR_3_PIN 34
#define MOISTURE_SENSOR_4_PIN 39
#define MOISTURE_SENSOR_5_PIN 36
#define WATER_VALVE_1_PIN 12
#define WATER_VALVE_2_PIN 14
#define WATER_VALVE_3_PIN 27
#define WATER_VALVE_4_PIN 26
#define WATER_VALVE_5_PIN 25
#define LAMP_1_PIN 8 // mcp2
#define LAMP_2_PIN 9 // mcp2
#define LAMP_3_PIN 10 // mcp2
#define LAMP_4_PIN 11 // mcp2
#define LAMP_5_PIN 12 // mcp2
// Buttons (mcp1)
#define START_RING_PIN 15
#define BUTTON_LED_PIN 4
#define START_BUTTON_PIN 6
#define MODE_BUTTON_PIN 14
#define GAME_BUTTON_1_PIN 0
#define GAME_BUTTON_2_PIN 13
#define GAME_BUTTON_3_PIN 1
#define GAME_BUTTON_4_PIN 12
#define GAME_BUTTON_5_PIN 2
#define GAME_BUTTON_6_PIN 11
#define GAME_BUTTON_7_PIN 3
#define GAME_BUTTON_8_PIN 10
#define GAME_BUTTON_9_PIN 4
#define GAME_BUTTON_10_PIN 9
#define GAME_BUTTON_11_PIN 5
#define GAME_BUTTON_12_PIN 8
// Reservoir
#define RESERVOIR_LED_PIN 2
#define RESERVOIR_PUMP_PIN 13
#define RESERVOIR_VALVE_PIN 33
#define RESERVIOR_SENSOR_1_PIN 5
#define RESERVIOR_SENSOR_2_PIN 16
#define RESERVIOR_SENSOR_3_PIN 17
#define RESERVIOR_SENSOR_4_PIN 19
// EEPROM Addresses
#define GAME_DURATION_ADDRESS
#define MEASUREMENT_INTERVAL_ADDRESS
#define DATA_SHARE_INTERVAL_ADDRESS
#define BATTERY_DRAIN_ADDRESS
#define RESERVOIR_DRAIN_ADDRESS
#define 
// Other
#define MCP_1_ADDRESS 0x20
#define MCP_2_ADDRESS 0x21
#define NUMBER_OF_PLANTS 5
#define NUMBER_OF_BUTTONS 12

Adafruit_MCP23X17 mcp1;
Adafruit_MCP23X17 mcp2;

Plant plants[];
Reservoir reservoir;
Battery battery;
Screen screen;

Button startButton(START_BUTTON_PIN, mcp1);
Button modeButton(MODE_BUTTON_PIN, mcp1);

MoistureSensor moistureSensors[] = {
  (MOISTURE_SENSOR_1_PIN), (MOISTURE_SENSOR_2_PIN), (MOISTURE_SENSOR_3_PIN), (MOISTURE_SENSOR_4_PIN), (MOISTURE_SENSOR_5_PIN)
};

WaterValve waterValves[] = {
  (WATER_VALVE_1_PIN), (WATER_VALVE_2_PIN), (WATER_VALVE_3_PIN), (WATER_VALVE_4_PIN), (WATER_VALVE_5_PIN)
};

PlantLamp lamps[] = {
  (LAMP_1_PIN, mcp2), (LAMP_2_PIN, mcp2), (LAMP_3_PIN, mcp2), (LAMP_4_PIN, mcp2), (LAMP_5_PIN, mcp2)
};

Button gameButtons[] = {
  (GAME_BUTTON_1_PIN, mcp1), (GAME_BUTTON_2_PIN, mcp1), (GAME_BUTTON_3_PIN, mcp1), (GAME_BUTTON_4_PIN, mcp1),
  (GAME_BUTTON_5_PIN, mcp1), (GAME_BUTTON_6_PIN, mcp1), (GAME_BUTTON_7_PIN, mcp1), (GAME_BUTTON_8_PIN, mcp1),
  (GAME_BUTTON_9_PIN, mcp1), (GAME_BUTTON_10_PIN, mcp1), (GAME_BUTTON_11_PIN, mcp1), (GAME_BUTTON_12_PIN, mcp1)
};

void setup() {
  Serial.begin(9600);
  mcp1.begin_I2C(MCP_1_ADDRESS);
  mcp2.begin_I2C(MCP_2_ADDRESS);
  // Create the plant objects and assign lamps, valves, and sensors to them
  for (int i = 2; i < NUMBER_OF_PLANTS; i++) {
    Plant plant(moistureSensors[i], waterValves[i], lamps[i]);
    plants[i] = plant;
  }
}

void loop() {
  
}