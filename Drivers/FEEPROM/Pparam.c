/**
   ******************************************************************************
   * @file      Param.c
   * @author	LeftRadio
   * @version 	V1.0.0
   * @date      14.04.2013 20:27:59
   * @brief
   ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "stm32f1xx_hal.h"
#include "Pparam.h"
#include "f_eeprom.h"
#include "I2CRoutines.h"
#include "GlobalInit.h"
#include "Outputs.h"
#include "Regulates.h"
#include "LCD.h"
#include "clock_calendar.h"
#include "STLM75.h"
#include "DHT11.h"
#include "Graphics.h"
#include "User_Interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define Timers_Varibles_Start			((uint16_t)0)
#define OneTimer_VariblesNum			((uint16_t)100)			// 99 переменных плюс контрольный байт
#define TimerProgVar					((uint16_t)18)

#define Reguls_Varibles_Start			((uint16_t)(OneTimer_VariblesNum * 15))
#define OneRegul_VariblesNum			((uint16_t)17)			// 15 переменных плюс контрольный байт

#define Outputs_Varibles_Start			((uint16_t)(Reguls_Varibles_Start + (OneRegul_VariblesNum * 9)))
#define OneOut_VariblesNum				((uint16_t)61)			// 60 переменных плюс контрольный байт

#define BackLight_Varibles_Start		((uint16_t)(Outputs_Varibles_Start + (OneOut_VariblesNum * 20)))
#define BackLight_VariblesNum			((uint16_t)6)			// 5 переменных плюс контрольный байт

#define FAN_PWM_Varibles_Start			((uint16_t)(BackLight_Varibles_Start + (BackLight_VariblesNum)))
#define FAN_PWM_VariblesNum				((uint16_t)2)			// 1 переменных плюс контрольный байт

#define EEPROM_ADDR						0xA0

#define I2C_RD_HAL_Delay					25
#define I2C_WR_HAL_Delay					25


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile Bool Date_Label_Change = FALSE;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//static void Save_To_FEEPROM_16_CS(uint16_t StartAddress, uint16_t DataNum, uint16_t *Data);
//static ErrorStatus Read_From_FEEPROM_16(uint16_t StartAddress, uint16_t DataNum, uint16_t *Data);

static void Save_To_FEEPROM_8_CS(uint16_t StartAddress, uint16_t DataNum, uint8_t *Data);
static ErrorStatus Read_From_FEEPROM_8_CS(uint16_t StartAddress, uint16_t DataNum, uint8_t *Data);
static void Save_To_EEPROM_8(uint16_t StartAddress, uint8_t DataNum, uint8_t *Data);
//static ErrorStatus Read_From_EEPROM_8(uint16_t StartAddress, uint8_t DataNum, uint8_t *Data);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : Save_StartAplicationFlag
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void Save_StartAplicationFlag(void)
{
	__disable_irq();
	HAL_FLASH_Unlock();

	EE_WriteVariable(8, 0xA5FD);
	EE_WriteVariable(9, 0xDF5A);

	HAL_FLASH_Lock();
	__enable_irq();
}




/*--------------------------------------- Запись и чтение параметров таймеров ---------------------------------------*/

/*******************************************************************************
 * Function Name  : SaveTimerSettings
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void SaveTimerSettings(uint8_t TimerIndx)
{
	uint8_t i;
	uint16_t sw = Timers_Varibles_Start + (TimerIndx * OneTimer_VariblesNum);
	uint8_t TimerSaveData[OneTimer_VariblesNum];

	/* Пять программ */
	for(i = 0; i < 5; i++)
	{
		/* Суточные параметры */
		TimerSaveData[(i*TimerProgVar) + 0] = (uint8_t)(Timers_Struct[TimerIndx].DailyProperty.TimeWork_Enable[i]);

		TimerSaveData[(i*TimerProgVar) + 1] = (uint8_t)((Timers_Struct[TimerIndx].DailyProperty.ON_Times[i] & 0xFF000000) >> 24);
		TimerSaveData[(i*TimerProgVar) + 2] = (uint8_t)((Timers_Struct[TimerIndx].DailyProperty.ON_Times[i] & 0x00FF0000) >> 16);
		TimerSaveData[(i*TimerProgVar) + 3] = (uint8_t)((Timers_Struct[TimerIndx].DailyProperty.ON_Times[i] & 0x0000FF00) >> 8);
		TimerSaveData[(i*TimerProgVar) + 4] = (uint8_t)(Timers_Struct[TimerIndx].DailyProperty.ON_Times[i] & 0x000000FF);


		/* Параметры обычного таймера */
		TimerSaveData[(i*TimerProgVar) + 5] = Timers_Struct[TimerIndx].Normal_Property.Start_Day_Index[i];
//		TimerSaveData[(i*TimerProgVar) + 6] = Timers_Struct[TimerIndx].Normal_Property.Start_Month_Index[i];

		TimerSaveData[(i*TimerProgVar) + 7] = Timers_Struct[TimerIndx].Normal_Property.Stop_Day_Index[i];
//		TimerSaveData[(i*TimerProgVar) + 8] = Timers_Struct[TimerIndx].Normal_Property.Stop_Month_Index[i];

		TimerSaveData[(i*TimerProgVar) + 9] = (uint8_t)((Timers_Struct[TimerIndx].Normal_Property.ON_Time[i] & 0xFF000000) >> 24);
		TimerSaveData[(i*TimerProgVar) + 10] = (uint8_t)((Timers_Struct[TimerIndx].Normal_Property.ON_Time[i] & 0x00FF0000) >> 16);
		TimerSaveData[(i*TimerProgVar) + 11] = (uint8_t)((Timers_Struct[TimerIndx].Normal_Property.ON_Time[i] & 0x0000FF00) >> 8);
		TimerSaveData[(i*TimerProgVar) + 12] = (uint8_t)(Timers_Struct[TimerIndx].Normal_Property.ON_Time[i] & 0x000000FF);

		TimerSaveData[(i*TimerProgVar) + 13] = (uint8_t)((Timers_Struct[TimerIndx].Normal_Property.OFF_Time[i] & 0xFF000000) >> 24);
		TimerSaveData[(i*TimerProgVar) + 14] = (uint8_t)((Timers_Struct[TimerIndx].Normal_Property.OFF_Time[i] & 0x00FF0000) >> 16);
		TimerSaveData[(i*TimerProgVar) + 15] = (uint8_t)((Timers_Struct[TimerIndx].Normal_Property.OFF_Time[i] & 0x0000FF00) >> 8);
		TimerSaveData[(i*TimerProgVar) + 16] = (uint8_t)(Timers_Struct[TimerIndx].Normal_Property.OFF_Time[i] & 0x000000FF);

		TimerSaveData[(i*TimerProgVar) + 17] = (uint8_t)Timers_Struct[TimerIndx].Normal_Property.TimeWork_Enable[i];
	}

	TimerSaveData[90] = (uint8_t)((Timers_Struct[TimerIndx].DailyProperty.Workings_Time & 0xFF000000) >> 24);
	TimerSaveData[91] = (uint8_t)((Timers_Struct[TimerIndx].DailyProperty.Workings_Time & 0x00FF0000) >> 16);
	TimerSaveData[92] = (uint8_t)((Timers_Struct[TimerIndx].DailyProperty.Workings_Time & 0x0000FF00) >> 8);
	TimerSaveData[93] = (uint8_t)(Timers_Struct[TimerIndx].DailyProperty.Workings_Time & 0x000000FF);

	TimerSaveData[94] = Timers_Struct[TimerIndx].Mode;
	TimerSaveData[95] = (uint8_t)Timers_Struct[TimerIndx].State;
	TimerSaveData[96] = Timers_Struct[TimerIndx].Out_Index;
	TimerSaveData[97] = (uint8_t)Timers_Struct[TimerIndx].Name[0];
	TimerSaveData[98] = (uint8_t)Timers_Struct[TimerIndx].Name[1];

	Save_To_FEEPROM_8_CS(sw, OneTimer_VariblesNum-1, TimerSaveData);
}


/*******************************************************************************
 * Function Name  : ReadTimerSettings
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
ErrorStatus ReadTimerSettings(uint8_t TimerIndx)
{
	uint8_t i;
	uint16_t sw = Timers_Varibles_Start + (TimerIndx * OneTimer_VariblesNum);
	uint8_t TimerReadData[OneTimer_VariblesNum];

	ErrorStatus ReadStatus = Read_From_FEEPROM_8_CS(sw, OneTimer_VariblesNum-1, TimerReadData);

	if(ReadStatus == SUCCESS)
	{
		for(i = 0; i < 5; i++)
		{
			/* Суточные параметры */
			Timers_Struct[TimerIndx].DailyProperty.TimeWork_Enable[i] = TimerReadData[(i*TimerProgVar) + 0];

			Timers_Struct[TimerIndx].DailyProperty.ON_Times[i] = ((uint32_t)(TimerReadData[(i*TimerProgVar) + 1]) << 24) | \
					((uint32_t)(TimerReadData[(i*TimerProgVar) + 2]) << 16) | \
					((uint32_t)(TimerReadData[(i*TimerProgVar) + 3]) << 8) | \
					((uint32_t)(TimerReadData[(i*TimerProgVar) + 4]));


			/* Параметры обычного таймера */
			Timers_Struct[TimerIndx].Normal_Property.Start_Day_Index[i] = TimerReadData[(i*TimerProgVar) + 5];
//			Timers_Struct[TimerIndx].Normal_Property.Start_Month_Index[i] = TimerReadData[(i*TimerProgVar) + 6];

			Timers_Struct[TimerIndx].Normal_Property.Stop_Day_Index[i] = TimerReadData[(i*TimerProgVar) + 7];
//			Timers_Struct[TimerIndx].Normal_Property.Stop_Month_Index[i] = TimerReadData[(i*TimerProgVar) + 8];

			Timers_Struct[TimerIndx].Normal_Property.ON_Time[i] = ((uint32_t)(TimerReadData[(i*TimerProgVar) + 9]) << 24) | \
					((uint32_t)(TimerReadData[(i*TimerProgVar) + 10]) << 16) | \
					((uint32_t)(TimerReadData[(i*TimerProgVar) + 11]) << 8) | \
					((uint32_t)(TimerReadData[(i*TimerProgVar) + 12]));

			Timers_Struct[TimerIndx].Normal_Property.OFF_Time[i] = ((uint32_t)(TimerReadData[(i*TimerProgVar) + 13]) << 24) | \
					((uint32_t)(TimerReadData[(i*TimerProgVar) + 14]) << 16) | \
					((uint32_t)(TimerReadData[(i*TimerProgVar) + 15]) << 8) | \
					((uint32_t)(TimerReadData[(i*TimerProgVar) + 16]));

			Timers_Struct[TimerIndx].Normal_Property.TimeWork_Enable[i] = TimerReadData[(i*TimerProgVar) + 17];
		}


		Timers_Struct[TimerIndx].DailyProperty.Workings_Time = ((uint32_t)TimerReadData[90] << 24) | \
				((uint32_t)TimerReadData[91] << 16) | \
				((uint32_t)TimerReadData[92] << 8) | \
				((uint32_t)TimerReadData[93]);

		Timers_Struct[TimerIndx].Mode = TimerReadData[94];
		Timers_Struct[TimerIndx].State = TimerReadData[95];
		Timers_Struct[TimerIndx].Out_Index = 255; //TimerReadData[54];

		Timers_Struct[TimerIndx].Name[0] = TimerReadData[97];
		Timers_Struct[TimerIndx].Name[1] = TimerReadData[98];
	}

	return ReadStatus;
}




/*******************************************************************************
 * Function Name  : ReadTimerSettings_ToHost
 * Description    : Read all timer setting from EEPROM and return
 * Input          :
 * Return         : None
 *******************************************************************************/
ErrorStatus Read_Block_ToHost(uint16_t StartBlockIndex, uint8_t* ReadedData)
{
	ErrorStatus ReadStatus;

	ReadStatus = Read_From_EEPROM_8_Host(StartBlockIndex * 64, 64, ReadedData);

	return ReadStatus;
}


/*******************************************************************************
 * Function Name  : ReadTimerSettings_ToHost
 * Description    : Read all timer setting from EEPROM and return
 * Input          :
 * Return         : None
 *******************************************************************************/
ErrorStatus Write_Block_FromHost(uint16_t StartBlockIndex, uint8_t* SaveData)
{
	ErrorStatus ReadStatus;

	ReadStatus = Write_EEPROM_8_Host(StartBlockIndex * 64, 64, SaveData);

	return ReadStatus;
}




/*--------------------------------------- Запись и чтение параметров регулировок ---------------------------------------*/

/*******************************************************************************
 * Function Name  : SaveRegulsSettings
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void SaveRegulsSettings(uint8_t RegulIndx)
{
	uint16_t sw = Reguls_Varibles_Start + (RegulIndx * OneRegul_VariblesNum);
	uint8_t RegulSaveData[OneRegul_VariblesNum];

	/* параметры */
	RegulSaveData[0] = Regulates_Struct[RegulIndx].SensorType;
	RegulSaveData[1] = Regulates_Struct[RegulIndx].SensorIndex;
	RegulSaveData[2] = Regulates_Struct[RegulIndx].RegulType;
	RegulSaveData[3] = Regulates_Struct[RegulIndx].Thresold;
	RegulSaveData[4] = Regulates_Struct[RegulIndx].Thresold_2;
	RegulSaveData[5] = Regulates_Struct[RegulIndx].Hysteresis;
	RegulSaveData[6] = Regulates_Struct[RegulIndx].PWM_ON_Parametr;
	RegulSaveData[7] = (uint8_t)Regulates_Struct[RegulIndx].PWM_State;
	RegulSaveData[8] = Regulates_Struct[RegulIndx].PWM;
	RegulSaveData[9] = (uint8_t)Regulates_Struct[RegulIndx].State;
	RegulSaveData[10] = Regulates_Struct[RegulIndx].Out_Index;
	RegulSaveData[11] = Regulates_Struct[RegulIndx].Name[0];
	RegulSaveData[12] = Regulates_Struct[RegulIndx].Name[1];
	RegulSaveData[13] = (uint8_t)Regulates_Struct[RegulIndx].INVERT;
	RegulSaveData[14] = Regulates_Struct[RegulIndx].MIN_SensorIndex;
	RegulSaveData[15] =  Regulates_Struct[RegulIndx].MAX_SensorIndex;

	Save_To_FEEPROM_8_CS(sw, OneRegul_VariblesNum - 1, RegulSaveData);
}


/*******************************************************************************
 * Function Name  : ReadRegulsSettings
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
ErrorStatus ReadRegulsSettings(uint8_t RegulIndx)
{
	uint16_t sw = Reguls_Varibles_Start + (RegulIndx * OneRegul_VariblesNum);
	uint8_t RegulReadData[OneRegul_VariblesNum];

	ErrorStatus ReadStatus = Read_From_FEEPROM_8_CS(sw, OneRegul_VariblesNum - 1, RegulReadData);

	if(ReadStatus == SUCCESS)
	{
		/* параметры */
		Regulates_Struct[RegulIndx].SensorType = RegulReadData[0];
		Regulates_Struct[RegulIndx].SensorIndex = RegulReadData[1];
		Regulates_Struct[RegulIndx].RegulType = RegulReadData[2];
		Regulates_Struct[RegulIndx].Thresold = RegulReadData[3];
		Regulates_Struct[RegulIndx].Thresold_2 = RegulReadData[4];
		Regulates_Struct[RegulIndx].Hysteresis = RegulReadData[5];
		Regulates_Struct[RegulIndx].PWM_ON_Parametr = RegulReadData[6];
		Regulates_Struct[RegulIndx].PWM_State = RegulReadData[7];
		Regulates_Struct[RegulIndx].PWM = RegulReadData[8];
		Regulates_Struct[RegulIndx].State = RegulReadData[9];
		Regulates_Struct[RegulIndx].Out_Index = 255; //(uint8_t)(RegulReadData[4] & 0x00FF);
		Regulates_Struct[RegulIndx].Name[0] = RegulReadData[11];
		Regulates_Struct[RegulIndx].Name[1] = RegulReadData[12];
		Regulates_Struct[RegulIndx].INVERT = RegulReadData[13];
		Regulates_Struct[RegulIndx].MIN_SensorIndex = RegulReadData[14];
		Regulates_Struct[RegulIndx].MAX_SensorIndex = RegulReadData[15];
	}

	return ReadStatus;
}



/*--------------------------------------- Запись и чтение параметров выходов ---------------------------------------*/

/*******************************************************************************
 * Function Name  : SaveOutputsSettings
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void SaveOutputsSettings(void)
{
	uint8_t j;
	uint16_t sw = Outputs_Varibles_Start, Num;
	uint8_t OutputSaveData[OneOut_VariblesNum];

	for(j = 0; j < 20; j++)
	{
		// вычисляем не назначен ли выход на таймер или регулировку
		Num = Find_OUT_OnTimer(j);
		if(Num != 255){ OutputSaveData[(j*3) + 0] = 0; OutputSaveData[(j*3) + 1] = Num;}
		else
		{
			Num = Find_OUT_OnRegulate(j);
			if(Num != 255){ OutputSaveData[(j*3) + 0] = 1; OutputSaveData[(j*3) + 1] = Num; }
			else
			{
				OutputSaveData[(j*3) + 0] = 0xFF;
				OutputSaveData[(j*3) + 1] = (uint8_t)OUT_SateRead(j);
			}
		}

		OutputSaveData[(j*3) + 2] = (uint8_t)StructOuts[j].Num;
	}

	// сохраняем
	Save_To_FEEPROM_8_CS(sw, OneOut_VariblesNum - 1, OutputSaveData);
}


/*******************************************************************************
 * Function Name  : ReadOutputsSettings
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void ReadOutputsSettings(void)
{
	uint8_t j, Type, State, Indx;
	uint16_t sw = Outputs_Varibles_Start;
	uint8_t OutputReadData[OneOut_VariblesNum];

	if(Read_From_FEEPROM_8_CS(sw, OneOut_VariblesNum - 1, &OutputReadData[0]) == SUCCESS)
	{
		for(j = 0; j < 20; j++)
		{
			// вычисляем не назначен ли выход на таймер или регулировку
			if(OutputReadData[(j*3) + 0] != 255)
			{
				Type = OutputReadData[(j*3) + 0];
				Indx = OutputReadData[(j*3) + 1];

				if(Type == 0)
				{
					Timers_Struct[Indx].Out_Index = j;			// Таймер
					// назначаем текст кнопке и конвертируем номер таймера в текст и назначаем выходу новый номер таймера
					memcpy(&StructOuts[j].FunctionText[0], "тайм. ", 6);
					ConvertToString((Indx + 1), &StructOuts[j].FunctionText[6], 2);
				}
				else if(Type == 1)
				{
					Regulates_Struct[Indx].Out_Index = j;	// Регулировка
					memcpy(&StructOuts[j].FunctionText[0], "регул. ", 7);
					ConvertToString((Indx + 1), &StructOuts[j].FunctionText[7], 2);
				}

				// иначе
				else
				{
					Timers_Struct[Indx].Out_Index = 255;
					Regulates_Struct[Indx].Out_Index = 255;
				}
			}
			else
			{
				State = OutputReadData[(j*3) + 1];
				OUT_NewState(j, (BitAction)State);
			}

			StructOuts[j].Num = OutputReadData[(j*3) + 2];
		}
	}
}



/*--------------------------------------- Запись и чтение параметров подсведки ---------------------------------------*/

/*******************************************************************************
 * Function Name  : SaveOutputsSettings
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void SaveBackLightSettings(uint8_t NewBackLight, uint32_t AutoOFF_BL)
{
	uint8_t OutputSaveData[BackLight_VariblesNum];

	OutputSaveData[0] = NewBackLight;
	OutputSaveData[1] = (uint8_t)(AutoOFF_BL >> 24);
	OutputSaveData[2] = (uint8_t)((AutoOFF_BL & 0x00FF0000) >> 16);
	OutputSaveData[3] = (uint8_t)((AutoOFF_BL & 0x0000FF00) >> 8);
	OutputSaveData[4] = (uint8_t)((AutoOFF_BL & 0x000000FF));

	// сохраняем
	Save_To_FEEPROM_8_CS(BackLight_Varibles_Start, BackLight_VariblesNum - 1, OutputSaveData);

}

/*******************************************************************************
 * Function Name  : ReadOutputsSettings
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void ReadBackLightSettings(void)
{
	uint8_t OutputReadData[BackLight_VariblesNum];

	if(Read_From_FEEPROM_8_CS(BackLight_Varibles_Start, BackLight_VariblesNum - 1, OutputReadData) == SUCCESS)
	{
		BL_Precent = OutputReadData[0];
		LCD_Set_BackLightPWM(BL_Precent);

		if(((OutputReadData[1] | OutputReadData[2]) || (OutputReadData[3] | OutputReadData[4])) != 0)
		{
			BL_AutoOFF_Status = TRUE;
			BL_AutoOff_Seconds = ((uint32_t)OutputReadData[1] << 24) | \
					((uint32_t)OutputReadData[2] << 16) | \
					((uint32_t)OutputReadData[3] << 8) | \
					((uint32_t)OutputReadData[4]);

			LCD_Verify_BL_AutoOFF(RTC_GetCounter(), SET);
		}
		else
		{
			BL_AutoOff_Seconds = 0;
			BL_AutoOFF_Status = FALSE;
		}
	}
}



/*******************************************************************************
 * Function Name  : SaveOutputsSettings
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void Save_FAN_PWM_Settings(uint8_t New_FAN_PWM)
{
	uint8_t OutputSaveData[FAN_PWM_VariblesNum];

	OutputSaveData[0] = New_FAN_PWM;

	// сохраняем
	Save_To_FEEPROM_8_CS(FAN_PWM_Varibles_Start, FAN_PWM_VariblesNum - 1, OutputSaveData);

}

/*******************************************************************************
 * Function Name  : ReadOutputsSettings
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void Read_FAN_PWM_Settings(void)
{
	uint8_t OutputReadData[FAN_PWM_VariblesNum];

	if(Read_From_FEEPROM_8_CS(FAN_PWM_Varibles_Start, FAN_PWM_VariblesNum - 1, OutputReadData) == SUCCESS)
	{
		FAN_Precent = OutputReadData[0];

		uint32_t PWM_Val = (TIM3->ARR * FAN_Precent) / 100;
		TIM3->CCR2 = (uint16_t)PWM_Val;	
	}
}




/*--------------------------------------- Запись и чтение графиков. часовые точки ---------------------------------------*/

/*******************************************************************************
 * Function Name  : Find_Day_From_Index
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
ErrorStatus Find_Day_From_Index(uint8_t Index)
{
	uint8_t OutputReadData[5];

	return Read_From_FEEPROM_8_CS(Graphics_Hours_Varibles_Start + (Index * Hours_VarNum), 3, &OutputReadData[0]);
}


/*******************************************************************************
 * Function Name  : Find_Month_From_Index
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
ErrorStatus Find_Month_From_Index(uint8_t Index)
{
	uint8_t OutputReadData[5];

	return Read_From_FEEPROM_8_CS(Graphics_Month_Varibles_Start + (Index * Month_VarNum), 3, &OutputReadData[0]);
}



/*******************************************************************************
 * Function Name  : Save_Graphics_HoursPoint
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void Save_Graphics_HoursPoint(void)
{
	uint8_t OutputSaveData[24];

	uint16_t SaveAddress = 0, SaveTempDataAddress = 0;
	uint8_t i = 0;
	uint32_t Curent_Hour_Counter = (RTC_GetCounter() / 3600);

	Date_Label_Change = FALSE;

	/* Ищем индекс для записи */
	Graphics_Save_DayIndex = Find_Last_Save_Index(s_DateStructVar, Graphics_Hours_Varibles_Start, Hours_VarNum, 7, _Day_, &Date_Label_Change);
	if(Date_Label_Change == FALSE)
	{
		Graphics_Save_DayIndex++;
		if(Graphics_Save_DayIndex >= 7) Graphics_Save_DayIndex = 0;
	}

	// вычисляем стартовый адресс записи начала метки
	SaveAddress = Graphics_Hours_Varibles_Start + (Graphics_Save_DayIndex * Hours_VarNum);
	SaveTempDataAddress = SaveAddress + 4;

	/* Если не нашли меток совпадающих с текущей датой, то сохраняем новую метку */
	if(Date_Label_Change == FALSE)
	{
		// сохраняем метку даты
		OutputSaveData[0] = s_DateStructVar.Day;
		OutputSaveData[1] = s_DateStructVar.Month;
		OutputSaveData[2] = s_DateStructVar.Year - 2000;
		Save_To_FEEPROM_8_CS(SaveAddress, 3, OutputSaveData);

		Erase_Data_EEPROM(SaveTempDataAddress, Hours_VarNum - 4);

		/* очистим старые данные если в меню графиков */
		if((pMenu == &GraphicsMenu) && (Graphics_Mode == Week)) Clear_Graphics(Graphics_Print_DayIndex, 255);
	}

	/* Сохраняем все датчики температуры */
	for(i = 0; i < 8; i++)
	{
		if(TempSensor[i].Status == ERROR) OutputSaveData[0] = 255;
		else OutputSaveData[0] = SensGraphics[i].WeekData[Curent_Hour_Counter];

		Save_To_EEPROM_8(SaveTempDataAddress + (24 * i) + Curent_Hour_Counter, 1, OutputSaveData);
	}

	/* Сохраняем датчик влжности */
	if(DHTSensor.Status == ERROR) OutputSaveData[0] = 255;
	else OutputSaveData[0] = SensGraphics[8].WeekData[Curent_Hour_Counter];
	Save_To_EEPROM_8(SaveTempDataAddress + (24 * 8) + Curent_Hour_Counter, 1, OutputSaveData);

	if(SHTSensor.Status == ERROR) OutputSaveData[0] = 255;
	else OutputSaveData[0] = SensGraphics[9].WeekData[Curent_Hour_Counter];
	Save_To_EEPROM_8(SaveTempDataAddress + (24 * 9) + Curent_Hour_Counter, 1, OutputSaveData);


	if(Date_Label_Change == FALSE)
	{
		/* перекючаем отображаемый день */
		Graphics_Print_DayIndex++;
		if(Graphics_Print_DayIndex >= 7) Graphics_Print_DayIndex = 0;
	}

	/* Обновляем график если находимся в меню графиков */
	if((pMenu == &GraphicsMenu)	&& (Graphics_Mode == Week))
	{
		Draw_Graphics(Graphics_Print_DayIndex);
	}
}


/*******************************************************************************
 * Function Name  : ReadGraphicsHoursPoint
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
ErrorStatus Read_Graphics_HoursPoint(int8_t Day_Index, int8_t Sens_Index)
{
	uint16_t StartAddress = 0;
	uint8_t i = 0;
	uint8_t OutputReadData[5] = {0};

	if((Day_Index < 0) || (Day_Index > 8)) return ERROR;

	if(Read_From_FEEPROM_8_CS(Graphics_Hours_Varibles_Start + (Day_Index * Hours_VarNum), 3, &OutputReadData[0]) == SUCCESS)
	{
		Graphic_PrintData.Day = OutputReadData[0];
		Graphic_PrintData.Month = OutputReadData[1];
		Graphic_PrintData.Year = OutputReadData[2] + 2000;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
		I2C_Cmd(I2C2, DISABLE);
		HAL_Delay(I2C_WR_HAL_Delay);

		// стартовый адресс для чтения по датчикам, начальный адресс плюс 4(пропустить метку)
		StartAddress = Graphics_Hours_Varibles_Start + (Day_Index * Hours_VarNum) + (Sens_Index * 24) + 4;

		// пишем адрес старта данных для чтения
		Buffer_Tx1[0] = (uint8_t)(StartAddress >> 8);
		Buffer_Tx1[1] = (uint8_t)(StartAddress & 0x00FF);
		I2C_Master_BufferWrite(I2C1, 2, EEPROM_ADDR);
		HAL_Delay(1);

		// читаем
		I2C_Master_BufferRead(I2C1, 24, EEPROM_ADDR);
		HAL_Delay(I2C_WR_HAL_Delay);

		// перекладываем прочитанные данные
		for(i = 0; i < 24; i++)
		{
			pGraphic->WeekData[i] = Buffer_Rx1[i];
		}

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		I2C_Cmd(I2C2, ENABLE);

		return SUCCESS;
	}

	return ERROR;
}



/*--------------------------------------- Запись и чтение графиков. дневные точки ---------------------------------------*/

/*******************************************************************************
 * Function Name  : Save_Graphics_MonthPoint
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void Save_Graphics_MonthPoint(void)
{
	uint8_t OutputSaveData[31];
	uint16_t SaveAddress = 0, SaveTempDataAddress = 0;
	uint8_t i = 0;
	uint8_t Date_SaveIndex = Graphics_Save_Date.Day - 1;

	Date_Label_Change = FALSE;

	/* Ищем индекс для записи */
	Graphics_Save_MonthIndex = Find_Last_Save_Index(Graphics_Save_Date, Graphics_Month_Varibles_Start, Month_VarNum, 12, _Month_, &Date_Label_Change);
	if(Date_Label_Change == FALSE)
	{
		Graphics_Save_MonthIndex++;
		if(Graphics_Save_MonthIndex >= 7) Graphics_Save_MonthIndex = 0;
	}

	// вычисляем стартовый адресс записи начала метки
	SaveAddress = Graphics_Month_Varibles_Start + (Graphics_Save_MonthIndex * Month_VarNum);
	SaveTempDataAddress = SaveAddress + 4;

	/* Если не нашли меток совпадающих с текущей датой, то сохраняем новую метку */
	if(Date_Label_Change == FALSE)
	{
		// сохраняем метку даты
		OutputSaveData[0] = Graphics_Save_Date.Day;
		OutputSaveData[1] = Graphics_Save_Date.Month;
		OutputSaveData[2] = Graphics_Save_Date.Year - 2000;
		Save_To_FEEPROM_8_CS(SaveAddress, 3, OutputSaveData);

		Erase_Data_EEPROM(SaveTempDataAddress, Month_VarNum - 4);

		/* очистим старые данные если в меню графиков */
		if((pMenu == &GraphicsMenu) && (Graphics_Mode == Month)) Clear_Graphics(Graphics_Print_MonthIndex, 255);
	}

	/* Сохраняем все датчики */
	for(i = 0; i < 9; i++)
	{
		OutputSaveData[0] = SensGraphics[i].MonthData[Date_SaveIndex];
		Save_To_EEPROM_8(SaveTempDataAddress + (31 * i) + Date_SaveIndex, 1, OutputSaveData);
	}

	/* перекючаем отображаемый месяц если нужно */
	if(Date_Label_Change == FALSE)
	{
		Graphics_Print_MonthIndex++;
		if(Graphics_Print_MonthIndex >= 12) Graphics_Print_MonthIndex = 0;
	}

	/* Обновляем график если находимся в меню графиков */
	if((pMenu == &GraphicsMenu) && (Graphics_Mode == Month))
	{
		Draw_Graphics(Graphics_Print_MonthIndex);
	}
}



/*******************************************************************************
 * Function Name  : Read_Graphics_MonthPoint
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
ErrorStatus Read_Graphics_MonthPoint(int8_t Month_Index, int8_t Sens_Index)
{
	uint16_t StartAddress = 0;
	uint8_t i = 0;
	uint8_t OutputReadData[5] = {0};

	if((Month_Index < 0) || (Month_Index > 12)) return ERROR;

	if(Read_From_FEEPROM_8_CS(Graphics_Month_Varibles_Start + (Month_Index * Month_VarNum), 3, &OutputReadData[0]) == SUCCESS)
	{
		Graphic_PrintData.Day = OutputReadData[0];
		Graphic_PrintData.Month = OutputReadData[1];
		Graphic_PrintData.Year = OutputReadData[2] + 2000;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
		I2C_Cmd(I2C2, DISABLE);
		HAL_Delay(I2C_WR_HAL_Delay);

		// стартовый адресс для чтения по датчикам, начальный адресс плюс 4(пропустить метку)
		StartAddress = Graphics_Month_Varibles_Start + (Month_Index * Month_VarNum) + (Sens_Index * 31) + 4;

		// пишем адрес старта данных для чтения
		Buffer_Tx1[0] = (uint8_t)(StartAddress >> 8);
		Buffer_Tx1[1] = (uint8_t)(StartAddress & 0x00FF);
		I2C_Master_BufferWrite(I2C1, 2, EEPROM_ADDR);
		HAL_Delay(1);

		// читаем
		I2C_Master_BufferRead(I2C1, 31, EEPROM_ADDR);
		HAL_Delay(I2C_WR_HAL_Delay);

		// перекладываем прочитанные данные
		for(i = 0; i < 31; i++)
		{
			pGraphic->MonthData[i] = Buffer_Rx1[i];
		}

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		I2C_Cmd(I2C2, ENABLE);

		return SUCCESS;
	}

	return ERROR;
}



/*******************************************************************************
 * Function Name  : Read_All_HoursPoint
 * Description    : читаем все корректные точки в указаном дне для записи дневной точки
 * Input          :
 * Return         : None
 *******************************************************************************/
ErrorStatus Read_All_HoursPoint(int8_t Day_Index, int8_t Sens_Index, uint8_t *OutData)
{
	uint16_t StartAddress = 0;
	uint8_t i = 0;
	uint8_t OutputReadData[5] = {0};

	if((Day_Index < 0) || (Day_Index > 8)) return ERROR;

	if(Read_From_FEEPROM_8_CS(Graphics_Hours_Varibles_Start + (Day_Index * Hours_VarNum), 3, &OutputReadData[0]) == SUCCESS)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
		I2C_Cmd(I2C2, DISABLE);
		HAL_Delay(I2C_WR_HAL_Delay);

		// стартовый адресс для чтения по датчикам, начальный адресс плюс 4(пропустить метку)
		StartAddress = Graphics_Hours_Varibles_Start + (Day_Index * Hours_VarNum) + (Sens_Index * 24) + 4;

		// пишем адрес старта данных для чтения
		Buffer_Tx1[0] = (uint8_t)(StartAddress >> 8);
		Buffer_Tx1[1] = (uint8_t)(StartAddress & 0x00FF);
		I2C_Master_BufferWrite(I2C1, 2, EEPROM_ADDR);
		HAL_Delay(1);

		// читаем
		I2C_Master_BufferRead(I2C1, 24, EEPROM_ADDR);
		HAL_Delay(I2C_WR_HAL_Delay);

		// перекладываем прочитанные данные
		for(i = 0; i < 24; i++)
		{
			(*OutData) = Buffer_Rx1[i];
			OutData++;
		}

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		I2C_Cmd(I2C2, ENABLE);

		return SUCCESS;
	}

	return ERROR;
}



/*******************************************************************************
 * Function Name  : Find_Last_Save_Index
 * Description    : Поиск последнего записанного индекса, Day_or_Month отвечает за то как искать,
 * 					0 - ищем включительно с днями,
 * 					1 - ищем по месяцу, дни отбрасываем
 * Input          :
 * Return         : None
 *******************************************************************************/
uint8_t Find_Last_Save_Index(Date_s CurentDate, uint16_t Var_SaveStart, uint16_t VarNum, uint8_t MaxIndex, uint8_t Day_or_Month, volatile Bool *Point)	// Graphics_Hours_Varibles_Start; Hours_VarNum
{
	uint8_t OutputReadData[5];
	Date_s Read_Date[7] = {{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}};
	uint8_t i = 0, Save_Index = 0;
	Bool Saved = FALSE;

	for(i = 0; i < MaxIndex; i++)
	{
		// Если нашли метку совпадающую с текущей датой, то Graphics_Save_DayIndex равен номеру этой метки
		if(Read_From_FEEPROM_8_CS(Var_SaveStart + (i * VarNum), 3, &OutputReadData[0]) == SUCCESS)
		{
			Read_Date[i].Day = OutputReadData[0]; Read_Date[i].Month = OutputReadData[1]; Read_Date[i].Year = OutputReadData[2] + 2000;

			if(Date_Compare(Read_Date[i], CurentDate, Day_or_Month) == 0)
			{
				if(Point != (void*)0) (*Point) = TRUE;
				return i;
			}
			else
			{
				Save_Index = i;
				Saved = TRUE;
			}
		}
	}

	/* проверяем есть ли вообще хоть один записаный день/месяц */
	if(Saved == FALSE) return 255;

	/* проверяем если все записаны но ни один не совпал с текущим, то ищем последнюю дату */
	else if(Save_Index == MaxIndex - 1) Save_Index = Date_Find_Last(Read_Date, MaxIndex, Day_or_Month);

	return Save_Index;
}










/*---------------------------------------------- Вспомогательные функции записи/чтения ---------------------------------------------*/

/*******************************************************************************
 * Function Name  : Find_CloseIn_Physical_Addr
 * Description    : вычисляем адрес ближайшей физичиской страницы 24LC256 (Max 511)
 * Input          :
 * Return         : None
 *******************************************************************************/
static void Find_CloseIn_Physical_Addr(uint16_t StartAddr, uint16_t NumOfVar, uint16_t *StartPageNum, uint16_t *StartPageAddr, int16_t *Different)
{
	*StartPageNum = StartAddr / 64;
	*StartPageAddr = (*StartPageNum) * 64;

	*Different = ((*StartPageAddr) - StartAddr) + 64;
	if(NumOfVar < (*Different)) *Different = NumOfVar;
}


/*******************************************************************************
 * Function Name  : Find_CloseIn_Physical_Addr
 * Description    : вычисляем адрес ближайшей физичиской страницы 24LC256 (Max 511)
 * Input          :
 * Return         : None
 *******************************************************************************/
static Bool Find_Full_Physical_Pages(uint16_t Start_PageNum, int16_t Different, uint16_t *NumOfVar, uint16_t *Full_Page, uint16_t *NextStartAddr)
{
	(*NumOfVar) = (*NumOfVar) - Different;
	if((*NumOfVar) == 0) return FALSE;

	(*Full_Page) = (*NumOfVar) / 64;
	*NextStartAddr = (Start_PageNum + 1) * 64;

	return TRUE;
}


/*******************************************************************************
 * Function Name  : Find_CloseIn_Physical_Addr
 * Description    : вычисляем адрес ближайшей физичиской страницы 24LC256 (Max 511)
 * Input          :
 * Return         : None
 *******************************************************************************/
static void Find_Rest_NumVar(uint16_t NextStartAddr, uint16_t Full_Page, uint16_t NumOfVar, int16_t *Different, uint16_t *NextEndAddr)
{
	*NextEndAddr = NextStartAddr + (Full_Page * 64);
	(*Different) = (int16_t)NumOfVar - (int16_t)(Full_Page * 64);
}



/*---------------------------------------------- Функции записи/чтения/стирания ---------------------------------------------*/

/*******************************************************************************
 * Function Name  : SaveSettings_To_FEEPROM
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void Save_To_FEEPROM_8_CS(uint16_t StartAddress, uint16_t DataNum, uint8_t *Data)
{
	uint16_t i;
	uint16_t StartPage_Num, FullPage;
	uint16_t Nstr, Nend;
	int16_t Diff;
	uint16_t Num = DataNum;
	uint16_t ControlNumAddr = StartAddress + DataNum;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
	I2C_Cmd(I2C2, DISABLE);
	HAL_Delay(I2C_WR_HAL_Delay);


	/* вычисляем адрес ближайшей физичиской страницы 24LC256 (Max 511) и
		   вычисляем разницу между границей страницы и стартовым адрессом */
	Find_CloseIn_Physical_Addr(StartAddress, Num, &StartPage_Num, &Nstr, &Diff);

	if(Diff > 0)
	{
		/* очищаем буфер и пишем до границы страницы */
		memcpy((uint8_t*)&Buffer_Tx1[2], Data, Diff);

		Buffer_Tx1[0] = (uint8_t)(StartAddress >> 8);
		Buffer_Tx1[1] = (uint8_t)(StartAddress & 0x00FF);

		I2C_Master_BufferWrite(I2C1, 2 + Diff, EEPROM_ADDR);
		HAL_Delay(I2C_WR_HAL_Delay);
	}
	HAL_Delay(I2C_WR_HAL_Delay);

	/* вычисляем сколько целых страниц и записываем их */
	if(Find_Full_Physical_Pages(StartPage_Num, Diff, &Num, &FullPage, &Nstr) != FALSE)
	{
		Data += Diff;

		if(FullPage > 0)
		{
			memcpy((uint8_t*)&Buffer_Tx1[2], Data, 64);

			for(i = 0; i < FullPage; i++)
			{
				Buffer_Tx1[0] = (uint8_t)((Nstr + (i * 64)) >> 8);
				Buffer_Tx1[1] = (uint8_t)((Nstr + (i * 64)) & 0x00FF);

				I2C_Master_BufferWrite(I2C1, 66, EEPROM_ADDR);
				HAL_Delay(I2C_WR_HAL_Delay);
				Data += 64;
			}
		}

		/* вычисляем сколько осталось записать и записываем если есть */
		Find_Rest_NumVar(Nstr, FullPage, Num, &Diff, &Nend);

		if(Diff > 0)
		{
			memcpy((uint8_t*)&Buffer_Tx1[2], Data, Diff);

			Buffer_Tx1[0] = (uint8_t)(Nend >> 8);
			Buffer_Tx1[1] = (uint8_t)(Nend & 0x00FF);

			I2C_Master_BufferWrite(I2C1, 2 + Diff, EEPROM_ADDR);
			HAL_Delay(I2C_WR_HAL_Delay);
		}
	}

	Buffer_Tx1[0] = (uint8_t)(ControlNumAddr >> 8);
	Buffer_Tx1[1] = (uint8_t)(ControlNumAddr & 0x00FF);
	Buffer_Tx1[2] = 0x77;
	HAL_Delay(I2C_WR_HAL_Delay);
	I2C_Master_BufferWrite(I2C1, 3, EEPROM_ADDR);
	HAL_Delay(I2C_WR_HAL_Delay);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	I2C_Cmd(I2C2, ENABLE);
	HAL_Delay(I2C_WR_HAL_Delay);
}


/*******************************************************************************
 * Function Name  : ReadSettings_From_FEEPROM
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
ErrorStatus Read_From_FEEPROM_8_CS(uint16_t StartAddress, uint16_t DataNum, uint8_t *Data)
{
	uint16_t i;
	ErrorStatus Status = ERROR;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
	I2C_Cmd(I2C2, DISABLE);
	HAL_Delay(I2C_WR_HAL_Delay);

	// пишем адрес старта данных для чтения
	Buffer_Tx1[0] = (uint8_t)(StartAddress >> 8);
	Buffer_Tx1[1] = (uint8_t)(StartAddress & 0x00FF);
	I2C_Master_BufferWrite(I2C1, 2, EEPROM_ADDR);
	HAL_Delay(1);

	// читаем
	I2C_Master_BufferRead(I2C1, DataNum + 1, EEPROM_ADDR);
	HAL_Delay(I2C_WR_HAL_Delay);

	// перекладываем прочитанные данные
	for(i = 0; i < DataNum; i++)
	{
		(*Data) = Buffer_Rx1[i];
		Data++;
	}

	// проверка на целостность данных
	if(Buffer_Rx1[DataNum] == 0x77 ) Status = SUCCESS;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	I2C_Cmd(I2C2, ENABLE);

	return Status;
}



/*******************************************************************************
 * Function Name  : Read_From_EEPROM_8_Host
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
ErrorStatus Read_From_EEPROM_8_Host(uint16_t StartAddress, uint16_t DataNum, uint8_t *Data)
{
	uint16_t i;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
	I2C_Cmd(I2C2, DISABLE);
	HAL_Delay(I2C_WR_HAL_Delay);

	// пишем адрес старта данных для чтения
	Buffer_Tx1[0] = (uint8_t)(StartAddress >> 8);
	Buffer_Tx1[1] = (uint8_t)(StartAddress & 0x00FF);
	I2C_Master_BufferWrite(I2C1, 2, EEPROM_ADDR);
	HAL_Delay(I2C_WR_HAL_Delay);

	// читаем
	I2C_Master_BufferRead(I2C1, DataNum, EEPROM_ADDR);
	while(NumbOfBytes1 > 1)
	{
	}
	HAL_Delay(1);

	// перекладываем прочитанные данные
	for(i = 0; i < DataNum; i++)
	{
		(*Data) = Buffer_Rx1[i];
		Data++;
	}

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	I2C_Cmd(I2C2, ENABLE);

	return SUCCESS;
}



/*******************************************************************************
 * Function Name  : Write_EEPROM_8_Host
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
ErrorStatus Write_EEPROM_8_Host(uint16_t StartAddress, uint16_t DataNum, uint8_t *Data)
{
	uint16_t i;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
	I2C_Cmd(I2C2, DISABLE);
	HAL_Delay(I2C_WR_HAL_Delay);

	Buffer_Tx1[0] = (uint8_t)(StartAddress >> 8);
	Buffer_Tx1[1] = (uint8_t)(StartAddress & 0x00FF);

	for(i = 0; i < DataNum; i++)
	{
		Buffer_Tx1[2 + i] = Data[i];
	}


	I2C_Master_BufferWrite(I2C1, 2 + DataNum, EEPROM_ADDR);
	while(NumbOfBytes1 > 1)
	{
	}
	HAL_Delay(1);


	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	I2C_Cmd(I2C2, ENABLE);
	HAL_Delay(I2C_WR_HAL_Delay);

	return SUCCESS;
}











/*******************************************************************************
 * Function Name  : Save_To_EEPROM_8bitData
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void Save_To_EEPROM_8(uint16_t StartAddress, uint8_t DataNum, uint8_t *Data)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
	I2C_Cmd(I2C2, DISABLE);
	HAL_Delay(I2C_WR_HAL_Delay);

	memset((uint8_t*)&Buffer_Tx1[2], 0, 64);

	// Записываем
	while(DataNum > 0)
	{
		Buffer_Tx1[0] = (uint8_t)(StartAddress >> 8);
		Buffer_Tx1[1] = (uint8_t)(StartAddress & 0x00FF);
		Buffer_Tx1[2] = (*Data);

		I2C_Master_BufferWrite(I2C1, 3, EEPROM_ADDR);
		HAL_Delay(I2C_WR_HAL_Delay);

		StartAddress++;
		DataNum--;
	}

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	I2C_Cmd(I2C2, ENABLE);
	HAL_Delay(I2C_WR_HAL_Delay);
}


///*******************************************************************************
// * Function Name  : Read_From_EEPROM_8bitData
// * Description    :
// * Input          :
// * Return         : None
// *******************************************************************************/
//ErrorStatus Read_From_EEPROM_8(uint16_t StartAddress, uint8_t DataNum, uint8_t *Data)
//{
////	uint16_t i;
//	ErrorStatus Status = ERROR;
//
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
//	I2C_Cmd(I2C2, DISABLE);
//	HAL_Delay(10);
//
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
//	I2C_Cmd(I2C2, ENABLE);
//
//	return Status;
//}





/*******************************************************************************
 * Function Name  : Erase_EEPROM
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void Erase_Data_EEPROM(uint16_t StartAddr, uint16_t Num)
{
	uint16_t i;
	uint16_t StartPage_Num, FullPage;
	uint16_t Nstr, Nend;
	int16_t Diff;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
	I2C_Cmd(I2C2, DISABLE);
	HAL_Delay(I2C_WR_HAL_Delay);


	/* вычисляем адрес ближайшей физичиской страницы 24LC256 (Max 511) и
	   вычисляем разницу между границей страницы и стартовым адрессом */
	Find_CloseIn_Physical_Addr(StartAddr, Num, &StartPage_Num, &Nstr, &Diff);


	if(Diff > 0)
	{
		/* очищаем буфер и пишем до границы страницы */
		memset((uint8_t*)&Buffer_Tx1[2], 255, Diff);

		Buffer_Tx1[0] = (uint8_t)(StartAddr >> 8);
		Buffer_Tx1[1] = (uint8_t)(StartAddr & 0x00FF);

		I2C_Master_BufferWrite(I2C1, 2 + Diff, EEPROM_ADDR);
		HAL_Delay(I2C_WR_HAL_Delay);
	}

	/* вычисляем сколько целых страниц и записываем их */
	if(Find_Full_Physical_Pages(StartPage_Num, Diff, &Num, &FullPage, &Nstr) == FALSE) return;

	if(FullPage > 0)
	{
		memset((uint8_t*)&Buffer_Tx1[2], 255, 64);

		for(i = 0; i < FullPage; i++)
		{
			Buffer_Tx1[0] = (uint8_t)((Nstr + (i * 64)) >> 8);
			Buffer_Tx1[1] = (uint8_t)((Nstr + (i * 64)) & 0x00FF);

			I2C_Master_BufferWrite(I2C1, 66, EEPROM_ADDR);
			HAL_Delay(I2C_WR_HAL_Delay);
		}
	}

	/* вычисляем сколько осталось записать и записываем если есть */
	Find_Rest_NumVar(Nstr, FullPage, Num, &Diff, &Nend);

	if(Diff > 0)
	{
		memset((uint8_t*)&Buffer_Tx1[2], 255, Diff);

		Buffer_Tx1[0] = (uint8_t)(Nend >> 8);
		Buffer_Tx1[1] = (uint8_t)(Nend & 0x00FF);

		I2C_Master_BufferWrite(I2C1, Diff, EEPROM_ADDR);
		HAL_Delay(I2C_WR_HAL_Delay);
	}

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	I2C_Cmd(I2C2, ENABLE);
}


/*******************************************************************************
 * Function Name  : Erase_EEPROM
 * Description    :
 * Input          :
 * Return         : None
 *******************************************************************************/
void Erase_Blocks_EEPROM(uint16_t StartBlock, uint16_t BlocksNum)
{
	uint16_t BlockAddr = StartBlock * 64;
	uint16_t i;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
	I2C_Cmd(I2C2, DISABLE);
	HAL_Delay(I2C_WR_HAL_Delay);

	memset((uint8_t*)&Buffer_Tx1[2], 0, 64);

	for(i = StartBlock; i < BlocksNum; i++)
	{
		HAL_IWDG_Refresh(&hiwdg);

		Buffer_Tx1[0] = (uint8_t)(BlockAddr >> 8);
		Buffer_Tx1[1] = (uint8_t)(BlockAddr & 0x00FF);

		I2C_Master_BufferWrite(I2C1, 66, EEPROM_ADDR);
		HAL_Delay(I2C_WR_HAL_Delay);

		BlockAddr += 64;
	}

	HAL_Delay(I2C_WR_HAL_Delay);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	I2C_Cmd(I2C2, ENABLE);
}





