#include "ambient.h"


uint8_t __is_ask = FALSE;

const uint8_t ask_fream[8]= {0x02,0x03,0x00,0x00,0x00,0x02,0xc4,0x38};

Modbus modbus_rtu;

void rs485_ch2_recv(void)
{
    uint8_t buff[64]= {0};

    if(TRUE == __RS485_Ch2_RX_Finish)
    {
        rs485_channel2_get_buffer(buff);
        mymemcpy(&modbus_rtu,buff,sizeof(Modbus));

        if(modbus_rtu.rs485_addr == 0x02 && modbus_rtu.funtion_code == 0x03)
        {
            if(modbus_rtu.crc16_code == crc16encode((uint8_t*)&modbus_rtu,sizeof(Modbus)-2))
            {
                vm_states.humidity = modbus_rtu.humidity;
                vm_states.temperature = modbus_rtu.temperature;

            }

        }
        __RS485_Ch2_RX_Finish = FALSE;
    }
}

void ask_ambient(void)
{
    if(TRUE == __is_ask)
    {
        rs485_channel2_send((uint8_t*)ask_fream,sizeof(ask_fream));
        __is_ask = FALSE;
    }
}
