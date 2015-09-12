#ifndef H_LIGHTNING
#define H_LIGHTNING

#include "Shader.h"

class LightningShader : 
public Shader {

public:
  inline uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining, uint32_t secondaryColor) {

    if (height > remaining) {
      return colorByBrightness(remaining, color);
    } else {
      return colorByBrightness(0.0, color);
    }  
};

#endif


