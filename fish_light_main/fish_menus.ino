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
void set_brightness_buttons(int led_string) {
  //setting_bright = 1;
  int numPeaks = 0;  // Assume at least one peak (though 0 is valid)

  print_brightness_peaks_lcd(numPeaks);
  delay(2000);
  print_brightness_peaks_lcd(numPeaks + 1);
  delay(2000);
  print_brightness_peaks_lcd(numPeaks + 2);
  delay(2000);
}

void print_brightness_peaks_lcd(int numPeaks) {
  lcd.clear();
  if (numPeaks == 0) {
    lcd.setCursor(0,0); lcd.print("Peaks:          ");
    lcd.setCursor(0,1); lcd.print("  0  ___________");
  } else if (numPeaks == 1) {
    lcd.setCursor(0,0); lcd.print("Peaks: /-----");
    lcd.write(byte(0));  // '\'
    lcd.setCursor(0,1); lcd.print("  1  _/       ");
    lcd.write(byte(0));  // '\'
    lcd.print("_");
  } else if (numPeaks == 2) {
    lcd.setCursor(0,0); lcd.print("Peaks: /");
    lcd.write(byte(0));  // '\'
    lcd.print("   /");
    lcd.write(byte(0));  // '\'
    lcd.setCursor(0,1); lcd.print("  2  _/  ");
    lcd.write(byte(0));
    lcd.print("_/  ");
    lcd.write(byte(0));
    lcd.print("_");
  } else if (numPeaks < 0 || numPeaks > 2) {
    lcd.print("ERR: Bad # Peaks");
  }
}

void setTimeButtons() {
  setting_time = 1;  // Flag that we are setting something with the buttons
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
  lcd.setCursor(1,1);
  lcd.print(mytmptime[5]);
  lcd.print(mytmptime[4]);
  lcd.print(":");
  lcd.print(mytmptime[3]);
  lcd.print(mytmptime[2]);

  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}

void saveTimeButtons() {
  setting_time = 0;
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(0,0);
  
  // If clicked No to not save
  if (lcd_cursor_loc[0] == 14) {
    lcd.print("Not Saving Time");
  } else {
    lcd.print("Saving Time...");
    for (int i = 0; i < totalDigs; i++) {
      mytime[i] = mytmptime[i];
    }
    checkTime();
    writeDateDS1307();
    calcTimeParams();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time Saved!");
  }
  
  delay (1000);
  MainScreenLCD(newtime, CWBright*100.0/255.0, WWBright*100.0/255.0, BLBright*100.0/255.0);
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}
