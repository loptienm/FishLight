// This file holds the fucntions to calculate the time, program the RTC time,
// and read the RTC to keep accurate time.

///////////////////////////////
//
// Time functions
//
///////////////////////////////
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val) {
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val) {
  return ( (val/16*10) + (val%16) );
}

void changeTimeDig(int location, int up_downb) {
  // mytime ([12:11] month, [10:9] dayofMonth, [8:7] year, [6] dayOfWeek,
  //         [5:4] hour, [3:2] minute, [1:0] second)
  int dig = 0;
  //Serial.print("In changeTimeDig, location = ");
  Serial.print(location);
  switch (location) {
    case 0: dig = 5; break;
    case 1: dig = 4; break;
    case 3: dig = 3; break;
    case 4: dig = 2; break;
    default: return;
  }
  //Serial.print(", dig = ");
  //Serial.println(dig);
  
  // Set new time
  if (up_downb) {
    mytime[dig]++;
  } else if (!up_downb) {
    mytime[dig]--;
  }
  
  // Print the new value
  checkTimeDigs();
  lcd.print(mytime[dig]);
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}

// Change time from array of digits to numerical values for each place (hours, minutes, seconds, etc.)
void calcTimeParams() { 
  month      = (byte) (mytime[12] * 10 + mytime[11]);
  dayOfMonth = (byte) (mytime[10] * 10 + mytime[9]);
  year       = (byte) (mytime[8] * 10 + mytime[7]);
  dayOfWeek  = (byte) (mytime[6]);
  hour       = (byte) (mytime[5] * 10 + mytime[4]);
  minute     = (byte) (mytime[3] * 10 + mytime[2]);
  second     = (byte) (mytime[1] * 10 + mytime[0]); 
  //newtime    = (hour * 60) + minute;
}

// Check to make sure programmed time is in correct time boundaries (24 hr clock)
void checkTime() {
  calcTimeParams();
  if (month > 12 || month <= 0) {
    mytime[12] = 0;
    mytime[11] = 1;
  }
  switch (month) {
    case 1: if (dayOfMonth > 31 || dayOfMonth <= 0) { mytime[10] = 0; mytime[9] = 1; }; break;  // Jan
    case 2: if (dayOfMonth > 28 || dayOfMonth <= 0) { mytime[10] = 0; mytime[9] = 1; }; break;  // Feb
    case 3: if (dayOfMonth > 31 || dayOfMonth <= 0) { mytime[10] = 0; mytime[9] = 1; }; break;  // Mar
    case 4: if (dayOfMonth > 30 || dayOfMonth <= 0) { mytime[10] = 0; mytime[9] = 1; }; break;  // Apr
    case 5: if (dayOfMonth > 31 || dayOfMonth <= 0) { mytime[10] = 0; mytime[9] = 1; }; break;  // May
    case 6: if (dayOfMonth > 30 || dayOfMonth <= 0) { mytime[10] = 0; mytime[9] = 1; }; break;  // Jun
    case 7: if (dayOfMonth > 31 || dayOfMonth <= 0) { mytime[10] = 0; mytime[9] = 1; }; break;  // Jul
    case 8: if (dayOfMonth > 31 || dayOfMonth <= 0) { mytime[10] = 0; mytime[9] = 1; }; break;  // Aug
    case 9: if (dayOfMonth > 30 || dayOfMonth <= 0) { mytime[10] = 0; mytime[9] = 1; }; break;  // Sep
    case 10: if (dayOfMonth > 31 || dayOfMonth <= 0) { mytime[10] = 0; mytime[9] = 1; }; break; // Oct
    case 11: if (dayOfMonth > 30 || dayOfMonth <= 0) { mytime[10] = 0; mytime[9] = 1; }; break; // Nov
    case 12: if (dayOfMonth > 31 || dayOfMonth <= 0) { mytime[10] = 0; mytime[9] = 1; }; break; // Dec
  }
  if (year > 99 || year < 0) {
    mytime[8] = 0;
    mytime[7] = 0;
  }
  if (dayOfWeek > 7 || dayOfWeek < 1)
    mytime[6] = 1;
  if (hour > 24 || hour < 0) {
    mytime[5] = 0;
    mytime[4] = 0;
  }
  if (minute > 59 || minute < 0) {
    mytime[3] = 0;
    mytime[2] = 0;
  }
  if (second > 59 || second < 0) {
    mytime[1] = 0;
    mytime[0] = 0;
  }
  calcTimeParams();  // recalc the time params just incase something was changed
}

// Check each time digit for rollover (only useful when using buttons to set time)
void checkTimeDigs() {
  // Time checking for upper bound rollover (ex: 32 days in a month)
  if (mytime[12] > 1)  // Month
    mytime[12] = 0;
  if (mytime[11] > 9)
    mytime[11] = 0;
  if (mytime[10] > 3)  // DayofMonth
    mytime[10] = 0;
  if (mytime[9] > 9)
    mytime[9] = 0;
  if (mytime[8] > 9)  // Year
    mytime[8] = 0;
  if (mytime[7] > 9)
    mytime[7] = 0;
  if (mytime[6] > 7)  // DayofWeek
    mytime[6] = 0;
  if (mytime[5] > 2)  // Hour
    mytime[5] = 0;
  if (mytime[4] > 9)
    mytime[4] = 0;
  if (mytime[3] > 5)  // Minute
    mytime[3] = 0;
  if (mytime[2] > 9)
    mytime[2] = 0;
  if (mytime[1] > 5)  // Second
    mytime[1] = 0;
  if (mytime[0] > 9)
    mytime[0] = 0;
    
  // Time checking for lower bound rollover (ex: -3 days in a month)
  if (mytime[12] < 0)  // Month
    mytime[12] = 1;
  if (mytime[11] < 0)
    mytime[11] = 9;
  if (mytime[10] < 0)  // DayofMonth
    mytime[10] = 3;
  if (mytime[9] < 0)
    mytime[9] = 9;
  if (mytime[8] < 0)  // Year
    mytime[8] = 9;
  if (mytime[7] < 0)
    mytime[7] = 9;
  if (mytime[6] < 0)  // DayofWeek
    mytime[6] = 7;
  if (mytime[5] < 0)  // Hour
    mytime[5] = 2;
  if (mytime[4] < 0)
    mytime[4] = 9;
  if (mytime[3] < 0)  // Minute
    mytime[3] = 5;
  if (mytime[2] < 0)
    mytime[2] = 9;
  if (mytime[1] < 0)  // Second
    mytime[1] = 5;
  if (mytime[0] < 0)
    mytime[0] = 9;
}

// 1) Sets the date and time on the ds1307 via SERIAL ONLY
// 2) Starts the clock
// 3) Sets hour mode to 24 hour clock
// Format: T(01-12}(01-31)(00-99)(1-7)(00-23)(00-59)(00-59)  (must use leading 0's: 01:59:08)
//           MO     DoM    YR     DoW  Hr     Min    Sec
//                  Day           Day
//                  of            of
//                  Month         Week (Sun(1) - Sat(7))
void setTimeSerial() {
  int incomingByte = 0;
  int numDigs = 0;         // Number of digits I have gotten from Serial so far
  int i = 0;               // temp placeholder variable
  byte time_error = 0;     // Time setting error flag
  
  // Init array
  for (i = 0; i <= totalDigs; i++) {
    mytime[i] = 0;
  }
  
  Serial.println("Setting the time, please use leading 0s.");
  for (i = 0; i <= totalDigs; i++) {
    switch (numDigs) {
      case 0: Serial.print("Enter the Month: "); break;
      case 2: Serial.println(""); Serial.print("Enter the Date: "); break;
      case 4: Serial.println(""); Serial.print("Enter the Year: "); break;
      case 6: Serial.println(""); Serial.print("Enter the Day of Week (Sun = 1, Sat = 7): "); break;
      case 7: Serial.println(""); Serial.print("Enter the Hour: "); break;
      case 9: Serial.println(""); Serial.print("Enter the Minute: "); break;
      case 11: Serial.println(""); Serial.print("Enter the Second: "); break;
      default: break;
    }
      
    // Wait for next byte to arrive
    while (Serial.available() <= 0){
      delay(10);
    }
    // read the incoming byte:
    incomingByte = Serial.read();
    
    if (incomingByte >= 48 && incomingByte <= 57) {
      mytime[totalDigs - numDigs] = incomingByte - 48;
      Serial.print(mytime[totalDigs - numDigs]);
    }
    if (numDigs == 6) {
      switch (mytime[totalDigs - numDigs]) {
        case 1: Serial.print(" (Sun)"); break;
        case 2: Serial.print(" (Mon)"); break;
        case 3: Serial.print(" (Tue)"); break;
        case 4: Serial.print(" (Wed)"); break;
        case 5: Serial.print(" (Thu)"); break;
        case 6: Serial.print(" (Fri)"); break;
        case 7: Serial.print(" (Sat)"); break;
        default: Serial.println(""); Serial.print("Error: Invalid day value ("); 
          Serial.print(mytime[totalDigs - numDigs]);
          Serial.print("), try again.");
          numDigs = 5;  // Set to 5 so that incrementer (3 lines lower) will increment to 6 which we want to retry
      }
    }
    numDigs++;
  }
  Serial.println("");
  
  // Check that the programmed time is valid
  // This will call calcTimeParams first before checking time
  checkTime();
  
  // Write the time to the RTC
  writeDateDS1307();  
}

// This function writes the currently programmed time values
// to the DS1307 RTC.  There is no value checking.
void writeDateDS1307() {
  int myerr = 0;
  
  Wire.beginTransmission(clockAddress);
  Wire.write(byte(0x00));  // 0 to bit 7 starts the clock
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));    // If you want 12 hour am/pm you need to set bit 6 (also need to change readDateDS1307)
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  myerr = Wire.endTransmission(1);
  if (myerr) {
    Serial.print("Ended transmission with err:");
    Serial.println(myerr);
    Serial.println("0: Success");
    Serial.println("1: Data too long to fit in transmit buffer");
    Serial.println("2: Received NACK on transmit of address");
    Serial.println("3: Received NACK on transmit of data");
    Serial.println("4: Other error");
  }
}

// Gets the date and time from the DS1307
void readDateDS1307() {
  // Reset the register pointer
  Wire.beginTransmission(clockAddress);
  Wire.write(byte(0x00));
  Wire.endTransmission();

  Wire.requestFrom(clockAddress, 7);

  // A few of these need masks because certain bits are control bits
  second     = bcdToDec(Wire.read() & 0x7f);
  minute     = bcdToDec(Wire.read());
  hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  dayOfWeek  = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month      = bcdToDec(Wire.read());
  year       = bcdToDec(Wire.read());
  
  newtime = (hour * 60) + minute;
}

void printCurrentTime() {  
  Serial.print("(newtime = ");
  Serial.print(newtime);
  Serial.print(")");
  Serial.print("  ");
  if (hour < 10)
    Serial.print("0");
  Serial.print(hour, DEC);
  Serial.print(":");
  if (minute < 10)
    Serial.print("0");
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second < 10)
    Serial.print("0");
  Serial.print(second, DEC);
  Serial.print("  ");
  if (month < 10)
    Serial.print("0");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(dayOfMonth, DEC);
  if (dayOfMonth < 10)
    Serial.print("0");
  Serial.print("/");
  if (year < 10)
    Serial.print("0");
  Serial.println(year, DEC);
}

void getClkStatus() {
  curr_clk = digitalRead(clkPin);
  Serial.print("CLK pin = ");
  Serial.println(curr_clk);

  if (curr_clk == 1 && prev_clk == 0) {  // Rising Edge
    clk_rise = 1;
    clk_high = 1;
    clk_fall = 0;
    clk_low = 0;
    Serial.println("CLK Posedge");
  } else if (curr_clk == 1 && prev_clk == 1) {  // CLK high
    clk_rise = 0;
    clk_high = 1;
    clk_fall = 0;
    clk_low = 0;
    Serial.println("CLK High");
  } else if (curr_clk == 0 && prev_clk == 1) {  // Falling Edge
    clk_rise = 0;
    clk_high = 0;
    clk_fall = 1;
    clk_low = 1;
    Serial.println("CLK Negedge");
  } else if (curr_clk == 0 && prev_clk == 0) {  // CLK low
    clk_rise = 0;
    clk_high = 0;
    clk_fall = 0;
    clk_low = 1;
    Serial.println("CLK Low");
  } else {
    Serial.print("Can't read CLK pin ");
    Serial.println(clkPin);
  }
  prev_clk = curr_clk;
}
