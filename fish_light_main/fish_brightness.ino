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

int last_prnt = 0;
void led_schedule(int curr_time, struct MY_LED my_led) {
  int prnt = 0;
  if (curr_time % 10 == 0 && last_prnt != curr_time) {
    prnt = 1;
    last_prnt = curr_time;
  } else 
    prnt = 0;
  
  if (prnt) {
  Serial.print(curr_time);
  Serial.print(", ");
  Serial.print(my_led.name);
  Serial.print(" led, ");
  }
  
  if (my_led.peaks > 0) {
    for (int i = 0; i < my_led.peaks; i++) {
      if (curr_time >= my_led.on_start[i] && curr_time <= my_led.on_time[i]) {  // Ramping up
        my_led.curr_bright = ((0.0 - my_led.on_bright[i]) / (my_led.on_start[i] - my_led.on_time[i])) * (curr_time - my_led.on_start[i]);
      }
      else if (curr_time >= my_led.off_start[i] && curr_time <= my_led.off_time[i]) {  // Ramping down
        my_led.curr_bright = ((my_led.on_bright[i] - 0.0) / (my_led.off_start[i] - my_led.off_time[i])) * (curr_time - my_led.off_time[i]);
      }
      else if (curr_time > my_led.on_time[i] && curr_time < my_led.off_start[i]) {  // Peak on
        my_led.curr_bright = my_led.on_bright[i];
      }
      else if (my_led.name == "bl") {
        if (curr_time > my_led.on_time[i] || curr_time < my_led.off_start[i]) {
          my_led.curr_bright = my_led.on_bright[i];
        }
      }
      else {
        my_led.curr_bright = 0;
      }
      if (prnt)
      Serial.print(my_led.curr_bright);
    }  // for (int i = 0; i < my_led.peaks; i++)
  }
  else {
    my_led.curr_bright = 0; 
  }  // if (my_led.peaks > 0)
  if (prnt)
  Serial.println("");
}

