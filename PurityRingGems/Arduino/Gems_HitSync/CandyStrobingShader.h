#ifndef H_CANDY_STROBE
#define H_CANDY_STROBE

#include "Shader.h"

class CandyStrobingShader :
public Shader {
    
public:
    inline uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining, uint32_t secondaryColor) {
        
        int stobeClock = (int) (remaining * 100.0);
        
        if ( 0 == (stobeClock % 2) ) {
            return colorByBrightness(remaining, candyColors[stobeClock%candyColorCount]);
        }
        
        return colorByBrightness(0.0, color);
        
    }
    
};

#endif //H_STROBE

