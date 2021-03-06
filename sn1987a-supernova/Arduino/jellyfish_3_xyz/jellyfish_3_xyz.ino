#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6

#define DEBUG false

#define HEIGHT 0
#define ANGLE 1
#define RADIUS 2


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
int effectMinDuration[] = {1000, 1000, 500};
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
  Serial2.begin(57600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


int colorDefault[] = {128, 64, 128};
int effectColor[] = {colorDefault[0], colorDefault[1], colorDefault[2]};

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

  for (uint16_t led = 0; led < strip.numPixels(); led++) {


    strip.setPixelColor(led,
                        getHeightColor(led),
                        getAngleColor(led),
                        getRadiusColor(led)
                       );

  }

  strip.show();

  delay(1);

  if (DEBUG) delay(100);
}

int getHeightColor(int led) {
  return effectColor[HEIGHT] * abs(effectProgress[HEIGHT] - getHeight(led));

}

int getAngleColor(int led) {
  return effectColor[ANGLE] * abs(effectProgress[ANGLE] - getAngle(led));

}

int getRadiusColor(int led) {
  return effectColor[RADIUS] * abs(effectProgress[RADIUS] - getRadius(led));
}

void serialEvent() {

  while (Serial2.available()) {
    // get the new byte:
    char inChar = (char)Serial2.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }


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
          resetEffect(effect);
        }

        break;
      case '0':
        resetEffect(HEIGHT);
        break;
      case '1':
        resetEffect(ANGLE);
        break;
      case '2':
        resetEffect(RADIUS);
        break;
      case 'x':
        setColor(HEIGHT, inputString[1]);
        break;
      case 'y':
        setColor(ANGLE, inputString[1]);
        break;
      case 'z':
        setColor(RADIUS, inputString[1]);
        break;
    }

    inputString = "";
  }

  return;
}


void resetEffect(int effect) {
  effectStart[effect] = millis();
  effectDuration[effect] = effectMinDuration[effect];
  effectColor[effect] = colorDefault[effect];
  return;

}

void setColor(int effect, char color) {
  effectColor[effect] = color;
  return;
}

