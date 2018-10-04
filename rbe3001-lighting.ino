#include "core.hpp"

uint64_t cur_time = 0;
uint64_t prev_time = 0;

void setup() {
	i2c_init();
	leds_init();
}

void loop() {
	leds_update();
	cur_time = millis() - prev_time;
	prev_time = millis();
	leds_run(cur_time, prev_time);
}
