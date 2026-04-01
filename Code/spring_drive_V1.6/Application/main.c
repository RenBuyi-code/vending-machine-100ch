#include "driver_config.h"

#include "SEGGER_RTT.h"

static VM_Data_Frame vm_data_hello;

static void board_init(void)
{

    SystemInit();            //系统时钟初始化
    drive_address_config();  //初   始化设备地址
    led_init();              //初始化LED
    ir_check_init();         //光栅检测
    motor_init();            //电机引脚初始化
    rs485_channel1_config(); //rs485 通道1初始化
    rs485_channel2_config(); //rs485 通道2初始化
    lock_io_Init();          //锁控制引脚初始化
    system_tick_init();      //SysTick	初始化
    SEGGER_RTT_Init();
    __is_ask = TRUE; //开机先问一下
    ask_ambient();   //环境量

    frame_default(&vm_data_hello);
    vm_data_hello.cmd = 0XCD;

    write_frame_checkcode(&vm_data_hello);                         //写入校验码
    rs485_channel1_send((uint8_t *)&vm_data_hello, VM_Frame_SIZE); //向上位机问好
    SEGGER_RTT_printf(0, "Hi!\r\n");

#if ENABLE_WDT > 0u
    WDT_Init(WDT, SystemCoreClock * 6, WDT_MODE_RESET); //每5秒需要喂狗一次
    WDT_Start(WDT);
#endif
}

int main(void)
{
    uint16_t cnt = 0;
    board_init();

    while (TRUE == TRUE)
    {
        switch (cnt)
        {
        case 1:
        {
#if ENABLE_WDT > 0u
            WDT_Feed(WDT);
#endif
        }
        break;

        case 2:
        {
            rs485_ch1_recv();
#if ENABLE_WDT > 0u
            WDT_Feed(WDT);
#endif
        }
        break;

        case 3:
        {
            rs485_ch1_reply();
#if ENABLE_WDT > 0u
            WDT_Feed(WDT);
#endif
        }
        break;

        case 4:
        {
            ask_ambient(); //询问环境传感器
#if ENABLE_WDT > 0u
            WDT_Feed(WDT);
#endif
        }
        break;

        case 5:
        {
            rs485_ch2_recv();
#if ENABLE_WDT > 0u
            WDT_Feed(WDT);
#endif
        }
        break;
        case 6:
        {
            if (led_tick >= 7) //700MS 闪灯
            {
                LED_INV();
                led_tick = 0;
            }
        }

        default:
            cnt = 0;
            break;
        }
        cnt++;
    }
}
