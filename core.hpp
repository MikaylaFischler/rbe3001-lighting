#ifndef RBE3001_CORE_H_
#define RBE3001_CORE_H_

#include <SoftwareSerial.h>
#include "globals.hpp"

// led util functions
void _leds_combined_write(uint8_t pixel, uint32_t color);
void _leds_combined_show(void);

#include "patterns.hpp"

// #define DEBUG

// SoftwareSerial config
#define SOFT_UART_TX 2
#define SOFT_UART_RX 3

// strip configurations
#define STRIP_LENGTH 9
#define PIN_STRIP_L 11
#define PIN_STRIP_R 10

// led animation bitfields
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

// mask checks
#define MODE_COMMS		0x80
#define MODE_RUNNING	0x01
#define MODE_COLOR		0x0E
#define MODE_YELLOW		0x02
#define MODE_GREEN		0x04
#define MODE_BLUE		0x08
#define MODE_WEIGH		0x10
#define MODE_PLACE		0x20
#define MODE_HEAVY		0x40

// SoftwareSerial
void serial_init(void);
void serial_request(void);
void serial_read(void);

// led functoins
void leds_init(void);
void leds_update(void);
void leds_run(uint64_t elapsed_time);

// check packet validity
inline uint8_t packet_malformed(uint8_t packet);

#endif
