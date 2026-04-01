#include "driver_config.h"
#include "SEGGER_RTT.h"
static const uint16_t MOTOR_EXTI_CNT =2;

//static void delay(uint32_t n)
//{
//	while(n--)
//	{
//		__NOP;
//	}
//}

/******************************************************************************************************************************************

外部中断

******************************************************************************************************************************************/
void IRQ0_Handler(void)
{
    if(TRUE == EXTI_State(MOTOR_FB1_PIN_AND_PART))
    {      
        fb_cnt_updata(FB_CH1);
				SEGGER_RTT_printf(0, "fb1 cnt is %d\r\n",fb_cnt_get(FB_CH1));
				if(vm_control.load_type == FB_MOTOR)
				{
					  if(fb_cnt_get(FB_CH1) >= MOTOR_EXTI_CNT) // 两次 一次上升 一次下降
						{
								//fb_cnt_clear(FB_CH1);
								motor_off( get_current_channel());
								vm_control.load_flag = MOTOR_OK;	//电机没毛病
								if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0))//不需要等红外 ||vm_control.load_type == FB_CRAWLER_TYPE
								{   // 红外检测到有东西掉落 出货成功
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;
								}
		#if DEBUG>0ul							
						SEGGER_RTT_printf(0, "IRQ1 motor cnt end\r\n");
		#endif 					
						}
				
				}
				else if(vm_control.load_type == FB_CRAWLER_TYPE &&(fb_cnt_get(FB_CH1) >= fb_exti_cnt_get()))
				{				
						vm_control.load_flag = MOTOR_OK;	//电机没毛病
						motor_off( get_current_channel());
						fb_cnt_set(0);	
						if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0) )
						{			
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;												
						}
				}
	
				EXTI_Clear(MOTOR_FB1_PIN_AND_PART);
    }
}


void IRQ1_Handler(void)
{
    if(TRUE == EXTI_State(MOTOR_FB2_PIN_AND_PART))
    {
        
        fb_cnt_updata(FB_CH2);
				if(vm_control.load_type == FB_MOTOR)
				{
					if(fb_cnt_get(FB_CH2) >= MOTOR_EXTI_CNT)
					{
							//fb_cnt_clear(FB_CH2);
							motor_off( get_current_channel() );
							vm_control.load_flag = MOTOR_OK;	//电机没毛病
							if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0))//不需要等红外
							{
									__current_result = RESULT_VM_FB_MOTOR_OUTPUT;
							}
							
#if DEBUG>0ul							
					SEGGER_RTT_printf(0, "IRQ2 motor cnt end\r\n");
#endif 							
					}
				}
				else if(vm_control.load_type == FB_CRAWLER_TYPE &&(fb_cnt_get(FB_CH2) >= fb_exti_cnt_get()))
				{				
						vm_control.load_flag = MOTOR_OK;	//电机没毛病
						motor_off(get_current_channel());
						fb_cnt_set(0);	
						if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0) )
						{			
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;												
						}
				}	
				EXTI_Clear(MOTOR_FB2_PIN_AND_PART);
    }

}

void IRQ2_Handler(void)
{
    if(TRUE == EXTI_State(MOTOR_FB3_PIN_AND_PART))
    {
        
        fb_cnt_updata(FB_CH3);
        if(vm_control.load_type == FB_MOTOR)
				{
						if(fb_cnt_get(FB_CH3) >= MOTOR_EXTI_CNT)
						{
								//fb_cnt_clear(FB_CH3);
								motor_off( get_current_channel() );
								vm_control.load_flag = MOTOR_OK;	//电机没毛病
								if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0))//不需要等红外
								{
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;
								}
#if DEBUG>0ul							
		SEGGER_RTT_printf(0, "IRQ2 motor cnt end\r\n");
#endif 							
						}
				}
				else if(vm_control.load_type == FB_CRAWLER_TYPE &&(fb_cnt_get(FB_CH3) >= fb_exti_cnt_get()))
				{				
						vm_control.load_flag = MOTOR_OK;	//电机没毛病
						motor_off(get_current_channel());
						fb_cnt_set(0);	
						if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0) )
						{			
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;												
						}
				}	
				EXTI_Clear(MOTOR_FB3_PIN_AND_PART);
    }

}


void IRQ3_Handler(void)
{
    if(TRUE == EXTI_State(MOTOR_FB4_PIN_AND_PART))
    {
        EXTI_Clear(MOTOR_FB4_PIN_AND_PART);
        fb_cnt_updata(FB_CH4);
				if(vm_control.load_type == FB_MOTOR)
				{
						if(fb_cnt_get(FB_CH4) >= MOTOR_EXTI_CNT)
						{
								fb_cnt_clear(FB_CH4);
								motor_off( get_current_channel() );
								vm_control.load_flag = MOTOR_OK;	//电机没毛病
								if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0))//不需要等红外
								{
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;
								}
#if DEBUG>0ul							
						SEGGER_RTT_printf(0, "IRQ3 motor cnt end\r\n");
#endif 							
						}
				}
				else if(vm_control.load_type == FB_CRAWLER_TYPE &&(fb_cnt_get(FB_CH4) >= fb_exti_cnt_get()))
				{				
						vm_control.load_flag = MOTOR_OK;	//电机没毛病
						motor_off(get_current_channel());
						fb_cnt_set(0);	
						if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0) )
						{			
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;												
						}
				}				
    }

}

void IRQ4_Handler(void)
{
    if(TRUE == EXTI_State(MOTOR_FB5_PIN_AND_PART))
    {
       
        fb_cnt_updata(FB_CH5);
				if(vm_control.load_type == FB_MOTOR)
				{
						 if(fb_cnt_get(FB_CH5) >= MOTOR_EXTI_CNT)
						{
								fb_cnt_clear(FB_CH5);
								motor_off( get_current_channel() );
								vm_control.load_flag = MOTOR_OK;	//电机没毛病
								if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0))//不需要等红外
								{
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;
								}
		#if DEBUG>0ul							
						SEGGER_RTT_printf(0, "IRQ4 motor cnt end\r\n");
		#endif 							
						}
				}
				else if(vm_control.load_type == FB_CRAWLER_TYPE &&(fb_cnt_get(FB_CH5) >= fb_exti_cnt_get()))
				{				
						vm_control.load_flag = MOTOR_OK;	//电机没毛病
						motor_off(get_current_channel());
						fb_cnt_set(0);	
						if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0) )
						{			
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;												
						}
				}	
				 EXTI_Clear(MOTOR_FB5_PIN_AND_PART);
    }	

}

void IRQ5_Handler(void)
{
    if(TRUE == EXTI_State(MOTOR_FB6_PIN_AND_PART))
    {
        
        fb_cnt_updata(FB_CH6);
				if(vm_control.load_type == FB_MOTOR)
				{
						if(fb_cnt_get(FB_CH6) >= MOTOR_EXTI_CNT)
						{
								fb_cnt_clear(FB_CH6);
								motor_off( get_current_channel() );
								vm_control.load_flag = MOTOR_OK;	//电机没毛病
								if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0))//不需要等红外
								{
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;
								}
		#if DEBUG>0ul							
						SEGGER_RTT_printf(0, "IRQ5 motor cnt end\r\n");
		#endif 							
						}
				}
				else if(vm_control.load_type == FB_CRAWLER_TYPE &&(fb_cnt_get(FB_CH6) >= fb_exti_cnt_get()))
				{				
						vm_control.load_flag = MOTOR_OK;	//电机没毛病
						motor_off(get_current_channel());
						fb_cnt_set(0);	
						if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0) )
						{			
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;												
						}
				}	
				EXTI_Clear(MOTOR_FB6_PIN_AND_PART);
    }

}

void IRQ6_Handler(void)
{
    if(TRUE == EXTI_State(MOTOR_FB7_PIN_AND_PART))
    {
        
        fb_cnt_updata(FB_CH7);
				if(vm_control.load_type == FB_MOTOR)
				{
						if(fb_cnt_get(FB_CH7) >= MOTOR_EXTI_CNT)
						{
								fb_cnt_clear(FB_CH7);
								motor_off( get_current_channel() );
								vm_control.load_flag = MOTOR_OK;	//电机没毛病
								if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0))//不需要等红外
								{
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;
								}
		#if DEBUG>0ul							
						SEGGER_RTT_printf(0, "IRQ6 motor cnt end\r\n");
		#endif 							
						}
				}
				else if(vm_control.load_type == FB_CRAWLER_TYPE &&(fb_cnt_get(FB_CH7) >= fb_exti_cnt_get()))
				{				
						vm_control.load_flag = MOTOR_OK;	//电机没毛病
						motor_off(get_current_channel());
						fb_cnt_set(0);	
						if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0) )
						{			
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;												
						}
				}	
				EXTI_Clear(MOTOR_FB7_PIN_AND_PART);
    }

}

void IRQ7_Handler(void)
{
    if(TRUE == EXTI_State(MOTOR_FB8_PIN_AND_PART))
    {
        
        fb_cnt_updata(FB_CH8);
				if(vm_control.load_type == FB_MOTOR)
				{
						if(fb_cnt_get(FB_CH8) >= MOTOR_EXTI_CNT)
						{
								fb_cnt_clear(FB_CH8);
								motor_off( get_current_channel() ); //里面有关闭电机超时中断检测的操作
								vm_control.load_flag = MOTOR_OK;	//电机没毛病
								if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0))//不需要等红外
								{
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;
								}
		#if DEBUG>0ul							
						SEGGER_RTT_printf(0, "IRQ7 motor cnt end\r\n");
		#endif 							
						}
				}
				else if(vm_control.load_type == FB_CRAWLER_TYPE &&(fb_cnt_get(FB_CH8) >= fb_exti_cnt_get()))
				{				
						vm_control.load_flag = MOTOR_OK;	//电机没毛病
						motor_off(get_current_channel());
						fb_cnt_set(0);	
						if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0) )
						{			
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;												
						}
				}	
				EXTI_Clear(MOTOR_FB8_PIN_AND_PART);

    }

}

void IRQ8_Handler(void)
{
    if(TRUE == EXTI_State(MOTOR_FB9_PIN_AND_PART))
    {
        
        fb_cnt_updata(FB_CH9);
				if(vm_control.load_type == FB_MOTOR)
				{
							if(fb_cnt_get(FB_CH9) >= MOTOR_EXTI_CNT)
							{
									motor_off( get_current_channel() );
									fb_cnt_clear(FB_CH9);
									vm_control.load_flag = MOTOR_OK;	//电机没毛病
									if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0))//不需要等红外
									{
											__current_result = RESULT_VM_FB_MOTOR_OUTPUT;
									}
			#if DEBUG>0ul							
							SEGGER_RTT_printf(0, "IRQ8 motor cnt end\r\n");
			#endif 							
							}
				}
				else if(vm_control.load_type == FB_CRAWLER_TYPE &&(fb_cnt_get(FB_CH9) >= fb_exti_cnt_get()))
				{				
						vm_control.load_flag = MOTOR_OK;	//电机没毛病
						motor_off(get_current_channel());
						fb_cnt_set(0);	
						if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0) )
						{			
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;												
						}
				}	
				EXTI_Clear(MOTOR_FB9_PIN_AND_PART);

    }

}


void IRQ9_Handler(void)
{
    if(TRUE == EXTI_State(MOTOR_FB10_PIN_AND_PART))
    {
        
        fb_cnt_updata(FB_CH10);
				if(vm_control.load_type == FB_MOTOR)
				{
						if(fb_cnt_get(FB_CH10) >= MOTOR_EXTI_CNT)
						{
								motor_off( get_current_channel() );
								vm_control.load_flag = MOTOR_OK;	//电机没毛病
								fb_cnt_clear(FB_CH10);
								if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get() != 0))//不需要等红外或已经出货
								{
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;
								}
		#if DEBUG>0ul							
						SEGGER_RTT_printf(0, "IRQ9 motor cnt end\r\n");
		#endif 	
						}
				}
				else if(vm_control.load_type == FB_CRAWLER_TYPE &&(fb_cnt_get(FB_CH10) >= fb_exti_cnt_get()))
				{				
						vm_control.load_flag = MOTOR_OK;	//电机没毛病
						motor_off(get_current_channel());
						fb_cnt_set(0);	
						if(vm_control.ir_flag == IR_CHECK_CLOSE || (ir_cnt_get()!=0) )
						{			
										__current_result = RESULT_VM_FB_MOTOR_OUTPUT;												
						}
				}	
				EXTI_Clear(MOTOR_FB10_PIN_AND_PART);

    }
}


void IRQ10_Handler(void)  //红外引脚中断
{
    if(TRUE == EXTI_State(IR_PART,IR_PIN))
    {
        EXTI_Clear(IR_PART,IR_PIN);
        TIMR_Stop(IR_TIMR);//停止红外超时检测
        //TIMR_Stop(Motor_TIMR);
			
        ir_cnt_updata();
        vm_control.ir_flag = IR_First_Blood;			//只掉落一个
        //vm_control.load_flag = MOTOR_OK;    
				//wait_ir_flag_clear();		
				__current_result = RESULT_VM_FB_MOTOR_OUTPUT;
			
				EXTI_Close(IR_PART,IR_PIN);
			
    }
}

void IRQ11_Handler(void) //检测电机超时 5S
{
    if(TIMR_INTStat(Motor_TIMR))
    {
        TIMR_INTClr(Motor_TIMR);
        //TIMR_Stop(IR_TIMR);
				
				if(vm_control.load_type == FB_MOTOR)
				{
					 motor_off( get_current_channel());
					 vm_control.load_flag = MOTOR_TIMER_OUT;	 //电机超时（没在指定时间内转回卡口） 有可能卡货		
				}
				else
				{
					
				}				
				if(IR_NOTWAIT == wait_ir_flag_get())
				{
					__current_result = RESULT_VM_FB_MOTOR_OUTPUT;
				}
   
#if DEBUG>0ul			
				SEGGER_RTT_printf(0, "IRQ11 motor time out\r\n");
#endif    
			  TIMR_Stop(Motor_TIMR);
		}
}

void IRQ12_Handler(void)	//检测 出货超时
{
    if(TIMR_INTStat(IR_TIMR))
    {	
        if(0 == ir_cnt_get())//出货超时
        {
						if(vm_control.load_type == FB_MOTOR)
						{
								vm_control.ir_flag = IR_TIMER_OUT; //没有检测到掉落      								
								__current_result = RESULT_VM_FB_MOTOR_OUTPUT; //输出结果
						}
						else if(vm_control.load_type == FB_CRAWLER_TYPE)
						{
								motor_off( get_current_channel());
								vm_control.ir_flag = IR_TIMER_OUT; //没有检测到掉落  
								//vm_control.load_flag = MOTOR_TIMER_OUT;
								__current_result = RESULT_VM_FB_MOTOR_OUTPUT; //输出结果
						}

        }
#if DEBUG>0ul			
				SEGGER_RTT_printf(0, "IRQ12 ir time out\r\n");
#endif 			
				TIMR_INTClr(IR_TIMR);		
        TIMR_Stop(IR_TIMR);
    }

}


void IRQ28_Handler(void)//串口0 中断
{
    uint32_t chr;
		int i =0;
	
		
    if(UART_INTRXThresholdStat(RS485_CH1_UART) ==1 ) //接收
    {

        while(UART_IsRXFIFOEmpty(RS485_CH1_UART) == 0) //fifo 不为空部分
        {
            if(UART_ReadByte(RS485_CH1_UART, &chr) == 0) //读fifo里的东西
            {
                if(RS485_Ch1_RXIndex < RS485_CH1_RX_LEN) //缓存区没超
                {
                    RS485_Ch1_Buffer[RS485_Ch1_RXIndex] = chr&0xFF;

                    RS485_Ch1_RXIndex++;
                }
            }
        }
    }
    if(UART_INTTimeoutStat(RS485_CH1_UART) == 1)
    {
        while(UART_IsRXFIFOEmpty(RS485_CH1_UART) == 0) //fifo 不为空部分
        {
            if(UART_ReadByte(RS485_CH1_UART, &chr) == 0) //读fifo里的东西
            {
                if(RS485_Ch1_RXIndex < RS485_CH1_RX_LEN) //缓存区没超
                {
                    RS485_Ch1_Buffer[RS485_Ch1_RXIndex] = chr&0xFF;
                    RS485_Ch1_RXIndex++;
                }
            }
        }
#if DEBUG>0ul							
				SEGGER_RTT_printf(0, "rec:");
				for(i=0;i<18;i++)
				{
					SEGGER_RTT_printf(0, "%x ",RS485_Ch1_Buffer[i]);
				
				}
				SEGGER_RTT_printf(0, "\r\n ");
#endif 				
        __RS485_Ch1_RX_Finish =1;

    }

}



// RS485_CH2 RX Handler
void IRQ29_Handler(void)
{
    uint32_t chr;

    if(UART_INTRXThresholdStat(RS485_CH2_UART) ==1)
    {
        while(UART_IsRXFIFOEmpty(RS485_CH2_UART) == 0)
        {
            if(UART_ReadByte(RS485_CH2_UART, &chr) == 0)
            {
                if(RS485_Ch2_RXIndex < RS485_CH2_RX_LEN)
                {
                    RS485_Ch2_Buffer[RS485_Ch2_RXIndex] = chr;

                    RS485_Ch2_RXIndex++;
                }
            }
        }
    }
    if(UART_INTTimeoutStat(RS485_CH2_UART) ==1 )
    {
        while(UART_IsRXFIFOEmpty(RS485_CH2_UART) == 0)
        {

            if(UART_ReadByte(RS485_CH2_UART, &chr) == 0)
            {
                if(RS485_Ch2_RXIndex < RS485_CH2_RX_LEN)
                {
                    RS485_Ch2_Buffer[RS485_Ch2_RXIndex] = chr;

                    RS485_Ch2_RXIndex++;
                }
            }
        }

        __RS485_Ch2_RX_Finish = TRUE;
    }
}




uint32_t scan_tick=0;
uint32_t led_tick=0;
void SysTick_Handler(void)
{
    scan_tick++;
    led_tick++;
    tick_updata();


    if(scan_tick == 100)// 100*100ms  10S
    {
        __is_ask = TRUE;
        scan_tick = 0;

    }

}

