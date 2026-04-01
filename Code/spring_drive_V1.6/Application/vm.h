#ifndef _VM_H
#define _VM_H

#include "swm181.h"

#include "motor_driver.h"
#include "uart.h"
#include "rs485.h"
#include "led.h"
#include "string.h"
#include "timer.h"
#include "ir.h"
#include "vm.h"
#include "lock.h"
#include "address.h"
#include "ambient.h"
#include "tick.h"

#include <string.h>
#include "mymalloc.h"	

/****定义设备类型*****/

#define MOTOR_DEV    ((uint8_t)0x01)
#define CHARGER_DEV  ((uint8_t)0x02)
#define RS485_LOCK	 ((uint8_t)0X04)

#define BROADCAST_ADDRESS ((uint8_t)0xFF)
#define DEFAULT_ADDRESS   ((uint8_t)0x01)

#define DEVICE_TYPE  MOTOR_DEV


#define OPEN_THE_DOOR     0XFF
#define	CLOSE_THE_DOOR	  0X00


#define FRAME_HEAD 0XDDCCBBAAUL  //AABBDDCC
#define FRAME_END  0XFBFCFDFEUL	 //FEFCFDFB

#define CMD_OUTPUT ((uint8_t)0x01)
#define CMD_READ	 ((uint8_t)0x02)
#define CMD_LOCK   ((uint8_t)0X03)
#define CMD_REPLY	 ((uint8_t)0X04)
#define CMD_MODIFICATION ((uint8_t)0X05)

#define RESULT_VM_FB_MOTOR_OUTPUT 0xC1		//输出
#define RESULT_VM_STATES 0xC2							//环境量 
#define RESULT_VM_ADDR_MODIFICATION 0xC3	//环境量 
#define RESULT_VM_LOCK 0xC4								//锁
	

#define RESULT_VM_OUTPUT_NFB_ELECTROMAGNET 0xC5
#define RESULT_VM_OUTPUT_FB_ELECTROMAGNET 0xC6

#define IR_NONE    			0xEE
#define IR_First_Blood	0X01 	//掉落一次
#define IR_TIMER_OUT		0x00
//#define IR_REPEAT	 		0x22	//掉落多了

#define FB_MOTOR						0	//有反馈弹簧电机
#define FB_ELECTROMAGNET		1 //有反馈电磁铁
#define NFB_ELECTROMAGNET   2	//无反馈电磁铁
#define FB_CRAWLER_TYPE			3	//有反馈履带电机	


#define  FB_ELECTROMAGNET_OK  		0x01
#define  FB_ELECTROMAGNET_ERROR   0xEE
#define  FB_ELECTROMAGNET_TIMEOUT 0x00

extern volatile uint8_t __current_result; //当前回复上位机状态

#pragma pack(1)
typedef struct
{
    uint8_t load_number;   //负载编号	
    uint8_t load_flag;		 //下位机->上位机 出货状态标志 1出货成功 0出货失败
    uint8_t ir_flag;		   //红外状态					
		uint8_t load_type;		 //负载类型   0 有反馈弹簧电机 1 有反馈电磁铁 2 无反馈电磁铁  0x03 有反馈履带电机
		uint8_t timer_value;	 //定时器时间
} VM_Control; //读写
#pragma pack()

#pragma pack(1)
typedef struct
{
    uint8_t lock_states;	//锁
    short humidity;			 	//湿度
    short temperature;		//温度

} VM_States; //读写
#pragma pack()

#pragma pack(1)
typedef struct
{
    uint8_t states;
} VM_Lock;
#pragma pack()

#pragma pack(1)
typedef struct
{
    uint8_t new_address;

} VM_Address;
#pragma pack()


#pragma pack(1)
typedef struct
{
		uint8_t channle_num;
    uint8_t Status;

} VM_FB_CH_Status;
#pragma pack()

#pragma pack(1)
typedef struct VM_Data_Frame_T
{
    uint32_t frame_head;  		//帧头
    uint8_t  device_type;			
    uint8_t  device_address;
    uint8_t  cmd;		
    union
    {
				
        VM_Lock    lock;			 //锁
        VM_States  states;		 //售货机反馈
        VM_Control control;	 	 //控制数据
        VM_Address address;		 //用于修改板卡地址
				
    } data;									   //数据

    uint16_t crc16_code;	     //校验码  2byte
    uint32_t frame_end;	       // 帧尾	 1byte

} VM_Data_Frame;
#pragma pack()



#define VM_Frame_SIZE  sizeof(VM_Data_Frame)
#define VM_CTRL_SIZE   sizeof(VM_Control)
#define VM_STATES_SIZE sizeof(VM_States)

extern VM_Control 		vm_control;
extern VM_States  		vm_states;
extern VM_Data_Frame 	vm_data_frame;

uint8_t frame_check(VM_Data_Frame* vdf);
void frame_default(VM_Data_Frame* vdf);
void write_frame_checkcode(VM_Data_Frame* vdf);

void rs485_ch1_recv(void);
void rs485_ch1_reply(void);
uint16_t crc16encode(uint8_t *ptr, uint16_t len);
#endif

