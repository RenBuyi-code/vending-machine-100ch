#include "motor_driver.h"
#include "SEGGER_RTT.h"

/**电机忙标志**/
static uint8_t  load_stat = 0;
/**电机忙标志**/

/**位置反馈计数标志**/
static uint8_t  fb_count_Y1 =	0;
static uint8_t  fb_count_Y2 =	0;
static uint8_t  fb_count_Y3 =	0;
static uint8_t  fb_count_Y4 =	0;
static uint8_t  fb_count_Y5 =	0;
static uint8_t  fb_count_Y6 =	0;
static uint8_t  fb_count_Y7 =	0;
static uint8_t  fb_count_Y8 =	0;
static uint8_t  fb_count_Y9 =	0;
static uint8_t  fb_count_Y10 =0;
/**位置反馈计数标志**/
static uint8_t fb_exti_cnt =0;

static uint8_t current_num =0;

/**控制及反馈引脚表**/
static const struct  MotorDrive_TAB 
{
    MotorDrive_GPIO_PART* part_x;
    MotorDrive_GPIO_Pin pin_x;

    MotorDrive_GPIO_PART* part_y;
    MotorDrive_GPIO_Pin pin_y;

    MotorFB_GPIO_PART*  fb_part;
    MotorFB_GPIO_Pin   fb_pin;

} motorDrive_tab[11]=
{
    {0,0,0,0,0,0},//占个坑 好计算
    {DRIVER_X1_PIN_AND_PART,DRIVER_Y1_PIN_AND_PART, MOTOR_FB1_PIN_AND_PART},
    {DRIVER_X2_PIN_AND_PART,DRIVER_Y2_PIN_AND_PART, MOTOR_FB2_PIN_AND_PART},
    {DRIVER_X3_PIN_AND_PART,DRIVER_Y3_PIN_AND_PART, MOTOR_FB3_PIN_AND_PART},
    {DRIVER_X4_PIN_AND_PART,DRIVER_Y4_PIN_AND_PART, MOTOR_FB4_PIN_AND_PART},
    {DRIVER_X5_PIN_AND_PART,DRIVER_Y5_PIN_AND_PART, MOTOR_FB5_PIN_AND_PART},
    {DRIVER_X6_PIN_AND_PART,DRIVER_Y6_PIN_AND_PART, MOTOR_FB6_PIN_AND_PART},
    {DRIVER_X7_PIN_AND_PART,DRIVER_Y7_PIN_AND_PART, MOTOR_FB7_PIN_AND_PART},
    {DRIVER_X8_PIN_AND_PART,DRIVER_Y8_PIN_AND_PART, MOTOR_FB8_PIN_AND_PART},
    {DRIVER_X9_PIN_AND_PART,DRIVER_Y9_PIN_AND_PART, MOTOR_FB9_PIN_AND_PART},
    {DRIVER_X10_PIN_AND_PART,DRIVER_Y10_PIN_AND_PART, MOTOR_FB10_PIN_AND_PART}

};
/**控制及反馈引脚表**/


/***********本地调用**********/
static uint8_t current_channel_updata(uint8_t number);
static uint8_t get_x_num(uint8_t number);
static uint8_t get_y_num(uint8_t number);
static uint8_t check_xy(uint8_t x,uint8_t y);
static uint8_t check_ir(void);
static uint8_t check_motor(uint8_t x, uint8_t y);
static void motor_driver_io_init(void);
static void motor_fb_io_init(void);
static void motor_fb_exti_init(void);
/**********本地调用************/


/****************************************************************************************************************************************** 
* 函数名称: motor_init
* 功能说明: 初始化电机控制以及反馈引脚
* 输    入: void
* 输    出: void
* 注意事项: 外部调用
******************************************************************************************************************************************/
void motor_init(void)
{

    motor_driver_io_init();
    motor_fb_io_init();
    motor_fb_exti_init();

}

/****************************************************************************************************************************************** 
* 函数名称: motor_fb_exti_enable
* 功能说明: 使能位置反馈引脚中断
* 输    入: y 电机列 
* 输    出: 成功返回1 否则返回-1
* 注意事项: 外部调用
******************************************************************************************************************************************/
int motor_fb_exti_enable(uint8_t y)//开电机位置线中断
{
    if(y > 0x0A || y==0)return -1;

    EXTI_Open(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin);

    return 1;
}
/****************************************************************************************************************************************** 
* 函数名称: motor_fb_exti_disable
* 功能说明: 失能位置反馈引脚中断
* 输    入: y 电机列 
* 输    出: 成功返回1 否则返回-1
* 注意事项: 外部调用
******************************************************************************************************************************************/
int motor_fb_exti_disable(uint8_t y)//关点击位置线中断
{
    if(y > 10 || y==0)return -1;

    EXTI_Close(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin);

    return 1;
}

/****************************************************************************************************************************************** 
* 函数名称: fb_motor_on
* 功能说明: 有反馈电机转一圈
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
int fb_motor_on(uint8_t number)	//电机旋转一圈
{
    uint8_t x =0;
    uint8_t y =0;
    uint8_t  err=0;
	
    err = current_channel_updata(number);
    if(err == 0xEE)return -1;
    
		x =	get_x_num(number);
    y = get_y_num(number);

    err = check_xy(x,y);
    if(err == 0xEE)return -1;
	
#if DEBUG >0ul
		SEGGER_RTT_printf(0, "channel is ture....\r\n");
#endif		
    if(vm_control.ir_flag == IR_CHECK_OPEN)
    {		
        err = check_ir();
        if(err == 0xEE)return -1;
				wait_ir_flag_set();		
			
				hw_timr_ir_init(vm_control.load_type);	
			
#if DEBUG > 0ul				
				SEGGER_RTT_printf(0, "Enable IR TIMER....\r\n");	
#endif					
			
        IR_EXTI_ENABLE();			 //打开红外引脚中断
			
#if DEBUG > 0ul
		SEGGER_RTT_printf(0, "Enable IR EXTI....\r\n");
#endif				
			
    }
    else if(vm_control.ir_flag == IR_CHECK_CLOSE)
    {
        IR_EXTI_DISABLE();
#if DEBUG >0ul
		SEGGER_RTT_printf(0, "Disable IR EXTI....\r\n");
#endif						
    }
    else
    {
        return -1; //这一步很重要
    }
		
		
		hw_timr_motor_init(vm_control.load_type);
		
#if ENABLE_WDT >0u		
		SEGGER_RTT_printf(0, "motor timer start....\r\n");
#endif
		

		err = check_motor(x,y);
    if(err == 0xEE)
    {
#if ENABLE_WDT >0u			
				SEGGER_RTT_printf(0, "motor error....\r\n");
#endif			
        return -1;
    }	

    if( load_status_get() == LOAD_IS_FREE)
    {				
        load_set_buys();
#if ENABLE_WDT >0u	
				SEGGER_RTT_printf(0, "motor outputting....\r\n");
#endif			
    }

    
		return 1;
}

/****************************************************************************************************************************************** 
* 函数名称: motor_off
* 功能说明: 
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
int motor_off(uint8_t number)	//电机停止旋转
{
    uint8_t x =0;
    uint8_t y =0;
    uint8_t err=0;
	
    x =	get_x_num(number);
    y = get_y_num(number);

    err = check_xy(x,y);
    if(err == 0xEE)return -1;


    if( x>10 || y>10 || x==0 || y==0 )
    {
        return -1;
    }
    else
    {
        GPIO_ClrBit(motorDrive_tab[x].part_x,motorDrive_tab[x].pin_x);
        GPIO_SetBit(motorDrive_tab[y].part_y,motorDrive_tab[y].pin_y);	
        motor_fb_exti_disable(y);
    }

		load_free_buys();
    return 1;
}

/****************************************************************************************************************************************** 
* 函数名称: load_status_get
* 功能说明: 
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
uint8_t load_status_get(void)
{				
			return load_stat;
}

/****************************************************************************************************************************************** 
* 函数名称: load_free_buys
* 功能说明: 
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
void load_free_buys(void)
{
		load_stat = LOAD_IS_FREE;
}

/****************************************************************************************************************************************** 
* 函数名称: load_set_buys
* 功能说明: 
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
void load_set_buys(void)
{
		load_stat = LOAD_IS_BUYS;
}

/****************************************************************************************************************************************** 
* 函数名称: fb_cnt_updata
* 功能说明: 
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
void fb_cnt_updata(uint8_t fb_num)	
{

    switch(fb_num)
    {
    case FB_CH1:
        fb_count_Y1++;
        break;
    case FB_CH2:
        fb_count_Y2++;
        break;
    case FB_CH3:
        fb_count_Y3++;
        break;
    case FB_CH4:
        fb_count_Y4++;
        break;
    case FB_CH5:
        fb_count_Y5++;
        break;
    case FB_CH6:
        fb_count_Y6++;
        break;
    case FB_CH7:
        fb_count_Y7++;
        break;
    case FB_CH8:
        fb_count_Y8++;
        break;
    case FB_CH9:
        fb_count_Y9++;
        break;
    case FB_CH10:
        fb_count_Y10++;
        break;
    default:
        break;
    }

}

/****************************************************************************************************************************************** 
* 函数名称: fb_cnt_clear_all
* 功能说明: 
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
void fb_cnt_clear_all()
{
    fb_count_Y1 =0;
    fb_count_Y2 =0;
    fb_count_Y3 =0;
    fb_count_Y4 =0;
    fb_count_Y5 =0;
    fb_count_Y6 =0;
    fb_count_Y7 =0;
    fb_count_Y8 =0;
    fb_count_Y9 =0;
    fb_count_Y10=0;
}

/****************************************************************************************************************************************** 
* 函数名称: fb_cnt_clear
* 功能说明: 
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
void fb_cnt_clear(uint8_t fb_num)
{
    switch(fb_num)
    {
    case FB_CH1:
        fb_count_Y1 =0;
        break;
    case FB_CH2:
        fb_count_Y2 =0;
        break;
    case FB_CH3:
        fb_count_Y3 =0;
        break;
    case FB_CH4:
        fb_count_Y4 =0;
        break;
    case FB_CH5:
        fb_count_Y5 =0;
        break;
    case FB_CH6:
        fb_count_Y6 =0;
        break;
    case FB_CH7:
        fb_count_Y7 =0;
        break;
    case FB_CH8:
        fb_count_Y8 =0;
        break;
    case FB_CH9:
        fb_count_Y9 =0;
        break;
    case FB_CH10:
        fb_count_Y10 =0;
        break;
    default:
        break;
    }
}

/****************************************************************************************************************************************** 
* 函数名称: fb_cnt_get
* 功能说明: 
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
uint8_t fb_cnt_get(uint8_t fb_num)
{
    uint8_t fb_cnt = 0;
    switch(fb_num)
    {
    case FB_CH1:
        fb_cnt = fb_count_Y1;
        break;
    case FB_CH2:
        fb_cnt = fb_count_Y2;
        break;
    case FB_CH3:
        fb_cnt = fb_count_Y3;
        break;
    case FB_CH4:
        fb_cnt = fb_count_Y4;
        break;
    case FB_CH5:
        fb_cnt = fb_count_Y5;
        break;
    case FB_CH6:
        fb_cnt = fb_count_Y6;
        break;
    case FB_CH7:
        fb_cnt = fb_count_Y7;
        break;
    case FB_CH8:
        fb_cnt = fb_count_Y8;
        break;
    case FB_CH9:
        fb_cnt = fb_count_Y9;
        break;
    case FB_CH10:
        fb_cnt = fb_count_Y10;
        break;
    default:
        break;
    }
    return fb_cnt;
}




/****************************************************************************************************************************************** 
* 函数名称: get_current_channel
* 功能说明: 获取当前工作的通道
* 输    入: void
* 输    出: current_num 当前通道号
* 注意事项: 
******************************************************************************************************************************************/
uint8_t get_current_channel(void)
{
		return current_num;
}


/****************************************************************************************************************************************** 
* 函数名称: clear_current_channel
* 功能说明: 通道清零
* 输    入: void
* 输    出: void
* 注意事项: 
******************************************************************************************************************************************/
void clear_current_channel(void)
{
		 current_num = 	0;
}




/****************************************************************************************************************************************** 
* 函数名称: read_fb_channel_status
* 功能说明: 读取反馈引脚的状态
* 输    入: number 通道号
* 输    出: 状态
* 注意事项: 
******************************************************************************************************************************************/
uint8_t read_fb_channel_status(uint8_t number)
{
	
	return GPIO_GetBit(motorDrive_tab[number].fb_part,motorDrive_tab[number].fb_pin);
	
}

/****************************************************************************************************************************************** 
* 函数名称: electromagnet_action
* 功能说明: 吸合有反馈电磁锁 电磁锁开了之后最好马上断电
* 输    入: number 通道号
* 输    出: EE 通道号无效  1 动作成功  0 动作失败 
* 注意事项: number > CHANNEL_MAX && number <1
******************************************************************************************************************************************/
uint8_t fb_electromagnet_action(uint8_t number)
{
		uint8_t result = 0;
		uint32_t delay_tick= 0;
		uint8_t x =0;
    uint8_t y =0;
    uint8_t err=0;
	
#if ENABLE_WDT >0u
            WDT_Feed(WDT);
#endif
			
    err = current_channel_updata(number);
    if(err == 0xEE)return err;
	
    x =	get_x_num(number);
    y = get_y_num(number);
	
		tick_reset(); //清空计数器
		delay_tick = 3+get_tick(); //100ms*3 = 300ms
    if( number > CHANNEL_MAX && number <1 )// 无效通道号
    {
        result = 0xEE;
        return result;  //大于100则返回
    }
    else
    {
  			GPIO_SetBit(motorDrive_tab[x].part_x,motorDrive_tab[x].pin_x);
				GPIO_ClrBit(motorDrive_tab[y].part_y,motorDrive_tab[y].pin_y);
				while(1)
				{				
						if( read_fb_channel_status(number) == 0)//NPN B极引脚被拉高 芯片引脚被拉低 门被打开
						{
								result = FB_ELECTROMAGNET_OK;
								break;
						}
						else if(delay_tick <= get_tick())//超时退出 300ms
						{
								result = FB_ELECTROMAGNET_ERROR; //没打开 建议再检查一下
								break;
						}
				}
			GPIO_ClrBit(motorDrive_tab[x].part_x,motorDrive_tab[x].pin_x);
			GPIO_SetBit(motorDrive_tab[y].part_y,motorDrive_tab[y].pin_y);
			
    }
		//result = read_fb_channel_status(number);
		
		//vm_control.load_flag = result;
		
    return result;

}

/****************************************************************************************************************************************** 
* 函数名称: nfb_electromagnet_action
* 功能说明: 吸合无反馈电磁铁
* 输    入: 负载编号 
* 输    出: err 0XEE 通道号错误  0 正常
* 注意事项: number > CHANNEL_MAX && number <1
******************************************************************************************************************************************/
uint8_t nfb_electromagnet_action(uint8_t number)
{

		uint8_t result = 0;
		uint32_t delay_tick= 0;
		uint8_t x =0;
    uint8_t y =0;
    uint8_t err=0;
		
#if ENABLE_WDT >0u
            WDT_Feed(WDT);
#endif
			
    err = current_channel_updata(number);
    if(err == 0xEE)return err;
	
    x =	get_x_num(number);
    y = get_y_num(number);
	
		tick_reset(); //清空计数器
		delay_tick = 3+get_tick(); //100ms*3 = 300ms
		
    if( number > CHANNEL_MAX && number <1 )// 无效通道号
    {
        result = 0xEE;
        return result;  //大于100则返回
    }
    else
    {
			load_set_buys();
			GPIO_SetBit(motorDrive_tab[x].part_x,motorDrive_tab[x].pin_x);
			GPIO_ClrBit(motorDrive_tab[y].part_y,motorDrive_tab[y].pin_y);
			while(1)
			{				
					if(delay_tick <= get_tick())//超时退出 300ms
					{
							result = FB_ELECTROMAGNET_OK; // 不带反馈怎样都算打开啦
							break;
					}
			}
				
				
			GPIO_ClrBit(motorDrive_tab[x].part_x,motorDrive_tab[x].pin_x);
			GPIO_SetBit(motorDrive_tab[y].part_y,motorDrive_tab[y].pin_y);
			
			tick_reset(); //清空计数器
			delay_tick = 1+get_tick(); //100ms*3 = 300ms
			while(1)
			{				
					if(delay_tick <= get_tick())//超时退出 300ms
					{					
							break;
					}
			}	
			load_free_buys();
			
    }
		
    return err;


}


/******************************************************************************************************************************************
* 函数名称: motor_driver_io_init
* 功能说明: 初始化电机驱动引脚
* 输    入: none
* 输    出: none
* 注意事项: 内部调用
******************************************************************************************************************************************/
static void motor_driver_io_init(void)
{

    uint8_t i;

    //config x
    for(i=1; i<CHANNEL_X_MAX+1; i++)
    {
        GPIO_Init(motorDrive_tab[i].part_x,motorDrive_tab[i].pin_x,DIR_OUTPUR, PULL_UP_DISALBE,PULL_DOWN_DISALBE, PUSH_PULL);
        GPIO_ClrBit(motorDrive_tab[i].part_x,motorDrive_tab[i].pin_x);
    }

    //config y
    for(i=1; i<CHANNEL_Y_MAX+1; i++)
    {
        GPIO_Init(motorDrive_tab[i].part_y,motorDrive_tab[i].pin_y,DIR_OUTPUR, PULL_UP_DISALBE,PULL_DOWN_DISALBE, PUSH_PULL);
        GPIO_SetBit(motorDrive_tab[i].part_y,motorDrive_tab[i].pin_y);
    }

}




/******************************************************************************************************************************************
* 函数名称: motor_fb_io_init
* 功能说明: 初始化电机位置线引脚
* 输    入: none
* 输    出: none
* 注意事项: 内部调用
******************************************************************************************************************************************/
static void motor_fb_io_init(void)
{

    uint8_t i = 0;
    // feedback config
    for(i=0; i<CHANNEL_FB_MAX+1; i++)
    {
        GPIO_Init( motorDrive_tab[i].fb_part, motorDrive_tab[i].fb_pin, 0, 1, 0, 0);
    }

}

/******************************************************************************************************************************************
* 函数名称: motor_fb_exti_init
* 功能说明: 初始化电机位置线引脚中断
* 输    入: none
* 输    出: none
* 注意事项: 内部调用
******************************************************************************************************************************************/
static void motor_fb_exti_init(void)
{
    uint8_t i = 0;
    // feedback config
    for(i=1; i<CHANNEL_FB_MAX+1; i++)
    {
        EXTI_Init(motorDrive_tab[i].fb_part,motorDrive_tab[i].fb_pin,EXTI_BOTH_EDGE);// 下降沿
        //EXTI_Init(motorDrive_tab[i].fb_part,motorDrive_tab[i].fb_pin,EXTI_RISE_EDGE);	//上升沿
        EXTI_Close(motorDrive_tab[i].fb_part,motorDrive_tab[i].fb_pin);
    }
    IRQ_Connect(MOTOR_FB1_PERIPH_INTEERUPT, MOTOR_FB1_IRQn, MOTOR_FB_IRQ_PRIORITY);
    IRQ_Connect(MOTOR_FB2_PERIPH_INTEERUPT, MOTOR_FB2_IRQn, MOTOR_FB_IRQ_PRIORITY);
    IRQ_Connect(MOTOR_FB3_PERIPH_INTEERUPT, MOTOR_FB3_IRQn, MOTOR_FB_IRQ_PRIORITY);
    IRQ_Connect(MOTOR_FB4_PERIPH_INTEERUPT, MOTOR_FB4_IRQn, MOTOR_FB_IRQ_PRIORITY);
    IRQ_Connect(MOTOR_FB5_PERIPH_INTEERUPT, MOTOR_FB5_IRQn, MOTOR_FB_IRQ_PRIORITY);
    IRQ_Connect(MOTOR_FB6_PERIPH_INTEERUPT, MOTOR_FB6_IRQn, MOTOR_FB_IRQ_PRIORITY);
    IRQ_Connect(MOTOR_FB7_PERIPH_INTEERUPT, MOTOR_FB7_IRQn, MOTOR_FB_IRQ_PRIORITY);
    IRQ_Connect(MOTOR_FB8_PERIPH_INTEERUPT, MOTOR_FB8_IRQn, MOTOR_FB_IRQ_PRIORITY);
    IRQ_Connect(MOTOR_FB9_PERIPH_INTEERUPT, MOTOR_FB9_IRQn, MOTOR_FB_IRQ_PRIORITY);
    IRQ_Connect(MOTOR_FB10_PERIPH_INTEERUPT, MOTOR_FB10_IRQn, MOTOR_FB_IRQ_PRIORITY);
}

/******************************************************************************************************************************************
* 函数名称: crawler_exti_init
* 功能说明: 初始化履带型货道电机位置线引脚中断
* 输    入: 
uint8_t  y        有效值有 1-A
uint32_t mode	   有效值有EXTI_FALL_EDGE、EXTI_RISE_EDGE、EXTI_BOTH_EDGE、EXTI_LOW_LEVEL、EXTI_HIGH_LEVEL
* 输    出: none
* 注意事项: 内部调用
******************************************************************************************************************************************/
//static void crawler_exti_init(uint8_t y,uint32_t mode)
//{

//    EXTI_Init(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin,mode);// 下降沿       
//	
//		switch(y)
//		{
//			case 1: IRQ_Connect(MOTOR_FB1_PERIPH_INTEERUPT, MOTOR_FB1_IRQn, MOTOR_FB_IRQ_PRIORITY); 	break;
//			case 2: IRQ_Connect(MOTOR_FB2_PERIPH_INTEERUPT, MOTOR_FB2_IRQn, MOTOR_FB_IRQ_PRIORITY); 	break;
//			case 3:	IRQ_Connect(MOTOR_FB3_PERIPH_INTEERUPT, MOTOR_FB3_IRQn, MOTOR_FB_IRQ_PRIORITY);		break;
//			case 4: IRQ_Connect(MOTOR_FB4_PERIPH_INTEERUPT, MOTOR_FB4_IRQn, MOTOR_FB_IRQ_PRIORITY); 	break;
//			case 5: IRQ_Connect(MOTOR_FB5_PERIPH_INTEERUPT, MOTOR_FB5_IRQn, MOTOR_FB_IRQ_PRIORITY); 	break;
//			case 6: IRQ_Connect(MOTOR_FB6_PERIPH_INTEERUPT, MOTOR_FB6_IRQn, MOTOR_FB_IRQ_PRIORITY); 	break;
//			case 7: IRQ_Connect(MOTOR_FB7_PERIPH_INTEERUPT, MOTOR_FB7_IRQn, MOTOR_FB_IRQ_PRIORITY); 	break;
//			case 8: IRQ_Connect(MOTOR_FB8_PERIPH_INTEERUPT, MOTOR_FB8_IRQn, MOTOR_FB_IRQ_PRIORITY); 	break;
//			case 9: IRQ_Connect(MOTOR_FB9_PERIPH_INTEERUPT, MOTOR_FB9_IRQn, MOTOR_FB_IRQ_PRIORITY); 	break;
//			case 10:IRQ_Connect(MOTOR_FB10_PERIPH_INTEERUPT, MOTOR_FB10_IRQn, MOTOR_FB_IRQ_PRIORITY); break;
//			default:
//				break;
//		}
//		EXTI_Open(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin);


//}

/****************************************************************************************************************************************** 
* 函数名称: get_x_num
* 功能说明: 计算X的值 (行)
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
static uint8_t get_x_num(uint8_t number)
{
    uint8_t x =0;

    if(number%10)
    {
        x= number%10;
    }
    else if(number%10 == 0)
    {
        x= 10;
    }
    return x;
}

/****************************************************************************************************************************************** 
* 函数名称: get_y_num
* 功能说明: 计算Y的值 (列)
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
static uint8_t get_y_num(uint8_t number)
{
    uint8_t y =0;

    if(number < 0x64)
    {
        if(number%10 == 0)
        {
            y = number/10;
        }
        else
        {
            y = (number/10) +1;
        }
    }
    else if(number >= 0x64)
    {
        y = number/10;
    }
    return y;
}
/****************************************************************************************************************************************** 
* 函数名称: check_xy
* 功能说明: 检查 XY 对不对
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
static uint8_t check_xy(uint8_t x,uint8_t y)
{
    uint8_t  err =0;

    if( x>10 || y>10 || x==0 || y==0 )
    {
        err = 0xEE;
    }

    err = 0;

    return err;
}
/****************************************************************************************************************************************** 
* 函数名称: check_ir
* 功能说明: 检查红外有没有问题 有问题则放弃本次出货的操作
* 输    入: void
* 输    出: void
* 注意事项: 
******************************************************************************************************************************************/
static uint8_t check_ir(void)
{
		uint8_t err =0;
	
		IR_POWER_ON();
		delay_ntick(3);//100ms
	
    if(GPIO_GetBit(IR_PART,IR_PIN) == 0) //光栅被挡住
		{
            err = 0XEE;
            vm_control.ir_flag = IR_NONE;  //没有检测到光栅
            __current_result = RESULT_VM_FB_MOTOR_OUTPUT;
						IR_POWER_OFF();       
    }
    
    return err;

}
/****************************************************************************************************************************************** 
* 函数名称: check_motor
* 功能说明: 检测电机是否正常 不正常则放弃本次操作
* 输    入: 行 列 
* 输    出: 0 成功 EE 电机异常 
* 注意事项: 
******************************************************************************************************************************************/
static uint8_t check_motor(uint8_t x, uint8_t y)
{
    uint32_t delay_tick=0;
    uint8_t err =0;
    uint32_t delay_cnt = 0x1FFFFF;
		GPIO_SetBit(motorDrive_tab[x].part_x,motorDrive_tab[x].pin_x);
    GPIO_ClrBit(motorDrive_tab[y].part_y,motorDrive_tab[y].pin_y);
		
		if(vm_control.load_type == FB_CRAWLER_TYPE)
		{										
					delay_cnt=0x1FFFFF;
					while( --delay_cnt ) {__nop();}//消抖	这一步很重要不然电平反应不过来
					if(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 1)
					{				
							fb_cnt_set(2);
							motor_fb_exti_enable(y);		
#if DEBUG>0ul					
							SEGGER_RTT_printf(0, "set fb cnt is %d\r\n",fb_exti_cnt_get());					
#endif											
					}
					else if(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 0)
					{			
							fb_cnt_set(3);	
							motor_fb_exti_enable(y);	
#if DEBUG>0ul					
							SEGGER_RTT_printf(0, "set fb cnt is %d\r\n",fb_exti_cnt_get());					
#endif						
					}
		}	
		else 
		{
			tick_reset(); //清空计数器，重新装载
			delay_tick = get_tick()+5; //500ms
			while( --delay_cnt ) {__nop();}//消抖
			while(1)//等待电机跑出缺口位置
			{
					if(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 0)//等待跑出缺口位置
					{
							if(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 0)
							{
									motor_fb_exti_enable(y);
									break;
							}
					}
					else if(delay_tick <= get_tick()) //超时 电机被卡住或者没电机
					{
							GPIO_ClrBit(motorDrive_tab[x].part_x,motorDrive_tab[x].pin_x);
							GPIO_SetBit(motorDrive_tab[y].part_y,motorDrive_tab[y].pin_y);

							motor_fb_exti_disable(y);
							IR_EXTI_DISABLE();
							TIMR_Stop(Motor_TIMR);
							TIMR_Stop(IR_TIMR);
							err = 0xEE;

							vm_control.load_number = get_current_channel(); //获取当前通道
							vm_control.load_flag = MOTOR_ERR;						//没检测到电机,可能是线断了
							__current_result = RESULT_VM_FB_MOTOR_OUTPUT;		  		//输出

							break;
					}
			}
		}
	

    return err;
}

/****************************************************************************************************************************************** 
* 函数名称: current_channel_updata
* 功能说明: 
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
static uint8_t current_channel_updata(uint8_t number)
{
    uint8_t err=1;

    if( number > CHANNEL_MAX || number < 1 )// 无效通道号
    {
        err = 0xEE;
        return err;  //大于100则返回
    }
    else
    {
        current_num = number;
    }
    return err;
}


void fb_test(void)
{

	int i =1;
	for(i=1;i<=50;i++)
	{
		nfb_electromagnet_action(i);
		
	}
	
}

//void crawler_output(uint8_t number)
//{
//		uint8_t x=0,y=0;
//	
//		x =	get_x_num(number);
//    y = get_y_num(number);
//		
//		if(check_xy(x,y) == 0xEE)return;
//	
//	
//		GPIO_SetBit(motorDrive_tab[x].part_x,motorDrive_tab[x].pin_x);
//		GPIO_ClrBit(motorDrive_tab[y].part_y,motorDrive_tab[y].pin_y);
//		if(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 0)
//		{
//			SEGGER_RTT_printf(0,"fb is %d\r\n ",GPIO_GetBit(MOTOR_FB1_PIN_AND_PART));
//			while(1)
//			{
//				if(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 1)
//				{
//						//SEGGER_RTT_printf(0,"fb is %d\r\n ",GPIO_GetBit(MOTOR_FB1_PIN_AND_PART));
//						delay_ntick(1);
//						SEGGER_RTT_printf(0,"fb is %d\r\n ",GPIO_GetBit(MOTOR_FB1_PIN_AND_PART));
//						while(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 0);					
//						break;				
//				}			
//			}
//			SEGGER_RTT_printf(0,"fb is %d\r\n ",GPIO_GetBit(MOTOR_FB1_PIN_AND_PART));
//			if(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 1)
//			{
//				while(1)
//				{
//					if(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 0)
//					{
//							SEGGER_RTT_printf(0,"fb is %d\r\n ",GPIO_GetBit(MOTOR_FB1_PIN_AND_PART));
//							break;				
//					}
//						
//				}
//					
//			}				
//		}
//		 
//		else if(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 1)
//		{
//				SEGGER_RTT_printf(0,"fb is %d\r\n ",GPIO_GetBit(MOTOR_FB1_PIN_AND_PART));
//				while(1)
//				{
//					if(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 0)
//					{
//							//SEGGER_RTT_printf(0,"fb is %d\r\n ",GPIO_GetBit(MOTOR_FB1_PIN_AND_PART));
//							delay_ntick(1);
//							SEGGER_RTT_printf(0,"fb is %d\r\n ",GPIO_GetBit(MOTOR_FB1_PIN_AND_PART));
//							while(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 1);					
//							break;				
//					}			
//				}
//				SEGGER_RTT_printf(0,"fb is %d\r\n ",GPIO_GetBit(MOTOR_FB1_PIN_AND_PART));
//				if(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 0)
//				{
//					while(1)
//					{
//						if(GPIO_GetBit(motorDrive_tab[y].fb_part,motorDrive_tab[y].fb_pin) == 1)
//						{
//								SEGGER_RTT_printf(0,"fb is %d\r\n ",GPIO_GetBit(MOTOR_FB1_PIN_AND_PART));
//								break;				
//						}
//							
//					}
//						
//				}				
//		}

//	
//		GPIO_ClrBit(motorDrive_tab[x].part_x,motorDrive_tab[x].pin_x);
//		GPIO_SetBit(motorDrive_tab[y].part_y,motorDrive_tab[y].pin_y);
//	
//}

void fb_cnt_set(uint8_t fb_cnt)
{
	fb_exti_cnt = fb_cnt;
}

uint8_t fb_exti_cnt_get(void)
{
	return fb_exti_cnt;
}


