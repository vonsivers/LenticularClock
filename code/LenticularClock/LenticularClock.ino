//==========================================================================
// Lenticular Clock - LenticularClock.ino
//
// Copyright (c) 2024, Moritz v. Sivers
//
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================

#include "servos.h"
#include "Clock.h"

#define CTRL_C  0x78 //'x'
#define UP_ARROW  0x75 // 'u'
#define DOWN_ARROW 0x64 // 'd'
#define PLUS_SIGN 0x2B
#define MINUS_SIGN 0x2D

void KeypadControl() {
  bool bLoop = true;
  int iCurrentMotor = 0;
  int currentPos = 200;
  int lSteps = 1;
  int in;

  while (bLoop) {  
 
    if(Serial.available()){
      in = Serial.read();

      switch (in) {
        case CTRL_C:      //'x'
            bLoop=false;
            return;
            break;
        case PLUS_SIGN:   //plus 
             lSteps++;
             Serial.print(F("Current Steps: "));
             Serial.println(lSteps);
            break;
        case MINUS_SIGN:        // minus
             lSteps--;
             Serial.print(F("Current Steps: "));
             Serial.println(lSteps);
             break;
        case DOWN_ARROW:
              currentPos = currentPos-lSteps;
              pwm.setPWM(iCurrentMotor, 0, currentPos); 	// move motor to position
              Serial.print("Position: ");
              Serial.println(currentPos);
            break;
        case UP_ARROW:
              currentPos = currentPos+lSteps;
              pwm.setPWM(iCurrentMotor, 0, currentPos); 	// move motor to position
              Serial.print("Position: ");
              Serial.println(currentPos);
              break;
        case '0':
          iCurrentMotor = 0;
          Serial.print("Current Motor: ");
          Serial.println(iCurrentMotor);
          break;
        case '1':
          iCurrentMotor = 1;
          Serial.print("Current Motor: ");
          Serial.println(iCurrentMotor);
          break;
        case '2':
          iCurrentMotor = 2;
          Serial.print("Current Motor: ");
          Serial.println(iCurrentMotor);
          break;
        case '3':
          iCurrentMotor = 3;
          Serial.print("Current Motor: ");
          Serial.println(iCurrentMotor);
          break;
       default:
          Serial.print(F("Input detected: '"));
          Serial.print(in);
          Serial.print(F("' 0x"));
          Serial.print(in, HEX);
          Serial.print("\n");
          break;
     }  //switch  

    } // if
  } // while

}


//-------------------------------------
// s e t u p
//-------------------------------------
void setup()
{
  Serial.begin(9600);
  Serial.print("Lenticular Clock version 1.0\n");


  pwm.begin();
  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  //KeypadControl();

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;
  //reset settings - for testing
  //wm.resetSettings();

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //and goes into a blocking loop awaiting configuration
  if (!wm.autoConnect("LentiClockAP")) {
    Serial.println("failed to connect Wifi and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }

  configTime(MY_TZ, MY_NTP_SERVER); // configure NTP server and timezone

  delay(1000);

  showTime();
  

  //initTime();    // only for show mode

  delay(1000);

}


//-------------------------------------
// l o o p
//
// 
//-------------------------------------
void loop()
{
  runClock();
  //runClockShow();
}
