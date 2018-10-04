#ifndef RBE3001_GLOBALS_H_
#define RBE3001_GLOBALS_H_

// libraries
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

// led strips
extern Adafruit_NeoPixel* strip_l;
extern Adafruit_NeoPixel* strip_r;

// current animation function pointer
extern void (*leds_cur_anim)(void);

// control modes
extern volatile uint8_t mode;
extern volatile uint8_t mode_changed;

#endif
