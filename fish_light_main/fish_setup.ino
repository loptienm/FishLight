///////////////////////////////
//
// This file has the setup function for the fish light project.
// This setup function does all the required initialization and gets run first.
//
///////////////////////////////

void setup()  {
  // Initialize I2C
  Wire.begin();
  
  // Initialize Serial for 115200 baud
  Serial.begin(115200);
  
  // Initialize the LCD Screen
  lcd.begin(16, 2);  // Initialize a 16x2 LCD screen
  lcd.createChar(0, backslash);  // setup custom character 0
  lcd.createChar(1, topbar);  // setup custom character 0
  lcd.clear();  // Clear anything that might still be on the LCD screen
  lcd_cursor_loc[0] = 0;  lcd_cursor_loc[1] = 0; // Init the cursor location
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);  // Set the cursor location
  analogWrite(LCDBackLightPin, lcd_use_lvl);  // Turn on the LCD backlight
  
  // Initialize potentiometer readings
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = analogRead(PotPin) * (255.0 / 1023.0);
    total = total + readings[thisReading];
  }
  average = total / numReadings;

  // Set up the Square Wave generator on the RTC
  //byte rtc_data = 0;
  //RtcWrite(0x07, 0xFF);  // Start the RTC square wave generator for 1Hz
  //rtc_data = RtcRead(0x07);
  //Serial.print("RTC data returned: 0x");
  //Serial.println(rtc_data, HEX);
  
  // Initialize the time variables
  readDateDS1307();
  targetnewtime = newtime;
  
  // Set brightness and on/off times for each LED
  s_led.s_cw.on_start[0]  = 8.0 * 60.0;  // 8am * 60 mins
  s_led.s_cw.on_time[0]   = 10.0 * 60.0; // 10am * 60 mins
  s_led.s_cw.on_bright[0] = 255.0;       // 100%
  s_led.s_cw.off_start[0] = 17.0 * 60.0; // 5pm * 60 mins
  s_led.s_cw.off_time[0]  = 19.0 * 60.0; // 7pm * 60 mins
  
  s_led.s_ww.on_start[0]  = 5.0 * 60;    // 5am * 60 mins
  s_led.s_ww.on_time[0]   = 9.0 * 60;    // 9am * 60 mins
  s_led.s_ww.on_bright[0] = 255.0;       // 100%
  s_led.s_ww.off_start[0] = 19.0 * 60.0; // 7pm * 60 mins
  s_led.s_ww.off_time[0]  = 21.0 * 60.0; // 9pm * 60 mins
  
  s_led.s_bl.on_start[0]  = 20.0 * 60.0; // 8pm * 60 mins
  s_led.s_bl.on_time[0]   = 21.0 * 60.0; // 9pm * 60 mins
  s_led.s_bl.on_bright[0] = 3.0;         // 1%
  s_led.s_bl.off_start[0] = 4.0 * 60.0;  // 4am * 60 mins
  s_led.s_bl.off_time[0]  = 5.0 * 60.0;  // 5am * 60 mins
  
  // State that the board is finished initializing
  lcd.clear();
  lcd.print("Board Starting!");
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
  Serial.println("Board Initialized, ready for commands.");
  printCurrentTime();  // Print the current time to the Serial port
  delay(2000);
  
  // Turn on the LCD cursor (looks like an underline)
  // after the lcd has been setup and displays "starting".
  // This is for aesthetics to not put a cursor on "Board Starting"
  lcd.cursor();
  
  // Start the loop!
}
