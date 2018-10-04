#include "core.hpp"

void i2c_init(void) {
	mode = 0;
	Wire.begin(REDBOARD_ADDR);
	Wire.setClock(I2C_FREQUENCY);
	Wire.onReceive(i2c_handle_receive);

	#ifdef DEBUG
	Serial.begin(9600);
	#endif
}

void i2c_handle_receive(int bytes) {
	for (int i = 0; i < bytes; i++) { mode = Wire.read(); }
	mode_changed = true;
}

void leds_init(void) {
	strip_l = new Adafruit_NeoPixel(STRIP_LENGTH, PIN_STRIP_L, NEO_GRB + NEO_KHZ800);
	strip_r = new Adafruit_NeoPixel(STRIP_LENGTH, PIN_STRIP_R, NEO_GRB + NEO_KHZ800);

	strip_l->begin();
	strip_r->begin();
	strip_l->show();
	strip_r->show();

	leds_cur_anim = anim__nocomms_idle;

	mode = ST_MODE_NOCOMMS_IDLE;
	mode_changed = 0;
}

void leds_update(void) {
	if (mode_changed) {
		if (mode & MODE_COMMS) {
			if (mode & MODE_RUNNING) {
				if (mode & MODE_WEIGH) {
					leds_cur_anim = anim__weigh;
				} else if (mode & MODE_PLACE) {
					leds_cur_anim = anim__place;
				} else {
					leds_cur_anim = anim__running;
				}
			} else {
				leds_cur_anim = anim__idle;
			}
		} else {
			leds_cur_anim = anim__nocomms_idle;
		}
	}
}

void leds_run(void) { leds_cur_anim(); }

void _leds_combined_write(uint8_t pixel, uint32_t color) {
	strip_l->setPixelColor(pixel, color);
	strip_r->setPixelColor(pixel, color);
}

void _leds_combined_show(void) {
	strip_l->show();
	strip_r->show();
}
