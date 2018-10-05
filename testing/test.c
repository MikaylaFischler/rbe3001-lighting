#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

uint8_t packet_malformed(uint8_t packet) {
	//		validate blank			validate mode				validate color				validate running
	return (packet & 0x40) || !((packet & 0x30) ^ 0x30) || !((packet & 0xE) ^ 0xE) || ((packet & 0x3E) && ~(packet & 0x1));
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

	return 0;
}
