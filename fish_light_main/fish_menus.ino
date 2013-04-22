///////////////////////////////
//
// This file holds the functions for the Fish LCD Menus.
// When the middle button is pressed, these are the
// functions that will be called.
//
///////////////////////////////

void get_lcd_menu() {
  
}

/* Peaks example:
 ----------------
|Peaks:          |
|  0  ___________|
 ----------------
 ----------------
|Peaks: /-----\  |
|  1  _/       \_|
 ----------------
 0123456789012345  <- lcd character position (0 - 15)
 ----------------
|Peaks: /\   /\  |
|  2  _/  \_/  \_|
 ----------------
*/
void set_brightness_buttons() {
  int numPeaks = 1;  // Assume at least one peak (though 0 is valid)

  print_brightness_peaks_lcd(numPeaks);
}

void print_brightness_peaks_lcd(int numPeaks) {
  if (numPeaks == 0) {
    lcd.clear();
    lcd.setCursor(0,1); lcd.print("Peaks:          ");
    lcd.setCursor(0,0); lcd.print("  0  ___________");
  } else if (numPeaks == 1) {
    lcd.clear();
    lcd.setCursor(0,1); lcd.print('Peaks: /-----\  ');
    lcd.setCursor(0,0); lcd.print('  1  _/       \_');
  } else if (numPeaks == 2) {
    lcd.clear();
    lcd.setCursor(0,1); lcd.print('Peaks: /\   /\  ');
    lcd.setCursor(0,0); lcd.print('  2  _/  \_/  \_');
  } else if (numPeaks < 0 || numPeaks > 2) {
    lcd.clear();
    lcd.print("ERR: Bad # Peaks");
  }
}

void setTimeButtons() {
  setting_value = 1;  // Flag that we are setting something with the buttons
  lcd.blink();
  readDateDS1307();
  calcTimeDigs();
  // Initialize the temporary time holder
  for (int i = 0; i < totalDigs; i++) {
    mytmptime[i] = mytime[i];
  }
  printTimeSetButtons();
  
}

void printTimeSetButtons() {
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

void saveTimeButtons() {
  setting_value = 0;
  for (int i = 0; i < totalDigs; i++) {
    mytime[i] = mytmptime[i];
  }
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving Time...");
  checkTime();
  writeDateDS1307();
  calcTimeParams();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Time Saved!");
  delay (1000);
  MainScreenLCD(newtime, CWBright*100.0/255.0, WWBright*100.0/255.0, BLBright*100.0/255.0);
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}
