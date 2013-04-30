///////////////////////////////
//
// This contains the functions used to find the max brightness
// from the POT and to set the brightness of the leds according
// to the time of day.
//
///////////////////////////////

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
  Serial.println(idle_flag);*/
  
  // Advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // Wrap around to the beginning: 
    index = 0;                           

  // Calculate the average:
  MaxBrightPWM = total / numReadings;    
}

void change_max_bright(int location, int up_downb) {
  // brightness is 3 digits wide
  // location starts at X coordinate 0 ([0:2])
  int mytmpbright[3];
  //int newbright = 0;

  // Set new brightness
  if (up_downb) {
    mytmpbright[location]++;
  } else if (!up_downb) {
    mytmpbright[location]--;
  }
  
  //newbright = mytmpbright[0] * 100 + mytmpbright[1] * 10 + mytmpbright[2];
  
  // Print the new value
  lcd.print(mytmpbright[location]);
  lcd.setCursor(lcd_cursor_loc[0],lcd_cursor_loc[1]);
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
   WWLevel = MaxBrightPWM * 0.01;
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
   WWLevel = MaxBrightPWM * 0.01;
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

/*
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
*/

if (curr_time >= (21 * 60) || curr_time <= (5 * 60)) {
    BlueLevel = MaxBrightPWM * 0.01;
  }

  return BlueLevel;
}

