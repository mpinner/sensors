#ifndef H_STROBE
#define H_STROBE

#include "Shader.h"

class StrobingShader : 
public Shader {

public:
  inline uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining, uint32_t secondaryColor) {
    
    

        int stobeClock = (int) (remaining * 100.0);
     
         if ( 0 == (stobeClock % 2) ) {
           return colorByBrightness(remaining, color);
           
         
         }

 
   return colorByBrightness(0.0, color);

  }
  
};

#endif //H_STROBE


