#include "core.hpp"

SoftwareSerial* serial = NULL;

void serial_init(void) {
	mode = ST_MODE_NOCOMMS_IDLE;
	mode_changed = false;

	pinMode(SOFT_UART_TX, OUTPUT);
	pinMode(SOFT_UART_RX, INPUT);

	serial = new SoftwareSerial(SOFT_UART_RX, SOFT_UART_TX);
	serial->begin(4800);
}

void serial_read(void) {
	uint8_t parity;
	uint8_t old_mode = mode;
	if (serial->available() >= 2) {
		mode = serial->read();
		parity = serial->read();

		if (in & parity) { mode = old_mode; }
	}

	mode_changed = (mode != old_mode);
}

void leds_init(void) {
	strip_l = new Adafruit_NeoPixel(STRIP_LENGTH, PIN_STRIP_L, NEO_GRB + NEO_KHZ800);
	strip_r = new Adafruit_NeoPixel(STRIP_LENGTH, PIN_STRIP_R, NEO_GRB + NEO_KHZ800);

	strip_l->setBrightness(50);
	strip_r->setBrightness(50);

	strip_l->begin();
	strip_r->begin();
	strip_l->show();
	strip_r->show();

	leds_cur_anim = anim__nocomms_idle;
}

void leds_update(void) {
	if (mode_changed) {
		if (/*packet_malformed(mode)*/ false) {
			// leds_cur_anim = anim__error;
		} else {
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

		// leds_run(0);
		mode_changed = false;
	}
}

void leds_run(uint64_t elapsed_time) { leds_cur_anim(elapsed_time); }

void _leds_combined_write(uint8_t pixel, uint32_t color) {
	strip_l->setPixelColor(pixel, color);
	strip_r->setPixelColor(pixel, color);
}

void _leds_combined_show(void) {
	strip_l->show();
	strip_r->show();
}

uint8_t packet_malformed(uint8_t packet) {
	// INCOMPLETE
	//		validate weight			validate mode				validate color				validate running
	return (packet & 0x40) || !((packet & 0x30) ^ 0x30) || !((packet & 0xE) ^ 0xE) || ((packet & 0x3E) && ~(packet & 0x1));
}
