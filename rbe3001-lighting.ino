#include "core.hpp"

uint64_t elapsed_time = 0;
uint64_t prev_time = 0;

void setup() {
	i2c_init();
	leds_init();
}

void loop() {
	leds_update();
	elapsed_time = micros() - prev_time;
	prev_time = micros();
	leds_run(elapsed_time);
}
