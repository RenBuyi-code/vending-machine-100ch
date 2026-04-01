#include "uart.h"




void uart_init(UART_TypeDef * UARTx,uint32_t baudrate)
{

    UART_InitStructure UART_initStruct;

    if( (uint32_t)UARTx > (uint32_t)UART3 )return;

    switch( (uint32_t)UARTx )
    {
    case (uint32_t)UART0:
    {
        PORT_Init(UART0_RX_PORT, UART0_RX_PIN, FUNMUX_UART0_RXD, 1);	//配置为UART0输入引脚
        PORT_Init(UART0_TX_PORT, UART0_TX_PIN, FUNMUX_UART0_TXD, 0);	//配置为UART0输出引脚
    }
    break;

    case (uint32_t)UART1:
    {
        PORT_Init(UART1_RX_PORT, UART1_RX_PIN, FUNMUX_UART1_RXD, 1);	//配置为UART1输入引脚
        PORT_Init(UART1_TX_PORT, UART1_TX_PIN, FUNMUX_UART1_TXD, 0);	//配置为UART1输出引脚
    }
    break;

    case (uint32_t)UART2:
    {
        PORT_Init(UART2_RX_PORT, UART2_RX_PIN, FUNMUX_UART2_RXD, 1);	//配置为UART2输入引脚
        PORT_Init(UART2_TX_PORT, UART2_TX_PIN, FUNMUX_UART2_TXD, 0);	//配置为UART2输出引脚
    }
    break;

    case (uint32_t)UART3:
    {
        //PORT_Init(UART3_RX_PORT, UART3_RX_PIN, FUNMUX_UART3_RXD, 1);	//配置为UART3输入引脚
        //PORT_Init(UART3_TX_PORT, UART3_TX_PIN, FUNMUX_UART3_TXD, 0);	//配置为UART3输出引脚
    }
    break;

    default:
        break;

    }

    UART_initStruct.Baudrate = baudrate;
    UART_initStruct.DataBits = UART_DATA_8BIT;
    UART_initStruct.Parity = UART_PARITY_NONE;
    UART_initStruct.StopBits = UART_STOP_1BIT;
    UART_initStruct.RXThreshold = 6;
    UART_initStruct.RXThresholdIEn = 1;
    UART_initStruct.TXThresholdIEn = 0;
    UART_initStruct.TimeoutTime = 50;		//10个字符时间内未接收到新的数据则触发超时中断
    UART_initStruct.TimeoutIEn = 1;


    UART_Init(UARTx, &UART_initStruct);
    UART_INTTimeoutEn(UARTx);
    UART_Open(UARTx);

}

void uart_send(UART_TypeDef * UARTx,uint8_t* data,uint32_t dataLength)
{
    uint8_t cnt=0;
    for(cnt=0 ; cnt<dataLength ; cnt++)
    {
        UART_WriteByte(UARTx,data[cnt]);
        while(UART_IsTXBusy(UARTx) == 1);
    }
}

void clear_uart_fifo(UART_TypeDef * UARTx)
{
    uint32_t data=0;
    switch ((uint32_t)UARTx)
    {
    case(uint32_t)UART0:
    {
        while(UART_IsRXFIFOEmpty(UART0) == 0)
        {
            UART_ReadByte(UART0, &data);
        }
    }
    break;

    case(uint32_t)UART1:
    {
        while(UART_IsRXFIFOEmpty(UART1) == 0)
        {
            UART_ReadByte(UART1, &data);
        }
    }
    break;

    case(uint32_t)UART2:
    {
        while(UART_IsRXFIFOEmpty(UART2) == 0)
        {
            UART_ReadByte(UART2, &data);
        }
    }
    break;
    case(uint32_t)UART3:
    {
        while(UART_IsRXFIFOEmpty(UART3) == 0)
        {
            UART_ReadByte(UART3, &data);
        }
    }
    break;
    default:
        break;
    }



}



