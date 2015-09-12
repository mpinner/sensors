
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
  void neighborHit(int distanceToHit);
  void animate();
  void setColor(uint32_t color);  
  void setDuration(int duration);  
  uint32_t getColor();

  void setSecondaryColor(uint32_t color);
  void setShader(Shader* shader);
  Shader* getShader();


 private:
  int gemIndex;
  int duration;
  OctoWS2811* strip;
  Shader* shader;
  int gemPixelCount;
  uint32_t color;
  uint32_t secondaryColor;
  int animationIndex;
  elapsedMillis msSinceHit;
  void setMsSinceHitIfLess(int possible);
};


#endif // H_GEM

