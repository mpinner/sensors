/*
 *  Original code by Bob Hartmann and Dave Vondle for http://labs.ideo.com March 2010
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
 */

#ifndef LEDSequence_H
#define LEDSequence_H

#include "LEDController.h"


class LEDSequence {
public:

  int* step;
  int stepCount;

  int stepCounter = 0;
  long stepStartTime = 0;
  long stepEndTime = 0;
    
int stepValues=3;

  LEDController* leds;


  LEDSequence(LEDController* ledsIn,
  int step[],
  int stepCountIn
    );



  void takeAction();

  void setLEDBrightness(int ledValue);

  void setAttributes(LEDController* ledsIn, int stepIn[], int stepCountIn);
    
    void incrAction();
    
    boolean isExpired();
    
    void initStepMetaData();
    
    long getCurrentDuration ();
    
    int getCurrentAction ();


    

};  // End of LEDSequence class
#endif


