// This file holds the loop function for the fish light project.
// This loop function runs in a continuous loop (hence the name) while
// the Arduino is powered on.  One loop does not correspond to a second
// and the time length of a loop completely finishing is dependent on
// what things happen in it (such as serial printing or button scanning).

void loop() {
  if (Serial.available()) {
    get_serial_command();
  }
  
  // Sample Buttons
  getButtonFunction();
  
  // Sample CLK
  //getClkStatus();
  
  // Get the time from RTC
  if (dbgBright) {  // Use debug time instead of actual time
    if (dbg_cnt >= dbgDelay) {
      newtime++;
      dbg_cnt = 0;
    }
    dbg_cnt++;
    if (newtime > 1440)
      newtime = 0;
  }
  else {            // Use the actual time
    readDateDS1307();
  }
  
  // Dont do stuff if we are setting the time with buttons
  if (!setting_value){
    // Get the brightness as defined by the silver potentiometer
    getmaxbrightness();
  
    // Get the LED brightness at the current time
    BLBright = blledsched(newtime);
    CWBright = cwledsched(newtime);
    WWBright = wwledsched(newtime);
  
    analogWrite(WWledPin, WWBright);
    analogWrite(CWledPin, CWBright);
    analogWrite(BLledPin, BLBright);

    // Redraw LCD only if we need to
    if (newtime == targetnewtime || idle_flag == 0) {
      MainScreenLCD(newtime, CWBright*100.0/255.0, WWBright*100.0/255.0, BLBright*100.0/255.0);
    }
  
    // Set the target time to be the next minute
    // This makes the lcd only redraw once a minute unless something else happens
    // This must be after the redraw so that the if condition above is true each minute
    targetnewtime = newtime + 1;
  
    //Serial.print("Cursor at position ");
    //Serial.println(lcd_cursor_loc[0]);
  
    // Get the status of idle_flag and dim_count
    // So that we can set the brightness of the LCD Back Light
    getLCDblStatus();
  
    //  delay (10);
    command = 0;  // Reset the Serial command holder
  }
}

