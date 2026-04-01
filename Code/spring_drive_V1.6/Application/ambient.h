#ifndef _AMBIENT_H_
#define _AMBIENT_H_

#include "driver_config.h"

#pragma pack(1)
typedef struct Modbus_T
{
    uint8_t  rs485_addr;
    uint8_t  funtion_code;
    uint8_t  byte_len;
    uint16_t humidity;
    uint16_t temperature;
    uint16_t crc16_code;

} Modbus;
#pragma pack()

void ask_ambient(void);
void rs485_ch2_recv(void);

extern Modbus modbus_rtu;
extern uint8_t __is_ask;

#endif

