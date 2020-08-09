/**
 * Beep!
 * onLength = how many MS the beeper will be on
 * offLength = how many MS the beeper will be off
 * amount = how many times to repeat the beep for
 */
void beep(unsigned long onLength, unsigned long offLength, byte amount) {
  unsigned long length = 0;
  for(byte i = 0; i < amount * 2; i++) {
    if(i % 2 == 0) {
      // Beeper should go ON in this stage.
      t.in(length, [](void *) -> bool {
        digitalWrite(PIN_BEEPER, LOW); // LOW -> ON (active low);
  
        return false; // Don't repeat
      });

      length += onLength;
    }else{
      // Beeper should go OFF in this stage.
      t.in(length, [](void *) -> bool {
        digitalWrite(PIN_BEEPER, HIGH); // HIGH -> OFF (active low);
  
        return false; // Don't repeat
      });

      length += offLength;
    }
  }
}

/**
 * Beep!
 * onOffLength = how many MS the beeper will be on and off
 * amount = how many times to repeat the beep for
 */
inline void beep(unsigned long onOffLength, byte amount) {
  beep(onOffLength, onOffLength, amount);
}

/**
 * Fills the led strip with a certain color
 */
void fillStrip(int r, int g, int b) {
  strip.begin();
  for(int i = 0; i < STRIP_LENGTH; i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}

/**
 * Simple map function with floats
 */
inline float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * Checks for the reset pattern (* #) on the keypad
 */
String reset_charsEntered = "";
void checkResetKeypadPattern(char justPressed) {
  if(justPressed) {
    if(reset_charsEntered.length() == 2) {
      reset_charsEntered = reset_charsEntered.substring(1);
    }
    reset_charsEntered += justPressed;
    if(reset_charsEntered.charAt(reset_charsEntered.length() - 2) == '*' &&
       reset_charsEntered.charAt(reset_charsEntered.length() - 1) == '#') {
      // Reset!
//      beep(20, 200, 2);
      
      reset();
    }
  }
}

/**
 * Prints a string to the lcd in a centered x position
 */
void lcdPrintCenter(String s, int y) {
  int x = LCD_COLS / 2 - ceil(s.length() / 2.0);
  lcd.setCursor(x, y);
  lcd.print(s);
}
