#ifndef _DRIVER_CONFIG_H_
#define _DRIVER_CONFIG_H_

#include "SWM181.h"

#include "motor_driver.h"
#include "uart.h"
#include "rs485.h"
#include "led.h"
#include "string.h"
#include "timer.h"
#include "ir.h"
#include "vm.h"
#include "lock.h"
#include "address.h"
#include "ambient.h"
#include "tick.h"

extern uint32_t led_tick;

#include <string.h>
#include "mymalloc.h"	

#define ENABLE_WDT 0

#define RTOS 0

#if RTOS>0u
#include "rtthread.h"
#endif

#define DIR_INPUT 			0
#define DIR_OUTPUR			1

#define PULL_UP_ENALBE	1
#define PULL_UP_DISALBE	0

#define PULL_DOWN_ENALBE	1
#define PULL_DOWN_DISALBE	0

#define PUSH_PULL  0
#define OPEN_DRAIN 1

#define ENABLE  1
#define DISABLE 0

#define TRUE  1
#define FALSE 0






#endif

