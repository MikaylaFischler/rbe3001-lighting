#include "patterns.hpp"

uint64_t time_cnt = 0;
uint16_t counter = 0;
uint8_t direction = 1;

void anim__nocomms_idle(uint64_t elapsed_time) {
	// reset on 0
	if (elapsed_time == 0) {
		time_cnt = 0;
		counter = 0;
		direction = 1;
	}

	if (time_cnt == 0) {
		_leds_combined_write(0, strip_l->Color(255,0,0));
		_leds_combined_write(4, strip_l->Color(255,0,0));
		_leds_combined_write(8, strip_l->Color(255,0,0));
	}

	if ((time_cnt / 1000) % 15 == 0) {
		for (int i = 1; i < 8; i++) {
			if (i == 4) { i = 5; }
			_leds_combined_write(i, strip_l->Color(counter,counter,counter));
			if (counter == 255) { direction = 0; } else if (counter == 25) { direction = 1; }
			if (direction) { counter += 1; } else { counter -= 1; }
		}
	}

	_leds_combined_show();

	// reset static pixels every 10s in case of static
	time_cnt += elapsed_time;
	if (time_cnt > 10000000) { time_cnt = 0; }
}

void anim__idle(uint64_t elapsed_time) {
	// reset on 0
	if (elapsed_time == 0) {
		time_cnt = 0;
		counter = 0;
		direction = 1;
	}

	if (time_cnt == 0) {
		_leds_combined_write(0, strip_l->Color(0,255,0));
		_leds_combined_write(4, strip_l->Color(0,255,0));
		_leds_combined_write(8, strip_l->Color(0,255,0));
	}

	if ((time_cnt / 1000) % 50 == 0) {
		for (int i = 1; i < 8; i++) {
			if (i == 4) { i = 5; }
			_leds_combined_write(i, strip_l->Color(counter,counter,counter));
			if (counter == 150) { direction = 0; } else if (counter == 25) { direction = 1; }
			if (direction) { counter += 1; } else { counter -= 1; }
		}
	}

	_leds_combined_show();

	// reset static pixels every 10s in case of static
	time_cnt += elapsed_time;
	if (time_cnt > 10000000) { time_cnt = 0; }
}

void anim__running(uint64_t elapsed_time) {
	// reset on 0
	if (elapsed_time == 0) {
		time_cnt = 0;
		counter = 1;
	}

	uint32_t color = color_from_mode();

	if (time_cnt == 0) {
		_leds_combined_write(0, strip_l->Color(150,0,150));
		_leds_combined_write(8, strip_l->Color(150,0,150));
	}

	if (time_cnt > 200000) {
		counter++;
		if (counter == 5) { counter = 1; }
		time_cnt = 0;
	}

	for (int i = 1; i < 8; i++) {
		if (i > 4) {
			if (7 - i < counter) {
				_leds_combined_write(i, color);
			} else {
				_leds_combined_write(i, 0);
			}
		} else {
			if (i <= counter) {
				_leds_combined_write(i, color);
			} else {
				_leds_combined_write(i, 0);
			}
		}
	}

	_leds_combined_show();

	time_cnt += elapsed_time;
}

void anim__weigh(uint64_t elapsed_time) {
	// reset on 0
	if (elapsed_time == 0) {
		time_cnt = 0;
		counter = 1;
		direction = 1;
	}

	uint32_t color = color_from_mode();

	if (time_cnt == 0) {
		_leds_combined_write(0, strip_l->Color(125,0,255));
		_leds_combined_write(8, strip_l->Color(125,0,255));
	}

	if (time_cnt > 150000) {
		if (direction) { counter++; } else { counter--; }
		if (counter == 6) { direction = 0; } else if (counter == 1) { direction = 1; }
		time_cnt = 0;
	}

	for (int i = 1; i < 8; i++) {
		if ((i == counter) || (i == (counter + 1))) {
			_leds_combined_write(i, color);
		} else {
			_leds_combined_write(i, 0);
		}
	}

	_leds_combined_show();

	time_cnt += elapsed_time;
}

void anim__place(uint64_t elapsed_time) {

}

uint32_t color_from_mode(void) {
	uint32_t color = 0;

	switch (mode & MODE_COLOR) {
		case MODE_YELLOW:
			color = strip_l->Color(255,255,0);
		break;
		case MODE_GREEN:
			color = strip_l->Color(75,255,25);
		break;
		case MODE_BLUE:
			color = strip_l->Color(20,115,255);
		break;
		default:
			color = strip_l->Color(255,255,255);
		break;
	}

	return color;
}
