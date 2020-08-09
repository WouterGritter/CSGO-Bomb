void exploded_setup() {
  t.every(1000 / 60, exploded_updateLedStrip);

  // Print state
  lcd.clear();
  lcdPrintCenter("Exploded!", 0);
  lcdPrintCenter("* # to reset", 1);
}

void exploded_loop() {
  // Listen for reset pattern on keypad
  checkResetKeypadPattern(keypad.getKey());
}

bool exploded_updateLedStrip(void *) {
  if(currentState != STATE_EXPLODED) {
    return false;
  }
  
  float brightness = (cos(millis() * 3.14 / 2000.0 ) + 1.0) / 2.0;
  fillStrip(255 * brightness, 0, 0);

  return true; // Repeat ?
}
