///////////////////////////////
//
// This file has the setup function for the fish light project.
// This setup function does all the required initialization and gets run first.
//
///////////////////////////////

void setup()  {
  Wire.begin();
  //Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.cursor();
  lcd_cursor_loc[0] = 0;  // Init the cursor location
  lcd_cursor_loc[1] = 0;
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
//  pinMode(A1, OUTPUT);
  analogWrite(LCDBackLightPin, lcd_use_lvl);
  
  // Initialize readings
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = analogRead(PotPin) * (255.0 / 1023.0);
    total = total + readings[thisReading];
  }
  average = total / numReadings;

  byte rtc_data = 0;
  //RtcWrite(0x07, 0xFF);  // Start the RTC square wave generator for 1Hz
  //rtc_data = RtcRead(0x07);
  //Serial.print("RTC data returned: 0x");
  //Serial.println(rtc_data, HEX);
  
  // Initialize the time variables
  readDateDS1307();
  Serial.begin(115200);
  printCurrentTime();  
  targetnewtime = newtime;
  lcd.print("Board Starting...");
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
  Serial.println("Board Initialized, ready for commands.");
  delay(2000);
}
