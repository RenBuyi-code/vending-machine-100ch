#include "vm.h"
#include "SEGGER_RTT.h"
VM_Control vm_control;
VM_States  vm_states;
VM_Data_Frame vm_data_frame;
volatile uint8_t __current_result =0;


/******************************************************************************************************************************************
* 函数名称:  crc16encode
* 功能说明:  计算CRC16校验码
* 输    入: 	uint8_t *ptr：数据流首地址    uint16_t len：数据长度
* 输    出: 	uint16_t crc  无符号整型crc16校验码
* 注意事项:
******************************************************************************************************************************************/
uint16_t crc16encode(uint8_t *ptr, uint16_t len)
{
    unsigned char i;
    unsigned short crc = 0xFFFF;
    if (len == 0) {
        len = 1;
    }
    while (len--) {
        crc ^= *ptr;
        for (i = 0; i<8; i++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
        ptr++;
    }
    return crc;
}
/******************************************************************************************************************************************
* 函数名称: frame_check
* 功能说明: 校验帧是否有效
* 输    入: VM_Data_Frame* vdf  通信帧结构体指针
* 输    出: 正确帧返回1 不正确返回0
* 注意事项:
******************************************************************************************************************************************/
#define  TEST 0
uint8_t frame_check(VM_Data_Frame* vdf)
{
    uint8_t flag =	FALSE;
    uint16_t crc16_code = vdf->crc16_code;  //小端模式传输

    uint16_t data_size = (sizeof(vm_data_frame.data)+2);  //有效数据大小 掐头去尾去校验
    uint8_t chek_buff[sizeof(vm_data_frame.data)+2]= {0};	//有效数据包 掐头去尾去校验

#if TEST >0
    uint16_t test_code =0;
#endif

    mymemcpy(chek_buff,&vdf->device_address,data_size);

    if(vdf->frame_head == FRAME_HEAD && vdf->frame_end == FRAME_END)
    {
        if(crc16_code == crc16encode(chek_buff,data_size))
        {
            flag =	TRUE;
        }
    }
#if TEST >0
    test_code = crc16encode(chek_buff,data_size);
    rs485_channel1_send((uint8_t*)&test_code,2);
#endif

    return flag;
}
/******************************************************************************************************************************************
* 函数名称: write_frame_checkcode
* 功能说明: 往通信帧结构体里写入校验码
* 输    入: void
* 输    出: void1
* 注意事项:
******************************************************************************************************************************************/
void write_frame_checkcode(VM_Data_Frame* vdf)
{

    vdf->crc16_code = crc16encode((uint8_t*)&vdf->device_address,7);
}
/******************************************************************************************************************************************
* 函数名称: frame_default
* 功能说明: 将通信帧设置为默认值，数据帧清空
* 输    入: VM_Data_Frame* vdf
* 输    出: void
* 注意事项:
******************************************************************************************************************************************/
void frame_default(VM_Data_Frame* vdf)
{
    vdf->frame_head = FRAME_HEAD;
    vdf->device_type = DEVICE_TYPE;
    vdf->device_address = get_driver_address();
    vdf->cmd = CMD_REPLY;

    /*********清空数据**************/
    memset(&vdf->data.states,0,sizeof(vdf->data));
    /***********************/
    vdf->crc16_code = 0x0000;
    vdf->frame_end = FRAME_END;
}


/******************************************************************************************************************************************
* 函数名称: fb_motor_output
* 功能说明: 售货机出货
* 输    入: void
* 输    出: void
* 注意事项: rs485_ch1_recv（）中调用 其他勿调用
******************************************************************************************************************************************/
static void fb_motor_output(void)
{
#if ENABLE_WDT >0u
            WDT_Feed(WDT);
#endif
    mymemcpy(&vm_control, &vm_data_frame.data.control,sizeof(VM_Control));
#if ENABLE_WDT >0u	
		SEGGER_RTT_printf(0, "copy data end....\r\n");
#endif	
    fb_motor_on(vm_control.load_number);//出货
}
/******************************************************************************************************************************************
* 函数名称: vm_elock(void)
* 功能说明: 操作售货机机柜锁
* 输    入: void
* 输    出: void
* 注意事项: rs485_ch1_recv（）中调用 其他勿调用
******************************************************************************************************************************************/
static void vm_elock(void)
{
    if(vm_data_frame.data.lock.states == TRUE)	//开锁
    {
        open_the_door(); //在这里会阻塞
        __current_result = RESULT_VM_LOCK;
    }
    else if(vm_data_frame.data.lock.states == FALSE)	//关锁
    {
        LOCK_OFF();
        LED_CLOSE();
        __current_result = RESULT_VM_LOCK;
    }


}

/****************************************************************************************************************************************** 
* 函数名称: output 
* 功能说明: 驱动负载
* 输    入: 负载类型
* 输    出: void 	
* 注意事项: 内部调用
******************************************************************************************************************************************/
static void output(uint8_t loader_type)
{
		switch(loader_type)
		{
			case FB_MOTOR	:				 //三线电机 即有反馈电机   						
			{					
				fb_motor_output();
			}	
			break;			
			case FB_ELECTROMAGNET :	// 有反馈电磁铁	
		  {					
			
								mymemcpy(&vm_control, &vm_data_frame.data.control,sizeof(VM_Control));
#if DEBUG>0ul				
								SEGGER_RTT_printf(0,"fb_ele outputting....\r\n");
#endif			
								vm_control.load_flag = fb_electromagnet_action(vm_control.load_number);
								__current_result = RESULT_VM_OUTPUT_FB_ELECTROMAGNET;
			}
								
			break;	
			case NFB_ELECTROMAGNET:	// 无反馈电磁铁
			{
								mymemcpy(&vm_control, &vm_data_frame.data.control,sizeof(VM_Control));
#if DEBUG>0ul				
								SEGGER_RTT_printf(0,"nfb_ele outputting....\r\n");
#endif			
								vm_control.load_flag = nfb_electromagnet_action(vm_control.load_number);	
								__current_result = RESULT_VM_OUTPUT_NFB_ELECTROMAGNET;
			}
			break;
			case FB_CRAWLER_TYPE:
			{					
#if DEBUG>0ul				
								SEGGER_RTT_printf(0,"fb crawler-type outputting....\r\n");
#endif			
								 fb_motor_output();	
						
			}	
			default:
				break;
		}
		
}

/******************************************************************************************************************************************
* 函数名称: rs485_ch1_recv
* 功能说明: 处理RS485 channel 1 的数据  上位机->下位机 下位机->上位机
* 输    入: void
* 输    出: void
* 注意事项:
******************************************************************************************************************************************/
void rs485_ch1_recv(void)
{
		
    if( 
			(	TRUE == __RS485_Ch1_RX_Finish )    && 
			( LOAD_IS_FREE == load_status_get() )&&
			( IR_NOTWAIT == wait_ir_flag_get()  ) 
		  )  
    {
        uint8_t buff[18]= {0};//应该先全部置零
        uint8_t new_address = 0;
        uint8_t frame_check_flag =0;

        __RS485_Ch1_RX_Finish = FALSE;
        rs485_channel1_get_buffer(buff);
								
        mymemcpy(&vm_data_frame,buff,VM_Frame_SIZE);//拷贝数据

        frame_check_flag = frame_check(&vm_data_frame); //检查数据
			
        if(vm_data_frame.device_type != DEVICE_TYPE) //判断设备类型
				{
					SEGGER_RTT_printf(0, "device_Type err %x\r\n",vm_data_frame.device_type);
					return;
				} 
				
				/** 检查设备地址 **/
				if(vm_data_frame.device_address != get_driver_address() && 
					 vm_data_frame.device_address != BROADCAST_ADDRESS )
				{
#if DEBUG>0ul						
						SEGGER_RTT_printf(0, "device_address err %x\r\n",vm_data_frame.device_address);
#endif 					
						return;
				}
			
        /**此段代码用于在广播地址下修改设备地址**/
        if((vm_data_frame.device_address == BROADCAST_ADDRESS)  //判断地址是否为广播地址
           && (TRUE == frame_check_flag) && vm_data_frame.cmd == CMD_MODIFICATION){
						
            new_address = (uint8_t)(vm_data_frame.data.address.new_address&0xff);
            modification_address(new_address);
            __current_result = RESULT_VM_ADDR_MODIFICATION; //返回售货机当前的地址 上位机注意查看地址
            vm_data_frame.cmd = 0;
        }
        else if(TRUE == frame_check_flag) //校验数据
				{	
#if DEBUG>0ul						
						SEGGER_RTT_printf(0, "CMD is true\r\n");	
#endif					
            switch(vm_data_frame.cmd)	//传入命令
            {
            case CMD_OUTPUT: //操作售货机出货
            {
                vm_data_frame.cmd = 0;
								
								output(vm_data_frame.data.control.load_type);              
            }
            break;

            case CMD_READ:	//读售货机环境量 锁状态
            {
                vm_data_frame.cmd = 0;
                __current_result = RESULT_VM_STATES;
            }
            break;
            case CMD_LOCK: //操作锁
            {
                vm_data_frame.cmd = 0;
                vm_elock();
            }
            break;
            case CMD_MODIFICATION:  // 在非广播地址中 修改地址
            {
                new_address = (uint8_t)(vm_data_frame.data.address.new_address&0xFF);

                if( (new_address != BROADCAST_ADDRESS) && (new_address != get_driver_address()) )
                {
                    modification_address(new_address);
                }
                __current_result = RESULT_VM_ADDR_MODIFICATION;
                vm_data_frame.cmd = 0;
            }

            break;
            default :
                __RS485_Ch1_RX_Finish = FALSE;
                break;
            }
        }
        else //校验码错误
        {
#if DEBUG>0ul						
						SEGGER_RTT_printf(0, "CMD is err\r\n");
#endif					
            frame_default(&vm_data_frame);
            vm_data_frame.cmd = 0xEE;
            write_frame_checkcode(&vm_data_frame);
            rs485_channel1_send((uint8_t*)&vm_data_frame,VM_Frame_SIZE);

        }
        __RS485_Ch1_RX_Finish = FALSE;
    }
    else if (TRUE == __RS485_Ch1_RX_Finish && (load_status_get() == LOAD_IS_BUYS) )
    {

      rs485_channel1_clear_buffer();
      __RS485_Ch1_RX_Finish = FALSE;
#if DEBUG>0ul				
			SEGGER_RTT_printf(0, "TRUE == __RS485_Ch1_RX_Finish && (load_status_get() == LOAD_IS_BUYS)\r\n");
#endif			
    }
    else if(  (TRUE == __RS485_Ch1_RX_Finish)  && (IR_WAIT == wait_ir_flag_get()) )
    {       
          __RS485_Ch1_RX_Finish = FALSE;
					rs485_channel1_clear_buffer();
#if DEBUG>0ul				
					SEGGER_RTT_printf(0, "(TRUE == __RS485_Ch1_RX_Finish)  && (IR_WAIT == wait_ir_flag_get())\r\n");
#endif			
    }
    else if( TRUE == __RS485_Ch1_RX_Finish)
    {
        //__RS485_Ch1_RX_Finish = FALSE;
        //rs485_channel1_clear_buffer(); 
				//SEGGER_RTT_printf(0, "Clear buffer \r\n");	
#if DEBUG>0ul			
				SEGGER_RTT_printf(0, "TRUE == __RS485_Ch1_RX_Finish\r\n");
#endif 			
    }
    else
    {
       // __RS485_Ch1_RX_Finish = FALSE;
    }

}


/******************************************************************************************************************************************
* 函数名称:  rs485_ch1_reply
* 功能说明:  回复上位机
* 输    入:  void
* 输    出:  void
* 注意事项:
******************************************************************************************************************************************/

void rs485_ch1_reply(void)
{
		int i =0;
    switch(__current_result)
    {
			
    case RESULT_VM_FB_MOTOR_OUTPUT:	//有反馈电机（三线电机） 动作回复
    {
        uint32_t delay_tick=0;
        tick_reset(); //清空计数器，重新装载
        delay_tick = get_tick()+20; //2S
				
       
#if ENABLE_WDT >0u
            WDT_Feed(WDT);
#endif
      
				if(vm_control.load_type == FB_MOTOR)
				{	
						while(1) //等待电机转完成
						{
								if( load_status_get() == LOAD_IS_FREE)//不忙了
								{
										break;
								}
								else if(delay_tick <= get_tick())//超时
								{		
										motor_off( get_current_channel()); //保险起见还是要关闭一下电机
										load_free_buys(); //释放电机忙标志	
										
										break;

								}
						}
				}
				else
				{
					motor_off( get_current_channel()); //保险起见还是要关闭一下电机
					load_free_buys(); //释放电机忙标志	
				}

				
				IR_POWER_OFF();
				EXTI_Close(IR_PART,IR_PIN);		
				TIMR_Stop(Motor_TIMR); //停止电机超时检测
				
#if DEBUG > 0ul				
				SEGGER_RTT_printf(0, "Stop Motor TIMER....\r\n");	
#endif					
				
				rs485_channel1_clear_buffer();	//清除缓存
				
#if DEBUG>0ul				
				SEGGER_RTT_printf(0, "rs485_channel1_clear_buffer \r\n");
#endif				
        

				if(vm_control.load_type == FB_MOTOR)
				{
					vm_data_frame.data.control.load_type = FB_MOTOR;				
				}
				else if(vm_control.load_type == FB_CRAWLER_TYPE)
				{
					vm_data_frame.data.control.load_type = FB_CRAWLER_TYPE;
				}
		
				frame_default(&vm_data_frame); //初始化一下结构体，以防被改过			
        vm_control.load_number = get_current_channel();
        memcpy(&vm_data_frame.data.control,&vm_control,sizeof(VM_CTRL_SIZE));//拷贝数据
#if DEBUG>0ul				
				SEGGER_RTT_printf(0, "copy replay struct\r\n");
#endif					
        vm_data_frame.cmd = CMD_OUTPUT;						
				fb_cnt_clear_all();		//电机计数清零
        ir_cnt_clear();				//红外计数清零
				wait_ir_flag_clear();	//光栅忙标志清除
				
        write_frame_checkcode(&vm_data_frame);//计算校验码
				
				
        rs485_channel1_send((uint8_t*)&vm_data_frame,sizeof(VM_Data_Frame));//发送数据

#if DEBUG>0ul
				SEGGER_RTT_printf(0, "replay motor output :");
				for(i=0;i<VM_Frame_SIZE;i++)
				{					
					SEGGER_RTT_printf(0, "%x ",((uint8_t*)&vm_data_frame)[i]);
				}
		  	SEGGER_RTT_printf(0, "\r\n ");
					
       
#endif 
				 clear_current_channel();
				 __current_result = 0;
    }
    break;

    case RESULT_VM_STATES:	//环境量
    {

        frame_default(&vm_data_frame); //初始化一下结构体，以防被改过

        vm_states.humidity = modbus_rtu.humidity;
        vm_states.temperature = modbus_rtu.temperature;
        vm_states.lock_states = GET_LOCK_SIG(); //0x01 门没关 0X00门关了;

        mymemcpy(&vm_data_frame.data.states,&vm_states,VM_STATES_SIZE);	//拷贝数据
        vm_data_frame.cmd = CMD_READ;
        write_frame_checkcode(&vm_data_frame);	//写入校验码
        rs485_channel1_send((uint8_t*)&vm_data_frame,VM_Frame_SIZE);
#if DEBUG>0ul			
				SEGGER_RTT_printf(0, "replay states:");
				for(i=0;i<VM_Frame_SIZE;i++)
				{					
					SEGGER_RTT_printf(0, "%x ",((uint8_t*)&vm_data_frame)[i]);
				}
		  	SEGGER_RTT_printf(0, "\r\n ");
#endif 			
			 __current_result = 0;	
    }
    break;
    
		case RESULT_VM_LOCK:
		{
        frame_default(&vm_data_frame); //初始化一下结构体，以防被改过
        vm_states.lock_states = GET_LOCK_SIG(); //0xFF 门没关 0XFE门关了;
        vm_data_frame.cmd = CMD_LOCK;
        write_frame_checkcode(&vm_data_frame);	//写入校验码
        rs485_channel1_send((uint8_t*)&vm_data_frame,VM_Frame_SIZE);
#if DEBUG>0ul			
				SEGGER_RTT_printf(0, "replay lock:");
				for(i=0;i<VM_Frame_SIZE;i++)
				{					
					SEGGER_RTT_printf(0, "%x ",((uint8_t*)&vm_data_frame)[i]);
				}
		  	SEGGER_RTT_printf(0, "\r\n ");
#endif 		
				 __current_result = 0;
			}	
    break;
    case RESULT_VM_ADDR_MODIFICATION:
    {
        frame_default(&vm_data_frame); //初始化一下结构体，以防被改过
        vm_data_frame.cmd = CMD_MODIFICATION;
        write_frame_checkcode(&vm_data_frame);	//写入校验码
			
        rs485_channel1_send((uint8_t*)&vm_data_frame,VM_Frame_SIZE);
	#if DEBUG>0ul					
			 SEGGER_RTT_printf(0, " replay addr:");
				for(i=0;i<VM_Frame_SIZE;i++)
				{					
					SEGGER_RTT_printf(0, "%x ",((uint8_t*)&vm_data_frame)[i]);
				}
		  	SEGGER_RTT_printf(0, "\r\n ");
#endif
				__current_result = 0;
		}
    break;

		case RESULT_VM_OUTPUT_NFB_ELECTROMAGNET : //无反馈电磁铁 动作后回复
		{
				frame_default(&vm_data_frame); //初始化一下结构体，以防被改过

        vm_control.load_number = get_current_channel();
        mymemcpy(&vm_data_frame.data.control,&vm_control,sizeof(VM_CTRL_SIZE));//拷贝数据
        vm_data_frame.cmd = CMD_OUTPUT;
				
				vm_data_frame.data.control.load_type = NFB_ELECTROMAGNET;
				
        write_frame_checkcode(&vm_data_frame);//计算校验码
        rs485_channel1_send((uint8_t*)&vm_data_frame,sizeof(VM_Data_Frame));//发送数据

#if DEBUG>0ul
				SEGGER_RTT_printf(0, " replay nfb_ele:");
				for(i=0;i<VM_Frame_SIZE;i++)
				{					
					SEGGER_RTT_printf(0, "%x ",((uint8_t*)&vm_data_frame)[i]);
				}
		  	SEGGER_RTT_printf(0, "\r\n ");
#endif 			
			
        clear_current_channel();
				__current_result = 0;
		}			
		break;
		
		case RESULT_VM_OUTPUT_FB_ELECTROMAGNET : //有反馈电磁铁 动作后回复
		{
				frame_default(&vm_data_frame); //初始化一下结构体，以防被改过

        vm_control.load_number = get_current_channel();
        mymemcpy(&vm_data_frame.data.control,&vm_control,sizeof(VM_CTRL_SIZE));//拷贝数据
        vm_data_frame.cmd = CMD_OUTPUT;
				
				vm_data_frame.data.control.load_type = FB_ELECTROMAGNET;
				
        write_frame_checkcode(&vm_data_frame);//计算校验码
        rs485_channel1_send((uint8_t*)&vm_data_frame,sizeof(VM_Data_Frame));//发送数据
#if DEBUG>0ul
				SEGGER_RTT_printf(0, " replay fb_ele:");
				for(i=0;i<VM_Frame_SIZE;i++)
				{					
					SEGGER_RTT_printf(0, "%x ",((uint8_t*)&vm_data_frame)[i]);
				}
		  	SEGGER_RTT_printf(0, "\r\n ");
#endif			
        clear_current_channel();
				__current_result = 0;
				
			
		}			
		break;
		
    default :
				
        break;
    }

   

}
