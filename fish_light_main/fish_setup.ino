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
