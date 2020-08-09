#define STA_SSID "SSID"
#define STA_PSK "PASSWORD"

ESP8266WebServer server(80);

void web_setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_SSID, STA_PSK);

  int dots = -1;
  while(WiFi.status() != WL_CONNECTED) {
    if(dots == -1 || dots > 3) {
      lcd.clear();
      
      lcd.setCursor(0, 2);
      lcd.print("Press any key");
      lcd.setCursor(0, 3);
      lcd.print("to cancel.");
      
      lcd.setCursor(0, 0);
      lcd.print("Connecting");
      dots = 0;
    }

    delay(500);
    
    lcd.print(".");
    dots++;

    if(keypad.getKey()) {
      // A key has been pressed. Cancel!
      break;
    }
  }

  server.begin();
  web_configureEndpoints();

  // Print stuff to lcd
  lcd.clear();
  
  lcd.setCursor(0, 0);
  if(WiFi.status() == WL_CONNECTED) {
    lcd.print("Connected!");
  }else{
    lcd.print("Cancelled.");
  }

  lcd.setCursor(0, 1);
  lcd.print("IP: ");
  lcd.print(WiFi.localIP());

  lcd.setCursor(0, 3);
  lcd.print("Press any key..");

  while(!keypad.getKey()) {
    delay(1);
  }

  lcd.clear();
  beep(30, 1);
}

void web_loop() {
  if(WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  }
}

void web_configureEndpoints() {
  server.on("/info", web_info);
}

void web_info() {
  String out = "";
  
  String state;
  switch(currentState) {
    case STATE_DISARMED:
      state = "disarmed";
      break;
    case STATE_ARMED:
      state = "armed";
      break;
    case STATE_DEFUSED:
      state = "defused";
      break;
    case STATE_EXPLODED:
      state = "exploded";
      break;
    case STATE_SETUP:
      state = "setup";
      break;
    
    default:
      state = currentState;
      break;
  }

  // General data
  out += "\nmillis=";
  out += millis();
  
  out += "\nstate=";
  out += state;

  // Config data
  out += "\nconfig_brightness=";
  out += config_brightness;

  out += "\nconfig_explodeTime=";
  out += config_explodeTime;

  out += "\nconfig_defuseTime=";
  out += config_defuseTime;

  // Disarmed state
  out += "\nenteredCode=";
  out += enteredCode;

  // Armed state
  out += "\nmillisArmed=";
  out += currentState == STATE_ARMED ? millisArmed() : 0;
  
  out += "\nmillisDefusing=";
  out += currentState == STATE_ARMED ? millisDefusing() : 0;

  server.send(200, "text/plain", out);
}
