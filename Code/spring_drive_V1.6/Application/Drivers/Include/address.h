#ifndef _ADDRESS_H_
#define _ADDRESS_H_

#include "driver_config.h"

void Address_GPIO_Init(void);
uint8_t get_driver_address(void);

void drive_address_config(void);
void modification_address(uint8_t new_address);



#endif
