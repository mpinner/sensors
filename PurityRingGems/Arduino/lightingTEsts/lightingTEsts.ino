#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>


#define PIN 6
#define BUTTON_PIN 1
#define LED_PIN 13



// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


/*
FFDAB9
 F3C7A9
 508679
 E6E6FA
 D6AEDA
 F9F1FE
 F1BA46
 */
/*

 uint32_t color0 = strip.Color(0xFF, 0xDA, 0xB9);
 uint32_t color1 = strip.Color(0xF3, 0xC7, 0xA9);
 uint32_t color2 = strip.Color(0x50, 0x86, 0x79);
 uint32_t color3 = strip.Color(0xE6, 0xE6, 0xFA);
 uint32_t color4 = strip.Color(0xD6, 0xAE, 0xDA);
 uint32_t color5 = strip.Color(0xF9, 0xF1, 0xFE);
 uint32_t color6 = strip.Color(0xF1, 0xBA, 0x46);
 
 uint32_t color[] = {
 color0,
 color1,
 color2,
 color3,
 color4,
 color5,
 color6
 };
 
 */


uint32_t color[] = {
  strip.Color(0xFF, 0x00, 0xcc),
  strip.Color(0x00, 0xFF, 0x44),
  
/*  strip.Color(0x50, 0x86, 0x79),
  strip.Color(0xE6, 0xE6, 0xFA),
  strip.Color(0xD6, 0xAE, 0xDA),
  */
  strip.Color(0xF9, 0xF1, 0xFE),
  strip.Color(0xF1, 0xBA, 0x46)
  };


  int maxColors = 4;

int colorIndex = 0;

void incrColor() {
  colorIndex++;
  colorIndex = colorIndex % maxColors;
  return;

}

boolean changedColor = true;

// Instantiate a Bounce object
Bounce debouncer = Bounce(); 

int ledState = LOW;

void buttonAction() {
  debouncer.update();

  // Get the update value
  boolean stateChanged = debouncer.update();

  int state = debouncer.read();


  // Detect the falling edge
  if ( stateChanged && state == LOW ) {

    if ( ledState == LOW ) {
      ledState = HIGH;



    } 
    else {
      ledState = LOW;


      if (changedColor ) {
        changedColor = false;

        doAction();
      } 
      else {

        incrColor();
        changedColor = true;

        doWipe();
      }
    }
    digitalWrite(LED_PIN,ledState);

  }

}


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Setup the button
  pinMode(BUTTON_PIN,INPUT);
  // Activate internal pull-up
  digitalWrite(BUTTON_PIN,HIGH);

  // After setting up the button, setup debouncer
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(40);

  //Setup the LED
  pinMode(LED_PIN,OUTPUT);

}

void loop() {

  buttonAction();

//doWipe();


//flickr
//fireflies


//doAction();

}





void doAction() {


  // Send a theater pixel chase in...
  theaterChase(color[colorIndex], 50); // White

}


void doWipe() {
  // Some example procedures showing how to display to the pixels:
  colorWipe(color[colorIndex], 30); // Red


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
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 7; q++) {
      for (int i=0; i < strip.numPixels(); i=i+7) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+7) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }


}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}







