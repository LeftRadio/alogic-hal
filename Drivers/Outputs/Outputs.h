/**
  ******************************************************************************
  * @file    Timers.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief   header
  ******************************************************************************
**/

#ifndef __OUTPUTS___H
#define __OUTPUTS___H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported define -----------------------------------------------------------*/
typedef enum {
	Regul1 = 0, Regul2 = 1, Regul3 = 2, Regul4 = 3, Regul5 = 4,
	Regul6 = 5, Regul7 = 6, Regul8 = 7, Regul9 = 8,

	Timer1 = 9, Timer2 = 10, Timer3 = 11, Timer4 = 12, Timer5 = 13,
	Timer6 = 14, Timer7 = 15, Timer8 = 16, Timer9 = 17, Timer10 = 18,
	Timer11 = 19, Timer12 = 20, Timer13 = 21, Timer14 = 22, Timer15 = 23, NotAssigned = 255
} Timer_Regul_Num;


//#define
//#define

/* Exported typedef ----------------------------------------------------------*/
typedef struct
{
	GPIO_TypeDef* OUT_Port;
	uint16_t GPIO_PIN;
} Out_TypeDef;


/** Структурный тип данных выходов **/
typedef struct
{
	Out_TypeDef Out;
	char FunctionText[10];
	Timer_Regul_Num Num;
	OutStatus ManualState;
} OutStruct_Typedef;



/* Exported variables --------------------------------------------------------*/
extern OutStruct_Typedef StructOuts[20];
extern volatile FunctionalState Outs_ENABLE[20];

/* Exported function ---------------------------------------------------------*/
void Outputs_Init(void);

void OUT_ON(uint8_t indxOUT);
void OUT_PWM_ON(uint8_t indxOUT);

void OUT_OFF(uint8_t indxOUT);
void OUT_PWM_OFF(uint8_t indxOUT);

void OUT_NewState(uint8_t indxOUT, BitAction NewState);
BitAction OUT_SateRead(uint8_t indxOUT);

uint8_t DeAssigned_OUT_FromTimer(uint8_t OUT_Indx);
uint8_t Find_OUT_OnTimer(uint8_t OUT_Indx);

uint8_t DeAssigned_OUT_FromRegulate(uint8_t OUT_Indx);
uint8_t Find_OUT_OnRegulate(uint8_t OUT_Indx);

void Update_ONE_OutButtonState(uint8_t Indx);
void Control_Outs_From_Internal_Temperature(ErrorStatus Status, uint8_t Data, uint8_t Group);

#endif /* __OUTPUTS___H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
