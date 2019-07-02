/*************************************************************************************
*
Description :  
Version     :  1.0.0
Date        :  
Author      :  Left Radio                          
Comments:  
*
**************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "f_eeprom.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define EEPROM_I2C_ADDRESS			0xA0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/*******************************************************************************
 * Function Name  : Error_message
 * Description    :
 * Input          :
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SaveSettings(uint16_t StartAddress, uint16_t DataNum, uint16_t *Data)
{
	static uint16_t ControlByte = 0xA5FD;
	uint8_t i;

	__disable_irq();	// запретить все прерывания

	/* Unlock the Flash Program Erase controller */
	HAL_FLASH_Unlock();

	/* записваем контрольное число */
	EE_WriteVariable(1, ControlByte);

	for(i = 0; i < DataNum; i++)
	{
		EE_WriteVariable(i + 1, *Data);
		Data++;
	}

	HAL_FLASH_Lock();
	HAL_Delay(1);

	__enable_irq();	// разрешить все прерывания
}





// ------------------------------------- Чтение параметров ----------------------------------------- //

/*******************************************************************************
 * Function Name  : ReadTimerSettings
 * Description    : вывод сообщения о ошибке записи/чтения EEPROM
 * Input          : *message_text  	-  указатель на текст ошибки
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ReadTimerSettings(void)
{
	static uint16_t control_temp_save_byte = 0;
	uint8_t i;

	__disable_irq();	// запретить все прерывания

	/* Unlock the Flash Program Erase controller */
	HAL_FLASH_Unlock();

	EE_ReadVariable(250, &control_temp_save_byte);
	if(control_temp_save_byte != 0xA5)
	{
		HAL_FLASH_Lock();
		__enable_irq ();
		return;
	}

	for(i = 0; i < 16; i++)
	{
		EE_ReadVariable(1 + i,  &timerON_Hours[i]);
		EE_ReadVariable(17 + i, &timerON_Minutes[i]);
		EE_ReadVariable(33 + i, &timerOFF_Hours[i]);
		EE_ReadVariable(49 + i, &timerOFF_Minutes[i]);
		EE_ReadVariable(65 + i, &timerOFF_Seconds[i]);
	}

	HAL_FLASH_Lock();
	__enable_irq();	// разрешить все прерывания
}


