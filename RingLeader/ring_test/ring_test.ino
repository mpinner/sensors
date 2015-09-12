/** kyle made a beacon. called the RingLeader. It is great for eerything
 * code : matt pinner
 * share : CC BY-NC-SA 2.0
 */

#include <Adafruit_NeoPixel.h>
#include <HSB_to_RGB.h>
#include <Bounce.h>
#include <Encoder.h>

// enables if (DEBUG) Serial communication
#define DEBUG false

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
#define LEDS_COUNT_UPPER 318 
#define LEDS_COUNT_LOWER 318
#define LEDS_PER_SEGMENT 53 // above count divide by 6

#define FADEOUT 8


#define STROBE_BRIGHTNESS 32

#define STROBE_TIMEOUT_MS 1500
#define hold_TIMEOUT_MS 15000


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
int MAX_CENTER_MODE = 6;

int endMode =0;
int MAX_END_MODE = 5;

boolean pulse = false;
boolean hold = false;


boolean shouldCheckDuration = false;

int GammaE[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
  2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
  6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11,
  11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18,
  19, 19, 20, 21, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28,
  29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40,
  40, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 54,
  55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
  71, 72, 73, 74, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 88, 89,
  90, 91, 93, 94, 95, 96, 98, 99,100,102,103,104,106,107,109,110,
  111,113,114,116,117,119,120,121,123,124,126,128,129,131,132,134,
  135,137,138,140,142,143,145,146,148,150,151,153,155,157,158,160,
  162,163,165,167,169,170,172,174,176,178,179,181,183,185,187,189,
  191,193,194,196,198,200,202,204,206,208,210,212,214,216,218,220,
  222,224,227,229,231,233,235,237,239,241,244,246,248,250,252,255};




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

  switch (centerMode%6) {
  case 0 : 
    makeRingRainbow();
    break;

  case 1 :
    rainbowRings();
    break;

  case 2 : 
    rainbowSpirals();
    break;

  case 3 : 
    rainbowDots(1);
    break;

  case 4 : 
    makeStripeRainbow();
    break;

  default:
    rainbow(1);
  }


  // if someone wants to a pulse
  if (pulse) {
    //pulseWhite();
    strobe();
    pulse = false; 
  }


  // if someone wants to a pulse
  if (hold) {
    //pulseWhite();
    holdStrobe();
    hold = false; 
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
  centerMode++;
  return;

}

void pulseWhite() {

  int lightness = 255;

  for(int i=0; i<stripUpper.numPixels(); i++) {
    stripUpper.setPixelColor(i, lightness, lightness, lightness);
    stripLower.setPixelColor(i, lightness, lightness, lightness);


  }
  myShow();
  delay(500);
  for(int i=0; i<stripUpper.numPixels(); i++) {
    stripUpper.setPixelColor(i, 0);
    stripLower.setPixelColor(i, 0);


  }
  myShow();
  return; 
}


void strobe() {


  long startTime = millis();

  int lightness = STROBE_BRIGHTNESS;

  while((millis() - STROBE_TIMEOUT_MS) > startTime) {

    int delayMs = (1000000+myEnc.read()*10) % 1000 + 1;


    for(int i=0; i<stripUpper.numPixels(); i++) {
      stripUpper.setPixelColor(i, lightness, lightness, lightness);
      stripLower.setPixelColor(i, lightness, lightness, lightness);
    }
    myShow();

    delay( delayMs);

    for(int i=0; i<stripUpper.numPixels(); i++) {
      stripUpper.setPixelColor(i, 0);
      stripLower.setPixelColor(i, 0);
    }

    if (myShow()) {
      return;
    }

    delay(delayMs);

  }

  return; 
}


void holdStrobe() {


  long startTime = millis();

  int lightness = STROBE_BRIGHTNESS;

  while((millis() - hold_TIMEOUT_MS) > startTime) {

    int delayMs = (1000000+myEnc.read()*10) % 1000 + 100;


    for(int i=0; i<stripUpper.numPixels(); i++) {
      stripUpper.setPixelColor(i, lightness, 0, 0);
      stripLower.setPixelColor(i, 0, 0, lightness);
    }
    myShow();

    delay( delayMs);

    for(int i=0; i<stripUpper.numPixels(); i++) {
      stripUpper.setPixelColor(i, 0, 0, lightness);
      stripLower.setPixelColor(i, lightness, 0, 0);
    }

    if (myShow()) {
      return;
    }

    delay(delayMs);

  }

  return; 
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

      stripUpper.setPixelColor(newIndex-1%stripUpper.numPixels(), 0);
      stripLower.setPixelColor(newIndex-1%stripUpper.numPixels(), 0);

      makeColor(newIndex,i);
    }

    //i = i+5;

    // stripLower.setPixelColor(i-LEDS_COUNT_LOWER-1, 0);
    //  stripLower.setPixelColor(i-LEDS_COUNT_LOWER, Wheel((i) & 255));

    if (myShow()) {
      return;
    }

    //delayMicroseconds(1);
  }
}







// make a ring color
void makeRingRainbow() {

  for (int i = 0 ; i < 106; i++ ) { 
    for(int i=0; i<stripUpper.numPixels(); i++) {
      stripUpper.setPixelColor(i, 0);
      stripLower.setPixelColor(i, 0);
    }    
    makeFadedRings(i, i*2);



    if (myShow()) return;

    //int delayMs = (1000000+myEnc.read()*10) % 10 + 1;
    //delayMicroseconds(delayMs);


  }



  return;
}




// make a ring color
void makeStripeRainbow() {

  for (int i = 0 ; i < 255; i++ ) { 
    for(int i=0; i<stripUpper.numPixels(); i++) {
      stripUpper.setPixelColor(i, 0);
      stripLower.setPixelColor(i, 0);
    }    
    makeStripe(i, i*4, -1);



    if (myShow()) return;
    delay(33);

    //int delayMs = (1000000+myEnc.read()*10) % 10 + 1;
    //delayMicroseconds(delayMs);


  }



  return;
}



// make a ring color
void makeFadedRings(int i, unsigned int hue) {
  makeFadedRings(i, hue, -1);  
}

// make a ring color
void makeFadedRings(int i, unsigned int hue, int lightness) {

  if (lightness < 0) {
    lightness = (10000+myEnc.read()*5);
    lightness = (lightness % 220 ) + 30;
  }

  int fadeAmount = lightness / FADEOUT;

  for(int j=0; j<FADEOUT; j++) {
    makeRing(i-j, hue, GammaE[lightness-(j*fadeAmount)]);
  }

  return;
}



// make a ring color
void makeRing(int i, unsigned int hue, int lightness) {

  i = i % (LEDS_PER_SEGMENT*2);
  if (hue > 255) hue = hue % 255;

  if (lightness < 0) {
    lightness = (10000+myEnc.read()*5);
    lightness = (lightness % 250 ) + 5;
  }

  for (int k = 0 ; k< 6; k++ ) {

    int ledIndex = i + (k * LEDS_PER_SEGMENT);

    if (i > LEDS_PER_SEGMENT) {
      ledIndex -= LEDS_PER_SEGMENT;
      setUpperPixelColor(ledIndex, hue, lightness);
    } 
    else {

      setLowerPixelColor(LEDS_COUNT_LOWER - ledIndex, hue, lightness);
    }
  }

  return;
}




// make a ring color
void makeStripe(int i, unsigned int hue, int lightness) {

  i = i % 6;
  if (hue > 255) hue = hue % 255;

  if (lightness < 0) {
    lightness = (10000+myEnc.read()*5);
    lightness = (lightness % 250 ) + 5;
  }

  for (int k = 0 ; k < LEDS_PER_SEGMENT; k++ ) {
    int ledIndex = k + (i * LEDS_PER_SEGMENT);
    // int newIndex = (k*LEDS_PER_SEGMENT);
    setUpperPixelColor(ledIndex, hue, lightness);
    setLowerPixelColor(LEDS_COUNT_LOWER-ledIndex, hue, lightness);
  }

  return;
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

  return colorOut & 255;
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

  int colors[3] = {
    0,0,0            };
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


  // debug LED
  if ( !dialButtonBouncer.read() || !endButtonBouncer.read()) {
    digitalWrite(PIN_LED, HIGH);
  } 
  else {
    digitalWrite(PIN_LED, LOW);
  }

  // encoder button updates
  if ( dialButtonBouncer.fallingEdge()) {
    centerMode++;
    centerMode = centerMode% MAX_CENTER_MODE;
    if (DEBUG) Serial.print("CLICK CENTER : ");
    if (DEBUG) Serial.println(centerMode);
    shouldExit = true;
  }

  // end button updates
  if ( endButtonBouncer.fallingEdge()) {
    endMode++;
    endMode = endMode% MAX_END_MODE;
    if (DEBUG) Serial.print("CLICK END : ");
    if (DEBUG) Serial.println(endMode);
    shouldExit = true;
    shouldCheckDuration = true;
    pulse = true;
  }

  // end button updates
  if ( shouldCheckDuration  && false == endButtonBouncer.read() && endButtonBouncer.duration() > 500 ) {
    if (DEBUG) Serial.print("CLICK END LONG : ");
    if (DEBUG) Serial.println(endMode);
    shouldCheckDuration = false;
    shouldExit = true;
    pulse = false;
    hold = true;
  }

  stripUpper.show();
  stripLower.show();

  return shouldExit;
};


void doDelay() {
  delay(endMode);
}



















