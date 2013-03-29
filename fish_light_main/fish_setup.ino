// This file has the setup function for the fish light project.
// This setup function does all the required initialization and gets run first.

void setup()  {
  Wire.begin();
  Serial.begin(115200);
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
  
  // Initialize the time variables
  readDateDS1307();
  printCurrentTime();  
  targetnewtime = newtime;
  Serial.println("Board Initialized, ready for commands.");
}
