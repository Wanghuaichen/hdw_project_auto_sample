/******************** (C) COPYRIGHT Headway ********************  
* File Name          : max1168.c  
* Author             : collin
* Version            : V1.0  
* Date               : 03/04/2015  
* Description        : This file provides a set of functions needed to manage the  
*                      communication between SPI peripheral and SPI MAX1168 ADC.  
*******************************************************************************/   

/* Includes ------------------------------------------------------------------*/   
#include "max1168.h" 
#include "function.h" 

/* Private typedef -----------------------------------------------------------*/   
#define SPI_FLASH_PageSize    256   
#define MAX1168_Reference_Voltage 2500.0

/* Private define ------------------------------------------------------------*/   
#define WRITE      0x02  /* Write to Memory instruction */   
#define WRSR       0x01  /* Write Status Register instruction */   
#define WREN       0x06  /* Write enable instruction */   

#define READ       0x03  /* Read from Memory instruction */   
#define RDSR       0x05  /* Read Status Register instruction  */   
#define RDID       0x9F  /* Read identification */   
#define SE         0xD8  /* Sector Erase instruction */   
#define BE         0xC7  /* Bulk Erase instruction */   

#define WIP_Flag   0x01  /* Write In Progress (WIP) flag */   

#define Dummy_Byte 0xA5   

/* Private macro -------------------------------------------------------------*/   
/* Private variables ---------------------------------------------------------*/  
/* Private function prototypes -----------------------------------------------*/   
/* Private functions ---------------------------------------------------------*/   

/*******************************************************************************  
* Function Name  : SPI_MAX1168_Init  
* Description    : Initializes the peripherals used by the SPI MAX1168 driver.  
* Input          : None  
* Output         : None  
* Return         : None  
*******************************************************************************/   
void SPI_MAX1168_Init(void)   
{   
	SPI_InitTypeDef  SPI_InitStructure;   
	GPIO_InitTypeDef GPIO_InitStructure;   

	/* Enable SPI2 and GPIO clocks */   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB2Periph_GPIOB, ENABLE);  
	#if 0
	/* Configure SPI2 pins:NSS */  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;// AdDrdy
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;//PC8ÅäÖÃ³ÉÉÏÀ­ÊäÈë
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	#endif
	/* Configure SPI2 pins:SCK, MISO and MOSI */   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_Init(GPIOB, &GPIO_InitStructure); 


	/* Configure I/O for MAX1168 select */
	/*   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOD, &GPIO_InitStructure);   
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOC, &GPIO_InitStructure);   
	/* Deselect the MAX1168: Chip Select high */
	SPI_MAX1168_CS_HIGH();

	/* SPI2 configuration */   
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;   
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;   
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;   
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;   
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;   
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;   
	SPI_InitStructure.SPI_CRCPolynomial = 7;   
	SPI_Init(SPI2, &SPI_InitStructure);   

	/* Enable SPI2  */   
	SPI_Cmd(SPI2, ENABLE);   
}


/*******************************************************************************  
* Function Name  : MAX1168_Config  
* Description    : Initializes MAX1168 ADC.  
* Input          : channel number  
* Output         : None  
* Return         : None  
*******************************************************************************/   
void MAX1168_Config(u8 channel)
{
switch(channel)
{
	case 0:
	//MAX1168_WriteByte(0x0);
	MAX1168_WriteWord(0x0);//write MAX1168 Command/Configuration/Control Register select channel 0,External Reference,External Clock	
	break;
	case 1:
	//MAX1168_WriteByte(0x20);
	MAX1168_WriteWord(0x2000);//write MAX1168 Command/Configuration/Control Register select channel 1,External Reference,External Clock	
	break;
	default:
	break;
}


}

/*******************************************************************************  
* Function Name  : MAX1168_WriteByte  
* Description    : Write command/configuration info to MAX1168.  
* Input          : configuration data  
* Output         : None  
* Return         : None  
*******************************************************************************/
void MAX1168_WriteWord(u16 buffer)
{
	SPI_MAX1168_CS_LOW();//enable MAX1168
	SPIx_ReadWriteWord(buffer);
	//SPI_MAX1168_CS_HIGH(); 

}

/*******************************************************************************  
* Function Name  : MAX1168_WriteByte  
* Description    : Write command/configuration info to MAX1168.  
* Input          : configuration data  
* Output         : None  
* Return         : None  
*******************************************************************************/
void MAX1168_WriteByte(u8 buffer)
{
	SPI_MAX1168_CS_LOW();//enable MAX1168
	SPIx_ReadWriteByte(buffer);
	//SPI_MAX1168_CS_HIGH(); 

}

/*******************************************************************************  
* Function Name  : SPIx_ReadWriteByte  
* Description    : read/write one byte.  
* Input          : write data  
* Output         : data read from MAX1168  
* Return         : None  
*******************************************************************************/
u8 SPIx_ReadWriteByte(u8 data)
{
u8 retry = 0;
while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);//¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ£»·¢ËÍ»º´æ¿Õ±êÖ¾Î»
/*
{
retry++;
if(retry>200)
	return 0;
}
*/
	SPI_I2S_SendData(SPI2, data); //Í¨¹ýÍâÉèSPIx·¢ËÍÒ»¸öÊý¾Ý
	retry=0;

	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ£»·¢ËÍ»º´æ¿Õ±êÖ¾Î»
/*
{
retry++;
if(retry>200)
	return 0;
}
*/
	return SPI_I2S_ReceiveData(SPI2); //·µ»ØÍ¨¹ýSPIx×î½ü½ÓÊÕµÄÊý¾Ý

}

/*******************************************************************************  
* Function Name  : SPIx_ReadWriteByte  
* Description    : read/write one byte.  
* Input          : write data  
* Output         : data read from MAX1168  
* Return         : None  
*******************************************************************************/
u16 SPIx_ReadWriteWord(u16 data)
{
u8 retry = 0;
while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);//¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ£»·¢ËÍ»º´æ¿Õ±êÖ¾Î»
/*
{
retry++;
if(retry>200)
	return 0;
}
*/
	SPI_I2S_SendData(SPI2, data); //Í¨¹ýÍâÉèSPIx·¢ËÍÒ»¸öÊý¾Ý
	retry=0;

	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);//¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ£»·¢ËÍ»º´æ¿Õ±êÖ¾Î»
/*
{
retry++;
if(retry>200)
	return 0;
}
*/
	return SPI_I2S_ReceiveData(SPI2); //·µ»ØÍ¨¹ýSPIx×î½ü½ÓÊÕµÄÊý¾Ý

}

/*******************************************************************************  
* Function Name  : GetDataMax1168_CH0 
* Description    : read MAX1168 conversion data from Channel0  
* Input          : None 
* Output         : conversion data  
* Return         : None  
*******************************************************************************/
u16 GetDataMax1168_CH0(void)
{
u16 NumByteToRead = 2;
u16 temp = 0;
u8 buffer[2];
int i;
MAX1168_Config(0);// select channel 0
temp = SPIx_ReadWriteWord(0xa5a5);
/*
for(i=0;i<2;i++)
{
buffer[i] = SPIx_ReadWriteByte(0xa5);
}
temp = (buffer[0]<<8) + buffer[1];
*/
SPI_MAX1168_CS_HIGH();
return temp;
}

/*******************************************************************************  
* Function Name  : GetDataMax1168_CH1 
* Description    : read MAX1168 conversion data from Channel1  
* Input          : None 
* Output         : conversion data  
* Return         : None  
*******************************************************************************/
u16 GetDataMax1168_CH1(void)
{
u16 NumByteToRead = 2;
u16 temp = 0;
u8 buffer[2];
int i;
MAX1168_Config(1);// select channel 1
/*
for(i=0;i<2;i++)
{
buffer[i] = SPIx_ReadWriteByte(0xa5);
}
temp = (buffer[0]<<8) + buffer[1];
*/
temp = SPIx_ReadWriteWord(0xa5a5);
SPI_MAX1168_CS_HIGH();
return temp;
}

/*******************************************************************************  
* Function Name  : Intensity_Data 
* Description    : Collect data and send data to PC  
* Input          : None 
* Output         : None  
* Return         : None  
*******************************************************************************/
void Intensity_Data(void)
{
	u8 num1[] = {"0000.0mv"};
	u8 num2[] = {"0000.0mv"};

	u16 RCH0_16bit, RCH1_16bit;
	//u32 l_ncm0,l_ncm1;
	float l_ncm0,l_ncm1;
	int tx_index = 0;
	int i,j;
	while(1)
	//for(i=0;i<2;i++)
	{
	
	RCH0_16bit = GetDataMax1168_CH0();
	//l_ncm0 = (u32)(RCH0_16bit*(MAX1168_Reference_Voltage/65535)); //Ëã³öÍ¨µÀ0µçÑ
	l_ncm0 = (RCH0_16bit*(MAX1168_Reference_Voltage/65535)); //Ëã³öÍ¨µÀ0µçÑ¹¹
	RCH1_16bit = GetDataMax1168_CH1();
	//l_ncm1 = (u32)(RCH0_16bit*(MAX1168_Reference_Voltage/65535)); //Ëã³öÍ¨µÀ1µçÑ¹
	l_ncm1 = (RCH0_16bit*(MAX1168_Reference_Voltage/65535)); //Ëã³öÍ¨µÀ1µçÑ¹

	//printf("C12Ô­Ê¼Êý¾ÝÊÇ£º%04X,  µ±Ç°µçÑ¹ÊÇ£º%4.3f¡£\r\n",RCH0_16bit,l_ncm0);

	//printf("C13Ô­Ê¼Êý¾ÝÊÇ£º%04X,  µ±Ç°µçÑ¹ÊÇ£º%4.3f¡£\r\n",RCH1_16bit,l_ncm1);

	 //¼ÓÈëÖ¡Í·
        Inqueue_uart1(0x02);
        //¼ÓÈëÃüÁî±êÖ¾
        Inqueue_uart1(0x01);
        Inqueue_uart1(RCH0_16bit&0xff);
        Inqueue_uart1((RCH0_16bit>>8)&0xff);
		Inqueue_uart1(RCH1_16bit&0xff);
        Inqueue_uart1((RCH1_16bit>>8)&0xff);
		Inqueue_uart1(0);
        Inqueue_uart1(0);		
        Inqueue_uart1(0);
        Inqueue_uart1(0);

        for(j=0; j<8; j++)
            Inqueue_uart1(0);
        Inqueue_uart1(0x07);//buf18
        Inqueue_uart1(0x07);//buf19
        for(j=0; j<3; j++)
            Inqueue_uart1(0);
        Inqueue_uart1(0x03);
		Delay5ms(100);
	#if 0	
		//¼ÓÈëÖ¡Í·
        Inqueue_uart2(0x02);
        //¼ÓÈëÃüÁî±êÖ¾
        Inqueue_uart2(0x01);
        Inqueue_uart2(RCH0_16bit&0xff);
        Inqueue_uart2((RCH0_16bit>>8)&0xff);
		Inqueue_uart2(RCH1_16bit&0xff);
        Inqueue_uart2((RCH1_16bit>>8)&0xff);
		Inqueue_uart2(0);
        Inqueue_uart2(0);		
        Inqueue_uart2(0);
        Inqueue_uart2(0);

        for(j=0; j<8; j++)
            Inqueue_uart2(0);
        Inqueue_uart2(0x07);//buf18
        Inqueue_uart2(0x07);//buf19
        for(j=0; j<3; j++)
            Inqueue_uart2(0);
        Inqueue_uart2(0x03);
		Delay5ms(100);
#endif	

#if 0	
		//¼ÓÈëÖ¡Í·
        Inqueue_uart3(0x02);
        //¼ÓÈëÃüÁî±êÖ¾
        Inqueue_uart3(0x01);
        Inqueue_uart3(RCH0_16bit&0xff);
        Inqueue_uart3((RCH0_16bit>>8)&0xff);
		Inqueue_uart3(RCH1_16bit&0xff);
        Inqueue_uart3((RCH1_16bit>>8)&0xff);
		Inqueue_uart3(0);
        Inqueue_uart3(0);		
        Inqueue_uart3(0);
        Inqueue_uart3(0);

        for(j=0; j<8; j++)
            Inqueue_uart3(0);
        Inqueue_uart3(0x07);//buf18
        Inqueue_uart3(0x07);//buf19
        for(j=0; j<3; j++)
            Inqueue_uart3(0);
        Inqueue_uart3(0x03);
		Delay5ms(100);
		
		#endif
			
	}
	while(1);
}
   

/******************* (C) COPYRIGHT  Headway *****END OF FILE****/   

