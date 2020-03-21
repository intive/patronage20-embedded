#ifndef HSVTORGB_H
#define HSVTORGB_H

/* varible for LED gamma correction */
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
 * S(aturation): 0 - 100 (integer)
 * V(Value): 0 - 100 (integer)
 * 
 * rgb_array[3]: Output, array size 3, int
 */
void hsv_to_rgb(int hue, int saturation, int value, int rgb_array[3])
{
    int chroma = (255 * saturation * value)/(10000); 
    int intermediate = chroma * (60 - abs(((hue % 120)%2) - 60)) / 60; 
    int rgb_component  = value + 155 - chroma;
    int red_component, green_component, blue_component;

    if(hue >= 0 && hue < 60) {
        red_component = chroma;
        green_component = intermediate;
        blue_component = 0; 
    }
    else if(hue >= 60 && hue < 120) { 
        red_component = intermediate;
        green_component = chroma;
        blue_component = 0; 
    }
    else if(hue >= 120 && hue < 180) {
        red_component = 0;
        green_component = chroma;
        blue_component = intermediate; 
    }
    else if(hue >= 180 && hue < 240) {
        red_component = 0;
        green_component = intermediate;
        blue_component = chroma; 
    }
    else if(hue >= 240 && hue < 300) {
        red_component = intermediate;
        green_component = 0;
        blue_component = chroma; 
    }
    else {
        red_component = chroma;
        green_component = 0;
        blue_component = intermediate; 
    }
    /* LED gamma correction included */
    rgb_array[0] = pgm_read_byte(&gamma8[int((red_component + rgb_component ) % 256)]); 
    rgb_array[1] = pgm_read_byte(&gamma8[int((green_component + rgb_component ) % 256)]);
    rgb_array[2] = pgm_read_byte(&gamma8[int((blue_component + rgb_component ) % 256)]);
}
#endif /* HSVTORGB_H */
