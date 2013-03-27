// These are functions that setup the debug capability of the fish light.
// The only function that is very useful right now is debug_brightness, but
//  this just allows the user to change the amount of delay between time 
//  steps from 60 seconds to X number of cycles.
// This also has the function to get and process serial commands.

void read_debug_delay() {
  int dly[4];
  int dly_digits;
  int tmp_dig;
  
  for (int i = 0; i <= 3; i++) {
    dly[i] = 0;
  }
  
  Serial.println("Enter number of cycles to delay between timesteps (0-9999):");
  // Wait for next byte to arrive
  while (Serial.available() <= 0){
    delay(10);
  }
  dly_digits = 0;
  while (Serial.available() && dly_digits <= 3) {
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
    
  
  dbgDelay = dly[3] * 1000 + dly[2] * 100 + dly[1] * 10 + dly[0];
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
  Serial.println("t | T - Time: Debug time functions.");
  Serial.println("o | O - Out: Select method of outputting debug messages.");
  Serial.println("w | W - Which: Print which debugging modes are active.");
  Serial.println("a | A - All: Debug all.");*/
}

void debug_time() {  
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

void debug_all() {
  debug_time();  // no functionality yet
  debug_brightness();
  debug_buttons();  // no functionality yet
  debug_lcd();  // no functionality yet
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
  if (! (dbgBright || dbgLcd || dbgButtons || dbgTime))
    Serial.println("No debugging enabled.");
}

void get_serial_command() {
  idle_flag = 0;  // reset idle flag since we got a command
  command = Serial.read();
  if (dbgTime || dbgBright || dbgLcd || dbgButtons) {  // only show what command we got if in debug mode
    Serial.print("Got Command:");
    Serial.println(command);
  }
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
  else if (command == 68 || command == 100) {  // command == 'D' | 'd': debug modes
    if (! Serial.available())
      Serial.println("Debug what?");
    
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
    else if (command == 85 || command == 117) { // command == 'U' | 'u': debug brightness
      debug_buttons();
    }
    else if (command == 76 || command == 108) { // command == 'L' | 'l': debug brightness
      debug_lcd();
    }
    else if (command == 87 || command == 119) { // command == 'W' | 'w': debug brightness
      debug_which();
    }
    else if (command == 65 || command == 97) { // command == 'A' | 'a': debug all
      debug_all();
    }
    else 
      Serial.println("Unknown debug command!");
  }
  else {
    Serial.println("Unknown serial command!");
  }
}

