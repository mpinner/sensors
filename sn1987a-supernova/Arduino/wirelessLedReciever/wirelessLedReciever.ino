/* Matt Pinner in collaboration w Anouk Wipprecht
 *
 * intent: 
 *  control and load sequences on multiple leds either on strips or connected directly to a teensy3. 
 *  new sequences can be uploaded via xbee api.
 *
 * xbee payloads can express a variity data: new color, change of brightness, speed, desired sequence, and new sequences.
 * much of this is meaning is derivived from the length of the payload.
 *
 * payload with SINGLE value:
 *  0 : all off
 *  1 : all on
 *  2 : switch color
 *  3-9 : speed control
 *  10-255 : brightness
 *
 * payload with EXACTLY 2 denote switching to a new sequence:
 *  0 : value of which led or region to apply this sequence (0 value should denote all)
 *  1 : the index of sequence to use
 *
 * payload with 4 or MORE denote a new adhoc sequence:
 *  0 : value of which led or region to apply this sequence (0 value should denote all)
 *  1 : the number of steps in the sequence
 *  2,3,4 : the duration, step type, and metadata for the first step
 *  ...
 *
 * 2013 NYE cirque du soleil las vegas 
 *
 * licensing :
 *  design and LEDControll code borrowed heavily from Bob Hartmann and Dave Vondle for http://labs.ideo.com March 2010
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
 
 */

// controls 8 leds strips from a teensy
#include <OctoWS2811.h>

// improves interaction and data handling with xbees configured in api mode
#include <XBee.h>

// manages each led or led strip
#include "LEDController.h"

// holds a current sequence for each led under control
#include "LEDSequence.h"

// used to turn off Serial out debugging data and improve performace
#define DEBUG true

// setup data for led strip handling
const int ledsPerStrip = 30; // maximum.. some of mine will have only 20 which is handled in LEDControll config
DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

///
// XBEE setup
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

long xbeeTimeoutDurationMs = 30000; // 5min // timeout in ms.. goto default behavior
long xBeeTimoutMs = 0;

// xbee debug leds
int statusLed = 13;
int errorLed = 13;
int dataLed = 13;


int stepValues = 3;



// number of strip loaded.. this is very octows11 specific and has predefined pins




// number of strip loaded.. this is very octows11 specific and has predefined pins
int leftStrip = 22;
int leftStepCount =10;
int leftStep[50] = {
  500, RAMP_CONT_HIGH_LOW, 16, 
  1000, RAMP_CONT_HIGH_LOW, 8, 
  1000, RAMP_CONT_HIGH_LOW, 4, 
  1000, RAMP_CONT_HIGH_LOW, 2, 
  1000, BLINK, 16,
  1000, BLINK, 32,
  1000, BLINK, 64,
  1000, RAMP_HIGH, 1,
  5000, RAMP_LOW, 1,
  5500, TURN_OFF, 0, 
};
LEDController left(leftStrip);
LEDSequence leftSeq(&left, leftStep, leftStepCount);




// number of strip loaded.. this is very octows11 specific and has predefined pins
int rightStrip = 23;
int rightStepCount = 10;
int rightStep[50]{
  1000, RAMP_CONT_HIGH_LOW, 16, 
  1000, RAMP_CONT_HIGH_LOW, 8, 
  1000, RAMP_CONT_HIGH_LOW, 4, 
  1000, RAMP_CONT_HIGH_LOW, 2, 
  1000, BLINK, 16,
  1000, BLINK, 32,
  1000, BLINK, 64,
  1000, RAMP_HIGH, 10,
  5000, RAMP_LOW, 10,
  5000, TURN_OFF, 0, 
};
int *rStepPtr = rightStep;
LEDController right(rightStrip);
LEDSequence rightSeq(&right, rightStep, rightStepCount);




/*
int strip[] = {4,3};
 int stepCount[] ={10,10};
 int *steps[] = {rStepPtr, rStepPtr};
 LEDController ledsControllers[] = {LEDController(strip[0], 30)};
 LEDSequence seq(steps[0], ledsControllers[0], stepCount[0]);
 */

int defaultSteps[10][50] = 
{
  {
    1, 
    1000, TURN_OFF, 0
  }
  ,
  {
    1, 
    1000, TURN_HIGH, 4
  }
  ,
  {
    1, 
    1000, BLINK, 4
  }
  ,
  {
    1, 
    1000, RAMP_CONT_HIGH_LOW, 4 
  }
  ,
  {
    10,
    1000, RAMP_CONT_HIGH_LOW, 16, 
    1000, RAMP_CONT_HIGH_LOW, 8, 
    1000, RAMP_CONT_HIGH_LOW, 4, 
    1000, RAMP_CONT_HIGH_LOW, 2, 
    1000, BLINK, 16,
    1000, BLINK, 32,
    1000, BLINK, 64,
    1000, RAMP_HIGH, 10,
    5000, RAMP_LOW, 10,
    5000, TURN_OFF, 0, 
  }
};

int colorIsWhite = 1;
int speed = 5;
int defaultBrightness = 16;


void setup(){
  if (DEBUG) Serial.begin(9600);

  leds.begin();

  ////
  // XBEE
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);
  pinMode(dataLed,  OUTPUT);
  Serial2.begin(9600);
  xbee.setSerial(Serial2);

  flashLed(statusLed, 3, 50);




  pinMode(13, OUTPUT);     

  left.action = 0;
  left.analogHighValue = defaultBrightness;


  right.action = 0;
  right.analogHighValue = defaultBrightness;

  if (DEBUG) Serial.println("hi");

}

void loop(){


  leftSeq.takeAction();
  rightSeq.takeAction();

  leds.show();

  handleXBee();


} // end of loop


////
// Called when data for this deviceis recieved
//
// packets contain
// number of sequences
// each sequence starts with the number of steps
// then a list of steps
void handleData(int length, uint8_t data[]) {

  xBeeTimoutMs = millis() + xbeeTimeoutDurationMs;

  Serial.print("IN:");
  Serial.print(length);
  Serial.print(": ");

  for (int i = 0; i < length; i++) {
    Serial.print(data[i]);
    Serial.print(", ");
  }
  Serial.println("");


  if (length < 1) {
    return; // nothing to do
  }

  if (length == 1) {

    /*
     *  0 : all off
     *  1 : all on
     *  2 : switch color
     *  4-9 : speed control
     *  10-255 : brightness
     */


    int input = data[0];

    if (0 == input) {
      fillSequences(0);
    }

    else if (1 == input) {
      fillSequences(1);
    }

    else if (2 == input) {
      colorIsWhite++;
      colorIsWhite %= 2;
      left.colorIsWhite=colorIsWhite;
      right.colorIsWhite=colorIsWhite;

    }

    else if (10 > input) {
      setSpeed(input);
    }

    else {
      setBrightness(input);
    }


  } 
  else if (length == 2) {

    fillSequences(data[1]);
  }
  else {

    int whichLed = data[0];

    int currentIndex = 0;

    int sequenceCount = data[currentIndex++];

    if (0 == whichLed ) {
      for (int i=0; i < sequenceCount; i++) {
        rightStepCount = data[currentIndex++];
        for (int step = 0; step < rightStepCount; step++) {
          rightStep[step*stepValues+0] = data[currentIndex++]*100;     
          rightStep[step*stepValues+1] = data[currentIndex++];     
          rightStep[step*stepValues+3] = data[currentIndex++];     
        }
      }
    }
    else {
      for (int i=0; i < sequenceCount; i++) {
        leftStepCount = data[currentIndex++];
        for (int step = 0; step < leftStepCount; step++) {
          leftStep[step*stepValues+0] = data[currentIndex++]*100;     
          leftStep[step*stepValues+1] = data[currentIndex++];     
          leftStep[step*stepValues+3] = data[currentIndex++];     
        }
      }
    }
  }

  Serial.print(rightStepCount);
  Serial.print(":");

  for (int i = 0; i < rightStepCount*stepValues; i++) {
    Serial.print(rightStep[i]);
    Serial.print(", ");
  }

  Serial.println(";");

  //maybe
  rightSeq.stepCounter=0;
  rightSeq.stepCount=rightStepCount;

  //rightSeq.stepStartTime = millis();
  rightSeq.incrAction();

  Serial.println(rightSeq.step[0]);

  //maybe
  leftSeq.stepCounter=0;
  leftSeq.stepCount=leftStepCount;

  //rightSeq.stepStartTime = millis();
  leftSeq.incrAction();

  Serial.println(leftSeq.step[0]);

  return; 

}

void fillSequences (int sequence) {

  rightStepCount  = defaultSteps[sequence][0];
  leftStepCount  = defaultSteps[sequence][0];

  for (int i = 0; i < rightStepCount*3 ; i++) {
    rightStep[i] = defaultSteps[sequence][i+1];   // +1 bc the first value is the count     
    leftStep[i] = defaultSteps[sequence][i+1];   // +1 bc the first value is the count     
  }


}

//todo impl
void setSpeed(int newSpeed) {

  speed = newSpeed;

}

//todo impl
void setBrightness(int newBrightness) {

  left.analogHighValue = newBrightness;
  right.analogHighValue = newBrightness;

}

////
// XBEE FUNCTION TO SHOW EVENTS
void flashLed(int pin, int times, int wait) {

  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);

    if (i + 1 < times) {
      delay(wait);
    }
  }
}

////
// XBEE FUNCTION TO HANDLE EVENTS
void handleXBee() {

  if (isXBeeTimeout()) {
    timeOutResetToDefault();     
  }

  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {
    // got something

    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      // got a zb rx packet

      // now fill our zb rx class
      xbee.getResponse().getZBRxResponse(rx);

      if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
        // the sender got an ACK
        flashLed(statusLed, 10, 10);
      }
      else {
        // we got it (obviously) but sender didn't get an ACK
        flashLed(errorLed, 2, 20);
      }

      Serial.println(rx.getDataLength());

      handleData(rx.getDataLength(), rx.getData());

    }
    else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
      xbee.getResponse().getModemStatusResponse(msr);
      // the local XBee sends this response on certain events, like association/dissociation

      if (msr.getStatus() == ASSOCIATED) {
        // yay this is great.  flash led
        flashLed(statusLed, 10, 10);
      }
      else if (msr.getStatus() == DISASSOCIATED) {
        // this is awful.. flash led to show our discontent
        flashLed(errorLed, 10, 10);
      }
      else {
        // another status
        flashLed(statusLed, 5, 10);
      }
    }
    else {
      // not something we were expecting
      flashLed(errorLed, 1, 25);    
    }
  } 
  else if (xbee.getResponse().isError()) {
    //nss.print("Error reading packet.  Error code: ");  
    //nss.println(xbee.getResponse().getErrorCode());
  }

  return;

}

boolean isXBeeTimeout() {  
  if (xBeeTimoutMs < millis()) {

    if (DEBUG) Serial.println("TIMEOUT");

    return true;


  }

  return false;

}

void timeOutResetToDefault() {
  setBrightness(defaultBrightness);

  uint8_t data[] = s{
    0, 5  };
  handleData(2, data );
  return; 
}










