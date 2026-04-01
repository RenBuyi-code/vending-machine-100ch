#include "timer.h"

/******************************************************************************************************************************************
* 函数名称: hw_timr_motor_init
* 功能说明: 将通信帧设置为默认值，数据帧清空
* 输    入: VM_Data_Frame* vdf
* 输    出: void
* 注意事项:
******************************************************************************************************************************************/
void hw_timr_motor_init(uint8_t type)
{
	switch(type)
	{
		case FB_MOTOR: 	 
					TIMR_Init(Motor_TIMR, TIMR_MODE_TIMER, SystemCoreClock*5, 3);	//5S中断一次,检测电机超时
					IRQ_Connect(IRQ_TIMR_PERIPH_INT_MOTOR, IRQ_NUMBER_MOTOR, 0);												//定时器0中断链接到IRQ5中断线
					TIMR_Start(Motor_TIMR);
		break;
		
		case FB_CRAWLER_TYPE:
					if(vm_control.timer_value>0x64) vm_control.timer_value =0x64;
					TIMR_Init(Motor_TIMR, TIMR_MODE_TIMER,(SystemCoreClock*vm_control.timer_value), 3);	//5S中断一次,检测电机超时
					IRQ_Connect(IRQ_TIMR_PERIPH_INT_MOTOR, IRQ_NUMBER_MOTOR, 0);												//定时器0中断链接到IRQ5中断线
					TIMR_Start(Motor_TIMR);
		break;
		default:
			break;
	}
}


/******************************************************************************************************************************************
* 函数名称: hw_timr_ir_init
* 功能说明: 将通信帧设置为默认值，数据帧清空
* 输    入: 
						uint8_t type  货道类型
						弹簧货道  FB_MOTOR  FB_CRAWLER_TYPE
* 输    出: void
* 注意事项:
******************************************************************************************************************************************/
void hw_timr_ir_init(uint8_t type)
{
	switch(type)
	{
		case FB_MOTOR: 	 
					TIMR_Init(IR_TIMR, TIMR_MODE_TIMER, SystemCoreClock*7, 3);	//5S中断一次,检测电机超时
					IRQ_Connect(IRQ_TIMR_PERIPH_INT_IR, IRQ_NUMBER_IR, 0);												//定时器0中断链接到IRQ5中断线
					TIMR_Start(IR_TIMR);
		break;
		
		case FB_CRAWLER_TYPE:		
					if(vm_control.timer_value>0x64) vm_control.timer_value =0x64;
					TIMR_Init(IR_TIMR, TIMR_MODE_TIMER, (SystemCoreClock*(vm_control.timer_value+2)), 3);	//5S中断一次,检测电机超时
					IRQ_Connect(IRQ_TIMR_PERIPH_INT_IR, IRQ_NUMBER_IR, 0);												//定时器0中断链接到IRQ5中断线
					TIMR_Start(IR_TIMR);
		break;
		default:
			
			break;
	}
}
