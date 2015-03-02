
#include <hsv2rgb.h>


#include <elapsedMillis.h>


#include "Gem.h"
#include "TopShader.h"
#include "TwinkleShader.h"
#include "GlowShader.h"
#include "PulsingShader.h"
#include "RisingShader.h"
#include "MultiColorShader.h"
#include "MultiGlowShader.h"
#include "MultiTwinkleShader.h"


#include "colors.h"

#include <OctoWS2811.h>



#include <OSCBundle.h>
#include <OSCBoards.h>

#ifdef BOARD_HAS_USB_SERIAL
#include <SLIPEncodedUSBSerial.h>
SLIPEncodedUSBSerial SLIPSerial( thisBoardsSerialUSB );
#else
#include <SLIPEncodedSerial.h>
SLIPEncodedSerial SLIPSerial(Serial1);
#endif

#define DEBUG true


////
// LED CONFIG
//const int ledsPerStrip = 90;
const int ledsPerStrip = 100;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);



Shader* behavior[] = {
  new TopShader(),
  new TwinkleShader(),
  new GlowShader(),
  new PulsingShader(),
  new RisingShader(),
  new MultiColorShader(),
  new MultiGlowShader(),
  new MultiTwinkleShader()
  };

  int behaviorCount = 8; 


static const uint32_t behaviorColor[8][2] = {
  { 
    0xD6AEDA, //lavender
    0xF1AF92 //  peach
  }
  ,
  { 
    0xD6AEDA, //lavender
    0xF1AF92 //  peach
  }
  ,
  { 
    0xD6AEDA, //lavender
    0xF1AF92 //  peach
  }
  ,
  { 
    0xD6AEDA, //lavender
    0xF1AF92 //  peach
  }
  ,
  { 
    0xD6AEDA, //lavender
    0xF1AF92 //  peach
  }
  ,
  { 
    0xD6AEDA, //lavender
    0xF1AF92 //  peach
  }
  ,
  { 
    0xD6AEDA, //lavender
    0xF1AF92 //  peach
  }
  ,
  { 
    0xD6AEDA, //lavender
    0xF1AF92 //  peach
  }
  ,

};





uint32_t incomingColor;

boolean hasIncomingColor = false;

long loopElapsed;
int fps;


Gem gems[8];
int gemCount = 8;

// todo : pixels per-gem
// led count of strip within each gem
int gemPixelCount = 100;


// serial event globals
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void setup() {

  // config led pins
  leds.begin();
  leds.show();


  // setup OSC listener
  SLIPSerial.begin(9600);   // set this as high as you can reliably run on your platform

  // fps calculation
  loopElapsed = millis();

  CRGB rgb = CRGB();

  for (int i = 0; i < gemCount; i++ ) {
    gems[i] = Gem(i, gemPixelCount, &leds, behavior[0]);
    gems[i].setColor(color[6]);
    gems[i].setSecondaryColor(color[7]);

  }

  return; 
}




void loop() {


  pollForNewOscMessages();

  updateGems();

  leds.show();

  fps = 1000 / (millis() - loopElapsed);
  loopElapsed = millis();


  return; 
}


// check serial for pending hits
void pollForNewOscMessages() {
  // process all serial stream

  // NotSerialEvent();

  OSCMessage msg;  
  int size;

  // to test hit all
  for (int i = 0; i<gemCount; i++){
    // hit(i);
  }

  if( (size =SLIPSerial.available()) > 0) {
    while(!SLIPSerial.endofPacket())
      if( (size =SLIPSerial.available()) > 0)
      {
        while(size--) {
          msg.fill(SLIPSerial.read());
        }
      }


    if(!msg.hasError()) {


      msg.route("/gem", routeGem);

      msg.route("/color", routeColor);
      msg.route("/behavior", routeBehavior);
      msg.route("/info", routeInfo);

    }

    msg.empty();

  }


  return;
}

void hit(int gemIndex) {
  if (DEBUG) {

    Serial.print("HIT:");
    Serial.println(gemIndex);
    Serial.print("fps: ");
    Serial.println(fps);
  }
  pushIncomingColor(gemIndex);
  gems[gemIndex].hit();
}


void updateGems() {

  // to test hit all
  for (int i = 0; i<gemCount; i++){
    gems[i].animate();
  }


  return;    
}



/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void NotSerialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 

    int gemIn = (int)inChar - 48;

    if ((gemIn > -1) && ( gemIn < 9)) {
      if (DEBUG) {
        Serial.print("HIT: ");
        Serial.println(inChar);
      }
      hit(gemIn);
    }

    Serial.print(inChar);  
  }

}



//called after matching '/0'
void routeGem(OSCMessage &msg, int patternOffset){
  //Serial.println("Match: '/gem'");
  msg.route("/1", routeGemOne, patternOffset);
  msg.route("/2", routeGemTwo, patternOffset);

}


//called after matching '/0'
void routeGemOne(OSCMessage &msg, int patternOffset){
  //Serial.println("Match: '/gem/1'");
  //msg.route("/1", routeGemOne, patternOffset);
  matchGemHit(msg, patternOffset, 0);
}


//called after matching '/0'
void routeGemTwo(OSCMessage &msg, int patternOffset){
  //Serial.println("Match: '/gem/2'");
  //msg.route("/2", routeGemOne, patternOffset);
  matchGemHit(msg, patternOffset, 4);
}


void matchGemHit(OSCMessage &msg, int patternOffset, int gemOffset) {   

  if ( 0 < msg.match("/1",patternOffset)) {
    hit(gemOffset+0);
  }
  if ( 0 < msg.match("/2",patternOffset)) {
    hit(gemOffset+1);
  }
  if ( 0 < msg.match("/3",patternOffset)) {
    hit(gemOffset+2);
  }
  if ( 0 < msg.match("/4",patternOffset)) {
    hit(gemOffset+3);
  }

}





/**
 * OSC ROUTE : /info
 */
void routeInfo(OSCMessage &msg, int patternOffset){
  Serial.print("fps: ");
  Serial.println(fps);

}



/**
 * OSC parsing helper : returns number portion from touchOsc buttons
 */
int matchNumbers(OSCMessage &msg, int patternOffset) {  
  if ( 0 < msg.match("/1/1",patternOffset)) {
    return 0;
  }
  if ( 0 < msg.match("/2/1",patternOffset)) {
    return 1;
  }
  if ( 0 < msg.match("/3/1",patternOffset)) {
    return 2;
  }
  if ( 0 < msg.match("/4/1",patternOffset)) {
    return 3;
  }
  if ( 0 < msg.match("/5/1",patternOffset)) {
    return 4;
  }
  if ( 0 < msg.match("/6/1",patternOffset)) {
    return 5;
  }
  if ( 0 < msg.match("/7/1",patternOffset)) {
    return 6;
  }
  if ( 0 < msg.match("/8/1",patternOffset)) {
    return 7;
  }

  return 0;
} 


/**
 * OSC ROUTE : /behavior
 */
void routeBehavior(OSCMessage &msg, int patternOffset){
  if (msg.isFloat(0)){

    int behavior = (int) map (msg.getFloat(0)*100, 0, 100, 0, behaviorCount);
    setBehaviors(behavior);

    return;
  } 

  int matchingIndex = matchNumbers(msg, patternOffset);
  setBehaviors(matchingIndex);
  if (DEBUG) {
    Serial.print("behavior matched #");
    Serial.println(matchingIndex);
    hit(0);
  }
}


/**
 * sets behavior of all gems
 */
void setBehaviors(int matchingIndex) {
  for (int i = 0; i < gemCount; i++ ) {
    gems[i].setShader(behavior[matchingIndex%behaviorCount]);
    
    // set colors
    
  }
}



////
// COLORS

/**
 * OSC ROUTE : /color
 *  - accepts index in touchOsc format
 *  - accepts r g b values as ints
 */
void routeColor(OSCMessage &msg, int patternOffset){

  if (msg.isInt(0) && msg.isInt(1) && msg.isInt(2) ){

    int r = msg.getInt(0);
    int g = msg.getInt(1);
    int b = msg.getInt(2);

    setIncomingColor(r, g, b);

  } 
  else {

    int matchingColorIndex = matchNumbers(msg, patternOffset);
    setIncomingColorIndex(matchingColorIndex);
    if (DEBUG) {

      Serial.print("color matched #");
      Serial.println(matchingColorIndex);

      //hit(0);

      setAllColors(color[matchingColorIndex], color[(matchingColorIndex+1)%colorCount]);

      Serial.println("no readable color data");
    }
  }

  //parse new color data 
}


/**
 * set the gem color if one is queued
 */
void pushIncomingColor(int gemIndex) {

  // check if there is a new color waiting
  if (hasIncomingColor) {
    hasIncomingColor = false;
    gems[gemIndex].setSecondaryColor(gems[gemIndex].getColor());
    gems[gemIndex].setColor(incomingColor);
  }

  return;
}

void setGemColors(int gemIndex, int color, int secondaryColor) {
  gems[gemIndex].setSecondaryColor(secondaryColor);
  gems[gemIndex].setColor(color); 
  return;
}


void setAllColors(int color, int secondaryColor) {
  for (int i = 0; i < gemCount; i++ ) {
    setGemColors(i, color, secondaryColor);
  }
  return;
}

/**
 * queue a color for use upon next gem hit
 */
void setIncomingColorIndex(int colorIndex) {
  hasIncomingColor = true;
  incomingColor = color[colorIndex];

}

/**
 * queue a color for use upon next gem hit
 */
void setIncomingColor(int r, int g, int b ) {

  hasIncomingColor = true;
  incomingColor = ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;

  if (DEBUG) {
    Serial.print("Color: ");
    Serial.print(r);
    Serial.print(" ");
    Serial.print(g);
    Serial.print(" ");
    Serial.println(b);
    Serial.print("  :  ");
    Serial.println(incomingColor);
  }

  return;

}
















