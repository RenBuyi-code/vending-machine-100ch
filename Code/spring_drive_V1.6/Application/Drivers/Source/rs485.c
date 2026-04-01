#include "RS485.h"

volatile uint8_t __RS485_Ch1_RX_Finish = 0;
volatile uint8_t __RS485_Ch2_RX_Finish = 0;


volatile uint32_t RS485_Ch2_RXIndex = 0;
volatile uint8_t  RS485_Ch2_Buffer[RS485_CH2_RX_LEN]= {0};

volatile uint32_t RS485_Ch1_RXIndex = 0;
volatile uint8_t  RS485_Ch1_Buffer[RS485_CH1_RX_LEN]= {0};


void rs485_channel1_config(void)
{
    uart_init(RS485_CH1_UART,9600);

    GPIO_Init(RS485_CH1_DIR_PART,RS485_CH1_DIR_PIN,1,0,0,0);

    IRQ_Connect(RS485_CH1_INTERRUPT, RS485_CH1_INTERRUPT_NUMBER, 0); //RS485 CH1 IRQ cfg

    RS485_CH1_RX();
}

void rs485_channel2_config(void)
{
    uart_init(RS485_CH2_UART,9600);

    GPIO_Init(RS485_CH2_DIR_PART,RS485_CH2_DIR_PIN,1,0,0,0);

    IRQ_Connect(RS485_CH2_INTERRUPT, RS485_CH2_INTERRUPT_NUMBER, 3);//RS485 CH2 IRQ cfg

    RS485_CH2_RX();

}

void rs485_channel1_send(uint8_t* data,uint32_t dataLength)
{
    RS485_CH1_TX();

    uart_send(RS485_CH1_UART,data,dataLength);

    RS485_CH1_RX();
}


void rs485_channel2_send(uint8_t* data,uint32_t dataLength)
{

    RS485_CH2_TX();

    uart_send(RS485_CH2_UART,data,dataLength);

    RS485_CH2_RX();

}



uint32_t rs485_channel1_get_buffer(uint8_t *data)
{
    uint32_t len = 0;

    if(RS485_Ch1_RXIndex != 0)
    {
        NVIC_DisableIRQ(RS485_CH1_INTERRUPT_NUMBER);		//从UART_RXBuffer读取数据过程中要关闭中断，防止读写混乱
				
				//UART_Close(RS485_CH1_UART);
        mymemcpy(data,(uint8_t*)RS485_Ch1_Buffer, RS485_Ch1_RXIndex);
        mymemset((uint8_t*)RS485_Ch1_Buffer,0,RS485_Ch1_RXIndex);
        len = RS485_Ch1_RXIndex;
        RS485_Ch1_RXIndex = 0;
        //UART_Open(RS485_CH1_UART);
				NVIC_EnableIRQ(RS485_CH1_INTERRUPT_NUMBER);
    }

    return len;
}


uint32_t rs485_channel2_get_buffer(uint8_t *data)
{
    uint32_t len = 0;

    if(RS485_Ch2_RXIndex != 0)
    {
        NVIC_DisableIRQ(RS485_CH2_INTERRUPT_NUMBER);		//从UART_RXBuffer读取数据过程中要关闭中断，防止读写混乱
        mymemcpy(data,(uint8_t*)RS485_Ch2_Buffer, RS485_CH2_RX_LEN);
        len = RS485_Ch2_RXIndex;
        RS485_Ch2_RXIndex = 0;
        NVIC_EnableIRQ(RS485_CH2_INTERRUPT_NUMBER);
    }

    return len;
}

void rs485_channel1_clear_buffer(void)
{
		clear_uart_fifo(RS485_CH1_UART);
    mymemset((uint8_t*)RS485_Ch1_Buffer,0,RS485_CH1_RX_LEN);
    RS485_Ch1_RXIndex =0;
}
