#include <EEPROM.h>
#include <timer.h>
#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <Keypad.h>

#include <Adafruit_NeoPixel.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Default config values..
byte config_brightness = 255;
byte config_explodeTime = 45;
byte config_defuseTime = 5;
//

// -- Keypad stuff
// KEYPAD PINS:
// 0  1  2  3  | 4  5  6  (CONNETOR)
// R0 R1 R2 R3 | C0 C1 C2 (COL/ROW PINS)
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 3

char keypadKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
// --

// -- LED strip stuff
#define STRIP_LENGTH 22
// --

// -- LCD stuff
#define LCD_ROWS 4
#define LCD_COLS 20
// --

// -- States
#define STATE_DISARMED 0
#define STATE_ARMED    1
#define STATE_DEFUSED  2
#define STATE_EXPLODED 3
#define STATE_SETUP    4
// --

// -- PINS
byte keypadRowPins[KEYPAD_ROWS] = {13, 12, 14, 27};
byte keypadColPins[KEYPAD_COLS] = {26, 25, 33};

#define PIN_BEEPER 32
#define PIN_STRIP 4
#define PIN_DEFUSE_LED 2
#define PIN_DEFUSE_BTN 15
//

auto t = timer_create_default();

Keypad keypad(makeKeymap(keypadKeys), keypadRowPins, keypadColPins, KEYPAD_ROWS, KEYPAD_COLS);

Adafruit_NeoPixel strip(STRIP_LENGTH, PIN_STRIP, NEO_GRB + NEO_KHZ800);

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

byte currentState = STATE_DISARMED;

void setup() {
  // Setup the configuration
  config_setup();
  
  // Set up the LCD
  lcd.init();
  lcd.backlight();
  
  // Set up bar graph stuff (special chars)
  initBarGraph();

  // Set up pins
  pinMode(PIN_BEEPER, OUTPUT);
  pinMode(PIN_DEFUSE_LED, OUTPUT);
  pinMode(PIN_DEFUSE_BTN, INPUT_PULLUP);
  
  digitalWrite(PIN_BEEPER, HIGH);
  digitalWrite(PIN_DEFUSE_LED, LOW);

  // Reset the led strip
  fillStrip(0, 0, 0);

  // Set up wifi
  web_setup(); // BLOCKING!

  // Reset the state of the bomb
  reset();
}

// Loop function
void loop() {
  // Tick the timer (timer.h)
  t.tick();

  // Loop web
  web_loop();

  // Call loop function for appropriate state
  switch(currentState) {
    case STATE_DISARMED:
      disarmed_loop();
      break;
    case STATE_ARMED:
      armed_loop();
      break;
    case STATE_DEFUSED:
      defused_loop();
      break;
    case STATE_EXPLODED:
      exploded_loop();
      break;
    case STATE_SETUP:
      setup_loop();
      break;
  }
}

/**
 * Resets the state
 */
void reset() {
  fillStrip(0, 0, 0);
  setState(STATE_DISARMED);
}

/**
 * Sets the state
 */
void setState(byte newState) {
  currentState = newState;

  // Call setup function for appropriate state
  switch(currentState) {
    case STATE_DISARMED:
      disarmed_setup();
      break;
    case STATE_ARMED:
      armed_setup();
      break;
    case STATE_DEFUSED:
      defused_setup();
      break;
    case STATE_EXPLODED:
      exploded_setup();
      break;
    case STATE_SETUP:
      setup_setup();
      break;
  }
}
