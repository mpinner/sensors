
/* matt : mpinner@gmail.com

intent : 
 the tail operates in 3 modes : 
  UNTOUCHED - rainbow fades
  TOUCHED - single leds rainbow fading. the position is a meter of how much capacitance (how much/tightly it is grabbed)
  GRABBED - generally if two hands grab the tail the metter with count all the way to the top and go into this crazy stroby rainbow mode
  
  the button : 
   - changes the brightness
   - rezeros the threshold on the capacitance metering

 pins : 
  0 - touch sensor
  1 - button
  2 - data for ws2801 leds
  3 - clock for ws2801 leds
  
  
  
  */
 

#include "SPI.h"
#include "WS2801.h"

#include <Bounce.h>

#define BUTTON 1
#define LED 13

#define TOUCH 0

long lastTouchRead = 0;
int touchReadRate = 50;
int touch = 0;
int touchSensitivity = 75;

int touchSenseBaseline = 1800;
int touchSenseMax = 0;

// Instantiate a Bounce object with a 5 millisecond debounce time
Bounce bouncer = Bounce( BUTTON, 5 ); 







/*****************************************************************************
 * Example sketch for driving WS2801 pixels
 *****************************************************************************/

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
// The colors of the wires may be totally different so
// BE SURE TO CHECK YOUR PIXELS TO SEE WHICH WIRES TO USE!
int dataPin = 2;
int clockPin = 3;
// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply


int sequenceMode = 0;
int maxSequence = 3;



// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
WS2801 strip = WS2801(10, dataPin, clockPin);

// Optional: leave off pin numbers to use hardware SPI
// (pinout is then specific to each board and can't be changed)
//WS2801 strip = WS2801(25);

void setup() {
  pinMode(BUTTON,INPUT_PULLUP);
  pinMode(LED,OUTPUT);

  strip.begin();

  // Update LED contents, to start they are all 'off'
  showAndRead();


  Serial.begin(9600);
}


void loop() {
  // Some example procedures showing how to display to the pixels

  // colorWipe(Color(255, 0, 0), 50);
  // colorWipe(Color(0, 255, 0), 50);
  // colorWipe(Color(128, 0, 255), 50);
  // rainbow(20);
  // rainbowCycle(20);

  setTailTouch(5) ;

}

void setTailTouch(uint8_t wait) {
  int i, j;
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel

    int touchI = strip.numPixels() - ( touch - touchSenseBaseline) / touchSensitivity;

    for (i=0; i < strip.numPixels(); i++) {

      if (i == touchI) {
        strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
      } 
      else {
        strip.setPixelColor(i, Color(0, 0, 0)); 
      }

      if (touchI > (strip.numPixels()-1))
        strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );

      if (touchI <= 0) {
        if (millis() % 1)
          strip.setPixelColor(i, Color(0, 0, 0));
        else

            strip.setPixelColor(i, Wheel( (((millis()%strip.numPixels()) * 256 / strip.numPixels()) + j) % 256) );
      }


      showAndRead();
      delay(wait);
    }
  }
}


void rainbow(uint8_t wait) {
  int i, j;

  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    showAndRead();
    delay(wait);
  }
}


// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;

  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    showAndRead();
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    showAndRead();
    delay(wait);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  int dimmer = 1 + (sequenceMode * 4);

  r /= dimmer;
  g /= dimmer;
  b /= dimmer;

  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{

  uint32_t color;

  if (WheelPos < 85) {
    color =  Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if (WheelPos < 170) {
    WheelPos -= 85;
    color =  Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170; 
    color =  Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }

  return color;
}

void readTouch() {


  if (lastTouchRead + touchReadRate > millis() ) {
    return;

  }



  touch = touchRead(TOUCH);

  if (touch < touchSenseBaseline) {
    touchSenseBaseline = touch;
  }
  if (touch > touchSenseMax) {
    touchSenseMax = touch;
  }



  Serial.println(touch);

  /* if (5000 < touch) {
   
   updateSequence();
   reportMode();   
   
   }*/

  lastTouchRead = millis();
  return; 
}

void readButton() {

  // Update the debouncer
  bouncer.update ( );

  // Get the update value
  int value = bouncer.read();

  // Turn on or off the LED
  if ( value == HIGH) {
    digitalWrite(LED, HIGH );
  } 
  else {
    digitalWrite(LED, LOW );
  }

  if(bouncer.fallingEdge()) {
    updateTouchBaseline();
    updateSequence();
    reportMode();
  }

}

void showAndRead() {
  readTouch();
  readButton();
  strip.show();   // write all the pixels out
  return;

}

void updateSequence() {
  sequenceMode++;
  sequenceMode %= maxSequence;
  return;

}

void updateTouchBaseline() {
  touchSenseBaseline = touch + (touchSensitivity*2);
  return;
}




void reportMode() {

  //  for (int i = 0; i < sequenceMode+1; i++) {
  colorWipe(Color(255, 255, 255), 5);
  colorWipe(Color(0, 0, 0), 10);
  colorWipe(Color(255, 255, 255), 20);


  // }


}














