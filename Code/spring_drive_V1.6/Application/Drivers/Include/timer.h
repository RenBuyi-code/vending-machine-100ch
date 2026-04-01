
#ifndef _TIMER_H_
#define _TIMER_H_

#include "driver_config.h"


#define Motor_TIMR 		TIMR0
#define IR_TIMR 	 		TIMR1
#define AMBIENT_TIMR  TIMR2

//periph_interrupt

#define IRQ_TIMR_PERIPH_INT_MOTOR 	IRQ0_15_TIMR0
#define IRQ_TIMR_PERIPH_INT_IR 		IRQ0_15_TIMR1

#define IRQ_NUMBER_MOTOR	IRQ11_IRQ
#define IRQ_NUMBER_IR			IRQ12_IRQ		
	
void hw_timr_Init(void);

void hw_timr_motor_init(uint8_t type);
void hw_timr_ir_init(uint8_t type);

#endif
