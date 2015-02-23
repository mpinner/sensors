
#ifndef H_GEM
#define H_GEM

#include <Adafruit_NeoPixel.h>
#include "Shader.h"




class Gem
{
 public:
  Gem();
  Gem(int gemIndex, int gemPixelCount, Adafruit_NeoPixel* strip, Shader* shader);
  void hit();
  void animate();
  void setColor(uint32_t color);

 private:
  int gemIndex;
  Adafruit_NeoPixel* strip;
  Shader* shader;
  int gemPixelCount;
  uint32_t color;
  int animationIndex;
  elapsedMillis msSinceHit;
};


#endif // H_GEM

