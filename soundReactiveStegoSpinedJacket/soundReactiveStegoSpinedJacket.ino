// stegosaurus jacket 
// author: matt pinner
//
// 
// cc-sa-by
//
// code baorrowed heavily from other arduino examples from these libraries
#include <Adafruit_NeoPixel.h>
#include <ADJDS311.h>
#include <Wire.h>

// microphone (analog, ideally with automatic gain control)
#define MIC_PIN A9

// jacket spine led stripes
#define LED_PIN 2

// test board
// #define LED_PIN 14

Adafruit_NeoPixel strip = Adafruit_NeoPixel(19, LED_PIN, NEO_GRB + NEO_KHZ800);

int count = 5;

// dim the jacket for everyday use
double brightness = .1;

int volume = 0;

// some flags to control outputs
boolean shouldGamma = true;
boolean shouldSense = false;

RGBC color; //read the color

// gamma correction lookup table
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



int sensorLed_pin = 2; //LED on the ADJDS-311
//ADJDS311 colorSensor(sensorLed_pin);



void setup() {

//  Serial.begin(9600);

// this was fun to make the color sync with ones surrounds but it never worked robustly enough. i might try again with EMWA smoothing
/*
  //colorSensor.init();
  //colorSensor.ledOn(); //turn LED on

    //Calibrate white 
  //Need to hold white card in front (1-3mm) of it to calibrate from
  // colorSensor.calibrate(); 
*/

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {


    greyCycle(5);

// color sensing
  //  RGBC color = colorSensor.read(); //read the color


  /*  Serial.print(color.red);
   Serial.print(" | ");
   Serial.print(color.green);
   Serial.print(" | ");
   Serial.print(color.blue);
   Serial.print(" | ");
   Serial.println(color.clear);
   */
  //  lightLED(color); //send color to the LED

  // delay(200); //just here to slow down the serial output

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i<strip.numPixels(); i++) {


      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void greyCycle(uint8_t wait) {
  uint16_t i, j;


  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel

    //   volume = analogRead (MIC_PIN);
    //   brightness = (double)volume / (double)512;
    //   wait = wait * brightness; 
    for(i=0; i<strip.numPixels(); i++) {


      strip.setPixelColor(i, greyWheel((((256 -i) * 256 / strip.numPixels()) + j) & 255));
    }

    strip.show();
 // color = colorSensor.read(); //read the color
 // int colorDelay =  (2048 - color.red - color.blue) / 300; 

    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t greyWheel(byte WheelPos) {

  volume = analogRead (MIC_PIN);
  brightness = (double)volume / (double)1024;

  // return gammaGreyColor(256-WheelPos);
  // return greyColor(256-WheelPos);

  if(WheelPos < 128) {
    return greyColor(WheelPos * brightness );
  } 
  else  {
    WheelPos -= 127;
    return greyColor((128-WheelPos) * brightness );
  }
}

uint32_t greyColor(byte color) { 

  if (shouldSense)
    return sensedColor(color);


  if (shouldGamma) 
    return gammaGreyColor(color);

  return strip.Color(color, color, color);

}


uint32_t gammaGreyColor(byte color) { 
  color = GammaE[color];
  return strip.Color(color, color, color);
}



uint32_t sensedColor(byte brightness) { 

  int min = color.red; 

  if (color.green < min)
    min = color.green;

  if (color.blue < min)
    min = color.blue;

  min = min / 20 * 19;

  int red = color.red - min;
  int green = color.green - min;
  int blue = color.blue - min;

  return strip.Color(GammaE[brightness-red], GammaE[brightness-green], GammaE[brightness-blue]);


  //  return strip.Color(GammaE[brightness-color.red/4], GammaE[brightness-color.green/4], GammaE[brightness-color.blue/4]);
}











