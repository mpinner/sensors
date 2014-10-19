#include <elapsedMillis.h> //https://github.com/pfeerick/elapsedMillis/archive/master.zip
#include <SM.h>  //http://playground.arduino.cc/Code/SMlib
#include <Adafruit_NeoPixel.h> 


SM Simple(S1);

#define D1 0
#define D2 1
#define D3 2
#define D4 3
#define NUM_LEDS 441// 147 LED per side
#define LEDS_PER_SIDE 147 
#define DATA_PIN 4 //output pin on ATTiny85
//#define BTN_PIN 0 //input pin on ATTiny85
//#define BTN_DELAY 250 //add delay for debounce
#define NUM_PATTERNS 12 //patterns avail
#define CTR_THRESH 16

uint8_t j = 0;
uint8_t pattern=1;
//uint8_t buttonState=0;
uint8_t lastPix=0; 
uint8_t myPix=0;
uint8_t direction=1;
uint8_t counter=0;
uint8_t colors[3];
uint32_t setColor=0;
unsigned long mark;
boolean flag1=0;
boolean flag2=0;
boolean flag3=0;
boolean flag4=0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

// button debouce
long debouncing_time = 15; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;

// states
boolean shouldFlipState = false;


//animation
int animationStep = 0;
int animationStepsMax = NUM_LEDS / 3;
elapsedMillis animationFrameElapsed = 0;
long animationFrameDurationMs = 100;


void setup(){
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);

  // call debounceInterrupt() on button press (or bounce)
  attachInterrupt(0, debounceInterrupt, RISING);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void colorFast(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);
}

void stateread() {
  flag1 == digitalRead(D1);
  flag2 == false;
  flag3 == digitalRead(D3);
  flag4 == false;

  // if(!flag1&&!flag2&&!flag3&&!flag4) Simple.Set(S0);
  // if(flag1&&!flag2&&!flag3&&!flag4) Simple.Set(S1);
  // if(!flag1&&flag2&&!flag3&&!flag4) Simple.Set(S2);
  //  if(flag1&&flag2&&!flag3&&!flag4) Simple.Set(S3);

  // if(!flag1&&!flag2&&flag3&&!flag4) Simple.Set(S4);
  // if(flag1&&!flag2&&flag3&&!flag4) Simple.Set(S5);
  // if(!flag1&&flag2&&flag3&&!flag4) Simple.Set(S6);
  //  if(flag1&&flag2&&flag3&&!flag4) Simple.Set(S7);

  // if(!flag1&&!flag2&&!flag3&&flag4) Simple.Set(S8);
  // if(flag1&&!flag2&&!flag3&&flag4) Simple.Set(S9);
  // if(!flag1&&flag2&&!flag3&&flag4) Simple.Set(S10);
  //  if(flag1&&flag2&&!flag3&&flag4) Simple.Set(S11);

  // if(!flag1&&!flag2&&flag3&&flag4) Simple.Set(S12);
  //  if(flag1&&!flag2&&flag3&&flag4) Simple.Set(S13);
  //  if(!flag1&&flag2&&flag3&&flag4) Simple.Set(S14);
  // if(flag1&&flag2&&flag3&&flag4) Simple.Set(S15);
}

void loop(){
  EXEC(Simple);
}

State S0(){
  for (uint16_t b = 5; b< 30; b++) {
    colorFast(strip.Color(b,b,b),5);
    colorFast(strip.Color(0,0,0),5);
  }
  stateread();
  for (uint16_t b = 30; b> 5; b=b-1) {
    colorFast(strip.Color(b,b,b),8);
  }
  stateread();
}

State S1(){
  //low white fade in/out 
  for (uint16_t b = 1; b< 30; b++) {
    colorFast(strip.Color(b,b,b),30-b);
    colorFast(strip.Color(0,0,0),30-b);
  }
  //stateread();
  if(digitalRead(D3)) Simple.Set(S2);
  for (uint16_t b = 30; b> 10; b=b-1) {
    colorFast(strip.Color(b-10,b-10,b),30-b);
  }
  if(digitalRead(D3)) Simple.Set(S2);
  for (uint16_t b = 10; b> 1; b=b-1) {
    colorFast(strip.Color(0,0,b),30-b);
  }
  colorFast(strip.Color(0,0,0),200);

  nextState(S2);
  return;
}

State S2(){
  // low fade green
  for (uint16_t b = 5; b< 50; b++) {
    colorFast(strip.Color(0,b,0),15);
  }

  if(digitalRead(D3)) Simple.Set(S1);
  for (uint16_t b = 50; b> 5; b=b-1) {
    colorFast(strip.Color(0,b,0),8);
  }

  nextState(S1);
  return;
}

State S3(){
  // odd fade
  incrAnimationStep();
  for (int i = 0; i < animationStep; i++) {
    strip.setPixelColor(i, strip.Color(50,0,100));
    strip.setPixelColor(i+LEDS_PER_SIDE, strip.Color(50,0,100));
    strip.setPixelColor(i+LEDS_PER_SIDE+LEDS_PER_SIDE, strip.Color(50,0,100));
  }
  strip.show();
  nextState(S0);
  return;
}

State S4(){
  // low red flashfade
  for (uint16_t b = 0; b< 20; b++) {
    colorFast(strip.Color(b,0,0),15);
  }
  stateread();
  for (uint16_t b = 20; b> 0; b=b-1) {
    colorFast(strip.Color(b,0,0),8);
  }
  stateread();

}
State S5(){
  // slow orange blinking
  int r=0;
  for (uint16_t b = 0; b< 30; b++) {
    r= b*2;
    colorFast(strip.Color(r,b,0),35);
  }
  stateread();
  r=0;
  for (uint16_t b = 30; b> 0; b=b-1) {
    r=b*2;
    colorFast(strip.Color(r,b,0),8);
  }
  stateread();

}
State S6(){
  // teal fade
  for (uint16_t b = 5; b< 30; b++) {
    colorFast(strip.Color(0,b,b),15);
  }
  stateread();
  for (uint16_t b = 30; b> 5; b=b-1) {
    colorFast(strip.Color(0,b,b),8);
  }
  stateread();

}
State S7(){
  //blue flashfade
  for (uint16_t b = 0; b< 15; b++) {
    colorFast(strip.Color(0,0,b),15);
  }
  stateread();
  for (uint16_t b = 15; b> 0; b=b-1) {
    colorFast(strip.Color(0,0,b),8);
  }
  stateread();

}
State S8(){
  // bright ass white
  for (uint16_t b = 1; b< 15; b++) {
    colorFast(strip.Color(250,250,250),15);
    delay(10);
  }
  stateread();
  for (uint16_t b = 15; b> 1; b=b-1) {
    colorFast(strip.Color(250,250,250),8);
    delay(10);
  }
  stateread();

}
State S9(){
  for (uint16_t b = 1; b< 15; b++) {
    colorFast(strip.Color(b,b,b),15);
  }
  stateread();
  for (uint16_t b = 15; b> 1; b=b-1) {
    colorFast(strip.Color(b,b,b),8);
  }
  stateread();

}
State S10(){
  for (uint16_t b = 1; b< 15; b++) {
    colorFast(strip.Color(b,b,0),15);
  }
  stateread();
  for (uint16_t b = 15; b> 1; b=b-1) {
    colorFast(strip.Color(b,b,0),8);
  }
  stateread();

}
State S11(){
  for (uint16_t b = 1; b< 15; b++) {
    colorFast(strip.Color(0,b,b),15);
  }
  stateread();
  for (uint16_t b = 15; b> 1; b=b-1) {
    colorFast(strip.Color(0,b,b),8);
  }
  stateread();

}
State S12(){
  for (uint16_t b = 1; b< 15; b++) {
    colorFast(strip.Color(b,0,b),15);
  }
  stateread();
  for (uint16_t b = 15; b> 1; b=b-1) {
    colorFast(strip.Color(b,0,b),8);
  }
  stateread();

}
State S13(){
  for (uint16_t b = 1; b< 15; b++) {
    colorFast(strip.Color(0,0,b),15);
  }
  stateread();
  for (uint16_t b = 15; b> 1; b=b-1) {
    colorFast(strip.Color(0,0,0),8);
  }
  stateread();

}
State S14(){
  for (uint16_t b = 1; b< 15; b++) {
    colorFast(strip.Color(0,b,0),15);
  }
  stateread();
  for (uint16_t b = 15; b> 1; b=b-1) {
    colorFast(strip.Color(0,0,0),8);
  }
  stateread();

}
State S15(){
  for (uint16_t b = 1; b< 15; b++) {
    colorFast(strip.Color(b,0,0),15);
  }
  stateread();
  for (uint16_t b = 15; b> 1; b=b-1) {
    colorFast(strip.Color(0,0,0),8);
  }
  stateread();

}


void debounceInterrupt() {
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    buttonPressed();
    last_micros = micros();
  }
}


void buttonPressed() {
  shouldFlipState = true;
}

void nextState(Pstate nextState) {
  if(shouldFlipState) {
    Simple.Set(nextState);
    shouldFlipState = false;
  }
  return;
}

void incrAnimationStep() {
  if (animationFrameElapsed > animationFrameDurationMs) {
    animationFrameElapsed = 0;
    animationStep++;
    animationStep = animationStep % animationStepsMax;
  }
  return; 
}




