///////////////////////////////
//
// This is the root file for the fish light project and 
// it only contains the global variables.  There is no 
// logic in this file at all.  Each block of code has been 
// placed into its corresponding <fish_*> file to make it 
// easier to maintain.
//
///////////////////////////////

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
const int clkPin = 12;           // RTC square wave pin

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
int dbncKey = 0;                 // The state of the button when we first started debouncing
int keyPressed = 0;              // Tells if a key is pressed, no keys are pressed initially
int keyReleased = 0;             // Tells if a key is released, no keys are pressed initially
int keyHeld = 0;                 // Tells if a key is being held, no keys are pressed initially
int keyHeldCnt = 0;              // Counter which counts cycles since button was pressed
int keyHeldDur = 700;            // Value to compare with to see how long a button has been held
int keyIntvlCnt = 0;             // Counter which counts cycles to determine how often the cursor gets moved
int keyHeldIntvl = 0;            // Holds the desired interval to count up to 
int long_intvl = 350;            // Value to count to in order to delay between changing the cursor location while a button is held
int short_intvl = 125;           // Value to count to in order to delay between changing the cursor location while a button is held
int keyHeldStrokeCnt = 0;        // Counts the number of cursor updates while a button is held. After KeyHeldStrokeLimit, it will count faster
int keyHeldStrokeLimit = 8;      // Value to count to for number of cursor updates while a button is held
int debounce_cnt = 0;            // Counter to count from a button value change up to the debounce_delay to debounce buttons
int debounce_delay = 35;         // Value to count up to once a button has been pressed.  If the button value is the same, its a true button press
int debounce_en = 0;             // Enable the button debouncing counter
int setting_value = 0;           // Flag to tell buttons to either move cursor or set a number value
//////////////////////////


//////////////////////////
// Global vars for Time / RTC
int clockAddress = 0x68;     // This is the RTCs I2C address
int command = 0;             // This is the command char, in ascii form, sent from the serial port     
long previousMillis = 0;     // Will store last time Temp was updated
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year, control;
const int totalDigs = 12;    // Number of digits needed to hold time
int mytime[totalDigs];       // My time holder ([12:11] month, [10:9] dayofMonth, [8:7] year, [6] dayOfWeek,
                             //                 [5:4] hour, [3:2] minute, [1:0] second)
int newtime = 0;             // My calculated time to pass to LED schedule tasks
int targetnewtime = 0;       // Previous value of my calculated time to tell if we need to redraw lcd
int second_count = 0;        // Counter for number of seconds since some resetting event (like idle_flag = 0)
int prev_second = 0;         // Placeholder for the previous 'second' value
byte curr_clk = 0;           // Current Clk status
byte prev_clk = 0;           // Previous Clk status
byte clk_rise = 0;           // Clk rising edge indicator
byte clk_high = 0;           // Clk high indicator
byte clk_fall = 0;           // Clk falling edge indicator
byte clk_low  = 0;           // Clk low indicator
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
//////////////////////////

//////////////////////////
// Global Vars for Serial/Debug
int dbgButtons = 0;              // Flag to debug button functions
int dbgTime = 0;                 // Flag to debug the time functions
int dbgBright = 0;               // Flag to debug the brightness functions for the LEDs
double dbgDelay = 0;             // Number of loop() cycles to count to between time steps
int dbg_cnt = 0;                 // Counter for keeping track of cycles betwen time steps (~300 for a second)
int dbgLcd = 0;                  // Flag to debug the LCD screen functions
int dbgSerial = 0;               // Flag to debug the Serial functions
const int num_digs = 10;         // Number of digits allowed when getting a serial number
//////////////////////////

//////////////////////////
// Macro Definitions
#define KEY_NONE  0  // No button pressed
#define KEY_RIGHT 1  // Right button pressed
#define KEY_LEFT  2  // Left button pressed
#define KEY_UP    3  // Up button pressed
#define KEY_DOWN  4  // Down button pressed
#define KEY_CENT  5  // Center button pressed

#define LCD_RIGHT_EDGE 15  // Character number of the lcd screens right edge
#define LCD_LEFT_EDGE  0   // Character number of the lcd screens left edge
#define LCD_TOP_EDGE   1   // Character number of the lcd screens top edge
#define LCD_BTM_EDGE   0   // Character number of the lcd screens bottom edge

//////////////////////////


