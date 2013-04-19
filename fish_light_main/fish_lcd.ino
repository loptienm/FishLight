///////////////////////////////
//
// This file holds the LCD functions for the fish light
//
///////////////////////////////

/*
//////////////////////////
// Global Vars for LCD screen
//  The LCD circuit:
// * LCD RS pin to digital pin 8
// * LCD Enable pin to digital pin 7
// * LCD D4 pin to digital pin 5
// * LCD D5 pin to digital pin 4
// * LCD D6 pin to digital pin 3
// * LCD D7 pin to digital pin 2
// * LCD R/W pin to ground

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD1Pin, LCD2Pin, LCD3Pin, LCD4Pin, LCD5Pin, LCD6Pin);
int lcd_use_lvl = 40;           // PWM value for LCD brightness while the system is in use (serial commands, buttons, change max brightness)
int lcd_dim_lvl = 10;           // PWM value for LCD brightness while the system has gone idle
int lcd_off_lvl = 0;            // PWM value for LCD brightness while the system has not been used for a while
int lcd_cur_lvl = lcd_use_lvl;  // Current PWM value for LCD brightness
int lcd_idle_time = 10;         // 10 seconds till the lcd dims its backlight
int lcd_off_time  = 15;         // 15 seconds till the lcd turns off its backlight
int idle_flag     = 0;          // Flag for determining if system is idle
int dim_count     = 0;          // Counter to delay between dimming the lcd each leveldim_count
int lcd_cursor_loc[2];          // Current location of the LCD cursor ([0] = horizontal dimension, [1] = vertical dimension)
int dbgLcd = 0;                 // Flag to debug the LCD screen functions
//////////////////////////
*/

///////////////////////////////
//
// LCD functions
//
///////////////////////////////

// Move Cursor Right
void moveCursorRight() {
  lcd_cursor_loc[0]++;
  if (lcd_cursor_loc[0] > LCD_RIGHT_EDGE)  // If cursor gets moved off screen, wrap to 0
    lcd_cursor_loc[0] = LCD_LEFT_EDGE;

  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}

// Move Cursor Left
void moveCursorLeft() {
  if (lcd_cursor_loc[0] == LCD_LEFT_EDGE)
    lcd_cursor_loc[0] = LCD_RIGHT_EDGE;
  else
    lcd_cursor_loc[0]--;

  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}

// Move Cursor Up
void moveCursorUp() {
  lcd_cursor_loc[1]++;
  if (lcd_cursor_loc[1] > LCD_TOP_EDGE)  // If cursor gets moved off screen, wrap to 0
    lcd_cursor_loc[1] = LCD_BTM_EDGE;
    
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}

// Move Cursor Down
void moveCursorDown() {
  if (lcd_cursor_loc[1] == LCD_BTM_EDGE)
    lcd_cursor_loc[1] = LCD_TOP_EDGE;
  else
    lcd_cursor_loc[1]--;
    
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}

// Get the status of the LCD Back Light
// Should it be on, dimming, or off
void getLCDblStatus() {
    
  // If LCD idle_flag has been reset, make sure to reset the dim_count
  if (idle_flag == 0 && (!keyHeld || !keyPressed)) {
    dim_count = 0;
    idle_flag = 1;  // flag to show LCD should be counting toward idle state
    second_count = 0;
  }
  
  // See if the second has changed
  if (prev_second != second) {
    second_count++;
  }
  prev_second = second;
  
  /*Serial.print("second: ");
  Serial.print(second);
  Serial.print(" prev_second: ");
  Serial.print(prev_second);
  Serial.print(" second_count: ");
  Serial.print(second_count);
  Serial.print(" idle_flag: ");
  Serial.print(idle_flag);
  Serial.print(" dim_count: ");
  Serial.print(dim_count);
  Serial.print(" lcd_cur_lvl: ");
  Serial.println(lcd_cur_lvl);*/
  if (idle_flag && second_count >= lcd_idle_time && second_count < lcd_off_time) {
    dimLCDbl(lcd_dim_lvl);
  }
  else if (idle_flag && second_count >= lcd_off_time) {
    second_count = lcd_off_time + 1;  // Held to make sure it doesnt roll over
    dimLCDbl(lcd_off_lvl);
  }
  else {
    lcd_cur_lvl = lcd_use_lvl;
    dimLCDbl(lcd_use_lvl);
  }
}

// This is the main screen displayed on the LCD
void MainScreenLCD(int mins, int CWp, int WWp, int BLp) {
  int tempCursorLoc[1];
  
  lcd.noCursor();
  lcd.clear();
  //tempCursorLoc[0] = lcd_cursor_loc[0];  // Save the current cursor location while we draw the screen
  //tempCursorLoc[1] = lcd_cursor_loc[1];
/*  Serial.print("Time: ");
  if( mins/60 < 10)
    Serial.print("0");
  Serial.print(mins/60);
  Serial.print(":");
  if( mins%60 < 10)
    Serial.print("0");
  Serial.print(mins%60);
  Serial.print(" Max Bright: ");
  Serial.print(MaxBrightPWM);
  Serial.print(" CW: ");
  Serial.print(CWp);
  Serial.print(", ");
  Serial.print(CWBright);
  Serial.print(" WW: ");
  Serial.print(WWp);
  Serial.print(", ");
  Serial.print(WWBright);
  Serial.print(" BL: ");
  Serial.print(BLp);
  Serial.print(", ");
  Serial.println(BLBright);*/
  
  // Print the Header
  lcd.setCursor(0,0);
  lcd.print("<Time: CW WW BL>");
  
  // Print the Time
  lcd.setCursor(1,1);  // Hours
  if (mins/60 < 10)
    lcd.print("0");
  lcd.print(mins/60);
  lcd.print(":");
  if (mins%60 < 10)  // Minutes
    lcd.print("0");
  lcd.print(mins%60);
  
  // Print the Cool White LED Power Percentage
  lcd.setCursor(7,1);
  if (CWp < 10)
    lcd.print("0");
  lcd.print(CWp); 
  
  // Print the Warm White LED Power Percentage
  lcd.setCursor(10,1);
  if (WWp < 10)
    lcd.print("0");
  lcd.print(WWp);
  
  // Print the Blue LED Power Percentage
  lcd.setCursor(13,1);
  if (BLp < 10)
    lcd.print("0");
  lcd.print(BLp);
  
//  lcd_cursor_loc[0] = 0;
//  lcd.setCursor(lcd_cursor_loc[0],1);
  lcd.cursor();
  //lcd_cursor_loc[0] = tempCursorLoc[0];  // Restore the cursor location after drawing the screen
  //lcd_cursor_loc[1] = tempCursorLoc[1];
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}

// Dim the LCD backlight
void dimLCDbl(int target_lvl) {
  // Counter to slowly dim the backlight
  if (dim_count == 10) {
    lcd_cur_lvl--;
    dim_count = 0;
  }
  
  // Hold brightness at target 
  if (lcd_cur_lvl <= target_lvl) {
    lcd_cur_lvl = target_lvl;
  }
  
  analogWrite(LCDBackLightPin, lcd_cur_lvl);
  dim_count++;
}

