#ifndef _IR_H_
#define	_IR_H_

#include "driver_config.h"

#define IR_PART GPIOA
#define IR_PIN	PIN10

#define IR_POWER_CTR_PART GPIOE
#define IR_POWER_CTR_PIN  PIN0


#define IR_EXTI_CHANNEL  IRQ0_15_GPIOA10
#define IR_IRQn IRQ10_IRQ

#define IR_EXTI_ENABLE()  do{EXTI_Clear(IR_PART,IR_PIN);EXTI_Open(IR_PART,IR_PIN);}while(0)													
													
#define IR_EXTI_DISABLE() do{EXTI_Clear(IR_PART,IR_PIN);EXTI_Close(IR_PART,IR_PIN);}while(0)								
								
#define IR_POWER_ON()  		do{GPIO_SetBit(IR_POWER_CTR_PART,IR_POWER_CTR_PIN);}while(0)
#define IR_POWER_OFF()		do{GPIO_ClrBit(IR_POWER_CTR_PART,IR_POWER_CTR_PIN);}while(0)

#define IR_READ_INPUT()   GPIO_GetBit(IR_PART,IR_PIN)

#define IR_CHECK_OPEN  0xFF
#define IR_CHECK_CLOSE 0XEF

#define IR_NOTWAIT 	0
#define IR_WAIT			1

//extern uint8_t ir_count;
void ir_check_init(void);
void ir_cnt_updata(void);
void ir_cnt_clear(void);
uint8_t ir_cnt_get(void);

void wait_ir_flag_set(void);
void wait_ir_flag_clear(void);
uint8_t wait_ir_flag_get(void);
 

#endif 

