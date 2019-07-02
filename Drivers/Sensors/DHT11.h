/**
  ******************************************************************************
  * @file    I2CRoutines.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief   header
  ******************************************************************************
**/

#ifndef __DHT11___H
#define __DHT11___H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint8_t Data[5];
	ErrorStatus Status;
} DHT_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define MAX_TICS 100000

#define T_measurement_Hold			0xE3
#define RH_measurement_Hold			0xE5
#define T_measurement_NO_Hold		0xF3
#define RH_measurement_NO_Hold		0xF5
#define Soft_reset					0xFE

/* Exported varibles ---------------------------------------------------------*/
extern volatile DHT_TypeDef *HTSensor;
extern volatile DHT_TypeDef DHTSensor;
extern volatile DHT_TypeDef SHTSensor;

extern ErrorStatus Read_I2C1, Read_I2C2;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
ErrorStatus SHT_Read(uint8_t* ReadData, uint8_t mode);
ErrorStatus DHT_Read(uint8_t* ReadData, uint8_t mode);

void DHT_Init(GPIOMode_TypeDef Mode);
void SHT_Init(Bool Read);

#endif /* __DHT11___H */

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


