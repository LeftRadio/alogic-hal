/**
  ******************************************************************************
  * @file    Timers.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief   header
  ******************************************************************************
**/

#ifndef __TIMERS___H
#define __TIMERS___H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported define -----------------------------------------------------------*/
#define Timer_OUT_NotAssigned			255
#define Timer_NormalMode				0
#define Timer_DailyMode					1

/* Exported typedef ----------------------------------------------------------*/
// Суточные параметры таймера
typedef struct
{
	int32_t ON_Times[5];
	FunctionalState TimeWork_Enable[5];
	int32_t Workings_Time;
} TimerDaily_Property;

// прoстые параметры таймера
typedef struct
{
	uint8_t Start_Day_Index[5];
	int32_t ON_Time[5];
	uint8_t Stop_Day_Index[5];
	int32_t OFF_Time[5];
	FunctionalState TimeWork_Enable[5];
} TimerNormal_Property;


/** Структурный тип данных таймера **/
typedef struct
{
	uint8_t Mode;
	TimerDaily_Property DailyProperty;
	TimerNormal_Property Normal_Property;
	FunctionalState State;
	uint8_t Out_Index;
	char Name[2];
} Timer_INFO;



/* Exported variables --------------------------------------------------------*/
extern volatile Timer_INFO Timers_Struct[15];
extern volatile Timer_INFO Timer_Temp_Struct;
extern volatile uint8_t Timer_ActiveIndx;

/* Exported function ---------------------------------------------------------*/
void Timers_Init(void);
void Timers_Default_Init(uint8_t FirstTimerNum, uint8_t LastTimerNum);
void One_Timer_Default_Init(Timer_INFO *pTimer, uint8_t NewOutIndx, uint8_t indx);
void Timer_CopyStruct_To_Timer(Timer_INFO* Timer_ToCopy, Timer_INFO* Timer_FromCopy);

void VerifyTimersProgram(uint32_t TimeSeconds);


#endif /* __TIMERS___H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
