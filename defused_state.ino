void defused_setup() {
  fillStrip(0, 255, 0);

  // Print state
  lcd.clear();
  lcdPrintCenter("Defused!", 0);
  lcdPrintCenter("* # to reset", 1);
}

void defused_loop() {
  // Listen for reset pattern on keypad
  checkResetKeypadPattern(keypad.getKey());
}
