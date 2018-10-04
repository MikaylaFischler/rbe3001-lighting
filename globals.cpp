#include "globals.hpp"

// led strips
Adafruit_NeoPixel* strip_l = NULL;
Adafruit_NeoPixel* strip_r = NULL;

// current animation function pointer
void (*leds_cur_anim)(uint64_t) = NULL;

// control modes
volatile uint8_t mode = 0;
volatile uint8_t mode_changed = 0;
