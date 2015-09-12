#ifndef H_BASS
#define H_BASS

#include "Shader.h"

class BassShader : 
public Shader {

public:
  inline uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining, uint32_t secondaryColor) {
    
    

    int scale = 2;
    height = height * scale;
    height = height - (int) height;
    height = 1.0 - height;
    
    int scaleRemaining = remaining/2 + .5;
    

    float closeness = 1.0 - abs(height - scaleRemaining);
    
    if (closeness > .90) {
      return colorByBrightness(closeness*scaleRemaining, color);
    } else if (closeness < .40) {
      return colorByBrightness(remaining, secondaryColor);
    }
    else {
      return colorByBrightness(0.0, secondaryColor);
    }

  }  
  
  
};

#endif //H_MULTI


