#define SETUP_NUM_VARS 3
#define SETUP_BRIGHTNESS 0
#define SETUP_EXPLODE_TIME 1
#define SETUP_DEFUSE_TIME 2

#define SETUP_SAVE 3

int setupCursor;
bool setupEditing;
String setupValues[SETUP_NUM_VARS];

void setup_setup() {
  fillStrip(0, 0, 0);
  digitalWrite(PIN_DEFUSE_LED, LOW);

  setupCursor = 0;
  setupEditing = false;

  // Load values from config
  setupValues[SETUP_BRIGHTNESS] = String(config_brightness);
  setupValues[SETUP_EXPLODE_TIME] = String(config_explodeTime);
  setupValues[SETUP_DEFUSE_TIME] = String(config_defuseTime);

  setup_updateLCD();
}

void setup_loop() {
  char key = keypad.getKey();
  if(key) {
    if(key == '*') {
      // Move cursor!
      if(!setupEditing) {
        setupCursor++;
        if(setupCursor >= LCD_ROWS) {
          setupCursor = 0;
        }
      }
    }else if(key == '#') {
      if(setupCursor == SETUP_SAVE) {
        // Save and exit!
        setup_save();
        reset();
        
        return;
      }else{
        // Edit/save value!
        if(!setupEditing) {
          // Start editing!
          
          setupEditing = true;
          setupValues[setupCursor] = "";
        }else{
          // End editing!
  
          setupEditing = false;
        }
      }
    }else{
      if(setupEditing) {
        setupValues[setupCursor] += key;
      }
    }

    setup_updateLCD();
    beep(20, 1);
  }
}

void setup_updateLCD() {
  lcd.clear();

  // Cursor
  lcd.setCursor(0, setupCursor);
  lcd.print(!setupEditing ? '*' : '#');

  // Options
  lcd.setCursor(1, SETUP_BRIGHTNESS);
  lcd.print("Brightness: ");
  lcd.print(setupValues[SETUP_BRIGHTNESS]);

  lcd.setCursor(1, SETUP_EXPLODE_TIME);
  lcd.print("Explode time: ");
  lcd.print(setupValues[SETUP_EXPLODE_TIME]);

  lcd.setCursor(1, SETUP_DEFUSE_TIME);
  lcd.print("Defuse time: ");
  lcd.print(setupValues[SETUP_DEFUSE_TIME]);

  lcd.setCursor(1, SETUP_SAVE);
  lcd.print(" Save & exit");
}

void setup_save() {
  config_save(
    setupValues[SETUP_BRIGHTNESS].toInt(),
    setupValues[SETUP_EXPLODE_TIME].toInt(),
    setupValues[SETUP_DEFUSE_TIME].toInt()
  );
}
