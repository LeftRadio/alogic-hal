/**
  ******************************************************************************
  * @file
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h"
#include "MainMenu.h"
#include "STLM75.h"
#include "DHT11.h"
#include "GlobalInit.h"

extern menuItem_INFO Main_Settings, Main_Timers, Main_Outs, Main_Regulate, Main_Graphics;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void MainMenu_Draw(void);

void Main_Timers_Event(void);
void Main_Outs_Event(void);
void Main_Regulate_Event(void);
void Main_Graphics_Event(void);
void Main_Settings_Event(void);

/* Private variables ---------------------------------------------------------*/
Menu_Struct_TypeDef MainMenu = {

		5,
		{ &Main_Timers, &Main_Outs, &Main_Regulate, &Main_Graphics, &Main_Settings },
		{ &Main_Timers_Event, &Main_Outs_Event, &Main_Regulate_Event, &Main_Graphics_Event, &Main_Settings_Event },
		TRUE,
		MainMenu_Draw,
		0,
		0,
};



/* Functions ----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void MainMenu_Draw(void)
{
	char FailDateString[9], FailTimeString[9];
	uint16_t X;

	FlashingText_Command(DISABLE);

	// рисуем линии
	LCD_SetGraphicsColor(0x4410);		//0x5cb
	LCD_DrawLine(0, 65, 479, 65);
	LCD_DrawLine(220, 80, 220, 280);
	LCD_DrawLine(230, 130, 470, 130);

	TemeratureSensors_InitDefault();
//	if(HHT_Read(DHTSensor.Data) != ERROR)
//	{
		//DHTSensor.Data[0] += 8;
	Update_HTSensor_OnScreen(DHTSensor.Data, 0);
	Update_HTSensor_OnScreen(SHTSensor.Data, 1);
//	}
//	else
//	{
//		Update_DHTSensor_OnScreen(DHTSensor.Data);
//	}

	LCD_SetFont(&arialUnicodeMS_16ptFontInfo);
	LCD_PutColorStrig(236, 100, 1, "Выходы 1 - 10", Orange1);
	LCD_PutColorStrig(236, 75, 1, "Выходы 11 - 20", Orange1);

	if((Read_Fail_From_BKP(FailDateString, FailTimeString, TRUE) == TRUE) && (Save_FailBKP == FALSE))
	{
		LCD_SetFont(&microsoftSansSerif_10ptFontInfo);
		X = LCD_PutColorStrig(236, 130, 1, "Сбой: ", Red);
		X = LCD_PutColorStrig(X, 130, 1, FailDateString, Red);
		LCD_PutColorStrig(X + 5, 130, 1, FailTimeString, Red);
	}

	Draw_All_Items_Menu(0, MainMenu.MaxItems);
}


/*******************************************************************************
* Function Name  : Переход в меню выбора таймера
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Main_Timers_Event(void)
{
	FlashingText_Command(DISABLE);

	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);
	LCD_ClearArea(Main_Timers.X0, Main_Timers.Y0 - 4, Main_Timers.X1 + 5, Main_Timers.Y1 - 10, Global_BackColor);

	SetActiveMenu(&AllTimersMenu);
	Draw_ActiveMenu();
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Main_Outs_Event(void)
{
	FlashingText_Command(DISABLE);

	LCD_ClearArea(0, 0, 479, 294, Global_BackColor);
//	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);

	SetActiveMenu(&OutsMenu);
	Draw_ActiveMenu();
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Main_Regulate_Event(void)
{
	FlashingText_Command(DISABLE);

	LCD_ClearArea(0, 0, 479, 294, Global_BackColor);
//	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);

	SetActiveMenu(&AllSensorsMenu);
	Draw_ActiveMenu();
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Main_Graphics_Event(void)
{
	FlashingText_Command(DISABLE);

	LCD_ClearArea(0, 0, 479, 294, Global_BackColor);
//	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);

	SetActiveMenu(&GraphicsMenu);
	Draw_ActiveMenu();
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Main_Settings_Event(void)
{
	FlashingText_Command(DISABLE);

	// очищаем место под иконку "назад"
	LCD_ClearArea(Main_Settings.X0, Main_Settings.Y0 - 4, Main_Settings.X1 + 5, Main_Settings.Y1 - 10, Global_BackColor);
	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);

	SetActiveMenu(&SettingsMenu);
	Draw_ActiveMenu();
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Down_Menu_Reset(void)
{
	if(Main_Timers.ICO != &Clock_ICOInfo)
	{
		Main_Timers.ICO = (BITMAP_INFO_16*)&Clock_ICOInfo;
		memcpy(Main_Timers.Text, "Таймеры", 7);
		LCD_ClearArea(Main_Timers.X0 - 1, Main_Timers.Y0, Main_Timers.X1 + 1, 64, Global_BackColor);
	}

	if(Main_Outs.ICO != (BITMAP_INFO_16*)&Outs_ICOInfo)
	{
		Main_Outs.ICO = (BITMAP_INFO_16*)&Outs_ICOInfo;
		memcpy(Main_Outs.Text,"Выходы", 6);
		LCD_ClearArea(Main_Outs.X0 - 1, Main_Outs.Y0, Main_Outs.X1 + 1, 64, Global_BackColor);
	}

	if(Main_Regulate.ICO != (BITMAP_INFO_16*)&Regulate_ICOInfo)
	{
		Main_Regulate.ICO = (BITMAP_INFO_16*)&Regulate_ICOInfo;
		memcpy(Main_Regulate.Text, "Регулировки", 11);
		Main_Regulate.X0 = 174;
		Main_Regulate.X1 = 253;
		LCD_ClearArea(Main_Regulate.X0 - 1, Main_Regulate.Y0, Main_Regulate.X1 + 1, 64, Global_BackColor);
	}

	if(Main_Graphics.ICO != (BITMAP_INFO_16*)&Graphics_ICOInfo)
	{
		Main_Graphics.ICO = (BITMAP_INFO_16*)&Graphics_ICOInfo;
		memcpy(Main_Graphics.Text, "Графики", 7);
		LCD_ClearArea(Main_Graphics.X0 - 1, Main_Graphics.Y0, Main_Graphics.X1 + 1, 64, Global_BackColor);
	}

	if(Main_Settings.ICO != (BITMAP_INFO_16*)&Settings_InfoICO)
	{
		Main_Settings.ICO = (BITMAP_INFO_16*)&Settings_InfoICO;
		memcpy(Main_Settings.Text, "Настройки", 9);
		LCD_ClearArea(Main_Settings.X0 - 1, Main_Settings.Y0, Main_Settings.X1 + 1, 64, Global_BackColor);
	}
}
