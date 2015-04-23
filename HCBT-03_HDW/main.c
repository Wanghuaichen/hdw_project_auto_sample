/******************** (C) COPYRIGHT Headway ********************
* File Name          : main.c
* Author             : collin
* Version            : V1.0
* Date               : 03/04/2015
* Description        : This file is the main function for the project
*******************************************************************************/

#include "stm32f10x_lib.h"
#include "function.h"
#include "motion.h"
#include "math.h"
#include "stm32_dsp.h"
#include "table_fft.h"
#include "i2c_ee.h"
#include "lcd.h"
#include "spi_flash.h"  
#include "sysdate.h"  
#include "max1168.h"

/* Private define -------------------------------------------------------------*/




/* Private macro -------------------------------------------------------------*/
//#define countof(a) (sizeof(a) / sizeof(*(a)))



//end
/* Private Functions ---------------------------------------------------------*/
void System_Init(void);



int main(void)
{
	System_Init();

	//Intensity data collection
	Intensity_Data();


}

void 	System_Init(void)
{
	
	RCC_Configuration();
	NVIC_Configuration();
	Delay5ms_Config(); 
	Sys_100msClock_Config();
	Sys_20msClock_Config();
	
	Speaker_Config();
		
	//GPIO_Configuration();
	//Speaker_Config();
	//Speaker(3);

	Uart1_Config(); 
	//Uart2_Config();
	//Uart3_Config();

	SPI_MAX1168_Init();
	//vibrating pump gpio config
	//Pump_GPIO_Config();
	Chopper_GPIO_Conifg();

	//while(1);

	//motor gpio config
	//Motor_GPIO_Config();
	
	//Valve_GPIO_Config();















}