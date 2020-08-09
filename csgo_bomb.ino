#include <EEPROM.h>
#include <timer.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

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
byte keypadRowPins[KEYPAD_ROWS] = {D7, D6, D5, D0};
byte keypadColPins[KEYPAD_COLS] = {D4, D3, RX};

#define PIN_BEEPER TX
#define PIN_STRIP D8

#define PIN_DEFUSE_BTN A0
//

auto t = timer_create_default();

Keypad keypad(makeKeymap(keypadKeys), keypadRowPins, keypadColPins, KEYPAD_ROWS, KEYPAD_COLS);

Adafruit_NeoPixel strip(STRIP_LENGTH, PIN_STRIP, NEO_GRB + NEO_KHZ800);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

byte currentState = STATE_DISARMED;

void setup() {
  // Setup the configuration
  config_setup();
  
  // Set up the LCD
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.backlight();
  
  // Set up bar graph stuff (special chars)
  initBarGraph();

  // Set up pins
  pinMode(PIN_BEEPER, OUTPUT);
  digitalWrite(PIN_BEEPER, HIGH);

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
