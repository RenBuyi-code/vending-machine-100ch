#ifndef __XMODEM_H__
#define __XMODEM_H__

#define DEF_SOH ((uint8_t)0x01)
#define DEF_EOT ((uint8_t)0x04)
#define DEF_ACK ((uint8_t)0x06)
#define DEF_NAK ((uint8_t)0x15)
#define DEF_CAN ((uint8_t)0x18)
#define DEF_CTRLZ ((uint8_t)0x1A)

#define BINDATABUFLEN (128)

#define END_XMODEM 1000 //END_XMODEM不是128的倍数，则只有在收到结束帧的时候才会赋值

typedef enum
{
    DEF_NO_Reflect = 0,
    DEF_Packet_OK
} XmodemStatus_TypeDef;

typedef struct
{
    XmodemStatus_TypeDef XmodemStatus;
    uint8_t XmodemPacketNum;
    uint8_t LastXmodemPacketNum;
    uint8_t XmodemCheckSum;
} XmodemData_TypeDef;

extern XmodemData_TypeDef XmodemData;

uint32_t Xmodem(uint32_t *Number, uint8_t BinDataBuf[]);
void XmodemDataInit(void);
uint32_t GetNewCode(void);

#endif //__XMODEM_H__
