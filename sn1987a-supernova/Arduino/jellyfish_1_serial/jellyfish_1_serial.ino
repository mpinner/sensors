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



int effectMaxDuration[] = {5000, 7000, 3000};
int effectMinDuration[] = {1000, 1000, 1000};
int effectDuration[] = {5000, 7000, 3000};
long effectStart[] = {0, 0, 0};

double effectProgress[] = {0.0, 0.0, 0.0};

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  inputString.reserve(200);
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}




void loop() {

  serialEvent();

  long currentMs = millis();

  for (int effect = 0 ; effect < 3; effect++) {

    if (currentMs > ( effectStart[effect] + effectDuration[effect]) ) {
      effectStart[effect] = currentMs;
    }

    if (effectDuration[effect] < effectMaxDuration[effect]) {
      effectDuration[effect]++;
    }

    long msRemaining = (effectDuration[effect] + effectStart[effect]) - currentMs;

    double percentRemaining = msRemaining / (double)effectDuration[effect];

   
      effectProgress[effect] = abs(sin(percentRemaining * TWO_PI));


  }



  /*
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
  */

  for (uint16_t i = 0; i < strip.numPixels(); i++) {


    strip.setPixelColor(i,
                        128 * abs(effectProgress[0] - getHeight(i)),
                        128 * abs(effectProgress[1] - getAngle(i)),
                        128 * abs(effectProgress[2] - getRadius(i))
                       );

  }

  strip.show();

  delay(1);

  if (DEBUG) delay(100);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }

  if (stringComplete) {
    stringComplete = false;
    switch (inputString[0])
    {
      case 'r' :
        for (int effect = 0 ; effect < 3; effect++) {
          effectStart[effect] = millis();
        }
        break;
      case '0':
        effectStart[0] = millis();
        effectDuration[0] = effectMinDuration[0];
        break;
      case '1':
        effectStart[1] = millis();
        effectDuration[1] = effectMinDuration[1];
        break;
      case '2':
        effectStart[2] = millis();
        effectDuration[2] = effectMinDuration[2];
        break;
    }



    inputString = "";
  }

  return;
}
