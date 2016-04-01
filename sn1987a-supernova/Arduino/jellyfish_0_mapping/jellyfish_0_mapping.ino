#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6

#define DEBUG false

Adafruit_NeoPixel strip = Adafruit_NeoPixel(128, PIN, NEO_GRB + NEO_KHZ800);

int rows = 8;
int columns = 16;

int ledsPerColumn = rows;
int ledsPerRow = columns;

double arcPercentPerColumn = 1 / 16.0;

int getColumn(int led) {
  return led / ledsPerColumn;
}
double getAngle(int led) {
  return getColumn(led) * arcPercentPerColumn;
}



double height[] = {
  0.125, 0.25, 0.375, 0.50, 0.625, 0.75, 0.875, 1.00,
  1.00, 0.875, 0.75, 0.625, 0.50, 0.375, 0.25, 0.125,
  0.125, 0.25, 0.375, 0.50, 0.625, 0.75, 0.875, 1.00,
  1.00, 0.875, 0.75, 0.625, 0.50, 0.375, 0.25, 0.125,
  0.125, 0.25, 0.375, 0.50, 0.625, 0.75, 0.875, 1.00,
  1.00, 0.875, 0.75, 0.625, 0.50, 0.375, 0.25, 0.125,
  0.125, 0.25, 0.375, 0.50, 0.625, 0.75, 0.875, 1.00,
  1.00, 0.875, 0.75, 0.625, 0.50, 0.375, 0.25, 0.125,
  0.125, 0.25, 0.375, 0.50, 0.625, 0.75, 0.875, 1.00,
  1.00, 0.875, 0.75, 0.625, 0.50, 0.375, 0.25, 0.125,
  0.125, 0.25, 0.375, 0.50, 0.625, 0.75, 0.875, 1.00,
  1.00, 0.875, 0.75, 0.625, 0.50, 0.375, 0.25, 0.125,
  0.125, 0.25, 0.375, 0.50, 0.625, 0.75, 0.875, 1.00,
  1.00, 0.875, 0.75, 0.625, 0.50, 0.375, 0.25, 0.125,
  0.125, 0.25, 0.375, 0.50, 0.625, 0.75, 0.875, 1.00,
  1.00, 0.875, 0.75, 0.625, 0.50, 0.375, 0.25, 0.125

};


double getHeight(int led) {

  return height[led];

}


double radii[] = {
  0.25, 0.50, 0.75, 1.0, 1.0, 0.75, 0.50, 0.25,
  0.50, 0.75, 1.0, 1.0, 0.75, 0.50, 0.25, 0.25,
  0.50, 0.25, 0.25, 0.50, 0.75, 1.0, 1.0, 0.75,
  1.0, 1.0, 0.75, 0.50, 0.25, 0.25, 0.50, 0.75,
  1.0, 0.75, 0.50, 0.25, 0.25, 0.50, 0.75, 1.0,
  0.75, 0.50, 0.25, 0.25, 0.50, 0.75, 1.0, 1.0,
  0.75, 1.0, 1.0, 0.75, 0.50, 0.25, 0.25, 0.50,
  0.25, 0.25, 0.50, 0.75, 1.0, 1.0, 0.75, 0.50,
  0.50, 0.75, 1.0, 1.0, 0.75, 0.50, 0.25, 0.25,
  0.50, 0.25, 0.25, 0.50, 0.75, 1.0, 1.0, 0.75,
  1.0, 1.0, 0.75, 0.50, 0.25, 0.25, 0.50, 0.75,
  1.0, 0.75, 0.50, 0.25, 0.25, 0.50, 0.75, 1.0,
  0.75, 0.50, 0.25, 0.25, 0.50, 0.75, 1.0, 1.0,
  0.75, 1.0, 1.0, 0.75, 0.50, 0.25, 0.25, 0.50,
  0.25, 0.25, 0.50, 0.75, 1.0, 1.0, 0.75, 0.50,
  0.25, 0.50, 0.75, 1.0, 1.0, 0.75, 0.50, 0.25

};


double getRadius(int led) {
  return radii[led];
}



int effectDuration = 2000;

long effectStart = 0;

double effectProgress = 0.0;


void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}




void loop() {

  long currentMs = millis();
  
  if (currentMs > ( effectStart + effectDuration) ) {
    effectStart = currentMs;
  }

  long msRemaining = (effectDuration + effectStart) - currentMs;

  effectProgress = msRemaining / (double)effectDuration;

  if (DEBUG) {
  Serial.print( " currentMs:" );
Serial.print( currentMs );
Serial.print( " effectStart:" );
Serial.print( effectStart );
Serial.print( " msRemaining:" );
Serial.print( msRemaining );
Serial.print( " effectProgress:" );
Serial.print( effectProgress );
Serial.println();
  }

double span = .3;

  for (uint16_t i = 0; i < strip.numPixels(); i++) {

    double place;
  
  //place = getHeight(i);
  //place = getAngle(i);
  place = getRadius(i);

 double distance = abs(effectProgress - place);

 //if (distance < span) {
   
      strip.setPixelColor(i, 128*distance, 0, 0);
   // } else {
   //   strip.setPixelColor(i, 0);
   // }
 
  }

  strip.show();

    delay(10);

    if (DEBUG) delay(100);
  }

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel(millis() & 255));
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
