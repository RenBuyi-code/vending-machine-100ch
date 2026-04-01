/******************************************************************************
* @file xmodem.c
* @author Zohar_Lee,Lik
* @version V1.00
* @date 2018.06.01
* @brief
******************************************************************************/
#include "string.h"

#include "SWM181.h"

#include "uart.h"
#include "xmodem.h"
#include "updata.h"
#include "driver_config.h"
XmodemData_TypeDef XmodemData;
uint8_t upgrade = 0;
// uint32_t LengthOfNewCode = 0;

void XmodemDataInit(void)
{
    XmodemData.XmodemStatus = DEF_NO_Reflect;
    XmodemData.XmodemCheckSum = 0;
    XmodemData.LastXmodemPacketNum = 0;
    XmodemData.XmodemPacketNum = 0;
}
uint8_t CheckSum(uint8_t *Buf, uint8_t Len)
{
    uint8_t Sum = 0;
    for (uint32_t i = 0; i < Len; i++)
    {
        Sum += Buf[i];
    }
    return Sum;
}

/*TODO: 将此函数就改为单次执行，返回接收的数据包编号。用来处理数据存储的问题*/
/******************************************************************************
* 功能说明: 解析xmodem协议
* 输入: 无
* 输出: Number：指向数据包的序号的指针,如果指向1000；表示接收到结束帧。
        BinDataBuf[]:存放接收到的bin文件
* 返回：0：接收失败；1：接收成功
* 注意：无
******************************************************************************/
uint32_t Xmodem(uint32_t *Number, uint8_t BinDataBuf[])
{

    uint8_t flag = 0;

    if (1 == __RS485_Ch1_RX_Finish &&  upgrade ==1 )
    {
        __RS485_Ch1_RX_Finish = 0;

        switch (RS485_Ch1_Buffer[0]) //以首个字节 区分 数据包 和 结束标志
        {
        case DEF_SOH:
            if (RS485_Ch1_Buffer[1] == (0xFF - RS485_Ch1_Buffer[2]))
            {
                XmodemData.XmodemPacketNum = RS485_Ch1_Buffer[1];
                memcpy(BinDataBuf, (RS485_Ch1_Buffer + 3), 128);
                XmodemData.XmodemCheckSum = RS485_Ch1_Buffer[131];

                switch (XmodemData.XmodemStatus)
                {
                case DEF_NO_Reflect: //还没有开始发送
                    if ((XmodemData.XmodemPacketNum == 1) && (CheckSum(BinDataBuf, BINDATABUFLEN) == XmodemData.XmodemCheckSum))
                    {
                        XmodemData.XmodemStatus = DEF_Packet_OK;

                        //TODO: save BinDataBuf
                        // ramsource = (uint32_t)&PacketData[PACKET_DATA_INDEX];
                        *Number = 1; //输出数据包序号
                        XmodemData.LastXmodemPacketNum = XmodemData.XmodemPacketNum;
                        //CRT_WriteByte(UART0, DEF_ACK);
                        rs485_channel1_send((uint8_t*)DEF_ACK,1);
                        flag =  1;
                    }
                    break;
                case DEF_Packet_OK: //已经开始发送
                    /*根据xmodem协议，信息包序号， 从 01 开始以发送一包将加 1，加到 FF hex 将循环。
                      此处如不与0xFF相与，则编译器可能会认为if条件不成立*/
                    if (XmodemData.XmodemPacketNum == ((XmodemData.LastXmodemPacketNum + 1) & 0xff))
                    {
                        if (CheckSum(BinDataBuf, BINDATABUFLEN) == XmodemData.XmodemCheckSum)
                        {
                            //TODO: save BinDataBuf
                            *Number = XmodemData.XmodemPacketNum;
                            XmodemData.LastXmodemPacketNum = XmodemData.XmodemPacketNum;
                            // CRT_WriteByte(UART0, DEF_ACK); //send response data to CRT
                            rs485_channel1_send((uint8_t*)DEF_ACK,1);
                            flag = 1;
                        }
                        else
                        {
                            //Line hit during transmission
                            //CRT_WriteByte(UART0, DEF_NAK);
                            rs485_channel1_send((uint8_t*)DEF_NAK,1);
                        }
                    }
                    break;
                default:
                    break;
                }
            }
            break;
        case DEF_EOT:
            //CRT_WriteByte(UART0, DEF_ACK);
            rs485_channel1_send((uint8_t*)DEF_ACK,1);
            XmodemDataInit();
            memset(RS485_Ch1_Buffer, 0, RS485_CH1_RX_LEN);
            *Number = END_XMODEM; //END_XMODEM不是128的倍数，则只有在收到结束帧的时候才会赋值
            flag = 1;
            break;
        default:
            break;
        }
    }
    return flag;
}

/******************************* end of file *********************************/
