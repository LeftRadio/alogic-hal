/**
   ******************************************************************************
   * @file      Host.c
   * @author	LeftRadio
   * @version 	V1.0.0
   * @date      14.04.2013 20:27:59
   * @brief
   ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "Host.h"
#include "User_Interface.h"
#include "Pparam.h"
#include "clock_calendar.h"
#include "STLM75.h"
#include "DHT11.h"
#include "Outputs.h"
#include "Regulates.h"
#include "GlobalInit.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void USB_SendData_Byte(uint8_t Byte);
void USB_SendData(uint8_t *pData, uint16_t Len);
void BT_SendData(uint8_t *pData, uint16_t Len);
void BT_SendData_Byte(uint8_t Byte);

void (*Send_DataByte_to_Host)(uint8_t data) = USB_SendData_Byte;
void (*Send_Data_to_Host)(uint8_t *pData, uint16_t Len) = USB_SendData;

static uint8_t Get_Sensors_Data(uint8_t *OutSensData);

uint8_t CRC8_Buff(uint8_t startCRC, uint8_t *pBuff, uint16_t NumBytes);
uint8_t CRC8(uint8_t Byte, uint8_t crc);

/* Private variables ---------------------------------------------------------*/
uint8_t  HostOUT_Data [HostOUT_RX_DATA_SIZE] = {0};
uint32_t HostOUT_Rx_ptr_in = 0;
uint32_t HostOUT_Rx_ptr_out = 0;
uint32_t HostOUT_Rx_length  = 0;

uint8_t HostIN_Data[256];
uint16_t HostIN_Nb_bytes = 0;
uint8_t HostProtocol = USB_MODE;

uint8_t RespondData[500] = {0x5B, 0x00};

volatile Bool SendHostData = FALSE;
volatile Bool Host_Pause = FALSE;


/* Functions ----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : USB_SendData
* Description    :
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_SendData(uint8_t *pData, uint16_t Len)
{
	uint8_t i;
	uint8_t Control_CRC = 0;

	Control_CRC = CRC8_Buff(Control_CRC, pData, Len);
	(*(pData + Len)) = Control_CRC;
	(*(pData + Len + 1)) = (uint8_t)'\r';
	(*(pData + Len + 2)) = (uint8_t)'\n';
	Len += 3;

	for(i = 0; i < Len; i++)
	{
		HostOUT_Data[HostOUT_Rx_ptr_in] = *pData;
		HostOUT_Rx_ptr_in++;
		pData++;

		/* To avoid buffer overflow */
		if(HostOUT_Rx_ptr_in == HostOUT_RX_DATA_SIZE) HostOUT_Rx_ptr_in = 0;
	}
}


/*******************************************************************************
* Function Name  : USB_SendData_Byte
* Description    : transmit data one byte
* Input          : None.
* Return         : none.
*******************************************************************************/
void USB_SendData_Byte(uint8_t data)
{
  	HostOUT_Data[HostOUT_Rx_ptr_in] = data;
	HostOUT_Rx_ptr_in++;

	/* To avoid buffer overflow */
	if(HostOUT_Rx_ptr_in == HostOUT_RX_DATA_SIZE) HostOUT_Rx_ptr_in = 0;
}


/*******************************************************************************
* Function Name  : BT_SendData
* Description    : transmit data
* Input          : pointer *pData
* Return         : None.
*******************************************************************************/
void BT_SendData(uint8_t *pData, uint16_t Len)
{
	uint8_t j;
	uint8_t Control_CRC = 0;

	Control_CRC = CRC8_Buff(Control_CRC, pData, Len);
	(*(pData + Len)) = Control_CRC;
	(*(pData + Len + 1)) = (uint8_t)'\r';
	(*(pData + Len + 2)) = (uint8_t)'\n';
	Len += 3;

	for(j = 0; j < Len; j++)
	{
		BT_USART->DR = ((*pData) & (uint16_t)0x01FF);
		while((BT_USART->SR & USART_FLAG_TXE) == (uint16_t)RESET);
		pData++;
	}
}



/*******************************************************************************
* Function Name  : BT_SendData_Byte
* Description    : transmit data one byte
* Input          :
* Return         : None.
*******************************************************************************/
void BT_SendData_Byte(uint8_t Byte)
{
	/* Send one byte from BT_USART_TX */
	USART_SendData(BT_USART, Byte);

	/* Loop until BT_USART DR register is empty */
	while(USART_GetFlagStatus(BT_USART, USART_FLAG_TXE) == RESET);
}


/*******************************************************************************
* Function Name  : USB_SendData_Byte
* Description    : transmit data one byte
* Input          : None.
* Return         : none.
*******************************************************************************/
void Host_To_MCU_Send_Data(uint8_t *Rx_Buffer, uint8_t Rx_Cnt)
{
  	uint8_t ErrorByte = 0;
  	uint8_t Control_CRC = 0;
  	uint16_t i, j, k;
  	uint16_t X = 0, Y = 0;

  	static uint8_t OutTypeGroup[20] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
  	static uint8_t OutIndexGroup[20] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
  	uint8_t Active_OUT = *(Rx_Buffer+3);
  	uint8_t indx = 255;

  	Control_CRC = CRC8_Buff(Control_CRC, Rx_Buffer, Rx_Cnt - 1);

  	if(Control_CRC == (*(Rx_Buffer + (Rx_Cnt - 1))))
  	{
  		switch(*(Rx_Buffer+1))
  		{

  		/*------------------ инициализация обмена ------------------*/
  		case 0x81:
  			if((*(Rx_Buffer+3) == 0x86) && (*(Rx_Buffer+4) == 0x93))
  			{
  				ErrorByte = 0x81 + 0x40;

  				/* запрещаем мигание */
  				FlashingText_Command(DISABLE);

  				//SetActiveMenu(&HostMenu);
  				Draw_ActiveMenu();
  			}
  			else ErrorByte = 0x7F;

  			*(Rx_Buffer+1) = ErrorByte;
  			Send_Data_to_Host(Rx_Buffer, 5);
  			break;

  		/*------------------ завершение обмена ------------------*/
  		case 0xFC:
  			if((*(Rx_Buffer+3) == 0x86) && (*(Rx_Buffer+4) == 0x93))
  			{
  				Return_ActiveMenu();
  			}
  			break;

  			/*------------------ переход в загрузчик ------------------*/
  		case 0x95:
  			if((*(Rx_Buffer+3) == 0x86) && (*(Rx_Buffer+4) == 0x93))
  			{
  				ErrorByte = 0x95 + 0x40;

  				*(Rx_Buffer+1) = ErrorByte;
  				Send_Data_to_Host(Rx_Buffer, 5);
  				//Send_Data_to_Host(Rx_Buffer, 5);

  				/* запрещаем мигание */
  				FlashingText_Command(DISABLE);

  				LCD_ClearArea(0, 0, 479, 319, Global_BackColor);
  				LCD_PutColorStrig(100, 150, 1, "ЗАГРУЗЧИК", GreenLine);

  				HAL_Delay(200);
  				Global_Periph_DeInit();
  			}
  			else ErrorByte = 0x7F;


  			break;






  		/*------------------ Запрос Всех настроек ------------------*/
  		case 0x35:

  			//
  			RespondData[1] = 0x35 + 0x40;
  			RespondData[2] = 64;

  			uint16_t r_addr_h = (uint16_t)(*(Rx_Buffer+3)) << 8;
  			uint16_t r_addr_l = *(Rx_Buffer+4);

  			if( Read_Block_ToHost(r_addr_h | r_addr_l, &RespondData[3]) != ERROR )
  			{
  				Send_Data_to_Host(RespondData, 67);
  			}

  			break;

  		/*------------------ Запрос ------------------*/
  		case 0x36:

  			//
  			RespondData[1] = 0x36 + 0x40;
  			RespondData[2] = 0x01;
  			RespondData[3] = 0x00;

  			uint16_t s_addr_h = (uint16_t)(*(Rx_Buffer + 3)) << 8;
  			uint16_t s_addr_l = *(Rx_Buffer + 4);

  			while(SendHostData != TRUE);
  			SendHostData = FALSE;

  			if( Write_Block_FromHost(s_addr_h | s_addr_l, Rx_Buffer) != ERROR )
  			{
  				Send_Data_to_Host(RespondData, 4);
  			}

  			break;

  		case 0x37:

  			RespondData[1] = 0x37 + 0x40;
  			RespondData[2] = 0x01;
  			RespondData[3] = 0x00;
  			Send_Data_to_Host(RespondData, 4);

  			HAL_Delay(500); HAL_Delay(500); HAL_Delay(500);
  			NVIC_SystemReset();

  			break;



  		/*------------------ Запрос управления силовыми выходами ------------------*/
  		case 0x10:
  			// Включить или Выключить
  			if((*(Rx_Buffer+4) == 0x00) ||  (*(Rx_Buffer+4) == 0x01))
  			{
  				if(OutIndexGroup[Active_OUT] == 255)
  				{
  					indx = DeAssigned_OUT_FromTimer(Active_OUT);
  					if(indx != 255)
  					{
  						OutTypeGroup[Active_OUT] = 0x00;
  					}
  					else
  					{
  						indx = DeAssigned_OUT_FromRegulate(Active_OUT);
  						if(indx != 255)
  						{
  							OutTypeGroup[Active_OUT] = 0x01;
  						}
  					}

  					OutIndexGroup[Active_OUT] = indx;
  				}

  				if(*(Rx_Buffer+4) == 0) OUT_OFF(Active_OUT);	// Выключить
  				else OUT_ON(Active_OUT);						// Включить
  			}
  			// По программе
  			else if (*(Rx_Buffer+4) == 0x02)
  			{
  				if(OutIndexGroup[Active_OUT] != 255)
  				{
  					if(OutTypeGroup[Active_OUT] == 0x00)
  					{
  						char NewText_0[6] = { "тайм. " };

  						for(k = 0; k < 6; k++)
  						{
  							StructOuts[Active_OUT].FunctionText[k] = NewText_0[k];
  						}

  						// конвертируем номер регулировки в текст и назначаем выходу новый номер регулировки
  						ConvertToString((OutIndexGroup[Active_OUT] + 1), &StructOuts[Active_OUT].FunctionText[5], 2);

  						Timers_Struct[OutIndexGroup[Active_OUT]].Out_Index = Active_OUT;
  					}
  					else if(OutTypeGroup[Active_OUT] == 0x01)
  					{
  						char NewText_1[6] = { "регул." };

  						for(k = 0; k < 6; k++)
  						{
  							StructOuts[Active_OUT].FunctionText[k] = NewText_1[k];
  						}

  						// конвертируем номер регулировки в текст и назначаем выходу новый номер регулировки
  						ConvertToString((OutIndexGroup[Active_OUT] + 1), &StructOuts[Active_OUT].FunctionText[6], 2);

  						Regulates_Struct[OutIndexGroup[Active_OUT]].Out_Index = Active_OUT;
  					}

  					// сбросить запомненое прикрепление
  					OutIndexGroup[Active_OUT] = 255;
  				}
  				else
  				{
  					OutTypeGroup[Active_OUT] = 255;
  				}
  			}

  			RespondData[1] = 0x10 + 0x40;
  			RespondData[2] = 0x01;
  			RespondData[3] = *(Rx_Buffer+3);

  			Send_Data_to_Host(RespondData, 4);
  			break;

  		/*------------------ Запрос состояния силовых выходов ------------------*/
  		case 0x12:

  			RespondData[1] = 0x12 + 0x40;
  			RespondData[2] = 0x03;

  			for(j = 0; j < 3; j++)
  			{
  				RespondData[j + 3] = 0;

  				for(i = 0; i < 8; i++)
  				{
  					k = (j * 8) + i;
  					if(k > 19) break;

  					// читаем состояние выхода
  					if(OUT_SateRead(k) == Bit_SET) RespondData[j + 3] |= (1 << i);
  				}
  			}

  			Send_Data_to_Host(RespondData, 6);
  			break;

  		/*------------------ Запрос синхронизации времени ------------------*/
  		case 0x15:

  			SetDate(*(Rx_Buffer+5), *(Rx_Buffer+4), (*(Rx_Buffer+3)) + 2000);
  			SetTime(*(Rx_Buffer+6), *(Rx_Buffer+7), *(Rx_Buffer+8));

  			RespondData[1] = 0x15 + 0x40;
  			RespondData[2] = 0x01;
  			RespondData[3] = 0x55;
  			Send_Data_to_Host(RespondData, 4);
  			break;

  		/*------------------ Запрос чтения времени ------------------*/
  		case 0x16:

  			Calculate_Time();

  			RespondData[1] = 0x16 + 0x40;
  			RespondData[2] = 3;
  			RespondData[3] = (s_TimeStructVar.HourHigh * 10) + s_TimeStructVar.HourLow;
  			RespondData[4] = (s_TimeStructVar.MinHigh * 10) + s_TimeStructVar.MinLow;
  			RespondData[5] = (s_TimeStructVar.SecHigh * 10) + s_TimeStructVar.SecLow;
  			Send_Data_to_Host(RespondData, 6);
  			break;

  		/*------------------ Запрос чтения датчиков ------------------*/
  		case 0x20:

  			RespondData[1] = 0x20 + 0x40;
  			RespondData[2] = Get_Sensors_Data(&RespondData[3]);

  			Send_Data_to_Host(RespondData, RespondData[2] + 3);
  			break;

  		/*------------------ Запрос на скриншот ------------------*/
  		case 0x30:

  			RespondData[1] = 0x30 + 0x40;
  			RespondData[2] = 0x01;
  			RespondData[3] = 0x70;

  			Host_Pause = TRUE;

//  			NVIC_DisableIRQ(I2C1_EV_IRQn);
//  			NVIC_DisableIRQ(I2C1_ER_IRQn);
//  			NVIC_DisableIRQ(I2C2_EV_IRQn);
//  			NVIC_DisableIRQ(I2C2_ER_IRQn);

  			Send_Data_to_Host(RespondData, 4);

  			break;
  		case 0x31:

  			RespondData[1] = 0x31 + 0x40;
  			RespondData[2] = 0x01;
  			RespondData[3] = 0x71;

  			Send_Data_to_Host(RespondData, 4);
  			Host_Pause = FALSE;

//  			NVIC_EnableIRQ(I2C1_EV_IRQn);
//  			NVIC_EnableIRQ(I2C1_ER_IRQn);
//  			NVIC_EnableIRQ(I2C2_EV_IRQn);
//  			NVIC_EnableIRQ(I2C2_ER_IRQn);

  			break;
  		case 0x32:

  			X = ((uint16_t)(*(Rx_Buffer+3)) << 8) + (uint16_t)(*(Rx_Buffer+4));
  			Y = ((uint16_t)(*(Rx_Buffer+5)) << 8) + (uint16_t)(*(Rx_Buffer+6));

  			RespondData[1] = 0x32 + 0x40;
  			RespondData[2] = 144;

  			LCD_SetCursor(X, Y);
  			LCD_ReadRAM_Prepare();

  			LCD_GetPixel(&RespondData[3], 48);

  			Send_Data_to_Host(RespondData, 144 + 3);


  			break;
  		default:
  			break;
  		}
  	}
}



/*******************************************************************************
* Function Name  : Get_Sensors_Data
* Description    : Функция чтения показаний всех датчиков
* Input          :
* Return         :
*******************************************************************************/
static uint8_t Get_Sensors_Data(uint8_t *OutSensData)
{
	uint8_t i, j = 0;
	uint8_t SensNum = 0;

	/* Опрашиваем все датчики температуры */
	for(i = 0; i < 8; i++)
	{
		/* проверяем статус датчика */
		if(TempSensor[i].Status == SUCCESS)
		{
			*(OutSensData + j) = i;
			*(OutSensData + (j + 1)) = TempSensor[i].Data;
			SensNum++;
			j += 2;
		}
	}

	for(i = 0; i < 2; i++)
	{
		if(DHTSensor.Status != ERROR)
		{
			*(OutSensData + j) = i + 8;
			*(OutSensData + (j + 1)) = DHTSensor.Data[i*2];
			SensNum++;
			j += 2;
		}
	}

	for(i = 0; i < 2; i++)
	{
		if(SHTSensor.Status != ERROR)
		{
			*(OutSensData + j) = i + 8;
			*(OutSensData + (j + 1)) = SHTSensor.Data[i*2];
			SensNum++;
			j += 2;
		}
	}

	return SensNum*2;
}



/*******************************************************************************
* Function Name  : Switch_host_Mode
* Description    :
* Input          : None.
* Return         : None.
*******************************************************************************/
void Switch_host_Mode(uint8_t NEW_MODE)
{
	if(NEW_MODE == USB_MODE)
	{
		/* Disable the BT_USART */
		USART_Cmd(BT_USART, DISABLE);

		/* Disable the BT Module */
		BT_DISABLE();

		Send_DataByte_to_Host = USB_SendData_Byte;
		Send_Data_to_Host = USB_SendData;
		HostProtocol = USB_MODE;
	}
	else if(NEW_MODE == BT_MODE)
	{
		/* Enable the BT_USART */
		USART_Cmd(BT_USART, ENABLE);

		/* Enable the BT Module */
		BT_ENABLE();

		Send_DataByte_to_Host = BT_SendData_Byte;
		Send_Data_to_Host = BT_SendData;

		HostProtocol = BT_MODE;
	}
}


/*******************************************************************************
* Function Name  : CRC8 Buff
* Description    :
* Input          :
* Return         :
*******************************************************************************/
uint8_t CRC8_Buff(uint8_t startCRC, uint8_t *pBuff, uint16_t NumBytes)
{
   uint16_t i;
   volatile uint8_t crcBuff = startCRC;

   for(i = 0; i < NumBytes; i++)
   {
	   crcBuff = CRC8(*pBuff, crcBuff);
	   pBuff++;
   }

   return (crcBuff);
}


/*******************************************************************************
* Function Name  : CRC8
* Description    :
* Input          :
* Return         :
*******************************************************************************/
uint8_t CRC8(uint8_t Byte, uint8_t crc)
{
   uint8_t i;

   for ( i = 0; i < 8 ; i++)
   {
      if (crc & 0x80)
      {
         crc <<= 1;
         if (Byte & 0x80) crc |= 0x01;
         else crc &= 0xFE;
         crc ^= 0x85;
      }
      else
      {
        crc <<= 1;
        if (Byte & 0x80) crc |= 0x01;
        else crc &= 0xFE;
      }
      Byte <<= 1;
   }
   return (crc);
}
