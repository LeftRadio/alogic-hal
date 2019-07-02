/**
  ******************************************************************************
  * @file    Regulates.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief   header
  ******************************************************************************
**/

#ifndef __REGULATES___H
#define __REGULATES___H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported define -----------------------------------------------------------*/
#define Regulates_OUT_NotAssigned			255
#define Regulates_Max_Count					9

//#define Timer_NormalMode				0
//#define Timer_DailyMode					1

/* Exported typedef ----------------------------------------------------------*/
typedef enum { STLM75 = (uint8_t)0, DHT11 = (uint8_t)1, SHT21 = (uint8_t)2 } Sensor_Typedef;
typedef enum {Thresold = (uint8_t)0, Proportional = (uint8_t)1} Regul_Typedef;

/** —труктурный тип данных регулировок **/
typedef struct
{
	Sensor_Typedef SensorType;
	uint8_t SensorIndex;
	uint8_t MIN_SensorIndex;
	uint8_t MAX_SensorIndex;
	Regul_Typedef RegulType;
	uint8_t Thresold;
	uint8_t Thresold_2;
	uint8_t Hysteresis;
	uint8_t PWM_ON_Parametr;
	int16_t PWM;
	FunctionalState PWM_State;
	FunctionalState State;
	uint8_t Out_Index;
	FunctionalState INVERT;
	char Name[3];
} Regulates_INFO;



/* Exported variables --------------------------------------------------------*/
extern __IO Regulates_INFO Regulates_Struct[Regulates_Max_Count];
extern __IO Regulates_INFO Regul_Temp_Struct;
//extern __IO uint8_t RgulTempStruct_Index;

/* Exported function ---------------------------------------------------------*/
void Regulates_Init(void);
void Regulates_Default_Init(uint8_t FirstRegulNum, uint8_t LastRegulNum);
void One_Regulate_Default_Init(Regulates_INFO *pRegulate, uint8_t indx, Sensor_Typedef NewType);
void Regulate_CopyStruct(Regulates_INFO* Regul_ToCopy, Regulates_INFO* Regul_FromCopy);
void Init_Regulates_Buttons(void);
void VerifyRegulatesProgram(uint32_t TimeSeconds);
uint8_t Regulate_Get_SensorIndex(uint8_t RegulateIndex);



#endif /* __REGULATES___H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
