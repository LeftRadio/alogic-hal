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
#include <string.h>

#include "Regulates.h"
#include "User_Interface.h"
#include "clock_calendar.h"
#include "Outputs.h"
#include "Pparam.h"
#include "STLM75.h"
#include "DHT11.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile Regulates_INFO Regulates_Struct[Regulates_Max_Count];

/* Private function prototypes -----------------------------------------------*/
void Regulation_Thresold(uint8_t Data, uint8_t i);
void Regulation_Proportional(uint8_t Data, uint8_t i);
void Regulation_Thresold_2(uint8_t Data, uint8_t i);

void (*pOut_ON)(uint8_t Index);
void (*pOut_OFF)(uint8_t Index);

extern void Update_Sensor_OUT(uint8_t Indx);


/* Functions ----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Timers_Default_Init
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Regulates_Init(void)
{
	Sensor_Typedef DefType = STLM75;
	uint8_t i;

	for(i = 0; i < Regulates_Max_Count; i++)
	{
		if(ReadRegulsSettings(i) == ERROR)
		{
			if(i == 8) DefType = DHT11;
			else if(i == 9) DefType = SHT21;
			else DefType = STLM75;

			One_Regulate_Default_Init((Regulates_INFO*)&Regulates_Struct[i], i, DefType);
		}
	}

	Init_Regulates_Buttons();
}


/*******************************************************************************
* Function Name  : Regulates_Default_Init
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Regulates_Default_Init(uint8_t FirstRegulNum, uint8_t LastRegulNum)
{
	uint8_t i;
	Sensor_Typedef DefType = STLM75;

	for(i = FirstRegulNum; i < LastRegulNum; i++)
	{
		if(i == 8) DefType = DHT11;
		else if(i == 9) DefType = SHT21;
		else DefType = STLM75;

		One_Regulate_Default_Init((Regulates_INFO*)&Regulates_Struct[i], i, DefType);
	}
}


/*******************************************************************************
* Function Name  : Regulates_Default_Init
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void One_Regulate_Default_Init(Regulates_INFO *pRegulate, uint8_t NameNum, Sensor_Typedef NewType)
{
	if((NewType == DHT11) || ((NewType == SHT21)))
	{
		pRegulate->MIN_SensorIndex = 8;
		pRegulate->MAX_SensorIndex = 9;
	}
	else
	{
		pRegulate->MIN_SensorIndex = 0;
		pRegulate->MAX_SensorIndex = 7;
	}

	pRegulate->SensorType = NewType;
	pRegulate->SensorIndex = NameNum;
	pRegulate->RegulType = Thresold;
	pRegulate->State = DISABLE;
	pRegulate->Thresold = 25;
	pRegulate->Thresold_2 = 100;
	pRegulate->Hysteresis = 0;
	pRegulate->PWM_ON_Parametr = 10;
	pRegulate->PWM = 100;
	pRegulate->PWM_State = DISABLE;
	pRegulate->INVERT = 0;
	pRegulate->Out_Index = Regulates_OUT_NotAssigned;

	ConvertToString(NameNum + 1, pRegulate->Name, 2);
}


/*******************************************************************************
* Function Name  : Regulate_CopyStruct
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Regulate_CopyStruct(Regulates_INFO* Regul_ToCopy, Regulates_INFO* Regul_FromCopy)
{
	Regul_ToCopy->SensorType = Regul_FromCopy->SensorType;
	Regul_ToCopy->SensorIndex = Regul_FromCopy->SensorIndex;
	Regul_ToCopy->MIN_SensorIndex = Regul_FromCopy->MIN_SensorIndex;
	Regul_ToCopy->MAX_SensorIndex = Regul_FromCopy->MAX_SensorIndex;
	Regul_ToCopy->RegulType = Regul_FromCopy->RegulType;
	Regul_ToCopy->State = Regul_FromCopy->State;
	Regul_ToCopy->Thresold = Regul_FromCopy->Thresold;
	Regul_ToCopy->Thresold_2 = Regul_FromCopy->Thresold_2;
	Regul_ToCopy->Hysteresis = Regul_FromCopy->Hysteresis;
	Regul_ToCopy->PWM_ON_Parametr = Regul_FromCopy->PWM_ON_Parametr;
	Regul_ToCopy->PWM = Regul_FromCopy->PWM;
	Regul_ToCopy->PWM_State = Regul_FromCopy->PWM_State;
	Regul_ToCopy->INVERT = Regul_FromCopy->INVERT;
	Regul_ToCopy->Name[0] = Regul_FromCopy->Name[0];
	Regul_ToCopy->Name[1] = Regul_FromCopy->Name[1];
	Regul_ToCopy->Out_Index = Regul_FromCopy->Out_Index;

	if((Regul_ToCopy->State == DISABLE) && (Regul_ToCopy->Out_Index != Regulates_OUT_NotAssigned))
	{
		OUT_OFF(Regul_ToCopy->Out_Index);
	}
}


/*******************************************************************************
* Function Name  : Init_Regulates_Buttons
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Init_Regulates_Buttons(void)
{
	uint8_t i;

	for(i = 0; i < Regulates_Max_Count; i++)
	{
		if(Regulates_Struct[i].State == ENABLE) AllSensorsMenu.Items[i]->TextColor = LighGreen;
		else AllSensorsMenu.Items[i]->TextColor = White;

		if(pMenu == &AllSensorsMenu) Update_Sensor_OUT(i);
	}
}


/* -------------------------------------- Проверки по регулировкам -------------------------------------- */

/*******************************************************************************
* Function Name  : VerifyRegulatesProgram
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void VerifyRegulatesProgram(uint32_t TimeSeconds)
{
	volatile uint8_t index, Data;
	uint8_t i;

	for(i = 0; i < Regulates_Max_Count; i++)
	{
		if(Regulates_Struct[i].SensorIndex == 8)
		{
			HTSensor = &DHTSensor;
			HTSensor->Status = DHTSensor.Status;
		}
		else
		{
			HTSensor = &SHTSensor;
			HTSensor->Status = SHTSensor.Status;
		}

		if(Regulates_Struct[i].State == ENABLE)
		{
			if(i == 8)
			{
				if((HTSensor->Status != ERROR) && (Regulates_Struct[i].Out_Index != Regulates_OUT_NotAssigned))
				{
					Data = HTSensor->Data[0];

					if(Regulates_Struct[i].RegulType == Thresold) Regulation_Thresold(Data, i);
					else if(Regulates_Struct[i].RegulType == Proportional) Regulation_Proportional(Data, i);

					Regulation_Thresold_2(Data, i);
				}

			}
			else
			{
				index = Regulates_Struct[i].SensorIndex;

				if(TempSensor[index].Status != ERROR)
				{
					if(Regulates_Struct[i].Out_Index != Regulates_OUT_NotAssigned)
					{
						Data = TempSensor[index].Data >> 1;

						if(Data >= 100)
						{
							Data = 101;
						}

						if(Regulates_Struct[i].RegulType == Thresold) Regulation_Thresold(Data, i);
						else if(Regulates_Struct[i].RegulType == Proportional) Regulation_Proportional(Data, i);

						Regulation_Thresold_2(Data, i);
					}
				}
				else if(Regulates_Struct[i].Out_Index != Regulates_OUT_NotAssigned) Regulates_Struct[i].PWM_State = DISABLE;
			}
		}
	}

}



/*******************************************************************************
* Function Name  : Regulation_Parametr
* Description    :
* Input          : Sens_Channel
* Return         : None
*******************************************************************************/
void Regulation_Thresold(uint8_t Data, uint8_t i)
{
	static FlagStatus Hysteresis_thresold[9] = {RESET};

	// если нагрев
	if(Regulates_Struct[i].INVERT == DISABLE)
	{
		// если текущая температура меньше пороговой минус гистерезис, то включить
		if(Data <= (Regulates_Struct[i].Thresold - Regulates_Struct[i].Hysteresis))
		{
			Regulates_Struct[i].PWM_State = ENABLE;
			Hysteresis_thresold[i] = RESET;
		}
		// если текущая температура меньше пороговой плюс гистерезис, то включить
		else if(Data <= (Regulates_Struct[i].Thresold + Regulates_Struct[i].Hysteresis))
		{
			if(Hysteresis_thresold[i] == RESET) Regulates_Struct[i].PWM_State = ENABLE;
			else Regulates_Struct[i].PWM_State = DISABLE;
		}
		else if(Data > (Regulates_Struct[i].Thresold + Regulates_Struct[i].Hysteresis))
		{
			Regulates_Struct[i].PWM_State = DISABLE;
			Hysteresis_thresold[i] = SET;
		}
	}
	else // иначе если охлаждение
	{
		// если текущая температура больше пороговой плюс гистерезис, то включить
		if(Data >= (Regulates_Struct[i].Thresold + Regulates_Struct[i].Hysteresis))
		{
			Regulates_Struct[i].PWM_State = ENABLE;
			Hysteresis_thresold[i] = RESET;
		}
		// если текущая температура больше пороговой минус гистерезис, то включить
		else if(Data >= (Regulates_Struct[i].Thresold - Regulates_Struct[i].Hysteresis))
		{
			if(Hysteresis_thresold[i] == RESET) Regulates_Struct[i].PWM_State = ENABLE;
			else Regulates_Struct[i].PWM_State = DISABLE;
		}
		else if(Data < (Regulates_Struct[i].Thresold - Regulates_Struct[i].Hysteresis))
		{
			Regulates_Struct[i].PWM_State = DISABLE;
			Hysteresis_thresold[i] = SET;
		}
	}
}


/*******************************************************************************
* Function Name  : Regulation_Parametr
* Description    :
* Input          : Sens_Channel
* Return         : None
*******************************************************************************/
void Regulation_Proportional(uint8_t Data, uint8_t i)
{
	static FlagStatus Hysteresis_thresold[9] = {RESET};

	// если нагрев
	if(Regulates_Struct[i].INVERT == DISABLE)
	{
		Regulates_Struct[i].PWM = ((Regulates_Struct[i].Thresold + Regulates_Struct[i].Hysteresis) - Data) * Regulates_Struct[i].PWM_ON_Parametr;
		if(Regulates_Struct[i].PWM < 0) Regulates_Struct[i].PWM = 0;
		else if(Regulates_Struct[i].PWM > 100) Regulates_Struct[i].PWM = 100;

		// если текущая температура меньше пороговой минус гистерезис, то включить
		if(Data <= (Regulates_Struct[i].Thresold - Regulates_Struct[i].Hysteresis))
		{
			Regulates_Struct[i].PWM_State = ENABLE;
			Hysteresis_thresold[i] = RESET;
		}
		// если текущая температура меньше пороговой плюс гистерезис, то включить
		else if(Data <= (Regulates_Struct[i].Thresold + Regulates_Struct[i].Hysteresis))
		{
			if(Hysteresis_thresold[i] == RESET) Regulates_Struct[i].PWM_State = ENABLE;
			else Regulates_Struct[i].PWM_State = DISABLE;
		}
		else if(Data >= (Regulates_Struct[i].Thresold + Regulates_Struct[i].Hysteresis))
		{
			Regulates_Struct[i].PWM_State = DISABLE;
			Hysteresis_thresold[i] = SET;
		}
	}
	else // иначе если охлаждение
	{
		Regulates_Struct[i].PWM = (Data - (Regulates_Struct[i].Thresold - Regulates_Struct[i].Hysteresis)) * Regulates_Struct[i].PWM_ON_Parametr;
		if(Regulates_Struct[i].PWM < 0) Regulates_Struct[i].PWM = 0;
		else if(Regulates_Struct[i].PWM > 100) Regulates_Struct[i].PWM = 100;

		// если текущая температура больше пороговой плюс гистерезис, то включить
		if(Data >= (Regulates_Struct[i].Thresold + Regulates_Struct[i].Hysteresis))
		{
			Regulates_Struct[i].PWM_State = ENABLE;
			Hysteresis_thresold[i] = RESET;
		}
		// если текущая температура больше пороговой минус гистерезис, то включить
		else if(Data >= (Regulates_Struct[i].Thresold - Regulates_Struct[i].Hysteresis))
		{
			if(Hysteresis_thresold[i] == RESET) Regulates_Struct[i].PWM_State = ENABLE;
			else Regulates_Struct[i].PWM_State = DISABLE;
		}
		else if(Data <= (Regulates_Struct[i].Thresold - Regulates_Struct[i].Hysteresis))
		{
			Regulates_Struct[i].PWM_State = DISABLE;
			Hysteresis_thresold[i] = SET;
		}
	}
}


/*******************************************************************************
* Function Name  : Regulation_Thresold_2
* Description    :
* Input          : Sens_Channel
* Return         : None
*******************************************************************************/
void Regulation_Thresold_2(uint8_t Data, uint8_t i)
{
	if(Data >= Regulates_Struct[i].Thresold_2 - 1)
	{
		Outs_ENABLE[Regulates_Struct[i].Out_Index] = DISABLE;
		OUT_OFF(Regulates_Struct[i].Out_Index);
	}
	else
	{
		Outs_ENABLE[Regulates_Struct[i].Out_Index] = ENABLE;
	}
}


/*******************************************************************************
* Function Name  : Regulate_Get_SensorIndex
* Description    :
* Input          :
* Return         : None
*******************************************************************************/
uint8_t Regulate_Get_SensorIndex(uint8_t RegulateIndex)
{
	return Regulates_Struct[RegulateIndex].SensorIndex;
}

