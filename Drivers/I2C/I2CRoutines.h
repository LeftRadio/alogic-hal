/**
  ******************************************************************************
  * @file    I2CRoutines.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief   header
  ******************************************************************************
**/

#ifndef __I2CROUTINES_H
#define __I2CROUTINES_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* I2C SPE mask */
#define CR1_PE_Set              ((uint16_t)0x0001)
#define CR1_PE_Reset            ((uint16_t)0xFFFE)

/* I2C START mask */
#define CR1_START_Set           ((uint16_t)0x0100)
#define CR1_START_Reset         ((uint16_t)0xFEFF)

#define CR1_POS_Set           ((uint16_t)0x0800)
#define CR1_POS_Reset         ((uint16_t)0xF7FF)

/* I2C STOP mask */
#define CR1_STOP_Set            ((uint16_t)0x0200)
#define CR1_STOP_Reset          ((uint16_t)0xFDFF)

/* I2C ACK mask */
#define CR1_ACK_Set             ((uint16_t)0x0400)
#define CR1_ACK_Reset           ((uint16_t)0xFBFF)

/* I2C ENARP mask */
#define CR1_ENARP_Set           ((uint16_t)0x0010)
#define CR1_ENARP_Reset         ((uint16_t)0xFFEF)

/* I2C NOSTRETCH mask */
#define CR1_NOSTRETCH_Set       ((uint16_t)0x0080)
#define CR1_NOSTRETCH_Reset     ((uint16_t)0xFF7F)

/* I2C registers Masks */
#define CR1_CLEAR_Mask          ((uint16_t)0xFBF5)

/* I2C DMAEN mask */
#define CR2_DMAEN_Set           ((uint16_t)0x0800)
#define CR2_DMAEN_Reset         ((uint16_t)0xF7FF)

/* I2C LAST mask */
#define CR2_LAST_Set            ((uint16_t)0x1000)
#define CR2_LAST_Reset          ((uint16_t)0xEFFF)

/* I2C FREQ mask */
#define CR2_FREQ_Reset          ((uint16_t)0xFFC0)

/* I2C ADD0 mask */
#define OAR1_ADD0_Set           ((uint16_t)0x0001)
#define OAR1_ADD0_Reset         ((uint16_t)0xFFFE)

/* I2C ENDUAL mask */
#define OAR2_ENDUAL_Set         ((uint16_t)0x0001)
#define OAR2_ENDUAL_Reset       ((uint16_t)0xFFFE)

/* I2C ADD2 mask */
#define OAR2_ADD2_Reset         ((uint16_t)0xFF01)

/* I2C F/S mask */
#define CCR_FS_Set              ((uint16_t)0x8000)

/* I2C CCR mask */
#define CCR_CCR_Set             ((uint16_t)0x0FFF)

/* I2C FLAG mask */
#define FLAG_Mask               ((uint32_t)0x00FFFFFF)

/* I2C Interrupt Enable mask */
#define ITEN_Mask               ((uint32_t)0x07000000)


#define I2C_IT_BUF                      ((uint16_t)0x0400)
#define I2C_IT_EVT                      ((uint16_t)0x0200)
#define I2C_IT_ERR                      ((uint16_t)0x0100)

#define  ClockSpeed            			100000

#define I2C_DIRECTION_TX 				0
#define I2C_DIRECTION_RX 				1

//#define OwnAddress1 0x28
//#define OwnAddress2 0x30

#define I2C1_DR_Address              0x40005410
#define I2C2_DR_Address              0x40005810



/* Exported macro ------------------------------------------------------------*/
/* Exported varibles ---------------------------------------------------------*/
/* Buffer of data to be received & transmitted by I2C1 */
extern volatile uint8_t Buffer_Rx1[200];
extern volatile uint8_t Buffer_Tx1[250];

/* Buffer of data to be received & transmitted by I2C1 */
extern volatile uint8_t Buffer_Rx2[10];
extern volatile uint8_t Buffer_Tx2[10];

extern volatile uint32_t I2CDirection;
extern volatile uint32_t NumbOfBytes1, NumbOfBytes2;
extern volatile uint8_t Address;

/* Exported functions ------------------------------------------------------- */
ErrorStatus I2C_Master_BufferRead(I2C_TypeDef* I2Cx, uint32_t NumByteToRead, uint8_t SlaveAddress);
ErrorStatus I2C_Master_BufferWrite(I2C_TypeDef* I2Cx, uint32_t NumByteToWrite, uint8_t SlaveAddress);

void I2C_NVIC_Configuration(void);
void I2C_LowLevel_Init(I2C_TypeDef* I2Cx);
void I2C_LowLevel_Reset(I2C_TypeDef* I2Cx);


#endif /* __I2CROUTINES_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
