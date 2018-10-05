#include "core.hpp"

uint64_t elapsed_time = 0;
uint64_t prev_time = 0;

void setup() {
	#ifdef DEBUG
	Serial.begin(115200);
	Serial.println("Setup starting...");
	#endif

	serial_init();

	#ifdef DEBUG
	Serial.println("Initialized Software Serial...");
	#endif

	leds_init();

	#ifdef DEBUG
	Serial.println("Initialized LEDs...");
	#endif

	leds_run(0);
}

void loop() {
	serial_read();
	leds_update();
	elapsed_time = micros() - prev_time;
	prev_time = micros();
	leds_run(elapsed_time);

	#ifdef DEBUG
	Serial.print(F("0x"));
	Serial.println(mode, HEX);
	#endif
}
