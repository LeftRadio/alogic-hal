/**
  ******************************************************************************
  * @file    HOST.h
  * @author  
  * @version V1.0.0
  * @date    
  * @brief   
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __P_PARAM_H
#define __P_PARAM_H

/* Includes ------------------------------------------------------------------*/
// #include "Timers.h"
#include "clock_calendar.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define Graphics_Hours_Varibles_Start	((uint16_t)(25000))
#define Hours_VarNum					((uint16_t)(244))

#define Graphics_Month_Varibles_Start	((uint16_t)(27000))
#define Month_VarNum					((uint16_t)(283))

/* Exported variables --------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Extern functions ----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Save_StartAplicationFlag(void);

void SaveTimerSettings(uint8_t TimerIndx);
ErrorStatus ReadTimerSettings(uint8_t TimerIndx);

void SaveRegulsSettings(uint8_t RegulIndx);
ErrorStatus ReadRegulsSettings(uint8_t RegulIndx);

void SaveOutputsSettings(void);
void ReadOutputsSettings(void);

void SaveBackLightSettings(uint8_t NewBackLight, uint32_t AutoOFF_BL);
void ReadBackLightSettings(void);

void Save_FAN_PWM_Settings(uint8_t FAN_Precent);
void Read_FAN_PWM_Settings(void);

void Save_Graphics_HoursPoint(void);
ErrorStatus Read_Graphics_HoursPoint(int8_t Day_Index, int8_t Sens_Index);

void Save_Graphics_MonthPoint(void);
ErrorStatus Read_Graphics_MonthPoint(int8_t Month_Index, int8_t Sens_Index);
ErrorStatus Read_All_HoursPoint(int8_t Day_Index, int8_t Sens_Index, uint8_t *OutData);

void Erase_Data_EEPROM(uint16_t StartAddr, uint16_t Num);
void Erase_Blocks_EEPROM(uint16_t StartBlock, uint16_t BlocksNum);

ErrorStatus Read_From_EEPROM_8_Host(uint16_t StartAddress, uint16_t DataNum, uint8_t *Data);
ErrorStatus Write_EEPROM_8_Host(uint16_t StartAddress, uint16_t DataNum, uint8_t *Data);

//uint8_t Find_Curent_Day(void);
ErrorStatus Find_Day_From_Index(uint8_t Index);

//uint8_t Find_Curent_Month(void);
ErrorStatus Find_Month_From_Index(uint8_t Index);

uint8_t Find_Last_Save_Index(RTC_DateTypeDef CurentDate, uint16_t Var_SaveStart, uint16_t VarNum, uint8_t MaxIndex, uint8_t Day_or_Month, volatile Bool *Point);


#endif  /*__P_PARAM_H*/
