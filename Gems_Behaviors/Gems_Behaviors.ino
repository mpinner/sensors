
#include <hsv2rgb.h>


#include <elapsedMillis.h>


#include "Gem.h"
#include "TopShader.h"

#include "colors.h"

#include <Adafruit_NeoPixel.h>


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

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(85, PIN, NEO_GRB + NEO_KHZ800);



Shader* behavior[] = {
  new TopShader()
};


uint32_t color[] = {
  strip.Color(0xFF, 0x00, 0xcc),
  strip.Color(0x00, 0xFF, 0x44),
  strip.Color(0xF9, 0xF1, 0xFE),
  strip.Color(0xF1, 0xBA, 0x46),  
  strip.Color(0x50, 0x86, 0x79),
  strip.Color(0xE6, 0xE6, 0xFA),
  strip.Color(0xD6, 0xAE, 0xDA),
  strip.Color(0x00, 0xAE, 0xDA)
  };

  int colorCount = 8;

uint32_t incomingColor;

boolean hasIncomingColor = false;

long loopElapsed;
int fps;


Gem gems[8];
int gemCount = 8;

// led count of strip within each gem
int gemPixelCount = 85;


typedef void (*FunctionPointer) (Gem*);
FunctionPointer animation[10] = {
  shadeBasicHit, shadeFade, shadeFall, shadeTwinkle};
int animationCount = 4;


// serial event globals
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void setup() {

  // config led pins
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // setup OSC listener
  SLIPSerial.begin(9600);   // set this as high as you can reliably run on your platform

  // fps calculation
  loopElapsed = millis();


  CRGB rgb = CRGB();

  TopShader* topShader = new TopShader();

  for (int i = 0; i < gemCount; i++ ) {
    gems[i] = Gem(i, gemPixelCount, &strip, behavior[0]);
    gems[i].setColor(color[i%colorCount]);
  }

  return; 
}




void loop() {


  pollForNewOscMessages();

  updateGems();

  strip.show();

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


void shadeBasicHit(Gem* gem) {
  return;
}


void shadeFade(Gem* gem) {
  return;
}

void shadeFall(Gem* gem) {
  return;
}

void shadeTwinkle(Gem* gem) {
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


//called after matching '/output/1' 
void routeColor(OSCMessage &msg, int patternOffset){
  Serial.println("Match: '/color'");

  if (msg.isInt(0) && msg.isInt(1) && msg.isInt(2) ){

    int r = msg.getInt(0);
    int g = msg.getInt(1);
    int b = msg.getInt(2);

    setIncomingColor(r, g, b);

  } 
  else {

    int matchingColorIndex = matchColor(msg, patternOffset);
    setIncomingColorIndex(matchingColorIndex);
    Serial.print("color matched #");
    Serial.println();

hit(0);

    if (DEBUG) {
      Serial.println("no readable color data");
    }
  }

  //parse new color data 
}


int matchColor(OSCMessage &msg, int patternOffset) {  
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

void setIncomingColorIndex(int colorIndex) {
  hasIncomingColor = true;
  incomingColor = color[colorIndex];

}


//called after matching '/output/1' 
void routeInfo(OSCMessage &msg, int patternOffset){
  Serial.println("Match: '/info'");
  Serial.print("fps: ");
  Serial.println(fps);

}

void pushIncomingColor(int gemIndex) {

  if (hasIncomingColor) {
    hasIncomingColor = false;
    gems[gemIndex].setColor(incomingColor);

  }

  return;
}

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










