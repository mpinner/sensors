/*
 *  Original code by Bob Hartmann and Dave Vondle for http://labs.ideo.com March 2010
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
*/

#ifndef LEDController_H
#define LEDController_H
 
#include <WProgram.h>

// Actions for takeAction case statement in LEDControllerSource
enum {
    TURN_HIGH = 0,  // If PWM pin then analog, else digital
    TURN_LOW = 1,
    TURN_OFF = 2,  // If PWM pin then analog, else digital
    BLINK = 3,     // If PWM pin then analog, else digital
    RAMP_HIGH = 4,   // If digital pin, defaults to TURN_ON behavior
    RAMP_LOW = 5,  // If digital pin, defaults to TURN_OFF behavior
    RAMP_CONT_HIGH_LOW = 6,   // If digital pin, defaults to BLINK behavior
    TURN_MID = 7,
    RAMP_CONT_HIGH_MID = 8
   
 };


class LEDController {
  public:
    // Did not use setters and gettersw
    boolean update;
    int action;
    int analogHighValue;
    int analogMidValue;
    int analogLowValue;
    int analogValue;
    int rampUpValue;
    int rampDownValue;
    int rampUpDelay;
    int rampDownDelay;
    int dwellHighDelay;
    int dwellLowDelay;
    int i2cAddress;  // This is the I2C address
    int ledAddress;
    int ioPin;        //for PWM on Arduino.
    
    boolean blinkState; // true = HIGH, false = LOW
    int blinkTimeHigh;
    int blinkTimeLow;
    unsigned long previousTime;
    boolean usingPWM;  //TRUE if PWM pin, FALSE if using I2C
    boolean usingStrip;  //TRUE if PWM pin, FALSE if using I2C
    int ledStrip;

    int actualLength;
    
    int colorIsWhite = 0;

 
      // Note: This set of attributes is for a Strip output, creates an ioPin attribute to help direct the analog output to that pin.
    LEDController(int ledStripIn,
                  int actualLengthIn,
                  boolean updateIn=true, 
                  int actionIn=TURN_HIGH,
                  int analogHighValueIn=255, 
                  int analogMidValueIn=127, 
                  int analogLowValueIn=0, 
                  int analogValueIn=0,
                  int rampUpValueIn=1, 
                  int rampDownValueIn=1, 
                  int rampUpDelayIn=10, 
                  int rampDownDelayIn=10, 
                  boolean blinkStateIn=false, 
                  int blinkTimeHighIn=100, 
                  int blinkTimeLowIn=900, 
                  unsigned long previousTimeIn=0
                  );
        
        
      // Note: This set of attributes is for a PWM output, creates an ioPin attribute to help direct the analog output to that pin.
    LEDController(int ioPinIn,
                  boolean updateIn=true, 
                  int actionIn=TURN_HIGH,
                  int analogHighValueIn=255, 
                  int analogMidValueIn=127, 
                  int analogLowValueIn=0, 
                  int analogValueIn=0,
                  int rampUpValueIn=1, 
                  int rampDownValueIn=1, 
                  int rampUpDelayIn=10, 
                  int rampDownDelayIn=10, 
                  boolean blinkStateIn=false, 
                  int blinkTimeHighIn=100, 
                  int blinkTimeLowIn=900, 
                  unsigned long previousTimeIn=0
                  );
        
        
      void takeAction();
      
      void setLEDBrightness(int ledValue);
     
//I2C interface  
      void LEDSetAttributes(int i2cAddressIn, int ledAddressIn, boolean updateIn, int actionIn, int analogHighValueIn, int analogMidValueIn, int analogLowValueIn, 
                                     int analogValueIn, int rampUpValueIn, int rampDownValueIn, int rampUpDelayIn, int rampDownDelayIn,
                                     boolean blinkStateIn, int blinkTimeHighIn, int blinkTimeLowIn, 
                                     unsigned long previousTimeIn
                                     );
//PWM output        
      void LEDSetAttributes(int ioPinIn, boolean updateIn, int actionIn, int analogHighValueIn, int analogMidValueIn, int analogLowValueIn, 
                                     int analogValueIn, int rampUpValueIn, int rampDownValueIn, int rampUpDelayIn, int rampDownDelayIn,
                                     boolean blinkStateIn, int blinkTimeHighIn, int blinkTimeLowIn, 
                                     unsigned long previousTimeIn
                                     );

};  // End of LEDController class
#endif

