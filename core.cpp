#include "core.hpp"

SoftwareSerial* serial = NULL;
uint64_t serial_idle_timeout = 0;

void serial_init(void) {
	mode = ST_MODE_NOCOMMS_IDLE;
	mode_changed = true;

	pinMode(SOFT_UART_TX, OUTPUT);
	pinMode(SOFT_UART_RX, INPUT);

	serial = new SoftwareSerial(SOFT_UART_RX, SOFT_UART_TX);
	serial->begin(4800);

	serial->write(0x7A); // our "ACK"
}

void serial_read(uint64_t elapsed_time) {
	uint8_t parity;
	uint8_t old_mode = mode;
	if (serial->available() >= 2) {
		mode = serial->read();
		parity = serial->read();
		serial->write(0x7A); // our "ACK"

		#ifdef DEBUG
		// Serial.print(mode, HEX);
		// Serial.print(" & ");
		// Serial.print(parity, HEX);
		// Serial.print(" : ");
		// Serial.print(mode & parity, HEX);
		// Serial.print(" with ");
		Serial.print(serial->available());
		Serial.println(" available.");
		#endif

		if (mode & parity) { mode = old_mode; }
	} else if (mode == ST_MODE_NOCOMMS_IDLE) {
		if (serial_idle_timeout == 0) { serial->write(0x7A); }
		serial_idle_timeout += elapsed_time;
		if (serial_idle_timeout >= 200000) { serial_idle_timeout = 0; }
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

		leds_run(0);
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
