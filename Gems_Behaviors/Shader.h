
#ifndef H_SHADER
#define H_SHADER

static const int GammaE[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
  2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
  6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11,
  11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18,
  19, 19, 20, 21, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28,
  29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40,
  40, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 54,
  55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
  71, 72, 73, 74, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 88, 89,
  90, 91, 93, 94, 95, 96, 98, 99,100,102,103,104,106,107,109,110,
  111,113,114,116,117,119,120,121,123,124,126,128,129,131,132,134,
  135,137,138,140,142,143,145,146,148,150,151,153,155,157,158,160,
  162,163,165,167,169,170,172,174,176,178,179,181,183,185,187,189,
  191,193,194,196,198,200,202,204,206,208,210,212,214,216,218,220,
  222,224,227,229,231,233,235,237,239,241,244,246,248,250,252,255
};

// abstract class Shader
class Shader
{
 public:
  //  Shader();
  virtual uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining);

  inline virtual uint32_t colorByBrightness(float brightness, uint32_t c) {
      uint8_t
    r = (uint8_t)(c >> 16),
  g = (uint8_t)(c >>  8),
  b = (uint8_t)c;

  r = ((uint8_t)((float)r * brightness));
  g = ((uint8_t)((float)g * brightness));
  b = ((uint8_t)((float)b * brightness));

  r = GammaE[r];
  g = GammaE[g];
  b = GammaE[b];


  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  }
  

};


#endif // H_SHADER
