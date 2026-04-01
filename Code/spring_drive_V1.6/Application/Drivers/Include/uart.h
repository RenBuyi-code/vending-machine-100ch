#ifndef __uart_H__
#define __uart_H__

#include "driver_config.h"

#define UART0_RX_PORT    PORTA 
#define UART0_RX_PIN		 PIN4	
#define UART0_TX_PORT 	 PORTA
#define UART0_TX_PIN		 PIN5


#define UART1_RX_PORT 	PORTB
#define UART1_RX_PIN		PIN4
#define UART1_TX_PORT 	PORTB
#define UART1_TX_PIN		PIN5

#define UART2_RX_PORT 	PORTA
#define UART2_RX_PIN		PIN8
#define UART2_TX_PORT 	PORTA
#define UART2_TX_PIN		PIN6


//#define UART3_RX_PORT 	
//#define UART3_RX_PIN		
//#define UART3_TX_PORT 	
//#define UART3_TX_PIN		

void uart_send(UART_TypeDef * UARTx,uint8_t* data,uint32_t dataLength);
void uart_init(UART_TypeDef * UARTx,uint32_t baudrate);
void clear_uart_fifo(UART_TypeDef * UARTx);
#endif
