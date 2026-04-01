#ifndef __RS485_H__
#define __RS485_H__

#include "driver_config.h"


#define RS485_CH1_DIR_PART  GPIOB
#define RS485_CH1_DIR_PIN		PIN4

#define RS485_CH2_DIR_PART	GPIOA
#define RS485_CH2_DIR_PIN		PIN7

#define RS485_CH1_RX() 		GPIO_ClrBit(RS485_CH1_DIR_PART,RS485_CH1_DIR_PIN)
#define RS485_CH1_TX()		GPIO_SetBit(RS485_CH1_DIR_PART,RS485_CH1_DIR_PIN)

#define RS485_CH2_RX() 		GPIO_ClrBit(RS485_CH2_DIR_PART,RS485_CH2_DIR_PIN)
#define RS485_CH2_TX()		GPIO_SetBit(RS485_CH2_DIR_PART,RS485_CH2_DIR_PIN)


#define RS485_CH1_UART   UART0
#define RS485_CH2_UART   UART2

#define RS485_CH1_INTERRUPT  IRQ16_31_UART0  //16-31 ∂‘”¶IRQ16 - IRQ 31
#define RS485_CH2_INTERRUPT  IRQ16_31_UART2

#define RS485_CH1_INTERRUPT_NUMBER IRQ28_IRQ
#define RS485_CH2_INTERRUPT_NUMBER IRQ29_IRQ

#define RS485_CH1_RX_LEN (0x12)
#define RS485_CH2_RX_LEN (0x20)


void rs485_channel1_config(void);
void rs485_channel2_config(void);

void rs485_channel1_send(uint8_t* data,uint32_t dataLength);
void rs485_channel2_send(uint8_t* data,uint32_t dataLength);
void rs485_channel1_clear_buffer(void);

uint32_t rs485_channel1_get_buffer(uint8_t *data);
uint32_t rs485_channel2_get_buffer(uint8_t *data);


extern	volatile uint8_t __RS485_Ch1_RX_Finish;
extern	volatile uint8_t __RS485_Ch2_RX_Finish;
extern	volatile uint8_t __UART2_RX_Finish;

extern volatile uint32_t RS485_Ch2_RXIndex;
extern volatile uint8_t  RS485_Ch2_Buffer[RS485_CH2_RX_LEN];

extern volatile uint32_t RS485_Ch1_RXIndex;
extern volatile uint8_t  RS485_Ch1_Buffer[RS485_CH1_RX_LEN];



#endif
