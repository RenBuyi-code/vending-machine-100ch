#include "tick.h"

uint32_t tick_cnt = 0;

void system_tick_init(void)
{
    SysTick_Config(SystemCoreClock/10);	//10Hz 100ms
}

void tick_updata(void)
{
    tick_cnt++;
}

uint32_t get_tick(void)
{
    return tick_cnt;
}

void tick_reset(void)
{
    tick_cnt =0;
}

void delay_ntick(uint32_t tick)
{
	uint32_t delay_tick= 0;
	tick_reset(); //Çå¿Õ¼ÆÊýÆ÷
	
	delay_tick = tick+get_tick(); //100ms*3 = 300ms
	while(1)
	{		
		if(delay_tick <= get_tick())
		{
				break;
		}	
	}

}


