#ifndef  __TICK__
#define  __TICK__

#include "driver_config.h"


void system_tick_init(void);

void tick_updata(void);
uint32_t get_tick(void);
void tick_reset(void);
void delay_ntick(uint32_t tick); // 1 tick = 100ms
#endif
