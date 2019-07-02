/**
  ******************************************************************************
  * @file
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
//#include <string.h>

#include "Timers.h"
#include "User_Interface.h"
#include "clock_calendar.h"
#include "Outputs.h"
#include "Pparam.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//static FunctionalState DailyProgramState(uint32_t ON_Time, uint32_t OFF_Time, uint32_t TimeSeconds);

/* Private variables ---------------------------------------------------------*/
volatile Timer_INFO Timers_Struct[15];
volatile Timer_INFO Timer_Temp_Struct;

volatile uint8_t Timer_ActiveIndx = 0;



/* Functions ----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Timers_Default_Init
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Timers_Init(void)
{
	uint8_t i;

	for(i = 0; i < 15; i++)
	{
		if(ReadTimerSettings(i) == ERROR)
		{
			One_Timer_Default_Init((Timer_INFO*)&Timers_Struct[i], Timer_OUT_NotAssigned, i);
		}
	}
}



/*******************************************************************************
* Function Name  : Timers_Default_Init
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Timers_Default_Init(uint8_t FirstTimerNum, uint8_t LastTimerNum)
{
	uint8_t i;

	for(i = FirstTimerNum; i < LastTimerNum; i++)
	{
		One_Timer_Default_Init((Timer_INFO*)&Timers_Struct[i], Timer_OUT_NotAssigned, i + 1);
	}
}


/*******************************************************************************
* Function Name  : One_Timer_Default_Init
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void One_Timer_Default_Init(Timer_INFO *pTimer, uint8_t NewOutIndx, uint8_t indx)
{
	uint8_t i;

	pTimer->State = DISABLE;
	if(NewOutIndx < 20) pTimer->Out_Index = NewOutIndx; //Timer_OUT_NotAssigned;
	else pTimer->Out_Index = Timer_OUT_NotAssigned;

	pTimer->Mode = Timer_NormalMode;

	/* --------- свойства Normal --------- */
	for(i = 0; i < 5; i++)
	{
		pTimer->Normal_Property.Start_Day_Index[i] = 1;
//		pTimer->Normal_Property.Start_Month_Index[i] = 1;
		pTimer->Normal_Property.Stop_Day_Index[i] = 1;
//		pTimer->Normal_Property.Stop_Month_Index[i]  = 1;
		pTimer->Normal_Property.ON_Time[i] = 0;
		pTimer->Normal_Property.OFF_Time[i] = 0;
		pTimer->Normal_Property.TimeWork_Enable[i] = DISABLE;
	}

	ConvertToString(indx+1, &pTimer->Name[0], 2);

	/* --------- свойства Daily --------- */
	for(i = 0; i < 5; i++)
	{
		pTimer->DailyProperty.TimeWork_Enable[i] = DISABLE;
		pTimer->DailyProperty.ON_Times[i] = 0;
	}

	pTimer->DailyProperty.Workings_Time = 0;
}


/*******************************************************************************
* Function Name  : Timer_CopyStruct_To_Timer
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Timer_CopyStruct_To_Timer(Timer_INFO* Timer_ToCopy, Timer_INFO* Timer_FromCopy)
{
	uint8_t i;

	Timer_ToCopy->State = Timer_FromCopy->State;
	Timer_ToCopy->Out_Index = Timer_FromCopy->Out_Index;
	Timer_ToCopy->Mode = Timer_FromCopy->Mode;
	Timer_ToCopy->Name[0] = Timer_FromCopy->Name[0];
	Timer_ToCopy->Name[1] = Timer_FromCopy->Name[1];

	for(i = 0; i < 5; i++)
	{
		Timer_ToCopy->Normal_Property.Start_Day_Index[i] = Timer_FromCopy->Normal_Property.Start_Day_Index[i];
//		Timer_ToCopy->Normal_Property.Start_Month_Index[i] = Timer_FromCopy->Normal_Property.Start_Month_Index[i];
		Timer_ToCopy->Normal_Property.ON_Time[i] = Timer_FromCopy->Normal_Property.ON_Time[i];

		Timer_ToCopy->Normal_Property.Stop_Day_Index[i] = Timer_FromCopy->Normal_Property.Stop_Day_Index[i];
//		Timer_ToCopy->Normal_Property.Stop_Month_Index[i] = Timer_FromCopy->Normal_Property.Stop_Month_Index[i];
		Timer_ToCopy->Normal_Property.OFF_Time[i] = Timer_FromCopy->Normal_Property.OFF_Time[i];

		Timer_ToCopy->Normal_Property.TimeWork_Enable[i] = Timer_FromCopy->Normal_Property.TimeWork_Enable[i];
	}

	for(i = 0; i < 5; i++)
	{
		Timer_ToCopy->DailyProperty.ON_Times[i] = Timer_FromCopy->DailyProperty.ON_Times[i];
//		Timer_ToCopy->DailyProperty.OFF_Times[i] = Timer_FromCopy->DailyProperty.OFF_Times[i];
		Timer_ToCopy->DailyProperty.TimeWork_Enable[i] = Timer_FromCopy->DailyProperty.TimeWork_Enable[i];
	}

	Timer_ToCopy->DailyProperty.Workings_Time = Timer_FromCopy->DailyProperty.Workings_Time;

	if((Timer_ToCopy->State == DISABLE) && (Timer_ToCopy->Out_Index != Timer_OUT_NotAssigned))
	{
		OUT_OFF(Timer_ToCopy->Out_Index);
	}

}




/*******************************************************************************
* Function Name  : NormalProgramState
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static FunctionalState NormalProgramState(uint8_t ON_Day, uint8_t OFF_Day, uint32_t ON_Time, uint32_t OFF_Time, uint8_t Day, uint32_t Time)
{
	FunctionalState Status = DISABLE;

	if((ON_Day == OFF_Day) && (ON_Time == OFF_Time)) {;}
	else
	{

		// если стартовый день больше текущего то выключить
		if(Day < ON_Day)
		{
			Status = DISABLE;
		}

		// иначе если стартовый день равен текущему то
		else if(Day == ON_Day)
		{
			// если стоповый день равен стартовому то проверяем время выключения
			if((ON_Day == OFF_Day) && (Time > OFF_Time))
			{
				Status = DISABLE;
			}
			else if(Time >= ON_Time)
			{
				Status = ENABLE;
			}
			else
			{
				Status = DISABLE;
			}
		}

		// если стартовый день меньше текущего и текущий день не стоповый то включаем
		else if((Day > ON_Day) && (Day != OFF_Day))
		{
			Status = ENABLE;
		}

		// если стоповый день равен текущему то проверяем время отключения
		else if(Day == OFF_Day)
		{
			if(Time < OFF_Time)
			{
				Status = ENABLE;
			}
			else
			{
				Status = DISABLE;
			}
		}

		// если стоповый день меньше текущего то выключаем без вариантов
		else if(Day > OFF_Day)
		{
			Status = DISABLE;
		}
	}

	return Status;
}


/*******************************************************************************
* Function Name  : DailyProgramState
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static FunctionalState DailyProgramState(uint32_t ON_Time, uint32_t OFF_Time, uint32_t TimeSeconds)
{
	if(OFF_Time > 86400) OFF_Time -= 86400;

	// если стоповое время больше стартового (вкл. - 12:00, выкл. - 13:00)
	if(OFF_Time >= ON_Time)
	{
		if(OFF_Time <= TimeSeconds)	return DISABLE;			// если стоповое время меньше текущего то проверяем  еще и стартовое время
		else if(ON_Time <= TimeSeconds)	return ENABLE;		// иначе если стартовое время меньше текущего то включаем
		else return DISABLE;
	}

	// если стартовое время больше стопового; вкл. - 13:00, время работы - 18:00(выкл. - 7:00)
	else
	{
		if(ON_Time <= TimeSeconds) return ENABLE;
		else if((ON_Time > TimeSeconds) && (OFF_Time <= TimeSeconds)) return DISABLE;
		else if(OFF_Time >= TimeSeconds) return ENABLE;
		else return DISABLE;
	}
}



/*******************************************************************************
* Function Name  : VerifyTimersProgram
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void VerifyTimersProgram(uint32_t TimeSeconds)
{
	uint8_t i = 0;
	int8_t program = 4;
//	uint8_t Curent_WeekDay;

	FunctionalState Program_State = DISABLE;
	uint8_t Program_ON_Day, Program_OFF_Day;
	uint32_t Program_ON_Time, Program_OFF_Time;

	Calculate_Date();	// получаем текущую дату, месяц, год
//	Calculate_WeekDay(s_DateStructVar.Year, s_DateStructVar.Month, s_DateStructVar.Day);

	for(i = 0; i < 15; i++)
	{
		if(Timers_Struct[i].Out_Index != Timer_OUT_NotAssigned)
		{
			switch (Timers_Struct[i].Mode)
			{

			/* Обычный режим таймера --------------------------- */
			case Timer_NormalMode:		// режим простого таймера
			{
				// пробегаемся по всем программам
				for(program = 4; program >= 0; program--)
				{
					// если программа таймера включена то проверяем
					if(Timers_Struct[i].Normal_Property.TimeWork_Enable[program] == ENABLE)
					{
						Program_ON_Day = Timers_Struct[i].Normal_Property.Start_Day_Index[program];
						Program_OFF_Day = Timers_Struct[i].Normal_Property.Stop_Day_Index[program];
						Program_ON_Time = Timers_Struct[i].Normal_Property.ON_Time[program];
						Program_OFF_Time = Timers_Struct[i].Normal_Property.OFF_Time[program];

						if(NormalProgramState(Program_ON_Day, Program_OFF_Day, Program_ON_Time, Program_OFF_Time, s_DateStructVar.Day, TimeSeconds) == ENABLE)
						{
							Program_State = ENABLE;
						}
					}
				}

				if(Program_State == ENABLE) OUT_ON(Timers_Struct[i].Out_Index);
				else OUT_OFF(Timers_Struct[i].Out_Index);
			}
			break;

			/* Суточный режим таймера --------------------------- */
			case Timer_DailyMode:
			{
				FunctionalState Program_State = DISABLE;

				// пробегаемся по всем программам
				for(program = 4; program >= 0; program--)
				{
					if(Timers_Struct[i].DailyProperty.TimeWork_Enable[program] == ENABLE)
					{
						Program_ON_Time = Timers_Struct[i].DailyProperty.ON_Times[program];
						Program_OFF_Time = Program_ON_Time + Timers_Struct[i].DailyProperty.Workings_Time;

						if(DailyProgramState(Program_ON_Time, Program_OFF_Time, TimeSeconds) == ENABLE)
						{
							Program_State = ENABLE;
						}
					}
				}

				if(Program_State == ENABLE) OUT_ON(Timers_Struct[i].Out_Index);
				else OUT_OFF(Timers_Struct[i].Out_Index);
			}
			break;

			default: return;
			}
		}
	}
}





