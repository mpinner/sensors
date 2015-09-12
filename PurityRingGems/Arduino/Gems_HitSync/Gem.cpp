#include "Arduino.h"
#include "Gem.h"
//#include <elapsedMillis.h>

#include "Shader.h"
#include "TopShader.h"
#include <OctoWS2811.h>




Gem::Gem(int gemIndex, int gemPixelCount, OctoWS2811* strip, Shader* shader)
{
  this->gemIndex = gemIndex;
  this->gemPixelCount = gemPixelCount;
  this->strip = strip;
  this->shader = shader;
  hit();



}

Gem::Gem()
{
  this->gemIndex = -1;
  hit();



}

void Gem::setColor(uint32_t color) {
  this->color = color; 
}


uint32_t Gem::getColor() {
  return this->color; 
}


void Gem::setSecondaryColor(uint32_t color) {
  this->secondaryColor = color; 
}


void Gem::setShader(Shader* shader) {
  this->shader = shader; 
}


Shader* Gem::getShader() {
  return this->shader; 
}


void Gem::setDuration(int duration) {
  this->duration = duration; 
}



void Gem::hit() {
  msSinceHit = 0;
  return;
}



void Gem::neighborHit(int distanceToHit) {

  if (distanceToHit == 0) hit();
  if (distanceToHit == 1) setMsSinceHitIfLess( duration - 0.75 * duration);
  if (distanceToHit == 2) setMsSinceHitIfLess( duration - 0.6 * duration);
  if (distanceToHit == 3) setMsSinceHitIfLess( duration - 0.5 * duration);
  if (distanceToHit == 4) setMsSinceHitIfLess( duration - 0.4 * duration);
  if (distanceToHit == 5) setMsSinceHitIfLess( duration - 0.3 * duration);
  if (distanceToHit == 6) setMsSinceHitIfLess( duration - 0.2 * duration);
  if (distanceToHit == 7) setMsSinceHitIfLess( duration - 0.1 * duration);

  return;
}



void Gem::setMsSinceHitIfLess(int possible) {
  if ( msSinceHit > possible) {
    msSinceHit = possible;
  }
  return;
}



uint32_t colorByBrightness(float brightness, uint32_t c) {

  uint8_t
    r = (uint8_t)(c >> 16),
  g = (uint8_t)(c >>  8),
  b = (uint8_t)c;

  r = ((uint8_t)((float)r * brightness));
  g = ((uint8_t)((float)g * brightness));
  b = ((uint8_t)((float)b * brightness));

  r = GammaE[r];
  g = GammaE[g];
  b = GammaE[b];


  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;

}


uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining) {


  float brightness = 1.0 - height;

  return colorByBrightness(brightness*remaining, color);
}


int getStripLedIndex(int pixel, int gemIndex, int gemPixelCount ) {
  return pixel+(gemIndex*gemPixelCount);
}

void Gem::animate() {


//  int duration = 2500;



 /*
 if ( msSinceHit > duration*2) { 
   msSinceHit = 0;   
   }
   */

  int led;

  if ( msSinceHit < duration) { 

    // pixel state
    float height;
    uint32_t currentColor;
    uint32_t nextColor;
    float remaining = 1.0 - (float)msSinceHit / duration;


    for (int pixel = 0; pixel < gemPixelCount; pixel++) {

      // populate pixel state
      height = (float)pixel / gemPixelCount;
      led = getStripLedIndex(pixel, gemIndex, gemPixelCount);
      currentColor = strip->getPixel(led);


      // call SHADER
      TopShader topShader;
      nextColor = shader->shade(height, color, currentColor, remaining, secondaryColor);

      // paint pixel
      strip->setPixel(led, nextColor);


    }
  }
  else {
    for (int pixel = 0; pixel < gemPixelCount; pixel++) {
      led = getStripLedIndex(pixel, gemIndex, gemPixelCount);
      strip->setPixel(led, 0, 0, 0);
    }
  }


  return;
}


/*

 
 
 void Gem::animate() {
 
 
 int duration = 1000;
 
 if ( msSinceHit > duration*2) { 
 msSinceHit = 0;   
 }
 
 
 
 if ( msSinceHit < duration) { 
 float remaining = 1.0 - (float)msSinceHit / duration;
 
 float height;
 int led;
 uint32_t currentColor;
 uint32_t nextColor;
 
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 
 height = (float)pixel / gemPixelCount;
 
 led = getStripLedIndex(pixel, gemIndex, gemPixelCount);
 
 currentColor = strip->getPixelColor(led);
 
 nextColor = shade(height, color, currentColor, remaining);
 
 strip->setPixelColor(led, nextColor);
 
 
 }
 }
 else {
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 0, 0, 0);
 }
 }
 
 
 return;
 }
 
 
 
 
 void Gem::animateTop() {
 
 
 int duration = 1000;
 
 if ( msSinceHit > duration*2) { 
 msSinceHit = 0;   
 }
 
 
 
 if ( msSinceHit < duration) { 
 float remaining = 1.0 - (float)msSinceHit / duration;
 
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 
 float height = (float)pixel / gemPixelCount;
 
 // distance form center
 float brightness = 1.0 - height;
 
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), colorByBrightness(brightness*remaining, color));
 
 
 }
 }
 else {
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 0, 0, 0);
 }
 }
 
 
 return;
 }
 
 
 
 void Gem::animateBottom() {
 
 
 int duration = 1000;
 
 if ( msSinceHit > duration*2) { 
 msSinceHit = 0;   
 }
 
 
 
 if ( msSinceHit < duration) { 
 float remaining = 1.0 - (float)msSinceHit / duration;
 
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 
 float height = (float)pixel / gemPixelCount;
 
 // distance form center
 float brightness = 1.0 - (abs(height - 1.0));
 
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), colorByBrightness(brightness*remaining, color));
 
 
 }
 }
 else {
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 0, 0, 0);
 }
 }
 
 
 return;
 }
 
 
 
 void Gem::animateCenter() {
 
 
 int duration = 2000;
 
 if ( msSinceHit > duration*2) { 
 msSinceHit = 0;   
 }
 
 
 
 if ( msSinceHit < duration) { 
 float remaining = 1.0 - (float)msSinceHit / duration;
 
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 
 float height = (float)pixel / gemPixelCount;
 
 // distance form center
 float brightness = 1.0 - (abs(height - 0.5) * 2);
 
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), colorByBrightness(brightness*remaining, color));
 
 
 }
 }
 else {
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 0, 0, 0);
 }
 }
 
 
 return;
 }
 
 
 
 void Gem::animateTwinkleOff() {
 
 
 int duration = 2000;
 
 if ( msSinceHit > duration*2) { 
 msSinceHit = 0;   
 }
 
 
 
 if ( msSinceHit < duration) { 
 float remaining = 1.0 - (float)msSinceHit / duration;
 
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 
 if(remaining > 0.9) {
 
 
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 100, 100, 100);
 
 } 
 
 
 if(10 > random(100)) {
 
 uint32_t currentColor = strip->getPixelColor(pixel+(gemIndex*gemPixelCount));
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), colorByBrightness(0.9, currentColor));
 
 }
 
 }
 }
 else {
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 0, 0, 0);
 }
 }
 
 
 return;
 }
 
 
 
 void Gem::animateTwinkle() {
 
 
 int duration = 2000;
 
 
 if (DEBUG) {
 if ( msSinceHit > duration*2) { 
 msSinceHit = 0;   
 }
 }
 
 
 
 if ( msSinceHit < duration) { 
 float remaining = 1.0 - (float)msSinceHit / duration;
 
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 
 if(remaining > 0.5) {
 if(1 > random(100)) {
 
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 100, 100, 100);
 }
 } 
 
 
 
 uint32_t currentColor = strip->getPixelColor(pixel+(gemIndex*gemPixelCount));
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), colorByBrightness(0.999, currentColor));
 
 
 }
 }
 else {
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 0, 0, 0);
 }
 }
 
 
 return;
 }
 
 
 void Gem::animateChase() {
 
 
 int duration = 2000;
 
 if ( msSinceHit > duration*3) { 
 msSinceHit = 0;   
 }
 
 
 
 if ( msSinceHit < duration) { 
 float remaining = 1.0 - (float)msSinceHit / duration;
 
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 
 
 
 float height = (float)pixel / gemPixelCount;
 
 int scale = 1;
 
 
 
 height = height * height * height * height * height * height;
 height = 1.0 - height;
 
 float brightness = 1.0 - abs(height - remaining);
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), colorByBrightness(brightness, color));
 }
 }
 else {
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 0, 0, 0);
 }
 }
 
 
 return;
 }
 
 
 void Gem::animateScaling() {
 
 int duration = 1500;
 
 float remaining = 1.0 - (float)msSinceHit / duration;
 
 if ( msSinceHit < duration) { 
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 
 
 
 float height = (float)pixel / gemPixelCount;
 
 int scale = 1;
 height = height * scale;
 height = height - (int) height;
 
 float brightness = 1.0 - abs(height - remaining);
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), colorByBrightness(brightness, color));
 }
 }
 else {
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 0, 0, 0);
 }
 }
 
 
 return;
 }
 
 
 
 
 void Gem::animateFalling() {
 
 int duration = 5000;
 
 float remaining = 1.0 - (float)msSinceHit / duration;
 
 if ( msSinceHit < duration) { 
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 
 
 
 float height = (float)pixel / gemPixelCount;
 
 
 float brightness = 1.0 - abs(height - remaining);
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), colorByBrightness(brightness, color));
 }
 }
 else {
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 0, 0, 0);
 }
 }
 
 
 return;
 }
 
 
 
 void Gem::animateFadeDown() {
 
 int duration = 1000;
 
 float remaining = 1.0 - (float)msSinceHit / duration;
 
 if ( msSinceHit < duration) { 
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 
 
 float height = (float)pixel / gemPixelCount;
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), colorByBrightness(height*remaining, color));
 }
 }
 else {
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 0, 0, 0);
 }
 }
 
 
 return;
 }
 
 
 void Gem::animateHeight() {
 
 if ( msSinceHit < 1000) { 
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 
 
 float height = (float)pixel / gemPixelCount;
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), colorByBrightness(height, color));
 }
 }
 else {
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 0, 0, 0);
 }
 }
 
 
 return;
 }
 
 
 void animateFlash(Gem* gem) {
 
 if ( msSinceHit < 1000) { 
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), color);
 }
 }
 else {
 for (int pixel = 0; pixel < gemPixelCount; pixel++) {
 strip->setPixelColor(pixel+(gemIndex*gemPixelCount), 0, 0, 0);
 }
 }
 
 
 return;
 }
 
 */









