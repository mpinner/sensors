
#ifndef H_GEM
#define H_GEM

//#include <Adafruit_NeoPixel.h>

#include "Shader.h"
#include <OctoWS2811.h>




class Gem
{
 public:
  Gem();
  Gem(int gemIndex, int gemPixelCount, OctoWS2811* strip, Shader* shader);
  void hit();
  void animate();
  void setColor(uint32_t color);  
  uint32_t getColor();

  void setSecondaryColor(uint32_t color);
  void setShader(Shader* shader);

 private:
  int gemIndex;
  OctoWS2811* strip;
  Shader* shader;
  int gemPixelCount;
  uint32_t color;
  uint32_t secondaryColor;
  int animationIndex;
  elapsedMillis msSinceHit;
};


#endif // H_GEM

