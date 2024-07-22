//==========================================================================
// Lenticular Clock -  Clock.h
//
// Copyright (c) 2024, Moritz v. Sivers
//
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================
#ifndef CLOCK_H
#define CLOCK_H

#include "servos.h"
#include <ESP8266WiFi.h>            // we need wifi to get internet access
#include <time.h>                   // time() ctime()

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

/* Configuration of NTP */
#define MY_NTP_SERVER "de.pool.ntp.org"            // set the best fitting NTP server (pool) for your location
#define MY_TZ "CET-1CEST,M3.5.0,M10.5.0/3"        // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

time_t now;                         // this is the epoch
tm tm;                              // the structure tm holds time information in a more convenient way

uint8_t lastMinute;                 // save minute when clock was last updated


// show time
void showTime() {
  time(&now);                       // read the current time
  localtime_r(&now, &tm);           // update the structure tm with the current time

  uint16_t toh = tm.tm_hour / 10;
  uint16_t hour = tm.tm_hour % 10;
  uint16_t tom = tm.tm_min / 10;
  uint16_t min = tm.tm_min % 10;

  showDigit(0, min);
  showDigit(1, tom);
  showDigit(2, hour);
  showDigit(3, toh);

  lastMinute = tm.tm_min;
}


// draw the current time
void updateDisplay() {

   printf("Setting time: %s", asctime(&tm));

  uint16_t toh = tm.tm_hour / 10;
  uint16_t hour = tm.tm_hour % 10;
  uint16_t tom = tm.tm_min / 10;
  uint16_t min = tm.tm_min % 10;

  // only minutes have changed
  showDigit(0, min);

  // time is xx:x0 
  if (min==0) {
    showDigit(1, tom);
    // time is xx:00
    if (tom==0) {
      showDigit(2, hour);
      // time is x0:00
      if (hour==0) {
        showDigit(3, toh);
      }
    }
  }
  
}


// runs the clock
void runClock() {
  time(&now);                       // read the current time
  localtime_r(&now, &tm);           // update the structure tm with the current time
  if(tm.tm_min!=lastMinute) {
    updateDisplay();
    lastMinute = tm.tm_min;
  }
  delay(1000);
}

// runs the clock in show mode
void runClockShow() {
  updateDisplay();
  tm.tm_min++;  // add one minute 
  mktime(&tm);
  delay(2000);


}

// set to 09:50 for show mode
void initTime() {
  tm.tm_year = 2024 - 1900;
  tm.tm_mon = 1;
  tm.tm_mday = 1;
  tm.tm_hour = 9;
  tm.tm_min = 50;
  tm.tm_sec = 0;

  uint16_t toh = tm.tm_hour / 10;
  uint16_t hour = tm.tm_hour % 10;
  uint16_t tom = tm.tm_min / 10;
  uint16_t min = tm.tm_min % 10;

  showDigit(0, min);
  showDigit(1, tom);
  showDigit(2, hour);
  showDigit(3, toh);


}


#endif //CLOCK_H
