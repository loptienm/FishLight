// This is the root file for the fish light project and 
// it only contains the global variables.  There is no 
// logic in this file at all.  Each block of code has been 
// placed into its corresponding <fish_*> file to make it 
// easier to maintain.

#include <Wire.h>
#include <LiquidCrystal.h>

//////////////////////////
// Global vars for pins
const int BLledPin  = 10;        // Blue LED connected to digital pin 9
const int CWledPin  = 11;        // Cool White LED connected to digital pin 10
const int WWledPin  = 9;         // Warm White LED connected to digital pin 11
const int PotPin    = A0;        // Silver Potentiometer is on pin A0
const int LCDBackLightPin = 6;   // PWM for LCD backlight
const int buttonPin = A1;        // The pushbutton array is on pin A1
//const int clkPin    = 8;         // Pin tied to RTC clk

// LCD pins
// LiquidCrystal lcd(8, 7, 5, 4, 3, 2);
const int LCD1Pin = 8;
const int LCD2Pin = 7;
const int LCD3Pin = 5;
const int LCD4Pin = 4;
const int LCD5Pin = 3;
const int LCD6Pin = 2;
//////////////////////////


//////////////////////////
// Global vars for buttons
int currKey = 0;                 // The state of the button that is most recently pressed
int prevKey = 0;                 // The state of the button that was last pressed
int keyPressed = 0;              // Tells if a key is pressed, no keys are pressed initially
int keyReleased = 1;             // Tells if a key is released, no keys are pressed initially
int keyHeld = 0;                 // Tells if a key is being held, no keys are pressed initially
int keyHeldCnt = 0;              // Counter which counts cycles since button was pressed
int keyHeldDur = 700;            // Value to compare with to see how long a button has been held
int keyIntvlCnt = 0;             // Counter which counts cycles to determine how often the cursor gets moved
int keyHeldIntvl = 0;            // Holds the desired interval to count up to 
int long_intvl = 350;            // Value to count to in order to delay between changing the cursor location while a button is held
int short_intvl = 175;           // Value to count to in order to delay between changing the cursor location while a button is held
int keyHeldStrokeCnt = 0;        // Counts the number of cursor updates while a button is held. After KeyHeldStrokeLimit, it will count faster
int keyHeldStrokeLimit = 8;      // Value to count to for number of cursor updates while a button is held
int debounce_cnt = 0;            // Counter to count from a button value change up to the debounce_delay to debounce buttons
int debounce_delay = 35;         // Value to count up to once a button has been pressed.  If the button value is the same, its a true button press
int debounce_en = 0;             // Enable the button debouncing counter
int setting_value = 0;           // Flag to tell buttons to either move cursor or set a number value
int dbgButtons = 0;              // Flag to debug button functions
//////////////////////////


//////////////////////////
// Global vars for Time
int clockAddress = 0x68;     // This is the I2C address
int command = 0;             // This is the command char, in ascii form, sent from the serial port     
long previousMillis = 0;     // Will store last time Temp was updated
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
const int totalDigs = 12;    // Number of digits needed to hold time
int mytime[totalDigs];       // My time holder ([12:11] month, [10:9] dayofMonth, [8:7] year, [6] dayOfWeek,
                             //                 [5:4] hour, [3:2] minute, [1:0] second)
int newtime = 0;             // My calculated time to pass to LED schedule tasks
int targetnewtime = 0;       // Previous value of my calculated time to tell if we need to redraw lcd
int second_count = 0;        // Counter for number of seconds since some resetting event (like idle_flag = 0)
int prev_second = 0;         // Placeholder for the previous 'second' value
int dbgTime = 0;             // Flag to debug the time functions
/*int curr_clk = 0;            // Current clk status
int prev_clk = 0;            // Previous clk status
int clk_rise = 0;            // Clk rising edge indicator
int clk_high = 0;            // Clk high indicator
int clk_fall = 0;            // Clk falling edge indicator
int clk_low = 0;             // Clk low indicator*/
//////////////////////////


//////////////////////////
// Global Vars for POT reading and brightness
const int numReadings = 10;  // Number of readings to keep, The higher the number, the smoother (but slower) the response
int readings[numReadings];   // The readings from the analog input
int index = 0;               // The index of the current reading
int total = 0;               // The running total
int average = 0;             // The average
int MaxBrightANA = 1023;     // Define the max brightness allowed by the silver potentiometer
int MaxBrightPWM = 255;      // Define the max brightness allowed in PWM terms
int BLBright = 0;            // The current brightness of the Blue LEDs
int CWBright = 0;            // The current brightness of the Cool White LEDs
int WWBright = 0;            // The current brightness of the Warm White LEDs
int dbgBright = 0;           // Flag to debug the brightness functions for the LEDs
int dbgDelay = 0;            // Number of loop() cycles to count to between time steps
int dbg_cnt = 0;             // Counter for keeping track of cycles betwen time steps (~300 for a second)
//////////////////////////


//////////////////////////
// Global Vars for LCD screen
//  The LCD circuit:
// * LCD RS pin to digital pin 8
// * LCD Enable pin to digital pin 7
// * LCD D4 pin to digital pin 5
// * LCD D5 pin to digital pin 4
// * LCD D6 pin to digital pin 3
// * LCD D7 pin to digital pin 2
// * LCD R/W pin to ground

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD1Pin, LCD2Pin, LCD3Pin, LCD4Pin, LCD5Pin, LCD6Pin);
int lcd_use_lvl = 40;           // PWM value for LCD brightness while the system is in use (serial commands, buttons, change max brightness)
int lcd_dim_lvl = 10;           // PWM value for LCD brightness while the system has gone idle
int lcd_off_lvl = 0;            // PWM value for LCD brightness while the system has not been used for a while
int lcd_cur_lvl = lcd_use_lvl;  // Current PWM value for LCD brightness
int lcd_idle_time = 10;         // 10 seconds till the lcd dims its backlight
int lcd_off_time  = 15;         // 15 seconds till the lcd turns off its backlight
int idle_flag     = 0;          // Flag for determining if system is idle
int dim_count     = 0;          // Counter to delay between dimming the lcd each leveldim_count
int lcd_cursor_loc[2];          // Current location of the LCD cursor ([0] = horizontal dimension, [1] = vertical dimension)
int dbgLcd = 0;                 // Flag to debug the LCD screen functions
//////////////////////////


//////////////////////////
// Macro Definitions
#define KEY_NONE  0  // No button pressed
#define KEY_RIGHT 1  // Right button pressed
#define KEY_LEFT  2  // Left button pressed
#define KEY_UP    3  // Up button pressed
#define KEY_DOWN  4  // Down button pressed
#define KEY_CENT  5  // Center button pressed
//////////////////////////











// Placed in fish_setup
/*
void setup()  { 
  Wire.begin();
  lcd.begin(16, 2);
  lcd.cursor();
  lcd_cursor_loc[0] = 0;  // Init the cursor location
  lcd_cursor_loc[1] = 0;
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
  Serial.begin(115200);
//  pinMode(A1, OUTPUT);
  analogWrite(LCDBackLightPin, lcd_use_lvl);
  
  // Initialize readings
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = analogRead(PotPin) * (255.0 / 1023.0);
    total = total + readings[thisReading];
  }
  average = total / numReadings;
  
  Serial.println("Board Initialized, ready for commands.");
  
  // Initialize the time variables
  readDateDS1307();
  printCurrentTime();
  targetnewtime = newtime;
}
*/

// Placed in fish_debug
/*
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
  Serial.println("a | A - All: Debug all.");
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
*/

// Placed in fish_loop
/*
void loop() {
  if (Serial.available()) {
    get_serial_command();
  }
  
  // Sample Buttons
  getButtonFunction();
  
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
*/

// Placed in fish_buttons
/*
///////////////////////////////
//
// Button functions
//
///////////////////////////////

// Function to decide what to do if there is a button press
void getButtonFunction() {
  getButtonState();
  if (currKey == KEY_RIGHT && (keyPressed || (keyHeld && keyHeldCnt > keyHeldDur && keyIntvlCnt == keyHeldIntvl)))
    if (setting_value) {
      moveCursorRight();
    } else {
      moveCursorRight();
    }
  else if (currKey == KEY_LEFT && (keyPressed || (keyHeld && keyHeldCnt > keyHeldDur && keyIntvlCnt == keyHeldIntvl)))
    if (setting_value) {
      moveCursorLeft();
    } else {
      moveCursorLeft();
    }
  else if (currKey == KEY_UP && (keyPressed || (keyHeld && keyHeldCnt > keyHeldDur && keyIntvlCnt == keyHeldIntvl)))
    if (setting_value) {
      changeTimeDig(lcd_cursor_loc[0], 1);  // Change value up
    } else {
      moveCursorUp();
    }
  else if (currKey == KEY_DOWN && (keyPressed || (keyHeld && keyHeldCnt > keyHeldDur && keyIntvlCnt == keyHeldIntvl)))
    if (setting_value) {
      changeTimeDig(lcd_cursor_loc[0], 0);  // Change value down
    } else {
      moveCursorDown();
    }
  else if (currKey == KEY_CENT && (keyPressed || (keyHeld && keyHeldCnt > keyHeldDur && keyIntvlCnt == keyHeldIntvl))){
    if (lcd_cursor_loc[1] == 1 && (lcd_cursor_loc[0] >= 0 && lcd_cursor_loc[0] <= 4 && lcd_cursor_loc[0] != 2)) {
      setting_value = 1;  // Set value if it is inside valid time area
      lcd.blink();
      setTimeButtons();
    } else if (setting_value && lcd_cursor_loc[0] == 12 && lcd_cursor_loc[1] == 1) {  // Clicked yes to save time
      lcd.noBlink();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Saving Time...");
      checkTime();
      writeDateDS1307();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Time Saved!");
      delay (1000);
      MainScreenLCD(newtime, CWBright*100.0/255.0, WWBright*100.0/255.0, BLBright*100.0/255.0);
      lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
    } else {
      setting_value = 0;
      lcd.noBlink();
      MainScreenLCD(newtime, CWBright*100.0/255.0, WWBright*100.0/255.0, BLBright*100.0/255.0);
    }
  }
  //Serial.print("lcd_cursor_loc[0] = ");
  //Serial.print(lcd_cursor_loc[0]);
  //Serial.print(" lcd_cursor_loc[1] = ");
  //Serial.println(lcd_cursor_loc[1]);
}

// Function to get the state of an analog-read button ladder
void getButtonState () {
  int buttonValue = analogRead(buttonPin);  // Get the new button press
  
  // Get which button was pressed based on the analog value of the button pin
  if (buttonValue >= 800 && buttonValue <= 850)  // Nominal @ 820
    currKey = KEY_CENT;
  else if (buttonValue >=740  && buttonValue <= 790)  // Nominal @ 770
    currKey = KEY_LEFT;
  else if (buttonValue >= 650 && buttonValue <= 710)  // Nominal @ 680
    currKey = KEY_UP;
  else if (buttonValue >= 480 && buttonValue <= 540)  // Nominal @ 512
    currKey = KEY_RIGHT;
  else if (buttonValue >= 0 && buttonValue <= 60)  // Nominal @ 0
    currKey = KEY_DOWN;
  else
    currKey = KEY_NONE;
    
  //Serial.print("debounce_en = ");
  //Serial.print(debounce_en);
  //Serial.print(" debounce_cnt = ");
  //Serial.print(debounce_cnt);
  //Serial.print(" currKey = ");
  //Serial.print(currKey);
  //Serial.print(" prevkey = ");
  //Serial.println(prevKey);
  
  if (currKey != prevKey) {  // New value so do debounce stuff
    debounce_en = 1;
  } else {
    debounce_en = 0;
  }
  if (debounce_en || keyHeld) {
    debounce_cnt++;
  } else {
    debounce_cnt = 0;
  }
  if (debounce_cnt > debounce_delay) {
    debounce_cnt = debounce_delay + 1;
  } else {  // if debounce_cnt < debounce_delay
    currKey = prevKey;
  }  // if debounce_cnt > debounce_delay
    
//    Serial.print("Button Value = ");
//    Serial.println(buttonValue);
    
    // If a button is pressed, increment the counter until its released
    if (currKey != KEY_NONE) {
      keyHeldCnt++;
      keyIntvlCnt++;
      if (keyIntvlCnt > keyHeldIntvl) {  // Rollover interval counter after the long count
        keyIntvlCnt = 0;
        keyHeldStrokeCnt++;
      }
      if (keyHeldStrokeCnt > keyHeldStrokeLimit) {  // After a few cursor updates at the long count, start updating faster
        keyHeldIntvl = short_intvl;
        keyHeldStrokeCnt = keyHeldStrokeLimit + 1;
      } else {
        keyHeldIntvl = long_intvl;
      }
      if (keyHeldCnt > keyHeldDur)     // Hold keyHeldCnt at keyHeldDur + 1 to keep from rolling over
        keyHeldCnt = keyHeldDur + 1;
    } else {
      keyHeldCnt = 0;
      keyIntvlCnt = 0;
      keyHeldStrokeCnt = 0;
    }
  
    //Serial.print(" keyHeldCnt = ");
    //Serial.print(keyHeldCnt);
    //Serial.print(" keyIntvlCnt = ");
    //Serial.println(keyIntvlCnt);
    
    // Set the button status flags based on button events
    if (prevKey == KEY_NONE && currKey != KEY_NONE) {
      keyPressed = 1;
      keyReleased = 0;
      keyHeld = 0;
      idle_flag = 0;     // Reset the lcd back light dimming idle flag
      //Serial.print("Key Pressed:  ");
      //Serial.println(currKey);
    } else if (prevKey != KEY_NONE && currKey == KEY_NONE) {
      keyPressed = 0;
      keyReleased = 1;
      keyHeld = 0;
      idle_flag = 0;     // Reset the lcd back light dimming idle flag
      //Serial.print("Key Released: ");
      //Serial.println(prevKey);
    } else if ((prevKey == currKey) && (keyPressed || keyHeld)) {
      //Serial.print("Key Held:     ");
      //Serial.println(currKey);
      keyPressed = 0;
      keyReleased = 0;
      keyHeld = 1;
      idle_flag = 0;     // Reset the lcd back light dimming idle flag
    } else if ((prevKey == currKey) && keyReleased) {
      //Serial.print("Keys Idle:    ");
      //Serial.println(currKey);
      keyPressed = 0;
      keyReleased = 0;
      keyHeld = 0;
    }
  
  prevKey = currKey;  // Save the previous buttons press
}

void setTimeButtons() {
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
  lcd.setCursor(0,1);
  if (newtime/60 < 10)
    lcd.print("0");
  lcd.print(newtime/60);
  lcd.print(":");
  if (newtime%60 < 10)  // Minutes
    lcd.print("0");
  lcd.print(newtime%60);
  
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
  
}
/*

// Placed in fish_lcd
/*
///////////////////////////////
//
// LCD functions
//
///////////////////////////////

// Move Cursor Right
void moveCursorRight() {
  lcd_cursor_loc[0]++;
  if (lcd_cursor_loc[0] >= 16)  // If cursor gets moved off screen, wrap to 0
    lcd_cursor_loc[0] = 0;
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}

// Move Cursor Left
void moveCursorLeft() {
  lcd_cursor_loc[0]--;
  if (lcd_cursor_loc[0] < 0)  // If cursor gets moved off screen, wrap to 15
    lcd_cursor_loc[0] = 15;
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}

// Move Cursor Up
void moveCursorUp() {
  lcd_cursor_loc[1]++;
  if (lcd_cursor_loc[1] >= 2)  // If cursor gets moved off screen, wrap to 0
    lcd_cursor_loc[1] = 0;
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}

// Move Cursor Down
void moveCursorDown() {
  lcd_cursor_loc[1]--;
  if (lcd_cursor_loc[1] < 0)  // If cursor gets moved off screen, wrap to 1
    lcd_cursor_loc[1] = 1;
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}

// Get the status of the LCD Back Light
// Should it be on, dimming, or off
void getLCDblStatus() {
    
  // If LCD idle_flag has been reset, make sure to reset the dim_count
  if (idle_flag == 0) {
    dim_count = 0;
    idle_flag = 1;  // flag to show LCD should be counting toward idle state
    second_count = 0;
  }
  
  // See if the second has changed
  if (prev_second != second) {
    second_count++;
  }
  prev_second = second;
  
  /*Serial.print("second: ");
  Serial.print(second);
  Serial.print(" prev_second: ");
  Serial.print(prev_second);
  Serial.print(" second_count: ");
  Serial.print(second_count);
  Serial.print(" idle_flag: ");
  Serial.print(idle_flag);
  Serial.print(" dim_count: ");
  Serial.print(dim_count);
  Serial.print(" lcd_cur_lvl: ");
  Serial.println(lcd_cur_lvl);
  if (idle_flag && second_count >= lcd_idle_time && second_count < lcd_off_time) {
    dimLCDbl(lcd_dim_lvl);
  }
  else if (idle_flag && second_count >= lcd_off_time) {
    second_count = lcd_off_time + 1;  // Held to make sure it doesnt roll over
    dimLCDbl(lcd_off_lvl);
  }
  else {
    lcd_cur_lvl = lcd_use_lvl;
    dimLCDbl(lcd_use_lvl);
  }
}

// This is the main screen displayed on the LCD
void MainScreenLCD(int mins, int CWp, int WWp, int BLp) {
  int tempCursorLoc[1];
  
  lcd.noCursor();
  lcd.clear();
  //tempCursorLoc[0] = lcd_cursor_loc[0];  // Save the current cursor location while we draw the screen
  //tempCursorLoc[1] = lcd_cursor_loc[1];
/*  Serial.print("Time: ");
  if( mins/60 < 10)
    Serial.print("0");
  Serial.print(mins/60);
  Serial.print(":");
  if( mins%60 < 10)
    Serial.print("0");
  Serial.print(mins%60);
  Serial.print(" Max Bright: ");
  Serial.print(MaxBrightPWM);
  Serial.print(" CW: ");
  Serial.print(CWp);
  Serial.print(", ");
  Serial.print(CWBright);
  Serial.print(" WW: ");
  Serial.print(WWp);
  Serial.print(", ");
  Serial.print(WWBright);
  Serial.print(" BL: ");
  Serial.print(BLp);
  Serial.print(", ");
  Serial.println(BLBright);
  
  // Print the Header
  lcd.setCursor(0,0);
  lcd.print("Time: CW WW BL");
  
  // Print the Time
  lcd.setCursor(0,1);  // Hours
  if (mins/60 < 10)
    lcd.print("0");
  lcd.print(mins/60);
  lcd.print(":");
  if (mins%60 < 10)  // Minutes
    lcd.print("0");
  lcd.print(mins%60);
  
  // Print the Cool White LED Power Percentage
  lcd.setCursor(6,1);
  if (CWp < 10)
    lcd.print("0");
  lcd.print(CWp); 
  
  // Print the Warm White LED Power Percentage
  lcd.setCursor(9,1);
  if (WWp < 10)
    lcd.print("0");
  lcd.print(WWp);
  
  // Print the Blue LED Power Percentage
  lcd.setCursor(12,1);
  if (BLp < 10)
    lcd.print("0");
  lcd.print(BLp);
  
//  lcd_cursor_loc[0] = 0;
//  lcd.setCursor(lcd_cursor_loc[0],1);
  lcd.cursor();
  //lcd_cursor_loc[0] = tempCursorLoc[0];  // Restore the cursor location after drawing the screen
  //lcd_cursor_loc[1] = tempCursorLoc[1];
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
}

// Dim the LCD backlight
void dimLCDbl(int target_lvl) {
  // Counter to slowly dim the backlight
  if (dim_count == 10) {
    lcd_cur_lvl--;
    dim_count = 0;
  }
  
  // Hold brightness at target 
  if (lcd_cur_lvl <= target_lvl) {
    lcd_cur_lvl = target_lvl;
  }
  
  analogWrite(LCDBackLightPin, lcd_cur_lvl);
  dim_count++;
}
*/

// Placed in fish_time
/*
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
*/

// Placed in fish_brightness
/*
///////////////////////////////
//
// POT and LED Brightness Functions
//
///////////////////////////////
int last_avg = 0;
int this_avg = 0;
void getmaxbrightness() {  
 
  // only save an average once every 10 reads to cut down on noise
  if (index == 0) {
    last_avg = total / numReadings;  // calculate the last average before updating readings
  }
  // Subtract the last potentiometer reading from the total:
  total = total - readings[index];
  // Read from the pot:  
  readings[index] = analogRead(PotPin) * (255.0 / 1023.0);
  // Add the reading to the total:
  total = total + readings[index];

  // If the most recent value is pretty different than previous value
  // then the system is being used, so set idle_flag to 0
  this_avg = total / numReadings;
  
  if (this_avg >= (last_avg + 2) || (this_avg <= last_avg - 2)) {
    idle_flag = 0;
  }
  /*Serial.print("index: ");
  Serial.print(index);
  Serial.print(" this_avg: ");
  Serial.print(this_avg);
  Serial.print(" last_avg: ");
  Serial.print(last_avg);
  Serial.print(" idle_flag: ");
  Serial.println(idle_flag);
  
  // Advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // Wrap around to the beginning: 
    index = 0;                           

  // Calculate the average:
  MaxBrightPWM = total / numReadings;    
}

////////////////////////////////////
//
// Cool White LED schedule
//
////////////////////////////////////
float cwledsched(int curr_time) {
 int CWLevel = 0;
 
 // Bring Cool led from 0% (about 8am) to 100% (about 10am)
 // Bring Cool led from 100% (about 5pm) to 0% (about 7pm)
  
 if (curr_time >= 0 && curr_time < (8 * 60)) {
   CWLevel = 0;
 }
 
 if (curr_time >= (8 * 60) && curr_time <= (10 * 60)) {
   CWLevel = ((MaxBrightPWM - 0) / ((10.0 * 60.0) - (8.0 * 60.0))) * (curr_time - (8.0 * 60.0));
 }
 
  if (curr_time > (10 * 60) && curr_time < (17 * 60)) {
   CWLevel = MaxBrightPWM;
 }
 
  if (curr_time >= (17 * 60) && curr_time <= (19 * 60)) {
   CWLevel = ((MaxBrightPWM - 0) / ((17.0 * 60.0) - (19.0 * 60.0))) * (curr_time - (19.0 * 60.0));
 }
 
 if (curr_time > (19 * 60)) {
   CWLevel = 0;
 }
 
 return CWLevel;
}

////////////////////////////////////
//
// Warm White LED schedule
//
////////////////////////////////////
float wwledsched(int curr_time) {
 int WWLevel = 0;
 
 // Bring Warm led from 0% (about 5am) to 100% (about 9am)
 // Bring Warm led from 100% (about 7pm) to 0% (about 9pm)
  
 if (curr_time >= 0 && curr_time < (5 * 60)) {
   WWLevel = 0;
 }
 
 if (curr_time >= (5 * 60) && curr_time <= (9 * 60)) {
   WWLevel = ((MaxBrightPWM - 0) / ((9.0 * 60.0) - (5.0 * 60.0))) * (curr_time - (5.0 * 60.0));
 }
 
  if (curr_time > (9 * 60) && curr_time < (19 * 60)) {
   WWLevel = MaxBrightPWM;
 }
 
  if (curr_time >= (19 * 60) && curr_time <= (21 * 60)) {
   WWLevel = ((MaxBrightPWM - 0) / ((19.0 * 60.0) - (21.0 * 60.0))) * (curr_time - (21.0 * 60.0));
 }
 
 if (curr_time > (21 * 60)) {
   WWLevel = 0;
 }
 
 return WWLevel;
}

////////////////////////////////////
//
// Blue LED schedule
//
////////////////////////////////////
float blledsched(int curr_time) {
  int BlueLevel = 0;
 
  // Bring Blue led from 100% (about 12am?) to 10% (about 12:30am?)
  // Bring Blue led from 10% (about 3am?) to 100% (about 3:30am?)
  // Bring Blue led from 100% (about 4am?) to 0% (about 6am?)
  // 1023|          /----\                        /----\
  //     |         /      \                      /      \
  //     |        /        \                    /        \
  //     |       /          \                  /          \
  //     |      /            \                /            \
  //511.5|     /              \              /              \
  //     |    /                \            /                \
  //     |   /                  \          /                  \
  //     |  /                    \        /                    \
  //102.3| /                      \______/                      \
  //    0|/______________________________________________________\__
  //      
   
  // Bring Blue led from 100% (about 12am?) to 10% (about 12:30am?) 0 - 30, 1023.0 - 102.3
  if (curr_time >= 0 && curr_time <= (0.5 * 60)) {
    BlueLevel = ((MaxBrightPWM - MaxBrightPWM * 0.1) / (0.0 - (0.5 * 60.0))) * (curr_time - (0.5 * 60.0)) + (MaxBrightPWM * 0.1); 
  }
  
  // Hold Blue led at 10% from about 12:30am? to about 3:30am? 31 - 239, 102.3
  if (curr_time > (0.5 * 60) && curr_time < (3.5 * 60)) {
    BlueLevel = MaxBrightPWM * 0.1;
  }
  
  // Bring Blue led from 10% (about 3:30am?) to 100% (about 4am?) 240 - 270, 102.3 - 1023.0
  if (curr_time >= (3.5 * 60) && curr_time < (4 * 60)) {
    BlueLevel = ((MaxBrightPWM - MaxBrightPWM * 0.1) / ((4.0 * 60.0) - (3.5 * 60.0))) * (curr_time - (3.5 * 60.0)) + (MaxBrightPWM * 0.1);
  }
  
//  // Hold Blue led at 100% from about 4am? to about 5am? 271 - 299, 1023.0
//  if (curr_time > (4.5 * 60) && curr_time < (5 * 60)) {
//    BlueLevel = MaxBrightPWM;
//  }
  
  // Bring Blue led from 100% (about 4am?) to 0% (about 6am?) 300 - 540, 1023.0 - 0
  if (curr_time >= (4 * 60) && curr_time <= (6 * 60)) {
    BlueLevel = ((MaxBrightPWM - 0) / ((4.0 * 60.0) - (6.0 * 60.0))) * (curr_time - (6.0 * 60.0));
  }
  
    // Hold Blue led at 0% between about 6am? and about 8pm? 541 - 1139, 0
  if (curr_time > (6 * 60) && curr_time < (20 * 60)) {
    BlueLevel = 0;
  }
  
  // Bring Blue led from 0% (about 8pm?) to 100% (about 10pm?) 1140 - 1260, 0 - 1023.0
  if (curr_time >= (20 * 60) && curr_time <= (22 * 60)) {
    BlueLevel = ((MaxBrightPWM - 0) / ((22.0 * 60.0) - (21.0 * 60.0))) * (curr_time - (20.0 * 60.0));
  }
  
  // Hold Blue led at 100% from about 9pm? to about 12am? 1261 - 1440, 1023.0
  if (curr_time > (22 * 60)) {
    BlueLevel = MaxBrightPWM;
  }
  
  return BlueLevel;
}
*/
