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
#include "STLM75.h"
#include "User_Interface.h"
#include "GlobalInit.h"
#include "Outputs.h"
#include "Regulates.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LM75_TypeDef TempSensor[TmpSensNum_MAX];
LM75_TypeDef IntTempSensor[2];

volatile int8_t OLD_TempSensor_Indx = -1;
volatile int8_t Int_OLD_TempSensor_Indx = -1;
volatile ErrorStatus OLD_TempStatus[8];

ErrorStatus Sensor_Status = ERROR, Sensor_Internal_Status = ERROR;
volatile Bool I2C1_ERROR = FALSE, I2C2_ERROR = FALSE;
Bool Read_STLM75_Flag = FALSE;
FunctionalState STLM75_IndicateLED[TmpSensNum_MAX] = { DISABLE, DISABLE, DISABLE, DISABLE, DISABLE, DISABLE, DISABLE, DISABLE };

/* Private function prototypes -----------------------------------------------*/
void LM75_Start_ReadReg(uint8_t RegName, uint8_t Sensor_ADDR, uint8_t Group);
void LM75_Start_WriteReg(uint8_t RegName, uint8_t Sensor_ADDR, uint16_t Data, uint8_t Group);
void LM75_Start_Write8bitReg(uint8_t RegName, uint8_t Sensor_ADDR, uint8_t Data, uint8_t Group);
void STLM75_LedState(FlagStatus NewStatus, uint8_t Indx);
static void STLM75_FindAndUpdate_SensorsData(LM75_TypeDef TempSensor, uint8_t OLD_TempStatus);

/* Functions ----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : TemeratureSensor_Read
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TemeratureSensors_InitDefault(void)
{
	uint8_t i;

	Sensor_Status = SUCCESS;
	Sensor_Internal_Status = SUCCESS;

	for(i = 0; i < 8; i++)
	{
		TempSensor[i].Address = 0x90 + (i << 1);
		TempSensor[i].Status = SUCCESS;
	}

	IntTempSensor[0].Address = 0x90 + (0 << 1);
	IntTempSensor[0].Status = SUCCESS;
	IntTempSensor[1].Address = 0x90 + (1 << 1);
	IntTempSensor[1].Status = SUCCESS;
}


/*******************************************************************************
* Function Name  : TemeratureSensor_Read
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TemeratureSensor_Read(void)
{
	int8_t i;

	for(i = 0; i < 8; i++)
	{
		if(i > OLD_TempSensor_Indx)
		{
			if(TempSensor[i].Status == SUCCESS) LM75_Start_ReadReg(0, TempSensor[i].Address, 0);
			else Sensor_Status = ERROR;

			OLD_TempSensor_Indx = i;
			break;
		}
	}
}



/*******************************************************************************
* Function Name  : LM75_Start_ReadReg
* Description    : Функция старта чтения регистра датчика температуры
* Input          :	- LM75_REG_TEMP: temperature register
* 					- LM75_REG_TOS: Over-limit temperature register
* 					- LM75_REG_THYS: Hysteresis temperature register
* Output         : None
* Return         : None
*******************************************************************************/
void LM75_Start_ReadReg(uint8_t RegName, uint8_t Sensor_ADDR, uint8_t Group)
{
	if(Group == 1)
	{
		Sensor_Status = ERROR;

		HAL_Delay(5);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
		I2C_Cmd(I2C2, DISABLE);
		HAL_Delay(10);

		I2C_Master_BufferRead(I2C1, 2, Sensor_ADDR);
		HAL_Delay(1);

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		I2C_Cmd(I2C2, ENABLE);
	}
	else
	{
		Sensor_Internal_Status = ERROR;
		I2C_Master_BufferRead(I2C2, 2, Sensor_ADDR);
	}
}



/*******************************************************************************
* Function Name  : LM75_Start_ReadReg
* Description    : Функция старта чтения регистра датчика температуры
* Input          :	- LM75_REG_TEMP: temperature register
* 					- LM75_REG_TOS: Over-limit temperature register
* 					- LM75_REG_THYS: Hysteresis temperature register
* Output         : None
* Return         : None
*******************************************************************************/
void LM75_Start_WriteReg(uint8_t RegName, uint8_t Sensor_ADDR, uint16_t Data, uint8_t Group)
{
	if(Group == 0)
	{
		Buffer_Tx2[0] = 0x03;
		Buffer_Tx2[1] = (uint8_t)(Data >> 8);
		Buffer_Tx2[2] = (uint8_t)(Data & 0x00FF);

		I2C_Master_BufferWrite(I2C2, 3, Sensor_ADDR);
		HAL_Delay(1);
	}
	else
	{

	}
}


/*******************************************************************************
* Function Name  : LM75_Start_Write8bitReg
* Description    :
* Input          :
* Output         :
* Return         :
*******************************************************************************/
void LM75_Start_Write8bitReg(uint8_t RegName, uint8_t Sensor_ADDR, uint8_t Data, uint8_t Group)
{
	if(Group == 0)
	{
		Buffer_Tx2[0] = 0x03;
		Buffer_Tx2[1] = Data;

		I2C_Master_BufferWrite(I2C2, 2, Sensor_ADDR);
		HAL_Delay(1);
	}
	else
	{

	}
}


/*******************************************************************************
* Function Name  : LM75_ReadTemp
* Description    : Функция чтения показаний датчика температуры
* Input          :
* Output         : LM75 temperature value
* Return         : Sensor read status - ERROR or SUCCESS
*******************************************************************************/
ErrorStatus LM75_ReadTemp(uint16_t *ReadData, uint8_t Group)
{
	if(Group == 1)
	{
		*ReadData = ((uint16_t)(Buffer_Rx1[0] << 8) | (uint16_t)Buffer_Rx1[1]) >> 7;
		return Sensor_Internal_Status;
	}
	else
	{
		*ReadData = ((uint16_t)(Buffer_Rx2[0] << 8) | (uint16_t)Buffer_Rx2[1]) >> 7;
		return Sensor_Status;
	}
}



/*******************************************************************************
* Function Name  : ReadAll_LM75
* Description    : Функция чтения показаний всех датчиков температуры
* Input          :
* Return         :
*******************************************************************************/
void ReadAll_LM75(void)
{
	uint8_t i;
	static FlagStatus LED_State[TmpSensNum_MAX] = { RESET };

	do
	{
		I2C2_ERROR = FALSE;

		/* Опрашиваем все датчики температуры */
		for(i = 0; i < TmpSensNum_MAX; i++)
		{
			/* запускаем чтение следующего датчика температуры */
			TemeratureSensor_Read();

			/* если уже запускали какие нибуть датчики то */
			if(OLD_TempSensor_Indx >= 0)
			{
				/* проверяем статус датчика */
				TempSensor[OLD_TempSensor_Indx].Status = LM75_ReadTemp(&TempSensor[OLD_TempSensor_Indx].Data, 0);

				if(pMenu == &MainMenu)
				{
					Update_TempSensors_OnScreen(TempSensor[OLD_TempSensor_Indx].Data >> 1, OLD_TempSensor_Indx, OLD_TempStatus[i], TempSensor[OLD_TempSensor_Indx].Status);

				}
				else if(pMenu == &AllSensorsMenu)
				{
					STLM75_FindAndUpdate_SensorsData(TempSensor[OLD_TempSensor_Indx], OLD_TempStatus[i]);
				}

				// Если разрешены мигания светодиода на датчике
				if((STLM75_IndicateLED[OLD_TempSensor_Indx] == ENABLE) && (TempSensor[OLD_TempSensor_Indx].Status != ERROR))
				{
					/* Включаем режим компаратора в регистре Configurate по адресу 0х01 */
					LM75_Start_Write8bitReg(0x01, TempSensor[OLD_TempSensor_Indx].Address, 4 << 3, 0);


					if(LED_State[OLD_TempSensor_Indx] == RESET)
					{
						LED_State[OLD_TempSensor_Indx] = SET;
					}
					else
					{
						LED_State[OLD_TempSensor_Indx] = RESET;
					}

					STLM75_LedState(LED_State[OLD_TempSensor_Indx], OLD_TempSensor_Indx);
				}
				else STLM75_LedState(RESET, OLD_TempSensor_Indx);

				OLD_TempStatus[i] = TempSensor[OLD_TempSensor_Indx].Status;

				/* если последний датчик то сбросить на опрос первого */
				if(OLD_TempSensor_Indx == TmpSensNum_MAX - 1) OLD_TempSensor_Indx = -1;
			}
		}

		GPIO_ResetBits(GPIOB, GPIO_Pin_1);

	} while(I2C2_ERROR == TRUE);
}



/*******************************************************************************
* Function Name  : ReadAll_LM75
* Description    : Функция чтения показаний всех датчиков температуры
* Input          :
* Return         :
*******************************************************************************/
void Read_Internal_LM75(void)
{
	uint8_t i, j;

	Read_STLM75_Flag = TRUE;

	do
	{
		I2C1_ERROR = FALSE;

		/* Опрашиваем все датчики температуры */
		for(i = 0; i < 2; i++)
		{
			/* запускаем чтение следующего датчика температуры */
			for(j = 0; j < 2; j++)
			{
				if((IntTempSensor[j].Status != ERROR) && (j > Int_OLD_TempSensor_Indx))
				{
					LM75_Start_ReadReg(0, IntTempSensor[j].Address, 1);
					Int_OLD_TempSensor_Indx = j;
					break;
				}
			}

			/* если уже запускали какие нибуть датчики то */
			if(Int_OLD_TempSensor_Indx >= 0)
			{
				/* проверяем статус датчика */
				IntTempSensor[Int_OLD_TempSensor_Indx].Status = LM75_ReadTemp(&IntTempSensor[Int_OLD_TempSensor_Indx].Data, 1);

				/* Тепловая защита выходов */
				Control_Outs_From_Internal_Temperature(IntTempSensor[Int_OLD_TempSensor_Indx].Status, \
							IntTempSensor[Int_OLD_TempSensor_Indx].Data >> 1, i);

				/* Обновление показаний в главном меню */
				if(pMenu == &MainMenu)
				{
					Update_InternalTempSensors_OnScreen(IntTempSensor[Int_OLD_TempSensor_Indx].Data >> 1,\
							Int_OLD_TempSensor_Indx, IntTempSensor[Int_OLD_TempSensor_Indx].Status);
				}

				/* если последний датчик то сбросить на опрос первого */
				if(Int_OLD_TempSensor_Indx == 1) Int_OLD_TempSensor_Indx = -1;
			}
		}

		GPIO_ResetBits(GPIOB, GPIO_Pin_1);

	} while(I2C2_ERROR == TRUE);

	Read_STLM75_Flag = FALSE;
}



/*******************************************************************************
* Function Name  : STLM75_LedState
* Description    :
* Input          :
* Return         :
*******************************************************************************/
void STLM75_LedState(FlagStatus NewStatus, uint8_t Indx)
{
	uint16_t Data = 0;

	/* Пишем в регистр 0х03(Tos) или -25С(зажечь), или +125(потушить) */
	if(NewStatus == SET) Data = (25 << 7) | (1 << 15);
	else Data = 125 << 7;

	LM75_Start_WriteReg(3, TempSensor[Indx].Address, Data, 0);
//	HAL_Delay(5);

	/* Возвращаем Pointer на адрес 0х00, регистр температуры T */
	Buffer_Tx2[0] = 0x00;
	I2C_Master_BufferWrite(I2C2, 1, TempSensor[Indx].Address);
//	HAL_Delay(5);
	LM75_Start_ReadReg(0, TempSensor[Indx].Address, 0);
//	HAL_Delay(5);
//	LM75_Start_ReadReg(0, TempSensor[Indx].Address, 0);
}


/*******************************************************************************
* Function Name  : STLM75_LedEnableState
* Description    :
* Input          :
* Return         :
*******************************************************************************/
void STLM75_LedEnableState(FlagStatus NewStatus, uint8_t Index)
{
	if((NewStatus == SET) && (Regulates_Struct[Index].SensorType == STLM75) && (TempSensor[Index].Status == SUCCESS))
	{
		STLM75_IndicateLED[Index] = ENABLE;
		STLM75_LedState(NewStatus, Index);
	}
	else STLM75_IndicateLED[Index] = DISABLE;
}


/*******************************************************************************
* Function Name  : STLM75_LedEnableState
* Description    :
* Input          :
* Return         :
*******************************************************************************/
void STLM75_LedDisableAll(void)
{
	uint8_t i;

	for(i = 0; i < TmpSensNum_MAX; i++)
	{
		STLM75_IndicateLED[i] = DISABLE;
	}
}


/*******************************************************************************
* Function Name  : STLM75_LedEnableState
* Description    :
* Input          :
* Return         :
*******************************************************************************/
static void STLM75_FindAndUpdate_SensorsData(LM75_TypeDef TempSensor, uint8_t OLD_TempStatus)
{
	uint8_t i = Regulates_Max_Count - 2;

	do
	{
		if(Regulate_Get_SensorIndex(i) == OLD_TempSensor_Indx)
		{
			Update_TempSensors_OnScreen_AllSensos(TempSensor.Data >> 1, \
						i, OLD_TempStatus, TempSensor.Status);
		}

	} while (i--);
}

