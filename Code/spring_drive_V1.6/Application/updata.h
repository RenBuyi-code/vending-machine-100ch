/******************************************************************************
* @file updata.h
* @author Zohar_Lee
* @version V1.00
* @date 2018.06.01
* @brief
******************************************************************************/
#ifndef __UPDATA_H__
#define __UPDATA_H__
#include "SWM181.h"

#define ADD_ASSIST_OF_UPDATA_CODE 0X19000 //100k Flash地址 更新代码辅助区存储地址
#define ADD_VERSION_OLD 0X19000           //100k Flash地址 旧版本号存储地址
#define ADD_VERSION_NEW 0X19004           //100k+4 Flash地址 新版本号存储地址
#define ADD_LENGTH_OF_NEW_CODE 0X19008    //100k+8 Flash地址 新代码长度存储地址
#define ADD_CRC_OF_NEW_CODE 0X1900C       //100k+12 Flash地址 新代码的CRC校验和的存储地址

#define ADD_CODE_NEW 0xA000 //新代码存放地址，40K，这就要求代码数量不能超过40K
//flash相关
#define SIZE_OF_SECTOR_4096_8BIT 4096 //扇区大小，单位：1 byte
#define SIZE_OF_PAGE_256_8BIT 256     //页大小，单位：1 byte
#define SIZE_OF_PAGE_64_32BIT 64      //页大小，单位：4 byte

typedef struct
{
    uint32_t version_of_old;
    uint32_t version_of_new;
    uint32_t length_of_new_code;
    uint32_t crc_of_new_code;
} UpdataCode_Struct;

void updata(void);
void PrintfVersionOfCode(void);

extern uint8_t upgrade;

#endif

/******************************* end of file *********************************/
