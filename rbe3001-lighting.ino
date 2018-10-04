#include "core.hpp"

void setup() {
	i2c_init();
	leds_init();
}

void loop() {
	leds_update();
	leds_run();
}
