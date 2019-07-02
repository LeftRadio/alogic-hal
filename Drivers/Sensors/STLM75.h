/**
  ******************************************************************************
  * @file    I2CRoutines.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief   header
  ******************************************************************************
**/

#ifndef __STLM75___H
#define __STLM75___H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x.h"
#include "I2CRoutines.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint8_t Address;
	uint16_t Data;
	ErrorStatus Status;
} LM75_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define LM75_FLAG_TIMEOUT			((uint32_t)0x1000)
#define LM75_LONG_TIMEOUT			((uint32_t)(10 * LM75_FLAG_TIMEOUT))

#define LM75_REG_TEMP				0x00  /*!< Temperature Register of LM75 */
#define LM75_REG_CONF				0x01  /*!< Configuration Register of LM75 */
#define LM75_REG_THYS				0x02  /*!< Temperature Register of LM75 */
#define LM75_REG_TOS				0x03  /*!< Over-temp Shutdown threshold Register of LM75 */
#define I2C_TIMEOUT					((uint32_t)0x3FFFF) /*!< I2C Time out */
#define LM75_ADDR					0x90   /*!< LM75 base address */

#define TmpSensNum_MAX				8

/* Exported varibles ---------------------------------------------------------*/
extern LM75_TypeDef TempSensor[TmpSensNum_MAX];
extern volatile int8_t OLD_TempSensorIndx;
extern ErrorStatus Sensor_Status, Sensor_Internal_Status;
extern volatile Bool I2C1_ERROR, I2C2_ERROR;
extern Bool Read_STLM75_Flag;
extern FunctionalState STLM75_IndicateLED[TmpSensNum_MAX];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TemeratureSensors_InitDefault(void);
void TemeratureSensor_Read(void);
ErrorStatus LM75_ReadTemp(uint16_t *ReadData, uint8_t Group);
void ReadAll_LM75(void);
void Read_Internal_LM75(void);
void STLM75_LedEnableState(FlagStatus NewStatus, uint8_t Index);
void STLM75_LedDisableAll(void);

//uint8_t Get_LM75_Data(uint8_t *OutSensData);

#endif /* __STLM75___H */

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
