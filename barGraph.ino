byte _level0[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
};

// -- character with one bar
byte _level1[8] = {
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000
};
// -- character with two bars
byte _level2[8] = {
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000
};
// -- character with three bars
byte _level3[8] = {
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100
};
// -- character with four bars
byte _level4[8] = {
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110
};

void initBarGraph() {
  // -- creating characters
  lcd.createChar(0, _level0);
  lcd.createChar(1, _level1);
  lcd.createChar(2, _level2);
  lcd.createChar(3, _level3);
  lcd.createChar(4, _level4);
}

int _prevValue = 0;
byte _lastFullChars = 0;
void drawBarGraph(byte _numCols, byte _startX, byte _startY, int value, int maxValue) {
  // -- calculate full (filled) character count
  byte fullChars = (long)value * _numCols / maxValue;
  // -- calculate partial character bar count
  byte mod = ((long)value * _numCols * 5 / maxValue) % 5;

  // -- if value does not change, do not draw anything
  int normalizedValue = (int)fullChars * 5 + mod;
  if(_prevValue != normalizedValue) {
      // -- do not clear the display to eliminate flickers
      lcd.setCursor(_startX, _startY);
      
      // -- write filled characters
      for(byte i=0; i<fullChars; i++) {
        lcd.write(0);
      }
      
      // -- write the partial character
      if(mod > 0) {
          lcd.write(mod); // -- index the right partial character
          ++fullChars;
      }
      
      // -- clear characters left over the previous draw
      for(byte i=fullChars;i<_lastFullChars;i++) {
          lcd.write(' ');
      }
      
      // -- save cache
      _lastFullChars = fullChars;
      _prevValue = normalizedValue;
  }
}
