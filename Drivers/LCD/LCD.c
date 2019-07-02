/*************************************************************************************
*
Description :  AutoLogic R61581 LCD Sourse
Version     :  1.0.0
Date        :
Author      :  Left Radio
Comments:
*
**************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "GlobalInit.h"
#include "LCD.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t LCD_TextColor;
uint16_t LCD_BackColor;
uint16_t LCD_GraphicsColor;
uint16_t LCD_GlobalBackColor;
uint16_t LCD_brdColor;
uint16_t LCD_Grid_Color;

volatile uint32_t BL_AutoOff_Seconds = 0;
uint8_t BL_OFF_ticks = 0;
uint8_t BL_Precent = 80;
uint8_t FAN_Precent = 80;
Bool BL_AutoOFF_Status = FALSE;
uint8_t gBL_Precent = 0;

extern uint16_t Global_BackColor;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : LCD_Configuration
* Description    : Configure the LCD Control pins and FSMC Parallel interface
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Configuration(void) {
	/* Reset LCD -----------------------------------------------------------------*/
	HAL_GPIO_WritePin(GPIOB, GPIO_Pin_14, GPIO_PIN_RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOB, GPIO_Pin_14, GPIO_PIN_SET);
	HAL_Delay(50);
	/* Initialization LCD Controller ---------------------------------------------*/
	LCD_Init();
	HAL_Delay(50);
}

/*******************************************************************************
* Function Name  : LCD_Configuration
* Description    : Configure the LCD Control pins and FSMC Parallel interface
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Set_BackLightPWM(uint8_t BL_Precent) {
	uint32_t PWM_Val = (TIM5->ARR * BL_Precent) / 100;
	TIM5->CCR1 = (uint16_t)PWM_Val;	
	gBL_Precent = BL_Precent;
}


/*******************************************************************************
* Function Name  : LCD_Configuration
* Description    : Configure the LCD Control pins and FSMC Parallel interface
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t LCD_Get_BackLightPWM(void) {
	return gBL_Precent;
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

