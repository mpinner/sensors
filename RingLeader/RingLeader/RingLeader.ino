/** kyle made a beacon. called the RingLeader. It is great for eerything
 * code : matt pinner
 * share : CC BY-NC-SA 2.0
 */

#include <Adafruit_NeoPixel.h>
#include <HSB_to_RGB.h>
#include <Bounce.h>
#include <Encoder.h>

// enables if (DEBUG) Serial communication
#define DEBUG true

// PINS

// end button
#define PIN_END_SWITCH 25
#define PIN_END_SWITCH_AUX 24

// quad encoder dial on the handle with a press button
#define PIN_ENCODER_SWITCH 0
#define PIN_ENCODER_LEFT 21
#define PIN_ENCODER_RIGHT 23

// debug LED
#define PIN_LED 13

// data lines for LED strips
#define PIN_LEDS_UPPER 2
#define PIN_LEDS_LOWER 3


// led configuration
#define LEDS_PER_SEGMENT 53
#define LEDS_COUNT_UPPER 320 
#define LEDS_COUNT_LOWER 320

// setup led pixel frame buffers
Adafruit_NeoPixel stripUpper = Adafruit_NeoPixel(LEDS_COUNT_UPPER, PIN_LEDS_UPPER, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripLower = Adafruit_NeoPixel(LEDS_COUNT_LOWER, PIN_LEDS_LOWER, NEO_GRB + NEO_KHZ800);

// easy of colors
HSB_to_RGB hsb = HSB_to_RGB();

// encoder setup
Encoder myEnc(PIN_ENCODER_LEFT, PIN_ENCODER_RIGHT);
long oldPosition  = -999;


// debounce timers for encoder and end buttons
Bounce dialButtonBouncer = Bounce(PIN_ENCODER_SWITCH, 5); 
Bounce endButtonBouncer = Bounce(PIN_END_SWITCH, 5); 



// states for various sequencing and button manipulations
int centerMode = 0;
int MAX_CENTER_MODE = 5;

int endMode =0;
int MAX_END_MODE = 5;

boolean pulse = false;

boolean shouldCheckDuration = false;



// SETUP
void setup() {
  if (DEBUG) Serial.begin(9600);
  if (DEBUG) Serial.println("Basic Encoder Test:");

  // debug led
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  // buttons
  pinMode(PIN_ENCODER_SWITCH, INPUT_PULLUP);
  pinMode(PIN_END_SWITCH, INPUT_PULLUP);

  // init led strip buffers to 'off'
  stripUpper.begin();
  stripUpper.show(); 
  stripLower.begin();
  stripLower.show(); 

}


// LOOP
void loop() {
  
  // default starting mode !
  if (0 == (endMode%2)) {
    rainbow(1);
  } 
  else {

    //  rainbowDots(1);
    // rainbowRings();
    rainbowSpirals();
  }

  // if someone wants to a pulse
  if (pulse) {
    rainbowRings();
    // end pulse
    pulse = false; 
  }

  if (DEBUG) {
    long newPosition = myEnc.read();
    if (newPosition != oldPosition) {
      oldPosition = newPosition;
      if (DEBUG) Serial.println(newPosition);
    }  
  }  
  
}


////
///
//
// SEQUENCE programs are built out from here 
//
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<255; j++) {

    for(i=0; i<stripUpper.numPixels(); i++) {
      makeColor(i,i+j);
    }
    if (myShow()) {
      return;
    }
    doDelay();
  }
}


void rainbowDots(uint8_t wait) {
  uint16_t  j;

  for(int i=0; i<stripUpper.numPixels(); i++) {
    stripUpper.setPixelColor(i, 0);
  }

  for(unsigned int i=0; i<stripUpper.numPixels(); i++) {
    stripUpper.setPixelColor(i-1, 0);
    stripLower.setPixelColor(i-1, 0);

    makeColor(i,i);

    // stripLower.setPixelColor(i-LEDS_COUNT_LOWER-1, 0);
    //  stripLower.setPixelColor(i-LEDS_COUNT_LOWER, Wheel((i) & 255));

    if (myShow()) {
      return;
    }

    doDelay();
  }
}



void rainbowSpirals() {
  rainbowRings(LEDS_PER_SEGMENT+10);
}


void rainbowRings() {
  rainbowRings(LEDS_PER_SEGMENT);
}


void rainbowRings(uint8_t segment) {
  uint16_t  j;

  //  for(int i=0; i<stripUpper.numPixels(); i++) {
  //    stripUpper.setPixelColor(i, 0);
  //  }

  for(unsigned int i=0; i<stripUpper.numPixels(); i++) {

    for (int k = 0 ; k< 6; k++ ) {

      int newIndex = (i%segment)+(k*segment);

      stripUpper.setPixelColor(newIndex-1, 0);
      stripLower.setPixelColor(newIndex-1, 0);

      makeColor(newIndex,i, 249);
    }



    // stripLower.setPixelColor(i-LEDS_COUNT_LOWER-1, 0);
    //  stripLower.setPixelColor(i-LEDS_COUNT_LOWER, Wheel((i) & 255));

    if (myShow()) {
      return;
    }

    delayMicroseconds(100);
  }
}




////
///
//
// UTIL functions for building out sequences

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  int colorOut;
  if(WheelPos < 85) {
    colorOut = stripUpper.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    colorOut = stripUpper.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    colorOut = stripUpper.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  
  return colorOut & 255
}


boolean readEncoderButton() {
  return digitalRead(PIN_ENCODER_SWITCH) ;
}


// make both halves a color with brightness determined by the defaults
void makeColor(int i, unsigned int hue) {

  if (hue > 255) hue = hue % 255;
  int lightness = (10000+myEnc.read()*5);

  makeColor(i, hue, lightness);

}


// make both halves a color with brightness determined by the given
void makeColor(int i, unsigned int hue, int lightness) {

  if (hue > 255) hue = hue % 255;

  lightness = (lightness % 250 ) + 5;

  setUpperPixelColor(i, hue, lightness);
  setLowerPixelColor(i, hue, lightness);

}



void setUpperPixelColor(uint16_t n, uint32_t c, uint16_t b) {


  int colors[3] = {
    0,0,0            };

  hsb.getRGB((int)c, 255, b, colors);

  stripUpper.setPixelColor(n, colors[0], colors[1], colors[2]);

  return;

}



void setLowerPixelColor(uint16_t n, uint32_t c, uint16_t b) {

  int colors[3] = {0,0,0};

  hsb.getRGB((int)c, 255, b, colors);

  stripLower.setPixelColor(n, colors[0], colors[1], colors[2]);

  return;

}


/////
///
//
// SHOW
//
// should protect some things 
// call for each frame to keep sensor readings fresh
// returns true if the sequence should be interuptted. generally for the pulses and mode changes
boolean myShow() {
  
  // return condition
  boolean shouldExit = false;


  // handle button state
  boolean button1Changed = dialButtonBouncer.update();
  boolean button2Changed = endButtonBouncer.update();


  // Turn on or off the debug LED
  if ( !dialButtonBouncer.read() || !endButtonBouncer.read()) {
    digitalWrite(PIN_LED, HIGH);
  } 
  else {
    digitalWrite(PIN_LED, LOW);
  }
  

  if ( dialButtonBouncer.fallingEdge()) {
    centerMode ++;
    centerMode = centerMode% MAX_CENTER_MODE;
    if (DEBUG) Serial.print("CLICK CENTER : ");
    if (DEBUG) Serial.println(centerMode);
    shouldExit = true;
  }


  if ( endButtonBouncer.fallingEdge()) {
    endMode ++;
    endMode = endMode% MAX_END_MODE;
    if (DEBUG) Serial.print("CLICK END : ");

    if (DEBUG) Serial.println(endMode);

    shouldExit = true;

    shouldCheckDuration = true;

    pulse = true;
  }

  if ( shouldCheckDuration && false == endButtonBouncer.read()  && endButtonBouncer.duration() > 500 ) {

    if (DEBUG) Serial.print("CLICK END LONG : ");

    if (DEBUG) Serial.println(endMode);

    shouldCheckDuration = false;

  }



  stripUpper.show();
  stripLower.show();

  return shouldExit;

};


int doDelay() {

  delay(endMode);
}






