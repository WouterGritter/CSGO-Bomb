#define CODE_CLEAR_TIME 6000

#define CORRECT_CODE "7355608"
#define SETUP_CODE "9999999"
#define CODE_LENGTH 7

// State vars
String enteredCode = "";
unsigned long lastKeyPress;

void disarmed_setup() {
  // Reset vars
  enteredCode = "";
  lastKeyPress = millis();
  
  fillStrip(0, 0, 255);
  digitalWrite(PIN_DEFUSE_LED, LOW);

  disarmed_updateLCD();
}

void disarmed_loop() {
  // Listen for keystrokes
  char key = keypad.getKey();
  if(key) {
    if(key == '*' || key == '#') {
      // Backspace
      enteredCode = enteredCode.substring(0, enteredCode.length() - 1);
      beep(85, 1);
    }else{
      // A number has been entered
      enteredCode += key;
    
      // Check if the correct code has been entered
      if(enteredCode.length() == CODE_LENGTH) {
        if(enteredCode == CORRECT_CODE) {
          // Entered correct code!
          beep(250, 1);
    
          setState(STATE_ARMED);
          return;
        }else if(enteredCode == SETUP_CODE) {
          // Entered setup code!
          beep(200, 2);

          setState(STATE_SETUP);
          return;
        }else{
          // Entered incorrect code!
          beep(250, 2);
        }
    
        enteredCode = "";
      }else{
        // Beep!
        beep(65, 1);
      }
    }

    // Update the LCD with the new code
    disarmed_updateLCD();
    
    lastKeyPress = millis();
  }

  // Clear the code if nothing has been typed on the keypad for a while
  if(enteredCode.length() > 0 && millis() > lastKeyPress + CODE_CLEAR_TIME) {
    // Clear the code!
    enteredCode = "";
    disarmed_updateLCD();
  }
}

void disarmed_updateLCD() {
  // Print the state
  lcd.clear();
  lcdPrintCenter("Disarmed", 0);

  // Format the code into a string which has a fixed length and replaces missing
  // characters with '_'
  String codeStr = "";
  for(int i = 0; i < CODE_LENGTH; i++) {
    if(i < enteredCode.length()) {
      codeStr += enteredCode.charAt(i);
    }else{
      codeStr += '_';
    }
  }

  // Print the currently entered code to the lcd
  lcdPrintCenter(codeStr, 2);
}
