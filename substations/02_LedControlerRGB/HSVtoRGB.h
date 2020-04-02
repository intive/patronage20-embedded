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

/* Function for checking the range of input variables */
void check_input_varibles(int *hue, int *saturation, int *value)
{
    /* Checking input values */
    *hue = abs(*hue) % 360;
    if (*saturation < 0) {
        *saturation = 0;
    }
    else if (*saturation > 100) {
        *saturation = 100;
    }
    if (*value < 0) {
        *value = 0;
    }
    else if (*value > 100) {
        *value = 100;
    }
}

/* Function for checking the range of output variables */
void check_output_varibles(int *red, int *green, int *blue, int rgb_component)
{
    *red += rgb_component;
    *green += rgb_component;
    *blue += rgb_component;

    if (*red > 255) {
        *red = 255;
    }
    else if (*red < 0) {
        *red = 0;
    }
    if (*green > 255) {
        *green = 255;
    }
    else if (*green < 0) {
        *green = 0;
    }
    if (*blue > 255) {
        *blue = 255;
    }
    else if (*blue < 0) {
        *blue = 0;
    }
}

/* Function to apply rgb values into output array */
void apply_rgb_to_array(int red_component, int green_component, int blue_component, int rgb_array[3])
{
    /* LED gamma correction included */
    rgb_array[0] = pgm_read_byte(&gamma8[red_component]);
    rgb_array[1] = pgm_read_byte(&gamma8[green_component]);
    rgb_array[2] = pgm_read_byte(&gamma8[blue_component]);
}
/*
 * H(Hue): 0 - 360 degree (integer)
 * S(aturation): 0 - 100 (integer)
 * V(Value): 0 - 100 (integer)
 *
 * rgb_array[3]: Output, array size 3, int
 */
void hsv_to_rgb(int hue, int saturation, int value, int rgb_array[3])
{
    check_input_varibles(&hue, &saturation, &value);

    int chroma = (255 * saturation * value)/(10000);
    int intermediate = chroma * (60 - abs((hue % 120) - 60)) / 60;
    int rgb_component  = value * 255 / 100 - chroma;
    int red_component, green_component, blue_component;

    if (hue >= 0 && hue < 60) {
        red_component = chroma;
        green_component = intermediate;
        blue_component = 0;
    }
    else if (hue >= 60 && hue < 120) {
        red_component = intermediate;
        green_component = chroma;
        blue_component = 0;
    }
    else if (hue >= 120 && hue < 180) {
        red_component = 0;
        green_component = chroma;
        blue_component = intermediate;
    }
    else if (hue >= 180 && hue < 240) {
        red_component = 0;
        green_component = intermediate;
        blue_component = chroma;
    }
    else if (hue >= 240 && hue < 300) {
        red_component = intermediate;
        green_component = 0;
        blue_component = chroma;
    }
    else {
        red_component = chroma;
        green_component = 0;
        blue_component = intermediate;
    }
    check_output_varibles(&red_component, &green_component, &blue_component, rgb_component);
    apply_rgb_to_array(red_component, green_component, blue_component, rgb_array);
}
#endif /* HSVTORGB_H */
