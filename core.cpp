#include "core.hpp"

struct comms {
	SoftwareSerial* uart;
	uint8_t ready;
	uint64_t last_request;
	uint64_t last_response;
	uint64_t timeout;
} serial_comms;

void serial_init(void) {
	mode = ST_MODE_NOCOMMS_IDLE;
	mode_changed = true;

	pinMode(SOFT_UART_TX, OUTPUT);
	pinMode(SOFT_UART_RX, INPUT);

	serial_comms.uart = new SoftwareSerial(SOFT_UART_RX, SOFT_UART_TX);
	serial_comms.uart->begin(19200);

	serial_comms.ready = 1;
	serial_comms.last_request = 0;
	serial_comms.last_response = 0;
	serial_comms.timeout = micros() + 1000000;
}

void serial_request(void) {
	if (serial_comms.ready && ((micros() - 50000) > serial_comms.last_request)) {
		serial_comms.uart->write(0xAC);
		serial_comms.last_request = micros();
		serial_comms.ready = 0;
	}
}

void serial_read(void) {
	static uint8_t data, parity, stop;
	uint8_t old_mode = mode;
	mode_changed = 0;

	if (serial_comms.uart->available() >= 4) {
		data = serial_comms.uart->read();
		if (data == 0x7A) {
			data = serial_comms.uart->read();
			parity = serial_comms.uart->read();
			stop = serial_comms.uart->read();

			if ((data & parity) || packet_malformed(data)) {
				mode = old_mode;
			} else {
				mode = data;
			}

			mode_changed = mode != old_mode;

			serial_comms.last_response = micros();
			serial_comms.ready = 1;
			serial_comms.timeout = micros() + 1000000;
		}
	} else if (serial_comms.timeout < micros()) {
		#ifdef DEBUG
		Serial.println("No Comms!");
		#endif
		mode = ST_MODE_NOCOMMS_IDLE;
		mode_changed = 1;
		serial_comms.ready = 1;
		serial_comms.timeout = micros() + 1000000;
	}
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

		leds_run(0);
		mode_changed = 0;
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
	return
	    // validate connection/enable bit
	    ((~packet & 0x80)   &&  (packet & 0x7F)) ||
	    // validate running bit
	    ((~packet & 0x1)    &&  (packet & 0x7E)) ||
	    // validate color uniqueness
	    (((packet & 0x4) >> 1)  & (packet & 0x2)) ||
	    (((packet & 0x8) >> 2)  & (packet & 0x2)) ||
	    (((packet & 0x8) >> 1)  & (packet & 0x4)) ||
	    // validate state
	    (((packet & 0x20) >> 1) & (packet & 0x10)) ||
	    (((packet & 0x40) >> 2) & (packet & 0x10)) ||
	    (((packet & 0x40) >> 1) & (~packet & 0x20));
}
