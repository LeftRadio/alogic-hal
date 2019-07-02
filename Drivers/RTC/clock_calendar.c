/**
  ******************************************************************************
  * @file RTC/src/clock_calendar.c 
  * @author  MCD Application Team
  * @version  V2.0.0
  * @date  04/27/2009
  * @brief  Clock Calendar basic routines
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


/* Includes ------------------------------------------------------------------*/
#include "clock_calendar.h"

/* Private variables--------------------------------------------------------- */
char *Months_Names[]={ "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", \
"Сентябрь", "Октябрь", "Ноябрь", "Декабрь" };

char *WeekDays_Names[]={ "Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Субота" };


/*Structure variable declaration for system time, system date,
alarm time, alarm date */

uint8_t s_WeekDay = 0;

uint16_t SummerTimeCorrect;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  _save_date_to_backup
  * @param  None
  * @retval : None
  */
static void _save_date_to_backup(RTC_DateTypeDef* date) {
    HAL_PWR_EnableBkUpAccess();
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, date->Month);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, date->Date);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, date->Year);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, CONFIGURATION_DONE);
    HAL_PWR_DisableBkUpAccess();
}

/**
  * @brief  Check whether the passed year is Leap or not.
  * @param  nYear  year to check
  * @retval 1: leap year
  *         0: not leap year
  */
static uint8_t RTC_IsLeapYear(uint16_t year) {
    if((year % 4U) != 0U) {
        return 0U;
    }  
    if((year % 100U) != 0U) {
        return 1U;
    }  
    if((year % 400U) == 0U) {
        return 1U;
    }
    else {
        return 0U;
    }
}


/**
  * @brief  COnfiguration of RTC Registers, Selection and Enabling of 
  *   RTC clock
  * @param  None
  * @retval : None
  */
void RTC_Configuration(void) {
  	// RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    /* */
  	if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) == CONFIGURATION_RESET)	{
        /* */
        HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BCD);  			
        _save_date_to_backup(&date);	
  	}	

  	/* Check if how many days are elapsed in power down/Low Power Mode-
     	   Updates Date that many Times*/
  	// CheckForDaysElapsed();

  	date.Month = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);
  	date.Date = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3);
  	date.Year = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4);
  	SummerTimeCorrect = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR7);

    HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BCD);
}

/**
  * @brief  Sets the RTC Current Counter Value
  * @param Hour, Minute and Seconds data
  * @retval : None
  */
void SetTime(uint8_t Hour, uint8_t Minute, uint8_t Seconds) {
    /* */
    RTC_TimeTypeDef time = {.Hours = Hour, .Minutes = Minute, .Seconds = Seconds};
    HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BCD);
}

/**
  * @brief  Sets the RTC Date(DD/MM/YYYY)
  * @param DD,MM,YYYY
  * @retval : None
  */
void SetDate(uint8_t Day, uint8_t Month, uint16_t Year) {
  RTC_DateTypeDef date;

  /*Check if the date entered by the user is correct or not, Displays an error
    message if date is incorrect  */
  if((( Month==4 || Month==6 || Month==9 || Month==11) && Day ==31) || (Month==2 && Day==31)|| (Month==2 && Day==30)|| (Month==2 && Day==29 && (RTC_IsLeapYear(Year)==0))) {
      /* */
      HAL_Delay(1000);
      HAL_Delay(1000);
  }
  /* if date entered is correct then set the date */
  else {
	  /* */
    HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BCD);
    _save_date_to_backup(&date);
  }
}

/**
  * @brief  Date_Find_Last
  * @param  None
  * @retval None
  */
uint8_t Date_Find_Last(RTC_DateTypeDef Date[], uint8_t NumToCompare, uint8_t Day_or_Month) {
  	RTC_DateTypeDef tData = {0, 0, 0};
  	RTC_DateTypeDef *BigDate = &tData;
  	uint8_t j = 255;
    /* */
  	for(uint8_t i = 0; i < NumToCompare; i++) {
    		if(Date_Compare(Date[i], (*BigDate), Day_or_Month) == 1) {
      			BigDate = &Date[i];
      			j = i;
    		}
  	}
  	return j;
}

/**
  * @brief  Date_Find_Last
  * @param  None
  * @retval None
  */
int8_t Date_Compare(RTC_DateTypeDef FirstDate, RTC_DateTypeDef SecondDate, uint8_t Day_or_Month) {
  	/* */
    if(FirstDate.Year > SecondDate.Year) {
        return 1;
    }
  	else if(FirstDate.Year < SecondDate.Year) {
        return -1;
    }
  	else {
    		if(FirstDate.Month > SecondDate.Month) {
            return 1;
        }
    		else if(FirstDate.Month < SecondDate.Month) {
            return -1;
        }
    		else {
      			if(Day_or_Month == 1) {
                return 0;
            }
      			else {
        				if(FirstDate.Date > SecondDate.Date) {
                    return 1;
                }
        				else if(FirstDate.Date < SecondDate.Date) {
                    return -1;
                }
        				else {
        					return 0;
        				}
      			}
    		}
  	}
}

/*******************************************************************************
      END FILE
*******************************************************************************/