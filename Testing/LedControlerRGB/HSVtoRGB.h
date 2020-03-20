#include<cmath> // Needed for fmod()
using namespace std;

//varible for LED gamma correction
const uint8_t PROGMEM gamma8[] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
        1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
        2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
        5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
       10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
       17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
       25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
       37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
       51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
       69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
       90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
      115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
      144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
      177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
      215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

/*
 * H(Hue): 0 - 360 degree (integer)
 * S(Saturation): 0 - 1.00 (double)
 * V(Value): 0 - 1.00 (double)
 * 
 * output[3]: Output, array size 3, int
 */
void HSVtoRGB(int H, double S, double V, int output[3]) {
  double C = S * V;
  double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
  double m = V - C;
  double Rs, Gs, Bs;



  if(H >= 0 && H < 60) {
    Rs = C;
    Gs = X;
    Bs = 0; 
  }
  else if(H >= 60 && H < 120) { 
    Rs = X;
    Gs = C;
    Bs = 0; 
  }
  else if(H >= 120 && H < 180) {
    Rs = 0;
    Gs = C;
    Bs = X; 
  }
  else if(H >= 180 && H < 240) {
    Rs = 0;
    Gs = X;
    Bs = C; 
  }
  else if(H >= 240 && H < 300) {
    Rs = X;
    Gs = 0;
    Bs = C; 
  }
  else {
    Rs = C;
    Gs = 0;
    Bs = X; 
  }       Serial.print("H S V "); Serial.print(H);Serial.print(S);Serial.println(V);
          Serial.print("HSVtoRGB R: "); Serial.println(int((Rs + m) * 255));
         Serial.print("HSVtoRGB G: "); Serial.println(int((Gs + m) * 255));
          Serial.print("HSVtoRGB B: "); Serial.println(int((Bs + m) * 255));
  output[0] = pgm_read_byte(&gamma8[int((Rs + m) * 255)]); //LED gamma correction included
  output[1] = pgm_read_byte(&gamma8[int((Gs + m) * 255)]);
  output[2] = pgm_read_byte(&gamma8[int((Bs + m) * 255)]);
}
