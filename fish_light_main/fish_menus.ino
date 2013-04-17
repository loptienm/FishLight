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
    lcd.setCursor(0,1); lcd.print("Peaks: /-----\  ");
    lcd.setCursor(0,0); lcd.print("  1  _/       \_");
  } else if (numPeaks == 2) {
    lcd.clear();
    lcd.setCursor(0,1); lcd.print("Peaks: /\   /\  ");
    lcd.setCursor(0,0); lcd.print("  2  _/  \_/  \_");
  } else if (numPeaks < 0 || numPeaks > 2) {
    lcd.clear();
    lcd.print("ERR: Bad # Peaks");
  }
}


