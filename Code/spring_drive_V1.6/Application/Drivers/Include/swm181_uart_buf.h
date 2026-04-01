 /**
 *********************************************************************************************************
 * @copy 
 *                                 (c) Copyright 2010-2016 
 *                                   All Rights Reserved
 *
 *                                 INTERNAL FILE,DON'T PUBLIC.
 * 
 * <h2><center>&copy; COPYRIGHT hnclcj@hotmail.com </center></h2>
 *********************************************************************************************************
 * @file      swm181_uart_buf.h
 * @history
 * @version   V1.0.0
 * @date      2018.11.08 把4个中断 统一用一个服务程序实现
 ********************************************************************************************************* 
 */ 
 
 
#ifndef _UART_H_
#define _UART_H_



#include <cpu.h>




#ifdef  UART_GLOBALS
#define UART_EXT
#else
#define UART_EXT extern
#endif





/*********************************************************************************************************
*                                              全 局 常 量
*********************************************************************************************************/
#ifndef UART_MODULE_EN
#define UART_MODULE_EN              (1)           // 串口模块总使能                                 0禁止
#endif

#if UART_MODULE_EN > 0                            // --------------- 功能模块使能控制 --------------------
#define UART0_UNIT_EN               (1)           // 串口0模块使能                                  0禁止
#define UART1_UNIT_EN               (1)           // 串口1模块使能                                  0禁止
#define UART2_UNIT_EN               (1)           // 串口2模块使能                                  0禁止
#define UART3_UNIT_EN               (1)           // 串口3模块使能                                  0禁止
                                                  //------------------------------------------------------
#define UART_MAX_NUM                (4)           // 最大串口数目前只能定义为3                   不能修改 
#endif
//**************************************---- UART0 ----***************************************************
#if UART0_UNIT_EN > 0
#define UART0_CLOSE_EN              (1u)            // 串口关闭功能使能                              0禁止
#define UART0_RXD_PORT              PORTA
#define UART0_RXD_IO                (0u)            // 配置的IO顺序号
#define UART0_TXD_PORT              PORTA
#define UART0_TXD_IO                (1u)            // 配置的IO顺序号
#define UART0_RXD_EN                (1u)            // 串口接收部分使能                              0禁止
#define UART0_TXD_EN                (1u)            // 串口发送部分使能                              0禁止
#define UART0_RXD_BUF_SIZE          (64)            // 接收缓存大小配置                  (<=1024)
#define UART0_TXD_BUF_SIZE          (64)            // 发送缓存大小配置                  (<=1024)
#define IRQn_UART0                  (IRQ_IRQ00)     // 中断号
#define UART0_Handler               IRQ0_Handler    // 中断句柄
#endif
//**************************************---- UART1 ----***************************************************
#if UART1_UNIT_EN > 0
#define UART1_CLOSE_EN              (1u)            // 串口关闭功能使能                              0禁止
#define UART1_RXD_PORT              PORTC
#define UART1_RXD_IO                (6u)            // 配置的IO顺序号
#define UART1_TXD_PORT              PORTC
#define UART1_TXD_IO                (7u)            // 配置的IO顺序号
#define UART1_RXD_EN                (1u)            // 串口接收部分使能                              0禁止
#define UART1_TXD_EN                (1u)            // 串口发送部分使能                              0禁止
#define UART1_RXD_BUF_SIZE          (64)            // 接收缓存大小配置                  (<=1024)
#define UART1_TXD_BUF_SIZE          (64)            // 发送缓存大小配置                  (<=1024)
#define IRQn_UART1                  (IRQ_IRQ01)     // 中断号
#define UART1_Handler               IRQ1_Handler    // 中断句柄
#endif
//**************************************---- UART2 ----***************************************************
#if UART2_UNIT_EN > 0
#define UART2_CLOSE_EN              (1u)            // 串口关闭功能使能                              0禁止
#define UART2_RXD_PORT              PORTA
#define UART2_RXD_IO                (6u)            // 配置的IO顺序号
#define UART2_TXD_PORT              PORTA
#define UART2_TXD_IO                (7u)            // 配置的IO顺序号
#define UART2_RXD_EN                (1u)            // 串口接收部分使能                              0禁止
#define UART2_TXD_EN                (1u)            // 串口发送部分使能                              0禁止
#define UART2_RXD_BUF_SIZE          (64u)           // 接收缓存大小配置                  (<=1024)
#define UART2_TXD_BUF_SIZE          (64u)           // 发送缓存大小配置                  (<=1024)
#define IRQn_UART2                  (IRQ_IRQ02)     // 中断号
#define UART2_Handler               IRQ2_Handler    // 中断句柄
#endif
//**************************************---- UART3 ----***************************************************
#if UART3_UNIT_EN > 0
#define UART3_CLOSE_EN              (1u)            // 串口功能使能                                  0禁止
#define UART3_RXD_PORT              PORTA
#define UART3_RXD_IO                (8u)            // 配置的IO顺序号
#define UART3_TXD_PORT              PORTA
#define UART3_TXD_IO                (9u)            // 配置的IO顺序号
#define UART3_RXD_EN                (1u)            // 串口接收部分使能                              0禁止
#define UART3_TXD_EN                (1u)            // 串口发送部分使能                              0禁止
#define UART3_RXD_BUF_SIZE          (64u)           // 接收缓存大小配置                  (<=1024)
#define UART3_TXD_BUF_SIZE          (64u)           // 发送缓存大小配置                  (<=1024)
#define IRQn_UART3                  (IRQ_IRQ03)     // 中断号
#define UART3_Handler               IRQ3_Handler    // 中断句柄
#endif
//********************************************************************************************************
#if UART_MODULE_EN > 0                            // ------------ 串口的各种返回信息 不能更改------------
#define UART_NONE                   (0)           // 无效验
#define UART_ODD                    (1)           // 奇效验
#define UART_EVEN                   (2)           // 偶效验
#define UART_ONE                    (3)           // 强制为1
#define UART_ZERO                   (4)           // 强制为0
                                                  // -----------------------------------------------------
                                                  // -- 当串口的接收需要任务协助处理时下面两个定义有效 --
#define UART_XON                    (17)          // 流控制字符 继续发送  30% (自定义流控)
#define UART_XOFF                   (19)          // 流控制字符 停止发送  70%
                                                  // -----------------------------------------------------
                                                  // 两个处理CLK模块临界数据的两个宏 这里使用OS的临界宏
#define UART_ENTER_CRITICAL()     __disable_irq() // 进入临界宏
#define UART_EXIT_CRITICAL()      __enable_irq()  // 退出临界宏
                                                  // -------------- 串口模块返回信息 --------------------
#define UART_ERR_NONE               (0)           // 串口配置没有错误
#define UART_ERR_ERR                (1)           // 串口配置有错误
#define UART_ERR_PORT               (2)           // 串口错误的端口号
#define UART_ERR_USE                (3)           // 串口已经配置 未关闭重新配置是不允许的
#define UART_ERR_NO_USE             (4)           // 串口没有使用 即可能未被配置
#define UART_ERR_RXD_EMPTY          (5)           // 接收缓存没有数据 为空
#define UART_ERR_RXD_NO_ERR         (6)           // 接收数据正确
#define UART_ERR_TXD_FALSE          (7)           // 发送失败
#define UART_ERR_TXD_NO_ERR         (8)           // 发送正确

#endif //UART_MODULE_EN
/*********************************************************************************************************
*                                           全局变量和数据类型
*********************************************************************************************************/
#if UART_MODULE_EN  > 0

typedef u32_t   UART_TYPE;                          //

typedef union  _uart_buf_ctrl_                      // 
{
    u32_t               value;                      // 控制值
    
    
    struct _uart_buf_ctrl_bit_                      // 控制相关
    {
        u32_t           bEn         : 1;            // 使能标志
        u32_t           TxEn        : 1;            // 直接发送标志
        
        u32_t           Cnt         : 10;           // 计数器
        u32_t           In          : 10;           // 缓存位置索引
        u32_t           Out         : 10;           // 缓存位置索引
    }bit;
}stUART_CTRL;

typedef struct _uart_buf_def_                       // 缓存结构
{
    stUART_CTRL         stCtrl;                     // 控制相关     
    
    u8_t               *pBuf;                       // 缓存
    UART_TYPE           BufSize;                    // 缓存大小
}stUART_BUF_DEF;

typedef struct _uart_buf_                           // -UART FIFO缓存结构
{
    stUART_BUF_DEF      Rx;                         // 接收
    stUART_BUF_DEF      Tx;                         // 发送
}stUART_BUF;


UART_EXT stUART_BUF UartBuf[UART_MAX_NUM];          // 缓存定义


                                                    //----------------------------------------------------
#if UART0_UNIT_EN > 0
#if UART0_RXD_EN > 0
UART_EXT u8_t Uart0RxBuf[UART0_RXD_BUF_SIZE+1];
#endif
#if UART0_TXD_EN > 0
UART_EXT u8_t Uart0TxBuf[UART0_TXD_BUF_SIZE+1];
#endif
#endif

#if UART1_UNIT_EN > 0
#if UART1_RXD_EN > 0
UART_EXT u8_t Uart1RxBuf[UART1_RXD_BUF_SIZE+1];
#endif
#if UART1_TXD_EN > 0
UART_EXT u8_t Uart1TxBuf[UART1_TXD_BUF_SIZE+1];
#endif
#endif

#if UART2_UNIT_EN > 0
#if UART2_RXD_EN > 0
UART_EXT u8_t Uart2RxBuf[UART2_RXD_BUF_SIZE+1];
#endif
#if UART2_TXD_EN > 0
UART_EXT u8_t Uart2TxBuf[UART2_TXD_BUF_SIZE+1];
#endif
#endif

#if UART3_UNIT_EN > 0
#if UART3_RXD_EN > 0
UART_EXT u8_t Uart3RxBuf[UART3_RXD_BUF_SIZE+1];
#endif
#if UART3_TXD_EN > 0
UART_EXT u8_t Uart3TxBuf[UART3_TXD_BUF_SIZE+1];
#endif
#endif

#endif // UART_MODULE_EN
/*********************************************************************************************************
*                                              全 局 函 数
*********************************************************************************************************/
#if UART_MODULE_EN > 0

void       UartInit           (void);

u8_t       UartCfgPort        (u8_t port, u32_t baud, u8_t parity, u8_t bits, u8_t stops);
void       UartClose          (u8_t port);
u8_t       UartCfgMode        (u8_t port, u8_t parity, u8_t bits, u8_t stops);

u8_t       UartRxGetChar      (u8_t port, u8_t *err);
u8_t       UartRxGetNChar     (u8_t port, u8_t *pRBuf, UART_TYPE len);
UART_TYPE  UartRxGetNMsgs     (u8_t port);
void       UartRxBufFlush     (u8_t port);

u8_t       UartTxPutChar      (u8_t port, u8_t Data);
UART_TYPE  UartTxPutNChar     (u8_t port, u8_t *pWBuf, UART_TYPE len);
UART_TYPE  UartTxGetNMsgs     (u8_t port);
UART_TYPE  UartTxGetEmptyMsgs (u8_t port);
void       UartTxBufFlush     (u8_t port);


#endif // UART_MODULE_EN
#endif // _UART_H_
/*********************************************************************************************************
*                                              End Of File
*********************************************************************************************************/
