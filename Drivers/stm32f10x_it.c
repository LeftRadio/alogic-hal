/**
  ******************************************************************************
  * @file RTC/src/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version  V2.0.0
  * @date  04/27/2009
  * @brief  Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and 
  *         peripherals interrupt service routine.
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
#include "stm32f10x.h"
#include "clock_calendar.h"
#include "TouchScreen.h"
#include "GlobalInit.h"
#include "User_Interface.h"
#include "usb_istr.h"
#include "STLM75.h"
#include "DHT11.h"
#include "Regulates.h"
#include "Outputs.h"
#include "Host.h"
#include "I2CRoutines.h"
#include "Pparam.h"
#include "Graphics.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern volatile uint32_t TimingDelay;
Bool USB_IstrConnect = FALSE;

extern Bool SecondEvent;
extern Bool FlashingButtonUpdate;

extern volatile uint16_t ADCConvertedValue[100];

extern uint16_t SensGraphics_Count;

/* ------nеременные I2C ----- */
volatile uint8_t Tx_Idx1=0, Rx_Idx1=0;
volatile uint8_t Tx_Idx2=0, Rx_Idx2=0;

volatile uint16_t Bit_HeightCount = 0;
volatile uint16_t Bit_LowCount = 0;
volatile Bool DHT_Read_timeout = 0;
uint8_t i;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void VerifyTimersProgram(uint32_t TimeSeconds);

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
	if (TimingDelay != 0)
	{
		TimingDelay--;
	}
}




// /**
//   * @brief  This function handles RTC_IRQHandler .
//   * @param  None
//   * @retval : None
//   */
// void RTC_IRQHandler(void)
// {
// 	ccccccccccccccc

// 	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
// 	{
// 		NVIC_ClearPendingIRQ(RTC_IRQn);
// 		RTC_ClearITPendingBit(RTC_IT_SEC);
// 		RTC_WaitForLastTask();
    
    

// 	}
// }


/**
  * @brief  This function handles External line 4 interrupt request.
  * @param  None
  * @retval : None
  */
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		/* Clear the  EXTI line 4 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line4);
		NVIC_DisableIRQ(EXTI4_IRQn);

		ADC_Configuration(Touch_X);
		Touch_Start();
	}
}


/**
  * @brief  This function handles DMA1_Channel1_IRQHandler
  * @param  None
  * @retval : None
  */
void DMA1_Channel1_IRQHandler(void)
{
	uint8_t i;
	uint32_t ADC_DATA = 0;
	Bool ADC_Correct = TRUE;

	if(DMA_GetITStatus(DMA1_IT_TC1))
	{
		Touch_Stop();

		for(i = 5; i < 49; i++)
		{
			if((int)ADCConvertedValue[i] - (int)ADCConvertedValue[i+1]  >= 1000)
			{
				ADC_Correct = FALSE;
				break;
			}

			ADC_DATA += ADCConvertedValue[i];
		}

		if(ADC_Correct == FALSE)
		{
			ADC_DATA = 4096;
		}
		else ADC_DATA /= 44;

		Touch_Get_Coordinate(ADC_DATA);
	}

	/* Clear DMA1 Channel 1 Half Transfer, Transfer Complete and Global interrupt pending bits */
	DMA_ClearITPendingBit(DMA1_IT_GL1);
}



/**
  * @brief  This function handles Timer 7 interrupt request.
  * @param  None
  * @retval : None
  */
void TIM7_IRQHandler(void)
{
	static uint8_t FlashingStatus = 0;

	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

		if(FlashingButton.Status == RESET)
		{
			FlashingButtonUpdate = FALSE;
		}
		else
		{
			if(FlashingStatus == 0) FlashingButton.TextColor = Red;
			else FlashingButton.TextColor = Global_BackColor;

			FlashingStatus ^= 1;
			FlashingButtonUpdate = TRUE;
		}
	}
}



/**
  * @brief  This function handles USB Low Priority interrupts
  * @param  None
  * @retval None
  */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	USB_Istr();
	USB_IstrConnect = TRUE;
}





/**
  * @brief  This function handles I2C1 Event interrupt request.
  * @param  None
  * @retval : None
  */
void I2C1_EV_IRQHandler(void)
{
    volatile uint32_t SR1Register = 0;
    volatile uint32_t SR2Register = 0;

    /* Read the I2C1 SR1 and SR2 status registers */
    SR1Register = I2C1->SR1;
    SR2Register = I2C1->SR2;


    /* If SB = 1, I2C1 master sent a START on the bus: EV5) */
    if ((SR1Register & 0x0001) == 0x0001)
    {
        /* Send the slave address for transmssion or for reception (according to the configured value
            in the write master write routine */
        I2C1->DR = Address;
        SR1Register = 0;
        SR2Register = 0;
    }

    /* If I2C1 is Master (MSL flag = 1) */
    else if ((SR2Register & 0x0001) == 0x0001)
    {
        /* If ADDR = 1, EV6 */
        if ((SR1Register & 0x0002) == 0x0002)
        {
            /* Write the first data in case the Master is Transmitter */
            if (I2CDirection == I2C_DIRECTION_TX)
            {
                /* Initialize the Transmit counter */
                Tx_Idx1 = 0;
                /* Write the first data in the data register */
                I2C1->DR = Buffer_Tx1[Tx_Idx1++];
                /* Decrement the number of bytes to be written */
                NumbOfBytes1--;
                /* If no further data to be sent, disable the I2C BUF IT
                in order to not have a TxE  interrupt */
                if (NumbOfBytes1 == 0)
                {
                    I2C1->CR2 &= (uint16_t)~I2C_IT_BUF;
                }
            }
            /* Master Receiver */
            else
            {
                /* Initialize Receive counter */
                Rx_Idx1 = 0;
                /* At this stage, ADDR is cleared because both SR1 and SR2 were read.*/
                /* EV6_1: used for single byte reception. The ACK disable and the STOP
                Programming should be done just after ADDR is cleared. */
                if (NumbOfBytes1 == 1)
                {
                    /* Clear ACK */
                    I2C1->CR1 &= CR1_ACK_Reset;
                    /* Program the STOP */
                    I2C1->CR1 |= CR1_STOP_Set;
                }
            }
            SR1Register = 0;
            SR2Register = 0;
        }
        /* Master transmits the remaing data: from data2 until the last one.  */
        /* If TXE is set */
        if ((SR1Register & 0x0084) == 0x0080)
        {
            /* If there is still data to write */
            if (NumbOfBytes1 != 0)
            {
                /* Write the data in DR register */
                I2C1->DR = Buffer_Tx1[Tx_Idx1++];
                /* Decrment the number of data to be written */
                NumbOfBytes1--;
                /* If  no data remains to write, disable the BUF IT in order
                to not have again a TxE interrupt. */
                if (NumbOfBytes1 == 0)
                {
                    /* Disable the BUF IT */
                    I2C1->CR2 &= (uint16_t)~I2C_IT_BUF;
                }
            }
            SR1Register = 0;
            SR2Register = 0;
        }
        /* If BTF and TXE are set (EV8_2), program the STOP */
        if ((SR1Register & 0x0084) == 0x0084)
        {
            /* Program the STOP */
            I2C1->CR1 |= CR1_STOP_Set;
            /* Disable EVT IT In order to not have again a BTF IT */
            I2C1->CR2 &= (uint16_t)~I2C_IT_EVT;
            SR1Register = 0;
            SR2Register = 0;
        }
        /* If RXNE is set */
        if ((SR1Register & 0x0040) == 0x0040)
        {
            /* Read the data register */
            Buffer_Rx1[Rx_Idx1++] = I2C1->DR;
            /* Decrement the number of bytes to be read */
            NumbOfBytes1--;
            /* If it remains only one byte to read, disable ACK and program the STOP (EV7_1) */
            if (NumbOfBytes1 == 1)
            {
                /* Clear ACK */
                I2C1->CR1 &= CR1_ACK_Reset;
                /* Program the STOP */
                I2C1->CR1 |= CR1_STOP_Set;

                if(Read_STLM75_Flag == TRUE) Sensor_Internal_Status = SUCCESS;
                else Read_I2C1 = SUCCESS;
            }
            SR1Register = 0;
            SR2Register = 0;
        }
    }
    else I2C_LowLevel_Reset(I2C1);
}


/**
  * @brief  This function handles I2C1 Error interrupt request.
  * @param  None
  * @retval : None
  */
void I2C1_ER_IRQHandler(void)
{
    volatile uint32_t SR1Register = 0;

    /* Read the I2C1 status register */
    SR1Register = I2C1->SR1;

    /* If AF = 1, NACK event */
    if ((SR1Register & 0x0400) == 0x0400)
    {
    	I2C1->SR1 &= 0xFBFF;	/* Clear ACK */
    	SR1Register = 0;

    	/* Program the STOP */
    	I2C1->CR1 |= CR1_STOP_Set;
    }
    else
    {
    	/* If ARLO = 1 */
    	if ((SR1Register & 0x0200) == 0x0200)
    	{
    		I2C1->SR1 &= 0xFBFF;
    		SR1Register = 0;
    	}
    	/* If BERR = 1 */
    	if ((SR1Register & 0x0100) == 0x0100)
    	{
    		I2C1->SR1 &= 0xFEFF;
    		SR1Register = 0;
    	}
    	/* If OVR = 1 */
    	if ((SR1Register & 0x0800) == 0x0800)
    	{
    		I2C1->SR1 &= 0xF7FF;
    		SR1Register = 0;
    	}

    	I2C_LowLevel_Reset(I2C1);
    }

    if(Read_STLM75_Flag == TRUE) Sensor_Internal_Status = ERROR;
    else Read_I2C1 = ERROR;
}




/**
  * @brief  This function handles I2C1 Event interrupt request.
  * @param  None
  * @retval : None
  */
void I2C2_EV_IRQHandler(void)
{
    volatile uint32_t SR1Register = 0;
    volatile uint32_t SR2Register = 0;

    /* Read the I2C1 SR1 and SR2 status registers */
    SR1Register = I2C2->SR1;
    SR2Register = I2C2->SR2;


    /* If SB = 1, I2C1 master sent a START on the bus: EV5) */
    if ((SR1Register & 0x0001) == 0x0001)
    {
        /* Send the slave address for transmssion or for reception (according to the configured value
            in the write master write routine */
        I2C2->DR = Address;
        SR1Register = 0;
        SR2Register = 0;
    }

    /* If I2C2 is Master (MSL flag = 1) */
    else if ((SR2Register & 0x0001) == 0x0001)
    {
        /* If ADDR = 1, EV6 */
        if ((SR1Register & 0x0002) == 0x0002)
        {
            /* Write the first data in case the Master is Transmitter */
            if (I2CDirection == I2C_DIRECTION_TX)
            {
                /* Initialize the Transmit counter */
                Tx_Idx2 = 0;
                /* Write the first data in the data register */
                I2C2->DR = Buffer_Tx2[Tx_Idx2++];
                /* Decrement the number of bytes to be written */
                NumbOfBytes2--;
                /* If no further data to be sent, disable the I2C BUF IT
                in order to not have a TxE interrupt */
                if (NumbOfBytes2 == 0)
                {
                    I2C2->CR2 &= (uint16_t)~I2C_IT_BUF;
                }
            }
            else // Master Receiver
            {
                /* Initialize Receive counter */
                Rx_Idx2 = 0;
                /* At this stage, ADDR is cleared because both SR1 and SR2 were read.*/
                /* EV6_1: used for single byte reception. The ACK disable and the STOP
                Programming should be done just after ADDR is cleared. */
                if (NumbOfBytes2 == 1)
                {
                    /* Clear ACK */
                    I2C2->CR1 &= CR1_ACK_Reset;
                    /* Program the STOP */
                    I2C2->CR1 |= CR1_STOP_Set;
                }
            }

            SR1Register = 0;
            SR2Register = 0;
        }

        /* Master transmits the remaing data: from data2 until the last one.  */
        /* If TXE is set */
        if ((SR1Register &0x0084) == 0x0080)
        {
            /* If there is still data to write */
            if (NumbOfBytes2 != 0)
            {
                /* Write the data in DR register */
                I2C2->DR = Buffer_Tx2[Tx_Idx2++];
                /* Decrment the number of data to be written */
                NumbOfBytes2--;
                /* If  no data remains to write, disable the BUF IT in order
                to not have again a TxE interrupt. */
                if (NumbOfBytes2 == 0)
                {
                    /* Disable the BUF IT */
                    I2C2->CR2 &= (uint16_t)~I2C_IT_BUF;
                }
            }
            SR1Register = 0;
            SR2Register = 0;
        }

        /* If BTF and TXE are set (EV8_2), program the STOP */
        if ((SR1Register &0x0084) == 0x0084)
        {
            /* Program the STOP */
            I2C2->CR1 |= CR1_STOP_Set;
            /* Disable EVT IT In order to not have again a BTF IT */
            I2C2->CR2 &= (uint16_t)~I2C_IT_EVT;
            SR1Register = 0;
            SR2Register = 0;
        }

        /* If RXNE is set */
        if ((SR1Register &0x0040) == 0x0040)
        {
            /* Read the data register */
            Buffer_Rx2[Rx_Idx2++] = I2C2->DR;
            /* Decrement the number of bytes to be read */
            NumbOfBytes2--;

            /* If it remains only one byte to read, disable ACK and program the STOP (EV7_1) */
            if (NumbOfBytes2 == 1)
            {
                /* Clear ACK */
                I2C2->CR1 &= CR1_ACK_Reset;
                /* Program the STOP */
                I2C2->CR1 |= CR1_STOP_Set;

                Read_I2C2 = Sensor_Status = SUCCESS;
            }

            SR1Register = 0;
            SR2Register = 0;
        }
    }
    else I2C_LowLevel_Reset(I2C2);
}


/**
  * @brief  This function handles I2C2 Error interrupt request.
  * @param  None
  * @retval : None
  */
void I2C2_ER_IRQHandler(void)
{
    volatile uint32_t SR1Register = 0;
//    static volatile uint8_t reset = 0;

    /* Read the I2C1 status register */
    SR1Register = I2C2->SR1;

    /* If AF = 1, NACK event */
    if ((SR1Register & 0x0400) == 0x0400)
    {
        I2C2->SR1 &= 0xFBFF;	/* Clear ACK */
        SR1Register = 0;

        /* Program the STOP */
        I2C2->CR1 |= CR1_STOP_Set;
    }
    else
    {
    	/* If ARLO = 1 */
    	if ((SR1Register & 0x0200) == 0x0200)
    	{
    		I2C2->SR1 &= 0xFBFF;
    		SR1Register = 0;
    	}
    	/* If BERR = 1 */
    	if ((SR1Register & 0x0100) == 0x0100)
    	{
    		I2C2->SR1 &= 0xFEFF;
    		SR1Register = 0;
    	}
    	/* If OVR = 1 */
    	if ((SR1Register & 0x0800) == 0x0800)
    	{
    		I2C2->SR1 &= 0xF7FF;
    		SR1Register = 0;
    	}

    	I2C_LowLevel_Reset(I2C2);
//    	I2C2_ERROR = TRUE;
    }

    Read_I2C2 = Sensor_Status = ERROR;
}




/**
  * @brief  This function handles TIM3 interrupt request.
  * @param  None
  * @retval : None
  */
void TIM2_IRQHandler(void)
{
	static uint8_t i = 0;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		i++;
		if(i >= 5)
		{
			DHT_Read_timeout = TRUE;
			TIM_Cmd(TIM2, DISABLE);
			i = 0;
		}

		TIM_SetCounter(TIM2, 0);
	}
}


/**
  * @brief  This function handles TIM3 interrupt request.
  * @param  None
  * @retval : None
  */
void TIM8_UP_IRQHandler(void)
{
	uint8_t i;
	static uint8_t ticks = 0;

	if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
	{
		TIM8->SR = (uint16_t)~TIM_IT_Update;

		for(i = 0; i < 9; i++)
		{
			if(Regulates_Struct[i].State == ENABLE)
			{
				if(Regulates_Struct[i].Out_Index != Regulates_OUT_NotAssigned)
				{
					if(Regulates_Struct[i].PWM_State == ENABLE)
					{
						if(ticks >= (Regulates_Struct[i].PWM/10)) OUT_PWM_OFF(Regulates_Struct[i].Out_Index);
						else OUT_PWM_ON(Regulates_Struct[i].Out_Index);

						if(StructOuts[Regulates_Struct[i].Out_Index].ManualState != SET)
						{
							StructOuts[Regulates_Struct[i].Out_Index].ManualState = SET;
							Update_Out_Flag[Regulates_Struct[i].Out_Index] = TRUE;
						}
					}
					else
					{
						OUT_PWM_OFF(Regulates_Struct[i].Out_Index);

						if(StructOuts[Regulates_Struct[i].Out_Index].ManualState != RESET)
						{
							StructOuts[Regulates_Struct[i].Out_Index].ManualState = RESET;
							Update_Out_Flag[Regulates_Struct[i].Out_Index] = TRUE;
						}
					}
				}
			}
		}

		ticks++;
		if(ticks >= 10) ticks = 0;
	}
}




/**
  * @brief  This function handles TIM3 interrupt request.
  * @param  None
  * @retval : None
  */
void TIM4_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

	if((USB_IstrConnect == TRUE) && (HostProtocol == BT_MODE))
	{
		Switch_host_Mode(USB_MODE);
	}
	else if((USB_IstrConnect == FALSE) && (HostProtocol == USB_MODE))
	{
		Switch_host_Mode(BT_MODE);
	}

	USB_IstrConnect = FALSE;
}


/**
  * @brief  This function handles TIM3 interrupt request.
  * @param  None
  * @retval : None
  */
void BT_USART_IRQHandler(void)
{
	static uint16_t cNumBytes = 0;

	if(USART_GetITStatus(BT_USART, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		HostIN_Data[cNumBytes] = USART_ReceiveData(BT_USART);


		if((HostIN_Data[cNumBytes - 1] == (uint8_t)'\r') && (HostIN_Data[cNumBytes] == (uint8_t)'\n'))
		{
			SendHostData = TRUE;
			HostIN_Nb_bytes = HostIN_Data[2] + 4;
			cNumBytes = 0;
			return;
		}

		if(cNumBytes++ > 256) cNumBytes = 0;
	}
}



/******************* (C) COPYRIGHT 2013 *****END OF FILE****/
