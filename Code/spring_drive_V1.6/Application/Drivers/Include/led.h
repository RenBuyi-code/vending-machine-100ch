#ifndef _led_h_
#define _led_h_

#include "driver_config.h"

#define LED_PART  GPIOC
#define LED_PIN 	PIN2

#define LED_OPEN()   GPIO_SetBit(LED_PART, LED_PIN);	
#define LED_CLOSE()	 GPIO_ClrBit(LED_PART, LED_PIN);  	
#define LED_INV()    GPIO_InvBit(LED_PART, LED_PIN);  //×´Ì¬·­×ª


void led_init(void);

#endif

