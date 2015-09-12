#ifndef H_RISE
#define H_RISE

#include "Shader.h"

class RisingShader : 
public Shader {

public:
  inline uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining, uint32_t secondaryColor) {

    int scale = 1;
    height = height * scale;
    height = height - (int) height;
    height = 1.0 - height;
    

    float closeness = 1.0 - abs(height - remaining);
    if (closeness > .90) {
      return colorByBrightness(closeness*remaining, color);
    } else {
     return colorByBrightness(0.0, color);
    }

  }  
};

#endif


