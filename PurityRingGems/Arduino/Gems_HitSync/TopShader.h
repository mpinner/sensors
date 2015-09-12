#ifndef H_TOP
#define H_TOP

#include "Shader.h"

class TopShader : 
public Shader {

public:
  inline uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining, uint32_t secondaryColor) {

  float brightness = 1.0 - height;

  return colorByBrightness(brightness*remaining, color);

  }  
};

#endif

