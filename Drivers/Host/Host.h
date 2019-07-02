/**
  ******************************************************************************
  * @file    Host.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief   header
  ******************************************************************************
**/

#ifndef __HOST_H
#define __HOST_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "hw_config.h"

/* Exported typedef ----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define	USB_MODE				0
#define BT_MODE					1

#define BT_ENABLE()					HAL_GPIO_WritePin(BT_RESET_GPIO_Port, BT_RESET_Pin, GPIO_PIN_SET)
#define BT_DISABLE()				HAL_GPIO_WritePin(BT_RESET_GPIO_Port, BT_RESET_Pin, GPIO_PIN_RESET)

/* Exported variables --------------------------------------------------------*/
extern uint8_t HostIN_Data[256];
extern uint16_t HostIN_Nb_bytes;
extern uint8_t HostProtocol;

extern uint8_t  HostOUT_Data [HostOUT_RX_DATA_SIZE];
extern uint32_t HostOUT_Rx_ptr_in;
extern uint32_t HostOUT_Rx_ptr_out;
extern uint32_t HostOUT_Rx_length;
extern uint8_t  USB_Tx_State;

extern volatile Bool SendHostData;
extern volatile Bool Host_Pause;

//extern volatile Bool GlobalPeriphDeInitState;

/* Exported function ---------------------------------------------------------*/
void Host_To_MCU_Send_Data(uint8_t* data_buffer, uint8_t Nb_bytes);

void (*Send_DataByte_to_Host)(uint8_t data);
void (*Send_Data_to_Host)(uint8_t *pData, uint16_t Len);

void Switch_host_Mode(uint8_t NEW_MODE);

#endif /* __HOST_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
