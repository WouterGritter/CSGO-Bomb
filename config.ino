void config_setup() {
  EEPROM.begin(32);
  
  if(EEPROM.read(0) == 0xFF) {
    // Reset!
    config_writeValues();
    delay(500);
  }

  config_readValues();

  config_apply();
}

void config_writeValues() {
  EEPROM.write(0, config_brightness);
  EEPROM.write(1, config_explodeTime);
  EEPROM.write(2, config_defuseTime);

  EEPROM.commit();
}

void config_readValues() {
  config_brightness  = EEPROM.read(0);
  config_explodeTime = EEPROM.read(1);
  config_defuseTime  = EEPROM.read(2);
}

void config_save(byte brightness, byte explodeTime, byte defuseTime) {
  config_brightness = brightness;
  config_explodeTime = explodeTime;
  config_defuseTime = defuseTime;

  config_writeValues();

  config_apply();
}

void config_apply() {
  strip.setBrightness(config_brightness);
}
