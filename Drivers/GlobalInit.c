/**
  ******************************************************************************
  * @file	 	AutoLogic Global Init Sourse
  * @author  	LeftRadio
  * @version 	V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "defines.h"
#include "GlobalInit.h"
#include "usb_lib.h"
#include "LCD.h"
#include "clock_calendar.h"
#include "TouchScreen.h"
#include "Outputs.h"
#include "STLM75.h"
#include "DHT11.h"
#include "Pparam.h"
#include "Host.h"
#include "Regulates.h"
#include "Graphics.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define SYSTICK_FREQUENCY 	720000


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

volatile uint16_t ADCConvertedValue[100];
uint16_t PrescalerValue = 0;

volatile uint32_t TimingDelay = 0;
uint8_t wd_cnt;
volatile Bool Save_FailBKP = FALSE;

/* Private function prototypes -----------------------------------------------*/
static void USB_Cable_Config(FunctionalState NewState);
static void Power_OUTS_Set_OFF(void);
static void Touch_EXTI_Configuration(void);
void ADC_Configuration(uint8_t Coordinate);


extern void AllTimersMenu__Init_Buttons(void);
extern void ConvertToString(uint16_t Num, char* Str, uint8_t NumSymbol);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main Global_Init.
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef Global_Init(void) {
	/* */
	Power_OUTS_Set_OFF();	
	/* */
	RTC_User_Init();
	
	/* Enable USB device */	
	USB_Cable_Config(ENABLE);

	/* инициализация LCD, TouchPanel */
	LCD_Configuration();
	/* */
	//--------------------  Touch_EXTI_Configuration();
	// -------------------- TouchPanel_EE(0);

	/* инициализация датчиков и I2C */
	TemeratureSensors_InitDefault();	
	/* инициализируем кнопки меню */
	AllTimersMenu__Init_Buttons();
	/* инициализируем таймера, выхода и т.д. */
	Timers_Init();
	Regulates_Init();
	/* Выходы должны инициализироватся последними,
	 * так как иначе в структурах таймеров и регулировок останутся неназначены OutIndex */
	Outputs_Init();
	/* */
	DHT_Init(GPIO_Mode_IN_FLOATING);
	SHT_Init(TRUE);
	/* */	
	BT_DISABLE();
	Switch_host_Mode(BT_MODE);
	TIM4_Configuration();
	/* */
	InitGraphicsObjects();
	/* */
	Read_FAN_PWM_Settings();

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
	/* */
	return HAL_OK;
}

/**
  * @brief  USB_Cable_Config
  * @param  None
  * @retval None
  */
static void USB_Cable_Config (FunctionalState NewState) {
  /* */
  if (NewState != DISABLE) {
        USB_DevConnect();
        HAL_GPIO_WritePin(USB_DISCONNECT_PIN_GPIO_Port, USB_DISCONNECT_PIN_Pin, GPIO_PIN_SET);
  }
  else{
        USB_DevDisconnect();
        HAL_GPIO_WritePin(USB_DISCONNECT_PIN_GPIO_Port, USB_DISCONNECT_PIN_Pin, GPIO_PIN_RESET);
  }
}

/**
  * @brief  Power_OUTS_Set_OFF
  * @param  None
  * @retval None
  */
static void Power_OUTS_Set_OFF(void) {
	/* инициализация выходов под нагрузки */
	for(uint8_t i i = 0; i < 20; i++) {
		HAL_GPIO_WritePin(StructOuts[i].Out.OUT_Port, StructOuts[i].Out.GPIO_PIN, GPIO_PIN_SET);
	}
}

/**
  * @brief  Touch_EXTI_Configuration
  * @param  None
  * @retval None
  */
static void Touch_EXTI_Configuration(void) {
	EXTI_InitTypeDef   EXTI_InitStructure;
	/* Connect EXTI0 Line to PA.04 pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	/* Configure EXTI4 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/**
  * @brief  ADC_Configuration
  * @param  None
  * @retval None
  */
void ADC_Configuration(uint8_t TouchMode) {
	if(TouchMode == Touch_INT)
	{
		/* -------------------------*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		// +3.3V на YD+ и на YD-
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		GPIO_SetBits(GPIOA, GPIO_Pin_7);

		/* Configure PA.04 as input with PD -------------------------*/
		GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_6);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		HAL_Delay(3);
		EXTI_ClearITPendingBit(EXTI_Line4);
		EXTI_ClearFlag(EXTI_Line4);

		/* Enable and set EXTI4 Interrupt to the lowest priority */
		NVIC_EnableIRQ(EXTI4_IRQn);
		NVIC_SetPriority(EXTI4_IRQn, 15);
	}
	else
	{
		if(TouchMode == Touch_X)
		{
			/* Configure PA.04 (ADC Channel4) as analog input -------------------------*/
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			/* ADC1 regular channel14 configuration */
			ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);

			/* -------------------------*/
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			/* -------------------------*/
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			// +3.3 на YD+ и 0 на YD-
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			GPIO_ResetBits(GPIOA, GPIO_Pin_7);
		}
		else if(TouchMode == Touch_Y)
		{
			/* Configure PA.04 (ADC Channel4) as analog input -------------------------*/
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			/* ADC1 regular channel14 configuration */
			ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5);

			/* -------------------------*/
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			/* -------------------------*/
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			// +3.3 на XD+ и 0 на XD-
			GPIO_SetBits(GPIOA, GPIO_Pin_6);
			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		}
	}
}

/**
  * @brief  Global_Periph_DeInit
  * @param  None
  * @retval None
  */
void Global_Periph_DeInit(void) {
	/* */
	Save_StartAplicationFlag();
	HAL_Delay(500);
	/* */
	USB_Cable_Config(DISABLE);
	HAL_TIM_Base_Stop(&htim1);
	HAL_Delay(100);
	/* Jump to bootloader */
	NVIC_SystemReset();
}



/*******************************************************************************
      END FILE
*******************************************************************************/