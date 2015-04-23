#include "lcd.h"
#include "sysdate.h"
#include "function.h" 
#define ui16 unsigned int
#define uc8 unsigned char
extern void	write_ds(u8,u8);
u8	 read_ds(u8);
void	SetTime(void);
void	SetDate(void);
void	GetTime(void);
void	GetDate(void);

u8 	ChangeTohex(u8 dat);
u8 	ChangeToDec(u8 dat);
//u16	year=0;
//u16	month=0;
//u16	date=0;
//u16	hour=0;
//u16	minute=0;
//u16	second=0;
//u16	week=0;

/*********RD=ds,WR=rw,ALE=as,CS1=CS**************************/



u8 read_ds(u8 add)
{
	unsigned char  tValue; 
	u8 ds_date;
	//	dsas=1;//P7.2
	ALE=1;
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	//	dsds=1;//P7.0
	RDT=1;
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	//	dsrw=1;//P7.1
	WRT=1;
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	//	dscs=0;
	CS1=0;
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	//DATABus=add;
	tValue=~add; 
	GPIOD->BSRR=(unsigned int)add; 
	GPIOD->BRR=(unsigned int)tValue; 
	//	dsas=0;P7.2
	ALE=0;
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	//	dsds=0;P7.0
	RDT=0;
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	//	DATABus=0xff;
	GPIOD->BSRR=0xff; 
	GPIOD->BRR=0x0; 
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	//	ds_date=DATABus;
	GPIOD->CRL&=0X0; 
	GPIOD->CRL|=0X88888888;//PD.10/11/12输入 
	ds_date	= GPIO_ReadInputData(GPIOD);
	ds_date&=0xFF;
	GPIOD->CRL&=0X0; 
	GPIOD->CRL|=0X33333333;//PD.10/11/12输出 
	//	dsds=1;
	RDT=1;
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	//	dsas=1;
	ALE=1;
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	//	dscs=1;P7.3
	CS1=1;
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();

	return ds_date;	
}

/************************************************************* 
函数功能：该函数用来设置时钟芯片的时间
*************************************************************/   
void SetTime(void)
{
	write_ds(0x04,timer.hour);
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	write_ds(0x02,timer.min);
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	write_ds(0x0, timer.sec);
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
}
/************************************************************* 
函数功能：该函数用来设置时钟芯片的日期
*************************************************************/   
void SetDate(void)
{
	write_ds(0x09,timer.w_year);
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	write_ds(0x08,timer.w_month);
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	write_ds(0x07,timer.w_date);
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
}  
void GetTime(void)
{	
//	hour	= read_ds(0x04);
	timer.hour	= read_ds(0x04);
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
//	minute	= read_ds(0x02);
	timer.min	= read_ds(0x02);
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
//	second	= read_ds(0x0);
	timer.sec	= read_ds(0x0);
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	if(timer.sec>59)
	{
		timer.sec=0;
		write_ds(0x0, timer.sec);
	}
}
void GetDate(void)
{
//	date	= read_ds(0x07);
	timer.w_date	= read_ds(0x07);
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
//	month	= read_ds(0x08);
	timer.w_month	= read_ds(0x08);
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	//year	= read_ds(0x09);
	timer.w_year	= read_ds(0x09);
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	timer.week=RTC_Get_Week(2000+timer.w_year,timer.w_month,timer.w_date);
}  



/**二进制数据转换为BCD码**/
u8 	ChangeTohex(u8 dat)
{
	u8 temp;
	temp=(dat/10)*16+dat%10;
	return temp;	
}

/**BCD码转换为二进制数据**/
u8 	ChangeToDec(u8 dat)
{
	u8 temp;
	temp=(dat/16)*10+dat%16;
	return temp;
}


