#ifndef H_BODYTWINKLE
#define H_BODYTWINKLE

#include "Shader.h"

class BodyTwinkler : 
public Shader {

public:
  inline uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining, uint32_t secondaryColor) {



    int scale = 2;
    height = height * scale;
    height = height - (int) height;
    height = 1.0 - height;


    float closeness = 1.0 - abs(height - remaining);
    if (closeness > .90) {
      return colorByBrightness(closeness*remaining, color);
    } 
    else if (closeness < .30) {
      return colorByBrightness(remaining, secondaryColor);
    }




    if(remaining > 0.95) {
      if(2 > random(100)) {
        return color;
      }
    } 

    if(remaining > 0.85) {
      if(2 > random(100)) {
        return secondaryColor;
      }
    } 


    if(10 > random(100)) {

      return colorByBrightness(0.9, currentColor);
    }

    return currentColor;

  }  
};

#endif





