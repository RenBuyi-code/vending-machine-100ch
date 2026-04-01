#ifndef _LOCK_H_
#define _LOCK_H_

#include "driver_config.h"

void lock_io_Init(void);

#define LOCK_PART GPIOE
#define LOCK_PIN  PIN1

#define LOCK_SIG_PART GPIOA
#define LOCK_SIG_PIN	PIN11

#define LOCK_ON()   	GPIO_SetBit(LOCK_PART,LOCK_PIN);
#define LOCK_OFF()		GPIO_ClrBit(LOCK_PART,LOCK_PIN);
#define GET_LOCK_SIG()	(uint8_t)((GPIO_GetBit(LOCK_SIG_PART,LOCK_SIG_PIN))&0x01);

void open_the_door(void);


#endif
