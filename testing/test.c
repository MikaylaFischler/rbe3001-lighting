#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

uint8_t packet_malformed(uint8_t packet) {
	return
		// validate connection/enable bit
		((~packet & 0x80) 	&&  (packet & 0x7F)) ||
		// validate running bit
		((~packet & 0x1)	&&  (packet & 0x7E)) ||
		// validate color uniqueness
		(((packet & 0x4) >> 1)	& (packet & 0x2)) ||
		(((packet & 0x8) >> 2)	& (packet & 0x2)) ||
		(((packet & 0x8) >> 1)	& (packet & 0x4)) ||
		// validate state
		(((packet & 0x20) >> 1) & (packet & 0x10)) ||
		(((packet & 0x40) >> 2) & (packet & 0x10)) ||
		(((packet & 0x40) >> 1) & (~packet & 0x20));
}

#define ST_MODE_NOCOMMS_IDLE	0x00
#define ST_MODE_IDLE			0x80
#define ST_MODE_RUNNING			0x81
#define ST_MODE_FOUND_YELLOW	0x83
#define	ST_MODE_FOUND_GREEN		0x85
#define ST_MODE_FOUND_BLUE		0x89
#define ST_MODE_WEIGH_YELLOW	0x93
#define ST_MODE_WEIGH_GREEN		0x95
#define ST_MODE_WEIGH_BLUE		0x99
#define ST_MODE_PLACE_YELLOW	0xA3
#define ST_MODE_PLACE_GREEN		0xA5
#define ST_MODE_PLACE_BLUE		0xA9
#define ST_MODE_PLACE_H_YELLOW	0xE3
#define ST_MODE_PLACE_H_GREEN	0xE5
#define ST_MODE_PLACE_H_BLUE	0xE9

#define INVALID_1				0x79
#define INVALID_2				0x8D
#define INVALID_3				0xF5

int main() {
	printf("%d\n", packet_malformed(ST_MODE_NOCOMMS_IDLE));
	printf("%d\n", packet_malformed(ST_MODE_IDLE));
	printf("%d\n", packet_malformed(ST_MODE_RUNNING));
	printf("%d\n", packet_malformed(ST_MODE_FOUND_YELLOW));
	printf("%d\n", packet_malformed(ST_MODE_FOUND_GREEN));
	printf("%d\n", packet_malformed(ST_MODE_FOUND_BLUE));
	printf("%d\n", packet_malformed(ST_MODE_WEIGH_YELLOW));
	printf("%d\n", packet_malformed(ST_MODE_WEIGH_GREEN));
	printf("%d\n", packet_malformed(ST_MODE_WEIGH_BLUE));
	printf("%d\n", packet_malformed(ST_MODE_PLACE_YELLOW));
	printf("%d\n", packet_malformed(ST_MODE_PLACE_GREEN));
	printf("%d\n", packet_malformed(ST_MODE_PLACE_BLUE));
	printf("%d\n", packet_malformed(ST_MODE_PLACE_H_YELLOW));
	printf("%d\n", packet_malformed(ST_MODE_PLACE_H_GREEN));
	printf("%d\n", packet_malformed(ST_MODE_PLACE_H_BLUE));

	printf("%d\n", packet_malformed(INVALID_1));
	printf("%d\n", packet_malformed(INVALID_2));
	printf("%d\n", packet_malformed(INVALID_3));

	return 0;
}
