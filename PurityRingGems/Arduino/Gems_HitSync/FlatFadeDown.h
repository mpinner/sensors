#ifndef H_FLAT
#define H_FLAT

#include "Shader.h"

class FlatFadeDown : 
public Shader {

public:
  inline uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining, uint32_t secondaryColor) {
    

      return colorByBrightness(remaining, color);
 
  }  
  
  
};

#endif //H_FLAT


