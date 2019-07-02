/*************************************************************************************
*
Description :  AutoLogic Global_Init header
Version     :  1.0.0
Date        :
Author      :  Left Radio
Comments    :
**************************************************************************************/

#ifndef __GLOBAL_INIT_H
#define __GLOBAL_INIT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Exported typedef -----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern volatile Bool Save_FailBKP;

/* Exported function --------------------------------------------------------*/
HAL_StatusTypeDef Global_Init(void);
void ADC_Configuration(uint8_t TouchMode);
void TIM2_CC_Mode(uint8_t Mode);
void Delay_ms(uint32_t ms);
void Global_Periph_DeInit(void);

void Save_Fail_To_BKP_Registers(void);
void Reset_Fail_In_BKP_Registers(void);
Bool Read_Fail_From_BKP(char *FailDate_OutString, char *FailTime_OutString, Bool str_out);


#endif /* __GLOBAL_INIT_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
