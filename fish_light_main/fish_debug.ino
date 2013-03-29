// These are functions that setup the debug capability of the fish light.
// The only function that is very useful right now is debug_brightness, but
//  this just allows the user to change the amount of delay between time 
//  steps from 60 seconds to X number of cycles.
// This also has the function to get and process serial commands.

double get_serial_number() {
  double serial_number = 0;
  double dly[num_digs];
  int dly_digits = 0;
  int tmp_dig = 0;
  
  for (int i = 0; i <= num_digs - 1; i++) {
    dly[i] = 0;
  }

  // Wait for next byte to arrive
  while (Serial.available() <= 0){
    delay(10);
  }
  dly_digits = 0;
  while (Serial.available() && dly_digits < num_digs) {
    tmp_dig = Serial.read();
    if (tmp_dig >= 48 && tmp_dig <= 57) {
      // Shift values over before saving new ones
      for (int i = dly_digits; i > 0; i--) {
        dly[i] = dly[i - 1];
      }
      dly[0] = tmp_dig - 48;
      dly_digits++;
    }
  }
  
  for (int i = 0; i < num_digs; i++) {
    serial_number = serial_number + dly[i] * pow(10, i);
  }
  return serial_number;
}

void read_debug_delay() {
  dbgDelay = 0;  // Clear before doing math
  Serial.println("Enter number of cycles to delay between timesteps (0-9999):");
  dbgDelay = get_serial_number();
  Serial.print("Delaying ");
  Serial.print(dbgDelay);
  Serial.println(" cycles (not seconds) between timesteps.");
}

// Debug functions
void debug_help() {
/*  THIS FUNCTION IS CAUSING THE BOARD TO NOT PROGRAM RIGHT.
    NEED TO FIGURE THIS OUT BEFORE I CAN USE THIS FUNCTION.
    IS IT BECAUSE ITS GETTING TOO BIG (~18000B) EVEN THO MAX CLAIMS 32256B?
  Serial.println("Debugging help message:");
  Serial.println("h | H - Helpmessage: Print this message.");
  Serial.println("b | B - Brightness: Debug Brightness functions.");
  Serial.println("u | U - Buttons: Debug Button functions.");
  Serial.println("l | L - LCD: Debug LCD functions.");
  Serial.println("s | S - Serial: Debug Serial functions.");
  Serial.println("t | T - Time: Debug time functions.");
  Serial.println("o | O - Out: Select method of outputting debug messages.");
  Serial.println("w | W - Which: Print which debugging modes are active.");
  Serial.println("a | A - All: Debug all.");
  Serial.println("n | N - None: Debug none.");*/
}

/*void debug_time() {  
  if (dbgTime) {  // If already debugging, turn it off
    Serial.println("Disabling time debug.");
    dbgTime = 0;
  }
  else {          // If not already debugging, turn it on
    dbgTime = 1;
    Serial.println("Enabling time debug. (NOT FUNCTIONAL)");
  }
}

void debug_brightness() {
  if (dbgBright){  // If already debugging, turn it off
    dbgBright = 0;
    Serial.println("Disabling brightness debug.");
  }
  else {           // If not already debugging, turn it on
    dbgBright = 1; // Set the dbgBright flag to use the counter instead of RTC to generate time
    dbg_cnt = 0;  // Reset the dbg_cnt counter used in the main loop()
    newtime = 0;  // Reset the time to start debugging
    Serial.println("Enabling brightness debug.");
    read_debug_delay();
  }
}

void debug_buttons() {  
  if (dbgButtons) {  // If already debugging, turn it off
    Serial.println("Disabling button debug.");
    dbgButtons = 0;
  }
  else {             // If not already debugging, turn it on
    dbgButtons = 1;
    Serial.println("Enabling button debug. (NOT FUNCTIONAL)");
  }
}

void debug_lcd() {  
  if (dbgLcd) {  // If already debugging, turn it off
    Serial.println("Disabling LCD debug.");
    dbgLcd = 0;
  }
  else {         // If not already debugging, turn it on
    dbgLcd = 1;
    Serial.println("Enabling LCD debug. (NOT FUNCTIONAL)");
  }
}

void debug_serial() {
  if (dbgSerial) {  // If already debugging, turn it off
    Serial.println("Disabling serial debug.");
    dbgSerial = 0;
  }
  else {         // If not already debugging, turn it on
    dbgSerial = 1;
    Serial.println("Enabling Serial debug. (NOT FUNCTIONAL)");
  }
}

void debug_all() {
  debug_time();  // no functionality yet
  debug_brightness();
  debug_buttons();  // no functionality yet
  debug_lcd();  // no functionality yet
  //debug_serial();
}

void debug_none() {
  dbgTime = 0;
  dbgBright = 0;
  dbg_cnt = 0;
  dbgButtons = 0;
  dbgLcd = 0;
  dbgSerial = 0;
}

void debug_which() {
  if (dbgTime)
    Serial.println("Time debug enabled.");
  if (dbgButtons)
    Serial.println("Buttons debug enabled.");
  if (dbgLcd)
    Serial.println("LCD debug enabled.");
  if (dbgBright)
    Serial.println("Brightness debug enabled.");
  if (dbgSerial)
    Serial.println("Serial debug enabled.");
  if (! (dbgBright || dbgLcd || dbgButtons || dbgTime || dbgSerial))
    Serial.println("No debugging enabled.");
}*/

void get_serial_command() {
  idle_flag = 0;  // reset idle flag since we got a command
  command = Serial.read();
  byte addr = 0;  // RTC address
  byte data = 0;  // RTC data
  //if (dbgTime || dbgBright || dbgLcd || dbgButtons || dbgSerial) {  // only show what command we got if in debug mode
    Serial.print("Got Command:");
    Serial.println(command);
  //}
  if (command == 84 || command == 116) {    // command == 'T' | 't': set date
    setTimeSerial();
    Serial.print("Time is set to : ");
    readDateDS1307();
    printCurrentTime();
  }
  else if (command == 71 || command == 103) {  // command == 'G' | 'g': get date
    Serial.print("Current Time is : ");
    printCurrentTime();
  }
  else if (command == 119 || command == 87) {  // command == 'W' | 'w': write RTC register
    Serial.println("Enter an address to write to:");
    addr = get_serial_number();
    Serial.println("Enter the data to write:");
    data = get_serial_number();
    RtcWrite(addr, data);
  }
  else if (command == 114 || command == 82) {  // command == 'R' | 'r': read RTC register
    Serial.println("Enter an address to read:");
    addr = get_serial_number();
    data = RtcRead(addr);
    Serial.print("RTC data in register ");
    Serial.print(addr);
    Serial.print(" = ");
    Serial.print(data);
    Serial.println(".");
  }
  /*else if (command == 68 || command == 100) {  // command == 'D' | 'd': debug modes
    if (! Serial.available())
      Serial.println("Debug what?");

    if (! Serial.available())
      //Serial.println("help message goes here");
      debug_help();  // print help message for debug commands, start over
    
    // Wait for next byte to arrive
    while (Serial.available() <= 0){
      delay(10);
    }
    command = Serial.read();
    
    if (command == 72 || command == 104) {  // command == 'H' | 'h': help message
      debug_help();  // print help message for debug commands, start over
    }
    else if (command == 84 || command == 116) {  // command == 'T' | 't': debug time
      debug_time();
    }
    else if (command == 66 || command == 98) { // command == 'B' | 'b': debug brightness
      debug_brightness();
    }
    else if (command == 85 || command == 117) { // command == 'U' | 'u': debug buttons
      debug_buttons();
    }
    else if (command == 76 || command == 108) { // command == 'L' | 'l': debug lcd
      debug_lcd();
    }
    else if (command == 83 || command == 115) { // command == 'S' | 's': debug serial
      //debug_serial();
    }
    else if (command == 87 || command == 119) { // command == 'W' | 'w': debug which
      debug_which();
    }
    else if (command == 65 || command == 97) { // command == 'A' | 'a': debug all
      debug_all();
    }
    else if (command == 78 || command == 110) { // command == 'N' | 'n': debug none
      debug_none();
    }
    else 
      Serial.println("Unknown debug command!");
  }*/
  else {
    Serial.println("Unknown serial command!");
  }
  command = 0;  // Reset the Serial command holder
}

