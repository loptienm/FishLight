///////////////////////////////
//
// Button functions
//
///////////////////////////////

// Function to decide what to do if there is a button press
void getButtonFunction() {
  getButtonState();
  if (currKey == KEY_RIGHT && (keyPressed || (keyHeld && keyHeldCnt > keyHeldDur && keyIntvlCnt == keyHeldIntvl)))
    if (setting_value) {
      moveCursorRight();
    } else {
      moveCursorRight();
    }
  else if (currKey == KEY_LEFT && (keyPressed || (keyHeld && keyHeldCnt > keyHeldDur && keyIntvlCnt == keyHeldIntvl)))
    if (setting_value) {
      moveCursorLeft();
    } else {
      moveCursorLeft();
    }
  else if (currKey == KEY_UP && (keyPressed || (keyHeld && keyHeldCnt > keyHeldDur && keyIntvlCnt == keyHeldIntvl)))
    if (setting_value) {
      changeTimeDig(lcd_cursor_loc[0], 1);  // Change value up
    } else {
      moveCursorUp();
    }
  else if (currKey == KEY_DOWN && (keyPressed || (keyHeld && keyHeldCnt > keyHeldDur && keyIntvlCnt == keyHeldIntvl)))
    if (setting_value) {
      changeTimeDig(lcd_cursor_loc[0], 0);  // Change value down
    } else {
      moveCursorDown();
    }
  else if (currKey == KEY_CENT && (keyPressed || (keyHeld && keyHeldCnt > keyHeldDur && keyIntvlCnt == keyHeldIntvl))) {
    if (lcd_cursor_loc[1] == 1 && (lcd_cursor_loc[0] >= 1 && lcd_cursor_loc[0] <= 5)) {
      setting_value = 1;  // Set value if it is inside valid time area
      lcd.blink();
      setTimeButtons();
    } else if (setting_value && lcd_cursor_loc[0] == 12 && lcd_cursor_loc[1] == 1) {  // Clicked yes to save time
      lcd.noBlink();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Saving Time...");
      checkTime();
      writeDateDS1307();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Time Saved!");
      delay (1000);
      MainScreenLCD(newtime, CWBright*100.0/255.0, WWBright*100.0/255.0, BLBright*100.0/255.0);
      lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
    } else {
      setting_value = 0;
      lcd.noBlink();
      MainScreenLCD(newtime, CWBright*100.0/255.0, WWBright*100.0/255.0, BLBright*100.0/255.0);
    }
  }
  //Serial.print("lcd_cursor_loc[0] = ");
  //Serial.print(lcd_cursor_loc[0]);
  //Serial.print(" lcd_cursor_loc[1] = ");
  //Serial.println(lcd_cursor_loc[1]);
}

// Function to get the state of an analog-read button ladder
void getButtonState () {
  int buttonValue = analogRead(buttonPin);  // Get the new button press
  
  // Get which button was pressed based on the analog value of the button pin
  if (buttonValue >= 800 && buttonValue <= 850)  // Nominal @ 820
    currKey = KEY_CENT;
  else if (buttonValue >=740  && buttonValue <= 790)  // Nominal @ 770
    currKey = KEY_LEFT;
  else if (buttonValue >= 650 && buttonValue <= 710)  // Nominal @ 680
    currKey = KEY_UP;
  else if (buttonValue >= 480 && buttonValue <= 540)  // Nominal @ 512
    currKey = KEY_RIGHT;
  else if (buttonValue >= 0 && buttonValue <= 60)  // Nominal @ 0
    currKey = KEY_DOWN;
  else
    currKey = KEY_NONE;
    
  //Serial.print("debounce_en = ");
  //Serial.print(debounce_en);
  //Serial.print(" debounce_cnt = ");
  //Serial.print(debounce_cnt);
  //Serial.print(" currKey = ");
  //Serial.print(currKey);
  //Serial.print(" prevkey = ");
  //Serial.println(prevKey);
  
  if (currKey != prevKey) {  // New value so do debounce stuff
    debounce_en = 1;
  } else {
    debounce_en = 0;
  }
  if (debounce_en || keyHeld) {
    debounce_cnt++;
  } else {
    debounce_cnt = 0;
  }
  if (debounce_cnt > debounce_delay) {
    debounce_cnt = debounce_delay + 1;
  } else {  // if debounce_cnt < debounce_delay
    currKey = prevKey;
  }  // if debounce_cnt > debounce_delay
    
//    Serial.print("Button Value = ");
//    Serial.println(buttonValue);
    
    // If a button is pressed, increment the counter until its released
    if (currKey != KEY_NONE) {
      keyHeldCnt++;
      keyIntvlCnt++;
      if (keyIntvlCnt > keyHeldIntvl) {  // Rollover interval counter after the long count
        keyIntvlCnt = 0;
        keyHeldStrokeCnt++;
      }
      if (keyHeldStrokeCnt > keyHeldStrokeLimit) {  // After a few cursor updates at the long count, start updating faster
        keyHeldIntvl = short_intvl;
        keyHeldStrokeCnt = keyHeldStrokeLimit + 1;
      } else {
        keyHeldIntvl = long_intvl;
      }
      if (keyHeldCnt > keyHeldDur)     // Hold keyHeldCnt at keyHeldDur + 1 to keep from rolling over
        keyHeldCnt = keyHeldDur + 1;
    } else {
      keyHeldCnt = 0;
      keyIntvlCnt = 0;
      keyHeldStrokeCnt = 0;
    }
  
    //Serial.print(" keyHeldCnt = ");
    //Serial.print(keyHeldCnt);
    //Serial.print(" keyIntvlCnt = ");
    //Serial.println(keyIntvlCnt);
    
    // Set the button status flags based on button events
    if (prevKey == KEY_NONE && currKey != KEY_NONE) {
      keyPressed = 1;
      keyReleased = 0;
      keyHeld = 0;
      idle_flag = 0;     // Reset the lcd back light dimming idle flag
      //Serial.print("Key Pressed:  ");
      //Serial.println(currKey);
    } else if (prevKey != KEY_NONE && currKey == KEY_NONE) {
      keyPressed = 0;
      keyReleased = 1;
      keyHeld = 0;
      idle_flag = 0;     // Reset the lcd back light dimming idle flag
      //Serial.print("Key Released: ");
      //Serial.println(prevKey);
    } else if ((prevKey == currKey) && (keyPressed || keyHeld)) {
      //Serial.print("Key Held:     ");
      //Serial.println(currKey);
      keyPressed = 0;
      keyReleased = 0;
      keyHeld = 1;
      idle_flag = 0;     // Reset the lcd back light dimming idle flag
    } else if ((prevKey == currKey) && keyReleased) {
      //Serial.print("Keys Idle:    ");
      //Serial.println(currKey);
      keyPressed = 0;
      keyReleased = 0;
      keyHeld = 0;
    }
  
  prevKey = currKey;  // Save the previous buttons press
}

void setTimeButtons() {
  lcd.clear();
  //Print header
  lcd.setCursor(0,0);
  lcd.print("Time:");
  
  // Print save command
  lcd.setCursor(10,0);
  lcd.print("Save?");
  lcd.setCursor(12,1);
  lcd.print("Y/N");  // Yes or No to save time to the DS1307
  
  // Print time
  lcd.setCursor(0,1);
  if (newtime/60 < 10)
    lcd.print("0");
  lcd.print(newtime/60);
  lcd.print(":");
  if (newtime%60 < 10)  // Minutes
    lcd.print("0");
  lcd.print(newtime%60);
  
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
  
}
