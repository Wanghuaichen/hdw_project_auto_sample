#include <stdio.h>

#ifndef __function_h_
#define __function_h_
#include "stm32f10x_lib.h"

#ifndef GPIO_SPEAKER
	#define GPIO_SPEAKER GPIOF
	#define GPIO_Pin_SPEAKER GPIO_Pin_14
#endif

//定义电磁阀端口
 #ifndef Valve_GPIO
	#define Valve_GPIO
	#define GPIO_Valve1 		GPIOF
	#define GPIO_Valve2_3 		GPIOG
	#define GPIO_Valve4_12 		GPIOE
	#define GPIO_Valve13_18		GPIOD
	#define GPIO_Pin_Valve1 	GPIO_Pin_15	
	#define GPIO_Pin_Valve2 	GPIO_Pin_0	
	#define GPIO_Pin_Valve3 	GPIO_Pin_1	
	#define GPIO_Pin_Valve4 	GPIO_Pin_7
	#define GPIO_Pin_Valve5 	GPIO_Pin_8	
	#define GPIO_Pin_Valve6 	GPIO_Pin_9
	#define GPIO_Pin_Valve7 	GPIO_Pin_10	
	#define GPIO_Pin_Valve8 	GPIO_Pin_11	
	#define GPIO_Pin_Valve9 	GPIO_Pin_12	
	#define GPIO_Pin_Valve10 	GPIO_Pin_13				 
 	#define GPIO_Pin_Valve11 	GPIO_Pin_14
	#define GPIO_Pin_Valve12 	GPIO_Pin_15
	#define GPIO_Pin_Valve13 	GPIO_Pin_10
	#define GPIO_Pin_Valve14 	GPIO_Pin_11
	#define GPIO_Pin_Valve15 	GPIO_Pin_12
	#define GPIO_Pin_Valve16 	GPIO_Pin_13
	#define GPIO_Pin_Valve17 	GPIO_Pin_14
	#define GPIO_Pin_Valve18 	GPIO_Pin_15
#endif

//5ms定时器变量
extern volatile u32 TimingDelay;

//串口通信变量uart1
#define FrameLength 24
#define BufferSize FrameLength*10+1
extern volatile unsigned short SendBackup_uart1[21][10];
extern volatile unsigned char SendBuffer_uart1[BufferSize];
extern volatile unsigned char ReadBuffer_uart1[FrameLength+30];
extern volatile unsigned int PHead_uart1,PEnd_uart1;
extern volatile unsigned int QueryFlag_uart1;
extern volatile unsigned int SendLength_uart1;
extern volatile unsigned int SendCounter_uart1;

extern volatile unsigned short SendBackup_uart2[21][10];
extern volatile unsigned char SendBuffer_uart2[BufferSize];
extern volatile unsigned char ReadBuffer_uart2[FrameLength+30];
extern volatile unsigned int PHead_uart2,PEnd_uart2;
extern volatile unsigned int QueryFlag_uart2;
extern volatile unsigned int SendLength_uart2;
extern volatile unsigned int SendCounter_uart2;

extern volatile unsigned short SendBackup_uart3[21][10];
extern volatile unsigned char SendBuffer_uart3[BufferSize];
extern volatile unsigned char ReadBuffer_uart3[FrameLength+30];
extern volatile unsigned int PHead_uart3,PEnd_uart3;
extern volatile unsigned int QueryFlag_uart3;
extern volatile unsigned int SendLength_uart3;
extern volatile unsigned int SendCounter_uart3;

extern volatile unsigned char chopper_flag;

void RCC_Configuration(void);
void NVIC_Configuration(void);
void Speaker_Config(void);
void Speaker(u8 Num);
void GPIO_Configuration(void);
void Uart1_Config(void);
void Uart2_Config(void);
void Uart3_Config(void);
void Pump_GPIO_Config(void);
void Chopper_GPIO_Conifg(void);
void Motor_GPIO_Config(void);
void Valve_GPIO_Config(void);

//队列函数 for uart1
void Inqueue_uart1(unsigned char byte);
unsigned char Exqueue_uart1(void);
unsigned char QLength_uart1(void);

//队列函数 for uart2
void Inqueue_uart2(unsigned char byte);
unsigned char Exqueue_uart2(void);
unsigned char QLength_uart2(void);

//队列函数 for uart3
void Inqueue_uart3(unsigned char byte);
unsigned char Exqueue_uart3(void);
unsigned char QLength_uart3(void);

void Delay5ms_Config(void);
//100ms系统时钟初始化
void Sys_100msClock_Config(void);

void Sys_20msClock_Config(void);
	
#endif
