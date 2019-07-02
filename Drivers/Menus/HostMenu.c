/**
   ******************************************************************************
   * @file      Graphics.c
   * @author	LeftRadio
   * @version 	V1.0.0
   * @date      14.04.2013 20:27:59
   * @brief
   ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h"
#include "Pparam.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void HostMenu_Draw(void);
static void HostMenu_ReturnEvent(void);

/* Private variables ---------------------------------------------------------*/

Menu_Struct_TypeDef HostMenu = {
		0,
		{ // кнопки меню

		},
		{ // функции кнопок меню

		},
		TRUE,
		&HostMenu_Draw,
		0,
		&HostMenu_ReturnEvent,
};



/* Functions ----------------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : GraphicsMenu_Draw
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void HostMenu_Draw(void)
{
	LCD_ClearArea(0, 0, 479, 294, Global_BackColor);

	LCD_SetFont(&sansation_22ptFontInfo);
	LCD_PutColorStrig(105, 150, 1, "Соединение с ПК", LightGreen);
}


/*******************************************************************************
 * Function Name  : Graphics_Exit_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void HostMenu_ReturnEvent(void)
{
	Down_Menu_Reset();

	SetActiveMenu(&MainMenu);
	LCD_ClearArea(0, 5, 479, 294, Global_BackColor);
	Draw_ActiveMenu();
}



