/**
  ******************************************************************************
  * @file    hw_config.h
  * @author  
  * @version V1.0.0
  * @date    
  * @brief   Hardware Configuration & Setup
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "usb_type.h"


/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define MASS_MEMORY_START     0x04002000
#define BULK_MAX_PACKET_SIZE  0x00000040

#define HostOUT_RX_DATA_SIZE   410

/* Exported functions ------------------------------------------------------- */
void Set_System(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void Handle_USBAsynchXfer (void);
void Get_SerialNum(void);

/* External functions ------------------------------------------------------- */
//extern void (*processing_command)(uint8_t* data);
//extern ErrorStatus (*Send_Data_current_protocol)(char *data, uint8_t num_bytes, uint8_t timeout_ms);

/* External variables --------------------------------------------------------*/
extern uint8_t  USART_Rx_Buffer [HostOUT_RX_DATA_SIZE];

#endif  /*__HW_CONFIG_H*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
