/****************************************************************
【文 件 名 称】lcd12864.h
【功 能 描 述】lcd12864 头文件
【 作      者 】shifu
*****************************************************************/

#ifndef __LCD_H
#define __LCD_H

//****************************************************************
#include "stm32f10x_lib.h"
//*****************************************************************
//管脚定义

#define GPIO_LCD    GPIOC   
#define RCC_APB2Periph_GPIO_LCD    RCC_APB2Periph_GPIOC
#define uchar unsigned char
#define uint unsigned int
#define u8 unsigned char
//********************函数声明************************************

/*****************************************************************
         液晶模块指令集定义
*****************************************************************
      0x01      //清显示指令
      0x06      //设置输入模式
      0x0c      //设置开显控制
      0x30      //功能设定(基本指令)
      0x34      //功能设定(扩充指令)
      0x36      //打开绘图(扩充指令)
*****************************************************************/
#define u8BAND(addr, u8num) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(u8num<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define u8_ADDR(addr, u8num)   MEM_ADDR(u8BAND(addr, u8num)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   u8_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    u8_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   u8_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    u8_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   u8_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    u8_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   u8_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    u8_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   u8_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    u8_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   u8_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    u8_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   u8_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    u8_ADDR(GPIOG_IDR_Addr,n)  //输入

/*****************显示器接口*******

sfr LCDData=0x90;//DB0-DB7定义使用P1口
su8 LCDWR=P2^0;//write signal
su8 LCDRD=P2^1;//read signal
su8 LCDCE=P2^2;//chip enable signal
su8 LCDCD=P2^3;//H:instruction code L:data
su8 LCDRE=P2^4;//reset signal
su8 LCDFS=P2^5;//font select signal(H:5X8 L:8X8) 
**********/
#define RW  PEout(1)    
#define RD  PEout(2)   
#define CE  PEout(3)  
#define CD  PEout(4)   
#define RE  PEout(5)   
#define FS  PEout(6) 
/*****************蜂鸣器接口*****************/
//#define Speaker  PEout(13)   
/*****************DS1302*****************/
//#define SCLK  PBout(6)
//#define RST   PEout(4)

/*****************时钟芯片接口*****************/
#define RDT  PCout(10)   
#define WRT  PCout(11)   
#define ALE  PGout(9)   	     
#define CS1  PGout(10)    
/*****************SPI芯片WR接口*****************/
#define WR  PFout(11)    
/*****************打印机接口*****************/
#define STB  PBout(8)
#define SEL  PEin(0)  
#define BUSY  PBin(9)

#define SAMPLE1	PFin(5)
#define SAMPLE2	PFin(4)
#define SAMPLE3	PFin(3)
#define SAMPLE4	PFin(2)
#define SAMPLE5	PFin(1)
#define SAMPLE6	PFin(0)

#define GPIO_SamplePosition GPIOF
#define SamplePosition_Pin_1 GPIO_Pin_5
#define SamplePosition_Pin_2 GPIO_Pin_4
#define SamplePosition_Pin_3 GPIO_Pin_3
#define SamplePosition_Pin_4 GPIO_Pin_2
#define SamplePosition_Pin_5 GPIO_Pin_1
#define SamplePosition_Pin_6 GPIO_Pin_0
  
/*****************卡在位信号及LED*****************/
#define LED_OUT    PBout(5)//LCD背光
#define LED_ERROR    PFout(13)//错误灯	
extern void Delay(vu32 nCount); 

extern void 	System_Init		(void);
extern void  	Port_Init 		(void);
extern uchar 	Read_State 		(void);

extern	void Delay(vu32 nCount);

 #endif


