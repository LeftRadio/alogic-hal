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
#include "DHT11.h"
#include "GlobalInit.h"
#include "STLM75.h"
#include "Regulates.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SHT_I2C_ADDRESS				0x80

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile DHT_TypeDef *HTSensor;
volatile DHT_TypeDef DHTSensor;
volatile DHT_TypeDef SHTSensor;

ErrorStatus Read_I2C1 = ERROR, Read_I2C2 = ERROR;

uint16_t Bit_Height = 0;
uint16_t Bit_Low = 0;

uint16_t DHT_Microseconds;

extern volatile Bool DHT_Read_timeout;

/* Private function prototypes -----------------------------------------------*/
uint8_t DHT_ReadCycle(uint16_t us);
void EXTI_Change(EXTITrigger_TypeDef New_Trigger_Mode);
uint16_t uint16_time_diff(uint16_t now, uint16_t before);
ErrorStatus DHT_Read(uint8_t* ReadData, uint8_t mode);
ErrorStatus SHT_Read(uint8_t* ReadData, uint8_t mode);
void DHT_DeInit(void);
void Select_HT_Sensor(void);

/* Functions ----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void DHT_Init(GPIOMode_TypeDef Mode) {
	/* */	
	DHTSensor.Status = SUCCESS;
	DHTSensor.Status = DHT_Read(DHTSensor.Data, 0);
	/* */
	Select_HT_Sensor();
}

/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void SHT_Init(Bool Read) {
	/* */
	SHTSensor.Status = SUCCESS;
	if(Read == TRUE) SHTSensor.Status = SHT_Read(SHTSensor.Data, RH_measurement_NO_Hold);

	Select_HT_Sensor();
}

/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Select_HT_Sensor(void) {
	if(Regulates_Struct[8].SensorIndex == 8) {
		HTSensor = &DHTSensor;
		HTSensor->Status = DHTSensor.Status;
	}
	else {
		HTSensor = &SHTSensor;
		HTSensor->Status = SHTSensor.Status;
	}
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void DHT_DeInit(void) {
	/* */
	DHTSensor.Status = ERROR;
}

/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
ErrorStatus HHT_Read(uint8_t* ReadData, uint8_t mode) {
#ifdef __HYMIDITY_DHT_
	return DHT_Read(ReadData, mode);
#else
	return SHT_Read(ReadData, mode);
#endif
}


/*******************************************************************************
* Function Name  : DHT Sensor Read
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
ErrorStatus DHT_Read(uint8_t* ReadData, uint8_t mode)
{
	uint8_t i, j;
	int8_t k;
	__IO uint8_t tmpData[5] = {0};
	__IO uint16_t buf[100] = {0};

	if(DHTSensor.Status == ERROR) return ERROR;

	DHT_Read_timeout = FALSE;

	//reset DHT11
	HAL_GPIO_WritePin(GPIOA, GPIO_Pin_1, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(GPIOA, GPIO_Pin_1, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_Pin_1) == Bit_SET){;}

	//start reading
//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM2->CNT = 1;
	TIM2->CR1 |= TIM_CR1_CEN;			// Start TIM2 while waiting for sensor response

	while((HAL_GPIO_ReadPin(GPIOA, GPIO_Pin_1) == Bit_RESET) && (DHT_Read_timeout == FALSE));
	if(DHT_Read_timeout == TRUE) return ERROR;
	TIM2->CNT = 1;
	while((HAL_GPIO_ReadPin(GPIOA, GPIO_Pin_1) == Bit_SET) && (DHT_Read_timeout == FALSE));
	if(DHT_Read_timeout == TRUE) return ERROR;

	// считываем 40 бит
	for(i = 0; i <= 80; i += 2)
	{
		TIM2->CNT = 1;
		while((HAL_GPIO_ReadPin(GPIOA, GPIO_Pin_1) == Bit_RESET) && (DHT_Read_timeout == FALSE)); // If there's no response within 256us, the Timer2 overflows
		if(DHT_Read_timeout == TRUE) break;
		buf[i] = TIM2->CNT - 3;

		TIM2->CNT = 1;
		while((HAL_GPIO_ReadPin(GPIOA, GPIO_Pin_1) == Bit_SET) && (DHT_Read_timeout == FALSE)); // If there's no response within 256us, the Timer2 overflows
		if(DHT_Read_timeout == TRUE) break;
		buf[i+1] = TIM2->CNT - 3;
	}

	if(DHT_Read_timeout == TRUE) return ERROR;
	TIM2->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));

	// конвертируем
    k = 7; j = 0;
	for(i = 2; i < 80; i += 2)
	{
		/* измеряем длительность нуля "передачи байта" */
		if(!((buf[i] >= 40) && (buf[i] <= 60)))	return ERROR;
		else
		{
			/* измеряем длительность единицы и определяем 0(25us) или 1(75us) */
			if(buf[i+1] >= 40)  tmpData[j] |= (1 << k);
			k--;
			if(k < 0)
			{
				*ReadData = tmpData[j];
				ReadData++;
				j++;
				k = 7;
			}
		}

	}

	// Если контрольная сумма совпала значит все ОК, иначе вернуть ошибку
	//if (tmpData[4] == ((tmpData[0] + tmpData[1] + tmpData[2] + tmpData[3]) & 0xFF)) return SUCCESS;
	//else return ERROR;

	return SUCCESS;
}


//void SHT_HAL_Delay(volatile uint32_t count)
//{
//	volatile i = count * 10000;
//	while(i--);
//}


/*******************************************************************************
* Function Name  : SHT Sensor Read
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
ErrorStatus SHT_Read(uint8_t* ReadData, uint8_t mode)
{
	volatile float Results;

	uint32_t EXT4_irq = NVIC_GetActive(EXTI4_IRQn);
	NVIC_DisableIRQ(EXTI4_IRQn);

	if(SHTSensor.Status == ERROR) return ERROR;
	else
	{
		Buffer_Tx2[0] = mode;
		SHTSensor.Status = I2C_Master_BufferWrite(I2C2, 1, SHT_I2C_ADDRESS);
		HAL_Delay(100);
		SHTSensor.Status = I2C_Master_BufferRead(I2C2, 3, SHT_I2C_ADDRESS);
		HAL_Delay(10);
//		SHTSensor.Status = Read_I2C2;

		if(SHTSensor.Status == ERROR)
		{
			*ReadData = 0;
		}
		else if(SHTSensor.Status == SUCCESS)
		{
			Results = (((uint16_t)Buffer_Rx2[0] << 8) + ((uint16_t)Buffer_Rx2[1] & 0xFC));

			if(mode == RH_measurement_NO_Hold)
			{
				*ReadData = (uint8_t)(-6.0 + ((125.0 * Results) / 65536.0));
			}
			else if(mode == T_measurement_NO_Hold)
			{
				*ReadData = (uint8_t)(-46.85 + ((175.72 * Results) / 65536.0));
			}
		}
	}

//	if(EXT4_irq == EXTI4_IRQn)
	NVIC_EnableIRQ(EXTI4_IRQn);

	return SHTSensor.Status;
}


