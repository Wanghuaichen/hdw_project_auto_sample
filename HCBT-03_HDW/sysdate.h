#ifndef __SYSDATE_H
#define __SYSDATE_H
//****************************************************************
extern void	write_ds(u8,u8);
extern u16	year;
extern u16	month;
extern u16	date;
extern u16	hour;
extern u16	minute;
extern u16	second;
extern u16	week;
extern u8	read_ds(u8);
extern void	SetTime(void);
extern void	SetDate(void);
extern void	GetTime(void);
extern void	GetDate(void);
extern  volatile  u32 TimingDelay;
extern	u8 	ChangeTohex(u8 dat);
extern	u8 	ChangeToDec(u8 dat);
#endif

