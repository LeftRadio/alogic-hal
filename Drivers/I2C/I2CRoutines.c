/**
   ******************************************************************************
   * @file      I2CRoutines.c
   * @author	LeftRadio
   * @version 	V1.0.0
   * @date      15.04.2013 23:08:21
   * @brief
   ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "I2CRoutines.h"
#include "User_Interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define timeout				3000000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t I2CDirection = I2C_DIRECTION_TX;
volatile uint32_t NumbOfBytes1, NumbOfBytes2;
volatile uint8_t Address;

/* Buffer of data to be received by I2C1 */
volatile uint8_t Buffer_Rx1[200];
/* Buffer of data to be transmitted by I2C1 */
volatile uint8_t Buffer_Tx1[250] = {0x0};

/* Buffer of data to be received by I2C2 */
volatile uint8_t Buffer_Rx2[10];
/* Buffer of data to be transmitted by I2C2 */
volatile uint8_t Buffer_Tx2[10] = {0x0};

extern volatile uint8_t Tx_Idx1 , Rx_Idx1;
extern volatile uint8_t Tx_Idx2 , Rx_Idx2;

extern volatile Bool I2C1_ERROR, I2C2_ERROR;

/* Private function prototypes -----------------------------------------------*/
extern void TemeratureSensors_InitDefault(void);

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Reads buffer of bytes  from the slave.
  * @param pBuffer: Buffer of bytes to be read from the slave.
  * @param NumByteToRead: Number of bytes to be read by the Master.
  * @param SlaveAddress: The address of the slave to be addressed by the Master.
  * @retval : None.
  */
ErrorStatus I2C_Master_BufferRead(I2C_TypeDef* I2Cx, uint32_t NumByteToRead, uint8_t SlaveAddress)
{
	volatile int32_t Timeout = 0;

    /* Enable I2C errors interrupts */
    I2Cx->CR2 |= I2C_IT_ERR;

    // I2Cx Master Reception using Interrupts with highest priority in an application

    /* Enable EVT IT*/
    I2Cx->CR2 |= I2C_IT_EVT;

    /* Enable BUF IT */
    I2Cx->CR2 |= I2C_IT_BUF;

    /* Set the I2C direction to reception */
    I2CDirection = I2C_DIRECTION_RX;
    SlaveAddress |= OAR1_ADD0_Set;
    Address = SlaveAddress;

    if (I2Cx == I2C1) NumbOfBytes1 = NumByteToRead;
    else NumbOfBytes2 = NumByteToRead;

    /* Send START condition */
    I2Cx->CR1 |= CR1_START_Set;

    /* Wait until the START condition is generated on the bus: START bit is cleared by hardware */
    Timeout = timeout;
    while (((I2Cx->CR1 & 0x100) == 0x100) && (Timeout-- > 0));
    if(Timeout <= 0)
    {
    	I2C_LowLevel_Reset(I2Cx);
    	return ERROR;
    }

    /* Wait until BUSY flag is reset (until a STOP is generated) */
    Timeout = timeout;
    while (((I2Cx->SR2 & 0x0002) == 0x0002) && (Timeout-- > 0));
    if(Timeout <= 0)
    {
    	I2C_LowLevel_Reset(I2Cx);
    	return ERROR;
    }

    /* Enable Acknowledgement to be ready for another reception */
    I2Cx->CR1 |= CR1_ACK_Set;

    return SUCCESS;
}



/**
  * @brief  Writes buffer of bytes.
  * @param pBuffer: Buffer of bytes to be sent to the slave.
  * @param NumByteToWrite: Number of bytes to be sent by the Master.
  * @param SlaveAddress: The address of the slave to be addressed by the Master.
  * @retval : None.
  */
ErrorStatus I2C_Master_BufferWrite(I2C_TypeDef* I2Cx, uint32_t NumByteToWrite, uint8_t SlaveAddress )
{
	volatile int32_t Timeout = 0;

	/* Enable Error IT */
	I2Cx->CR2 |= I2C_IT_ERR;

	/* I2Cx Master Transmission using Interrupt with highest priority in the application */

	/* Enable EVT IT*/
	I2Cx->CR2 |= I2C_IT_EVT;

	/* Enable BUF IT */
	I2Cx->CR2 |= I2C_IT_BUF;

	/* Set the I2C direction to Transmission */
	I2CDirection = I2C_DIRECTION_TX;
	SlaveAddress &= OAR1_ADD0_Reset;
	Address = SlaveAddress;

	if (I2Cx == I2C1) NumbOfBytes1 = NumByteToWrite;
	else NumbOfBytes2 = NumByteToWrite;

	/* Send START condition */
	I2Cx->CR1 |= CR1_START_Set;

	/* Wait until the START condition is generated on the bus: START bit is cleared by hardware */
	Timeout = timeout;
	while (((I2Cx->CR1 & 0x100) == 0x100) && (Timeout-- > 0)){;}
	if(Timeout <= 0)
	{
		I2C_LowLevel_Reset(I2Cx);
		return ERROR;
	}

	/* Wait until BUSY flag is reset (until a STOP is generated) */
	Timeout = timeout;
	while (((I2Cx->SR2 & 0x0002) == 0x0002) && (Timeout-- > 0)){;}
	if(Timeout <= 0)
	{
		I2C_LowLevel_Reset(I2Cx);
		return ERROR;
	}

//	/* Wait until the START condition is generated on the bus: the START bit is cleared by hardware */
//	while ((I2Cx->CR1 & 0x100) == 0x100);
//
//	/* Wait until BUSY flag is reset: a STOP has been generated on the bus signaling the end
//        of transmission */
//	while ((I2Cx->SR2 & 0x0002) == 0x0002);

	return SUCCESS;
}


/*******************************************************************************
* Function Name  : I2C_NVIC_Configuration
* Description    : I2C_NVIC_Configuration
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_NVIC_Configuration(void)
{
    /* 1 bit for pre-emption priority, 3 bits for subpriority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    NVIC_SetPriority(I2C1_EV_IRQn, 0x00);
    NVIC_EnableIRQ(I2C1_EV_IRQn);

    NVIC_SetPriority(I2C1_ER_IRQn, 0x01);
    NVIC_EnableIRQ(I2C1_ER_IRQn);


    NVIC_SetPriority(I2C2_EV_IRQn, 0x00);
    NVIC_EnableIRQ(I2C2_EV_IRQn);

    NVIC_SetPriority(I2C2_ER_IRQn, 0x01);
    NVIC_EnableIRQ(I2C2_ER_IRQn);
}


/*******************************************************************************
* Function Name  : I2C_LowLevel_Init
* Description    : Initializes peripherals: I2Cx
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_LowLevel_Init(I2C_TypeDef* I2Cx)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    I2C_InitTypeDef  I2C_InitStructure;

    if (I2Cx == I2C1)
    {
        /* I2C1 clock enable */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

        /* I2C1 SDA and SCL configuration */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* Enable I2C1 reset state */
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
        /* Release I2C1 from reset state */
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
    }

    else /* I2Cx = I2C2 */
    {
        /* I2C2 clock enable */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

        /* I2C1 SDA and SCL configuration */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* Enable I2C1 reset state */
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
        /* Release I2C1 from reset state */
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
    }

    /* I2C1 and I2C2 configuration */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
    I2C_Init(I2Cx, &I2C_InitStructure);

    /*!< Enable interrupt */
    I2C_ITConfig(I2Cx, I2C_IT_EVT | I2C_IT_ERR, ENABLE);

    /*!< I2C Enable */
    I2C_Cmd(I2Cx, ENABLE);
}


/*******************************************************************************
* Function Name  : I2C_LowLevel_Reset
* Description    : ReInitializes peripherals(Reset): I2Cx
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_LowLevel_Reset(I2C_TypeDef* I2Cx)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_1);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	I2C_Cmd(I2Cx, DISABLE);
	I2C_DeInit(I2Cx);

	if(I2Cx == I2C1)
	{
		/* I2C1 clock disable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, DISABLE);

		/* I2C1 SDA and SCL pins default configuration */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	}
	else
	{
		/* I2C2 clock disable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, DISABLE);

		/* I2C1 SDA and SCL pins default configuration */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	}

	GPIO_Init(GPIOB, &GPIO_InitStructure);


	I2C_LowLevel_Init(I2Cx);
	TemeratureSensors_InitDefault();

	if(I2Cx == I2C1)
	{
		I2C1_ERROR = TRUE;
		LCD_ClearArea(420, 70, 470, 125, Global_BackColor);
	}
	else
	{
		I2C2_ERROR = TRUE;
		SHT_Init(FALSE);
		LCD_ClearArea(150, 70, 210, 290, Global_BackColor);
	}
}





/**
  * @}
  */


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
