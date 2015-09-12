#ifndef H_GLOW
#define H_GLOW

#include "Shader.h"

class GlowShader : 
public Shader {

public:
  inline uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining, uint32_t secondaryColor) {
   
  // distance from center
  float brightness = 1.0 - (abs(height-0.5) *2);

  return colorByBrightness(brightness*remaining, color);

  }  
};

#endif

