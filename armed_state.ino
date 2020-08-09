// State variables
unsigned long armBegin;
unsigned long nextBeep;
unsigned long defusingSince;

void armed_setup() {
  // Initiate variables
  armBegin = millis();
  nextBeep = millis();
  defusingSince = -1;

  fillStrip(255, 0, 0);

  armed_updateLCD();

  // Loop to update the LCD
  t.every(250, [](void *) -> bool {
    if(currentState != STATE_ARMED) {
      return false; // Don't repeat
    }

    // Check if we should be defusing right now or not
    if(analogRead(PIN_DEFUSE_BTN) < 512) { // Button is on analog pin and has a pullup resistor!
      if(defusingSince == -1) {
        // Just started defusing!
        defusingSince = millis();
      }
    }else{
      defusingSince = -1;
    }
    
    armed_updateLCD();
    
    return true; // Repeat
  });
}

void armed_loop() {
  // Beep if we need to!
  if(millis() > nextBeep) {
    int waitMS = armed_beep();
    nextBeep = millis() + waitMS;
  }

  // Check if we should explode
  if(millisArmed() > (unsigned long) config_explodeTime * 1000) {
    // Explode!!!
    setState(STATE_EXPLODED);
    beep(400, 250, 3);
  }

  // Check if we should be defused
  if(millisDefusing() > (unsigned long) config_defuseTime * 1000) {
    // Defuse!!
    setState(STATE_DEFUSED);
    beep(50, 125, 4);
  }
}

void armed_updateLCD() {
  lcd.clear();

  // Print state
  if(defusingSince == -1) {
    lcdPrintCenter("Armed", 1);
  }else{
    lcdPrintCenter("Armed (defusing)", 0);
  }

  // Print a bar graph of the defusing time if we are defusing
  if(defusingSince != -1) {
    drawBarGraph(LCD_COLS - 4, 2, 1, millisDefusing(), (unsigned long) config_defuseTime * 1000);
  }
}

/**
 * Time between beeps is an exponential function
 * 
 * A0 = 1.04865151217746
 * A1 = 0.244017811416199
 * A2 = 1.76379778668885
 * 
 * Y(bps) = A0 * E^(A1*X + A2*X^2)
 * X = % OF TIME PASSED (0.0 - 1.0)
 * Y = BEEPS PER SECOND
 */
#define BEEP_A0 1.04865151217746
#define BEEP_A1 0.244017811416199
#define BEEP_A2 1.76379778668885
#define BOMB_BEEP_LENGTH 125
int armed_beep() {
  // Calculate next beep time
  float x = (float) millisArmed() / ((float) config_explodeTime * 1000.0);
  float n = BEEP_A1 * x + BEEP_A2 * x * x;
  float bps = BEEP_A0 * exp(n);

  // Convert bps (beeps per second) to a wait time in milliseconds
  int waitMS = (int) (1000.0 / bps);

  // Beep and flash LED strip
  int bombBeepLength = BOMB_BEEP_LENGTH;
  if(waitMS <= bombBeepLength) {
    bombBeepLength = waitMS - 10;
  }
  if(bombBeepLength > 0) {
    beep(BOMB_BEEP_LENGTH, 1);
    fillStrip(255, 0, 0);
    t.in(BOMB_BEEP_LENGTH, [](void *) -> bool {
      if(currentState == STATE_ARMED) {
        fillStrip(50, 0, 0);
      }
    });
  }
  
  return waitMS;
}

/**
 * How many milliseconds the bomb has been armed for
 */
unsigned long millisArmed() {
  return millis() - armBegin;
}

/**
 * How many milliseconds the bomb is in a defusing state (0 if not in a defusing state)
 */
unsigned long millisDefusing() {
  if(defusingSince == -1) {
    return 0;
  }
  
  return millis() - defusingSince;
}
