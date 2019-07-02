/**
  ******************************************************************************
  * @file RTC/inc/clock_calendar.h 
  * @author  MCD Application Team
  * @version  V2.0.0
  * @date  04/27/2009
  * @brief  This files contains the Clock Calendar functions prototypes
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CLOCK_CALENDAR_H
#define __CLOCK_CALENDAR_H

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* Exported types ------------------------------------------------------------*/
/* Time Structure definition */
typedef struct
{
  uint8_t SecLow;
  uint8_t SecHigh;
  uint8_t MinLow;
  uint8_t MinHigh;
  uint8_t HourLow;
  uint8_t HourHigh;
} Time_s;

extern Time_s s_TimeStructVar;

/* Date Structure definition */
// typedef struct
// {
//   uint8_t Month;
//   uint8_t Day;
//   uint16_t Year;
// } Date_s ;

// extern Date_s s_DateStructVar;

extern char *Months_Names[];
extern char *WeekDays_Names[];
extern uint8_t s_WeekDay;


/* Exported constants --------------------------------------------------------*/
#define SECONDS_IN_DAY 86399
#define CONFIGURATION_DONE 0xAAAA
#define CONFIGURATION_RESET 0x0000
#define OCTOBER_FLAG_SET 0x4000
#define MARCH_FLAG_SET 0x8000
#define DEFAULT_DAY 20
#define DEFAULT_MONTH 12
#define DEFAULT_YEAR 2012
#define DEFAULT_HOURS 0
#define DEFAULT_MINUTES 0
#define DEFAULT_SECONDS 0
#define LEAP 1
#define NOT_LEAP 0

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void RTC_User_Init(void);

// установки времени и даты
void SetTime(uint8_t,uint8_t,uint8_t);
void SetDate(uint8_t,uint8_t,uint16_t);

// получение даты, времени и т.д.
// void Calculate_RTC_All(void);
// void Calculate_Time(void);
// void Calculate_Date(void);
// uint8_t Calculate_WeekDay(uint16_t tYear, uint8_t tMonth, uint8_t tDay);

// uint8_t CheckLeap(uint16_t);			// проверка високосного года
// void CheckForDaysElapsed(void);			// проверка количества дней простоя
// void SummerTimeCorrection(void);		// коррекция летнего/зимнего времени

// void DateUpdate(Date_s *s_Date, Bool SystemData);

/* Сравнение дат */
uint8_t Date_Find_Last(RTC_DateTypeDef Date[], uint8_t NumToCompare, uint8_t Day_or_Month);
int8_t Date_Compare(RTC_DateTypeDef FirstDate, RTC_DateTypeDef SecondDate, uint8_t Day_or_Month);

#endif /* __CLOCK_CALENDAR_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
