#include "ir.h"



static uint8_t ir_count =0;
static uint8_t wait_ir_timeout_flag = IR_NOTWAIT;


static void IR_GPIO_Init(void)
{
    GPIO_Init(IR_PART,IR_PIN,0,1,0,1);
		GPIO_Init(IR_POWER_CTR_PART,IR_POWER_CTR_PIN,1,0,0,0);
    GPIO_SetBit(IR_PART,IR_PIN);
		GPIO_ClrBit(IR_POWER_CTR_PART,IR_POWER_CTR_PIN);	//先不给电
	
}

static void IR_EXTI_Init(void)
{
    EXTI_Init(IR_PART,IR_PIN,EXTI_FALL_EDGE); //下降沿触发

    IRQ_Connect(IR_EXTI_CHANNEL,IR_IRQn,1);

    IR_EXTI_DISABLE();
}


void ir_check_init(void)
{
    IR_GPIO_Init();
    IR_EXTI_Init();
}

void ir_cnt_updata(void)
{
		ir_count++;
}

void ir_cnt_clear(void)
{
		ir_count =0;
}

uint8_t ir_cnt_get(void)
{
	return ir_count;
}

void wait_ir_flag_set(void)
{
		wait_ir_timeout_flag = IR_WAIT;

}

void wait_ir_flag_clear(void)
{
		wait_ir_timeout_flag = IR_NOTWAIT;

}

uint8_t wait_ir_flag_get(void)
{
		return wait_ir_timeout_flag;
	
}

	
	
