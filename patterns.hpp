#ifndef RBE3001_PATTERNS_H_
#define RBE3001_PATTERNS_H_

#include "globals.hpp"
#include "core.hpp"

void anim__nocomms_idle(uint64_t elapsed_time);
void anim__idle(uint64_t elapsed_time);
void anim__running(uint64_t elapsed_time);
void anim__weigh(uint64_t elapsed_time);
void anim__place(uint64_t elapsed_time);

uint32_t color_from_mode(void);

#endif
