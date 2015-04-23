#include "function.h" 
#include "math.h" 
#include "limits.h" 
#include "motion.h"
#include "lcd.h"
#include "spi_flash.h" 
#include "sysdate.h"
#include "hpir.h"
#include "i2c_ee.h"


#define TIMx TIM3 


//5ms定时器变量
volatile u32 TimingDelay=0;

//串口1通信变量
volatile unsigned char SendBuffer_uart1[BufferSize]={0};
volatile unsigned char ReadBuffer_uart1[FrameLength+30]={0};
volatile unsigned int PHead_uart1=0,PEnd_uart1=0;
volatile unsigned int QueryFlag_uart1=1;
volatile unsigned int SendLength_uart1=0;
volatile unsigned int SendCounter_uart1=0; 

//串口2通信变量
volatile unsigned char SendBuffer_uart2[BufferSize]={0};
volatile unsigned char ReadBuffer_uart2[FrameLength+30]={0};
volatile unsigned int PHead_uart2=0,PEnd_uart2=0;
volatile unsigned int QueryFlag_uart2=1;
volatile unsigned int SendLength_uart2=0;
volatile unsigned int SendCounter_uart2=0;

//串口3通信变量
volatile unsigned char SendBuffer_uart3[BufferSize]={0};
volatile unsigned char ReadBuffer_uart3[FrameLength+30]={0};
volatile unsigned int PHead_uart3=0,PEnd_uart3=0;
volatile unsigned int QueryFlag_uart3=1;
volatile unsigned int SendLength_uart3=0;
volatile unsigned int SendCounter_uart3=0;

//切光片控制标志
volatile unsigned char chopper_flag = 0;

void RCC_Configuration(void)
{
	/* RCC system reset(for debug purpose) */
	ErrorStatus HSEStartUpStatus;
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK/2 */
		RCC_PCLK2Config(RCC_HCLK_Div2);

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

		/* PLLCLK = 8MHz * 8 = 64 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_5);

		/* Enable PLL */
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while (RCC_GetSYSCLKSource() != 0x08)
		{}
	}

	/* Enable ALL peripheral clocks --------------------------------------------------*/
	//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_ALL, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1|RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_SPI2|RCC_APB1Periph_USART3|RCC_APB1Periph_USART2, ENABLE);
	//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD
		|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG|RCC_APB2Periph_TIM1|RCC_APB2Periph_ADC1
		|RCC_APB2Periph_USART1,ENABLE);
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* SPI2 IRQ Channel configuration */
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//行程开关外部中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	/* TIM1 update Channel configuration */
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//timer2
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//usart1 
	//collin open20141028
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//usart2	 
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//timer3
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	   										  
}

void Speaker_Config(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPEAKER;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_SPEAKER, &GPIO_InitStructure);
	GPIO_WriteBit(GPIO_SPEAKER, GPIO_Pin_SPEAKER, Bit_RESET);
}

void Speaker(u8 Num)
{
	while(Num-->0)
	//while(1)
	{
		GPIO_WriteBit(GPIO_SPEAKER, GPIO_Pin_SPEAKER, Bit_SET);
		//500ms
		Delay5ms(100);
		GPIO_WriteBit(GPIO_SPEAKER, GPIO_Pin_SPEAKER, Bit_RESET);
		Delay5ms(100);
	}
}
                           
void Delay5ms(u32 nTime)
{
	/* Enable the SysTick Counter 允许SysTick计数器*/
	SysTick_CounterCmd(SysTick_Counter_Enable);

	TimingDelay = 5*nTime;

	while(TimingDelay != 0)
		;  //等待计数至0

	/* Disable the SysTick Counter 禁止SysTick计数器*/
	// SysTick_CounterCmd(SysTick_Counter_Disable);
	/* Clear the SysTick Counter 清零SysTick计数器*/
	SysTick_CounterCmd(SysTick_Counter_Clear);
}

void Uart1_Config()
{
	USART_InitTypeDef USART_InitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;
		/* Configure USARTx_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USARTx_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	USART_InitStructure.USART_BaudRate = 128000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* Configure the USART1 */ 
	USART_Init(USART1, &USART_InitStructure);
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);
	//启动串口接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		
}

#if 1
void Uart2_Config()
{
	USART_InitTypeDef USART_InitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;
		/* Configure USARTx_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USARTx_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	USART_InitStructure.USART_BaudRate = 19200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* Configure the USART1 */ 
	USART_Init(USART2, &USART_InitStructure);
	/* Enable the USART1 */
	USART_Cmd(USART2, ENABLE);
	//启动串口接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
}
#endif

void Uart3_Config()
{
	USART_InitTypeDef USART_InitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;
		/* Configure USARTx_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure USARTx_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	USART_InitStructure.USART_BaudRate = 9600;
	//USART_InitStructure.USART_BaudRate = 256000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* Configure the USART3 */ 
	USART_Init(USART3, &USART_InitStructure);
	/* Enable the USART3 */
	USART_Cmd(USART3, ENABLE);
	//启动串口接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);		
}


void Pump_GPIO_Config(void)
{

GPIO_InitTypeDef GPIO_InitStructure;



GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOD,&GPIO_InitStructure);
//Delay5ms(200);
GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_SET);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOD,&GPIO_InitStructure);
//Delay5ms(200);
GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_SET);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOD,&GPIO_InitStructure);
//Delay5ms(200);
GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_SET);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOE,&GPIO_InitStructure);
//Delay5ms(200);
GPIO_WriteBit(GPIOE, GPIO_Pin_15, Bit_RESET);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOE,&GPIO_InitStructure);
//Delay5ms(200);
GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_RESET);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOE,&GPIO_InitStructure);
//Delay5ms(200);
GPIO_WriteBit(GPIOE, GPIO_Pin_14, Bit_RESET);



GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOB,&GPIO_InitStructure);

/*
while(1)
{
GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);
Delay5ms(1);
GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET);
Delay5ms(1);
}
*/

//PBL3717 PHASE脚输入低电平 pump 停止振动
//while(1)
//{
//GPIO_WriteBit(GPIOB, GPIO_Pin_11,  Bit_RESET);
 //Delay5ms(100);

//Delay5ms(100);
//}
}

void Chopper_GPIO_Conifg(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

/*	
while(1)
{
GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_SET);
Delay5ms(4);
GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);
Delay5ms(4);
}
*/
}

void Motor_GPIO_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//输出低电平
	GPIO_WriteBit(GPIOB, GPIO_Pin_8,  Bit_RESET);		
}

void Valve_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Valve1 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_Valve1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Valve2;
	GPIO_Init(GPIO_Valve2_3, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Valve8 | GPIO_Pin_Valve9;
	GPIO_Init(GPIO_Valve4_12, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_Valve14 |GPIO_Pin_Valve15 |GPIO_Pin_Valve16|GPIO_Pin_Valve17 |GPIO_Pin_Valve18;;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_Valve13_18, &GPIO_InitStructure);

	//所有阀门低电平
	GPIO_WriteBit(GPIO_Valve1, GPIO_Pin_Valve1,  Bit_RESET);
	GPIO_WriteBit(GPIO_Valve2_3,  GPIO_Pin_Valve2,  Bit_RESET);
	GPIO_WriteBit(GPIO_Valve4_12, GPIO_Pin_Valve8,  Bit_RESET);
	GPIO_WriteBit(GPIO_Valve4_12, GPIO_Pin_Valve9,  Bit_RESET);
	GPIO_WriteBit(GPIO_Valve13_18,GPIO_Pin_Valve14, Bit_RESET);
	GPIO_WriteBit(GPIO_Valve13_18,GPIO_Pin_Valve15, Bit_RESET);
	GPIO_WriteBit(GPIO_Valve13_18,GPIO_Pin_Valve16, Bit_RESET);
	GPIO_WriteBit(GPIO_Valve13_18,GPIO_Pin_Valve17, Bit_RESET);
	GPIO_WriteBit(GPIO_Valve13_18,GPIO_Pin_Valve18, Bit_RESET);
}

void Inqueue_uart1(unsigned char byte)
{
	if(PHead_uart1-1==PEnd_uart1 || PHead_uart1+BufferSize-1==PEnd_uart1)
		return;//循环队列已满

	SendBuffer_uart1[PEnd_uart1++]=byte;
	if(PEnd_uart1>=BufferSize) 
		PEnd_uart1-=BufferSize;
}

unsigned char Exqueue_uart1(void)
{
	unsigned char ReturnChar_uart1;
	if(PEnd_uart1==PHead_uart1)	//如果空队列
		return 0;
	ReturnChar_uart1=SendBuffer_uart1[PHead_uart1++];
	if(PHead_uart1>=BufferSize) PHead_uart1-=BufferSize;
	return ReturnChar_uart1;
} 

void Delay5ms_Config(void)
{
	/* Disable SysTick Counter */
	SysTick_CounterCmd(SysTick_Counter_Disable);        //失能计数器 

	/* Disable the SysTick Interrupt */
	SysTick_ITConfig(DISABLE);   //关闭中断

	/* Configure HCLK clock as SysTick clock source */
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);   //8分频
	/* SysTick interrupt each 1000 Hz with HCLK equal to 61.44MHz */
	SysTick_SetReload(3840);   //周期10ms
	/* Enable the SysTick Interrupt */
	SysTick_ITConfig(ENABLE);  //打开中断
}

void Sys_100msClock_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//周期100ms
	TIM_TimeBaseStructure.TIM_Period	= 6143; 
	TIM_TimeBaseStructure.TIM_Prescaler = 999;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	//计数器上溢中断设定
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);/*清除更新标志位*/
	TIM_ARRPreloadConfig(TIMx, DISABLE);/*预装载寄存器的内容被立即传送到影子寄存器 */
	//启动定时器
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);   
	TIM_Cmd(TIMx, ENABLE);
}

void Sys_20msClock_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//周期100ms
	TIM_TimeBaseStructure.TIM_Period	= 615; 
	TIM_TimeBaseStructure.TIM_Prescaler = 999;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//计数器上溢中断设定
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);/*清除更新标志位*/
	TIM_ARRPreloadConfig(TIM2, DISABLE);/*预装载寄存器的内容被立即传送到影子寄存器 */
	//启动定时器
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);   
	TIM_Cmd(TIM2, ENABLE);
}

unsigned char QLength_uart1(void)
{
	if((int)(PEnd_uart1-PHead_uart1)>=0)
		return PEnd_uart1-PHead_uart1;
	else
		return PEnd_uart1-PHead_uart1+BufferSize;
}	

unsigned char QLength_uart2(void)
{
	if((int)(PEnd_uart2-PHead_uart2)>=0)
		return PEnd_uart2-PHead_uart2;
	else
		return PEnd_uart2-PHead_uart2+BufferSize;
}

void Inqueue_uart2(unsigned char byte)
{
	if(PHead_uart2-1==PEnd_uart2 || PHead_uart2+BufferSize-2==PEnd_uart2)
		return;//循环队列已满

	SendBuffer_uart2[PEnd_uart2++]=byte;
	if(PEnd_uart2>=BufferSize) 
		PEnd_uart2-=BufferSize;
}

unsigned char Exqueue_uart2(void)
{
	unsigned char ReturnChar_uart2;
	if(PEnd_uart2==PHead_uart2)	//如果空队列
		return 0;
	ReturnChar_uart2=SendBuffer_uart2[PHead_uart2++];
	if(PHead_uart2>=BufferSize) PHead_uart2-=BufferSize;
	return ReturnChar_uart2;
}

unsigned char QLength_uart3(void)
{
	if((int)(PEnd_uart3-PHead_uart3)>=0)
		return PEnd_uart3-PHead_uart3;
	else
		return PEnd_uart3-PHead_uart3+BufferSize;
}

void Inqueue_uart3(unsigned char byte)
{
	if(PHead_uart3-1==PEnd_uart3 || PHead_uart3+BufferSize-2==PEnd_uart3)
		return;//循环队列已满

	SendBuffer_uart3[PEnd_uart3++]=byte;
	if(PEnd_uart3>=BufferSize) 
		PEnd_uart3-=BufferSize;
}

unsigned char Exqueue_uart3(void)
{
	unsigned char ReturnChar_uart3;
	if(PEnd_uart3==PHead_uart3)	//如果空队列
		return 0;
	ReturnChar_uart3=SendBuffer_uart3[PHead_uart3++];
	if(PHead_uart3>=BufferSize) PHead_uart3-=BufferSize;
	return ReturnChar_uart3;
}  

