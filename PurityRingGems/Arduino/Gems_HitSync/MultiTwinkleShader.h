#ifndef H_MULTWINKLE
#define H_MULTWINKLE

#include "Shader.h"

class MultiTwinkleShader : 
public Shader {

public:
  inline uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining, uint32_t secondaryColor) {

    if(remaining > 0.85) {
      if(2 > random(100)) {
        return color;
      }
    } 

    if(remaining > 0.7) {
      if(2 > random(100)) {
        return secondaryColor;
      }
    } 


     if(5 > random(100)) {

      return colorByBrightness(0.9, currentColor);
    }

    return currentColor;

  }  
};

#endif


/*

    if(remaining > 0.85) {
      if(3 > random(100)) {
        return color;
      }
    } 
    
    if(5 > random(100)) {

      return colorByBrightness(0.9, currentColor);
    }
    
    return currentColor;
    
    */


