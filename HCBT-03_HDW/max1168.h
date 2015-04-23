/******************** (C) COPYRIGHT Headway ********************
* File Name          : max1168.h
* Author             : collin
* Version            : V1.0
* Date               : 03/04/2015
* Description        : Header for max1168.c file.
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_MAX1168_H
#define __SPI_MAX1168_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Select SPI FLASH: Chip Select pin low  */
#define SPI_MAX1168_CS_LOW()       GPIO_ResetBits(GPIOC, GPIO_Pin_8)
/* Deselect SPI FLASH: Chip Select pin high */
#define SPI_MAX1168_CS_HIGH()      GPIO_SetBits(GPIOC, GPIO_Pin_8)

/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
void SPI_MAX1168_Init(void);
void MAX1168_Config(u8 channel);
void MAX1168_WriteByte(u8 buffer);
void MAX1168_WriteWord(u16 buffer);
u16 SPIx_ReadWriteWord(u16 data);
u8 SPIx_ReadWriteByte(u8 data);
u16 GetDataMax1168_CH0(void);
u16 GetDataMax1168_CH1(void);
void Intensity_Data(void);

#endif /* __SPI_FLASH_H */

/******************* (C) COPYRIGHT Headway *****END OF FILE****/

