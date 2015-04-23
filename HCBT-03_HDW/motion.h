#ifndef _moto_action_h_
#define _moto_action_h_
#include "stm32f10x_lib.h"
#define T m_lCf[7]	  //可改变试压最大压力，T务必小于5400
#define Length 10.16   //可改变丝杠导程
//AK5351 变量
#define DataLength 3600
extern volatile int LastCh;
//小电机加速和记脉冲变量
extern volatile unsigned int Step_LM;
extern const unsigned int Speedup[200];

extern volatile unsigned int RxLeftIdx;
extern volatile unsigned int RxRightIdx;
extern volatile u16 AK_Left_Data[DataLength*2];
extern volatile u16 AK_Right_Data[DataLength*2];
//汽缸电机变量
extern volatile u16 PulseNum;
extern volatile u16 PulseCount;
extern volatile u8 	StartSpeed;
extern volatile u8 	TargeSpeed;
extern volatile u8 	StepTimes_1;
extern volatile u16 StepCount_1;
extern volatile u8 	EndSpeed;
extern volatile u8 	StepTimes_2;
extern volatile u16 StepCount_2;
extern volatile u16 AddedNum;	 
extern volatile u8 	cw_status;
extern 	u8 ToMeasPosition[12];
extern 	u8	ch_checked[6];
extern volatile u8 flagPress;
extern 	u32	Intensity1,Intensity2;
extern u8	m_bChopper;
extern 	double m_lCfC12[6];
extern 	double m_lCfC13[6];
//collin add20141031
extern double m_lCfC12_backup[6];
extern double m_lCfC13_backup[6];
//end
extern  u16  Pressure0,Pressure1,Pressure2;  
extern	u16 serial;
extern 	double	Percent;
extern 	u8	m_bDilution; 
extern 	u8	m_bChanged;
extern 	u8	m_DilutionChannel;
extern 	u8	m_mesureType;
extern 	u8  reTest;
extern double 	m_lCf[9] ;
#define  ADCNUM 25	  //延时2s后进行ADC采样,点数25，去除5个最大值和5个最小值后取平均作为ADC转换值。
extern u16	ADCConv[ADCNUM];
//定义开关
#ifndef ON_OFF
	#define ON_OFF
	#define ON 1
	#define OFF 0
#endif
//定义切光片电机端口
#ifndef GPIO_M2
	#define GPIO_M2
	#define GPIO_M2CL 		GPIOA
	#define GPIO_Pin_M2CL 	GPIO_Pin_8
	#define GPIO_M2RESET 	GPIOG
	#define GPIO_Pin_M2RESET GPIO_Pin_4
	#define GPIO_M2EN		GPIOC
	#define GPIO_Pin_M2EN	GPIO_Pin_8	
#endif
//定义气缸电机端口
#ifndef GPIO_M1
	#define FORWARD	 0
	#define BACKWARD 1
	
	#define GPIO_M1
	#define GPIO_M1CL 		GPIOG
	#define GPIO_Pin_M1CL	GPIO_Pin_7
	#define GPIO_M1RESET 	GPIOG
	#define GPIO_Pin_M1RESET GPIO_Pin_3
	#define GPIO_M1EN		GPIOG
	#define GPIO_Pin_M1EN	GPIO_Pin_5
	#define GPIO_M1CW		GPIOG
	#define GPIO_Pin_M1CW	GPIO_Pin_8
	#define GPIO_M1MO		GPIOC
	#define GPIO_Pin_M1MO	GPIO_Pin_9
#endif
#ifndef GPIO_AK5385
	#define GPIO_AK5385
	#define GPIO_AK5385_BICK GPIOB
	#define Pin_AK5385_BICK	GPIO_Pin_13
	#define GPIO_AK5385_LRCK GPIOB
	#define Pin_AK5385_LRCK GPIO_Pin_12
	#define GPIO_AK5385_SDATA GPIOB
	#define Pin_AK5385_SDATA GPIO_Pin_15
	#define GPIO_AK5385_DIF GPIOD
	#define Pin_AK5385_DIF GPIO_Pin_9	
	#define GPIO_AK5385_PDN 	GPIOB
	#define Pin_AK5385_PDN 	GPIO_Pin_14
	#define GPIO_AK5385_MS GPIOD
	#define Pin_AK5385_MS GPIO_Pin_8

//	#define GPIO_AK5385_DFS1 GPIOB
//	#define Pin_AK5385_DFS1 GPIO_Pin_10
//	#define GPIO_AK5385_DFS0 GPIOB
//	#define Pin_AK5385_DFS0 GPIO_Pin_11
	#define GPIO_AK5385_MCLK GPIOC
	#define Pin_AK5385_MCLK GPIO_Pin_6
	#define GPIO_AK5385_CKS0 GPIOC
	#define Pin_AK5385_CKS0 GPIO_Pin_7	
	#define GPIO_AK5385_CKS1 GPIOG
	#define Pin_AK5385_CKS1 GPIO_Pin_2
#endif
/*--------------------------------------
  口  口  口  口    口    口    口    口
				    	Peak Press	 Power
---------------------------------------*/

extern 	u8	sampleTooless;
extern 	u8  functioncheck;
extern 	u8	tocontinue;
extern 	u8  TripSwitchStatus;
extern 	u32 QualifyTimes;
void 	SelfCheckProc(void);
void 	LedCheck(void);
void LeakageCheck(u8 Go);
void	ChannelMeas(u8 Pnum);
s16 	Test(void);
void  	Cartridge(u8 wash);
void  	Air(void);
void  	AirFirst(void);
//collin add20141031
void  	AirSecond(void);
//end
void 	Baseline(u8 Pnum,u16 pulse,u16 percent);
void 	Sample(u8 Pnum,u16 pulse,double percent,u8 t);
void	BaseFirst(u8 Pnum,u16 pulse);  //初步底气测量程序
void	SampleFirst(u8 Pnum,u16 pulse);  //初步样气测量程序
void  	CheckSamplePress(void);
//汽缸电机归位
void 	Motor_Init(void);
void ADC_Config(void);
unsigned int ADC_Press(unsigned char ch);
u16 weedavg(u16 a[ADCNUM],u8 num);
unsigned int ADC(unsigned char ch);
void LightModulation(u8 status);
void LightModulation_Config(void);
void AK_Conv(void);
void AK5385_Config(void);
void Motorun(u8 En,u16 FPulseNum,u8 Cw,u8 IfConv,
			u8 FStartSpeed,u8 FTargeSpeed,u8 FStepTimes_1,u16 FStepCount_1,
			u8 FEndSpeed,u8 FStepTimes_2,u16 FStepCount_2,u16 FAddedNum,
			u8 vnum1,u8 vnum2,u8 vnum3);
void Motor_Config(void);
void Delay1ms(u32 nTime);
void Delay5ms(u32 nTime);
void Valve(unsigned char Vnum); 
/*行程开关是否被遮挡
1=被遮挡
0=未被遮挡
*/
u8 IsTripSwitchClosed(void);
void SPI2_IRQHandler(void);
#endif
