/**
   ******************************************************************************
   * @file      SettingsMenu.c 
   * @author		LeftRadio
   * @version 	V1.0.0
   * @date      15.04.2013 0:44:43
   * @brief
   ******************************************************************************
**/ 

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h" 
#include "clock_calendar.h"
#include "GlobalInit.h"
#include "f_eeprom.h"
#include "TouchScreen.h"
#include "Pparam.h"
#include "LCD.h"
#include "Graphics.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define defYear			13
#define defMonth		1
#define defDay			1
#define defHours		0
#define defMinutes		0
#define defBL_Precent	80
#define defFAN_Precent	80

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SettingsMenu_Draw(void);
void SettingsMenu_ReturnEvent(void);

static void Settings_ActiveSetDateTime_ItemEvent(void);
static void Settings_ActiveSetLCD_ItemEvent(void);
static void Settings_ActiveFan_ItemEvent(void);
static void Settings_ActiveSetEraseEEPROM_ItemEvent(void);
static void Settings_TouchCalibrate_ItemEvent(void);
static void Settings_SetDate_ItemEvent(void);
static void Settings_SetTime_ItemEvent(void);
static void Settings_SetBL_ItemEvent(void);
static void Settings_SetBL_OFFTime_ItemEvent(void);
static void Settings_SetFanPWM_ItemEvent(void);
static void Settings_CancelEraseEEPROM_ItemEvent(void);
static void Settings_YesEraseEEPROM_ItemEvent(void);

static void OK_ItemEvent(void);
static void Up_ItemEvent(void);
static void Down_ItemEvent(void);
static void UP_DOWN_Event(int8_t sign);
static void Right_ItemEvent(void);
static void Left_ItemEvent(void);
static void RIGHT_LEFT_Event(int8_t sign);

extern void Main_Timers_Event(void);
extern void Main_Outs_Event(void);
extern void Main_Regulate_Event(void);
extern void Main_Graphics_Event(void);

static void SettingsResetButtons(void);
static void SettingsMenu_Enable_Contol_Icons(void);
static void SettingsUpdateSetParametr(uint32_t NewData, uint8_t txtShift, uint8_t charNum);
static void Check_SavedDate(FlagStatus InitStatus);
static void SettingsMenu_Init_Buttons_and_Values(void);


/* Private variables ---------------------------------------------------------*/
extern menuItem_INFO Settings_ActiveSetDateTime_Item, Settings_ActiveSetLCD_Item, Settings_ActiveFan_Item, Settings_ActiveSetEraseEEPROM_Item, Settings_SetDate_Item;
extern menuItem_INFO Settings_SetTime_Item, Settings_SetBL_Item, Settings_SetBL_OFFTime_Item, Settings_SetFanPWM_Item, Settings_CancelEraseEEPROM_Item;
extern menuItem_INFO Settings_TouchCalibrate_Item, Settings_YesEraseEEPROM_Item;
extern menuItem_INFO OK_Item, Down_Item, Up_Item, Right_Item, Left_Item;
extern menuItem_INFO Main_Timers, Main_Outs, Main_Regulate, Main_Graphics, Main_Settings;

Menu_Struct_TypeDef SettingsMenu = {
		17,
		{ // кнопки меню
				&Main_Timers, &Main_Outs, &Main_Regulate, &Main_Graphics, &Main_Settings,
				&Settings_ActiveSetDateTime_Item, &Settings_ActiveSetLCD_Item, &Settings_ActiveFan_Item, &Settings_ActiveSetEraseEEPROM_Item, &Settings_SetDate_Item,
				&Settings_SetTime_Item, &Settings_SetBL_Item, &Settings_SetBL_OFFTime_Item, &Settings_SetFanPWM_Item, &Settings_CancelEraseEEPROM_Item,
				&Settings_YesEraseEEPROM_Item, &Settings_TouchCalibrate_Item,
				&OK_Item, &Down_Item, &Up_Item, &Right_Item, &Left_Item,
		},
		{ // функции кнопок меню
				&Main_Timers_Event, &Main_Outs_Event, &Main_Regulate_Event, &Main_Graphics_Event, &SettingsMenu_ReturnEvent,
				&Settings_ActiveSetDateTime_ItemEvent, &Settings_ActiveSetLCD_ItemEvent, &Settings_ActiveFan_ItemEvent, &Settings_ActiveSetEraseEEPROM_ItemEvent, &Settings_SetDate_ItemEvent,
				&Settings_SetTime_ItemEvent, &Settings_SetBL_ItemEvent, &Settings_SetBL_OFFTime_ItemEvent, &Settings_SetFanPWM_ItemEvent, &Settings_CancelEraseEEPROM_ItemEvent,
				&Settings_YesEraseEEPROM_ItemEvent, &Settings_TouchCalibrate_ItemEvent,
				&OK_ItemEvent, &Down_ItemEvent, &Up_ItemEvent, &Right_ItemEvent, &Left_ItemEvent,
		},
		TRUE,
		&SettingsMenu_Draw,
		0,
		0,
}; 
 

ParamStruct_TypeDef Settings_Date = { 3, { {defDay, 31, 1, 2}, {defMonth, 12, 1, 2}, {defYear, 99, 0, 2} },	0 };
ParamStruct_TypeDef Settings_Time = { 2, { {defHours, 23, 0, 2}, {defMinutes, 59, 0, 2} }, 0 };
ParamStruct_TypeDef Settings_BL_Precent = { 1, { {defBL_Precent, 100, 5, 3} }, 0 };
ParamStruct_TypeDef Settings_BL_AutoOff = { 2, { {defHours, 23, 0, 2}, {defMinutes, 59, 0, 2} }, 0 };
ParamStruct_TypeDef Settings_FAN_Precent = { 1, { {defFAN_Precent, 100, 5, 3} }, 0 };

ParamStruct_TypeDef *Settings_SettParametr = (void*)0;
uint8_t SettActiveButtonIndx = 0;



/* Functions ----------------------------------------------------------------*/
 
/*******************************************************************************
 * Function Name  : SettingsMenu_Draw
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SettingsMenu_Draw(void)
{ 
	Down_Menu_Reset();

	/* заменяем иконку у кнопки настроек, теперь кнопка выхода */
	Main_Settings.ICO = (BITMAP_INFO_16*)&Home_ICOInfo;
	memcpy(Main_Settings.Text, "  Главная  ", 11);

	// рисуем линии
	LCD_SetGraphicsColor(GreenLine);
	LCD_DrawLine(415, 75, 415, 285);
	LCD_DrawLine(0, 65, 479, 65);

	// инициализируем
	SettingsMenu_Init_Buttons_and_Values();

	// рисуем все кнопки меню
	Draw_All_Items_Menu(0, SettingsMenu.MaxItems);
}
 
 
/*******************************************************************************
 * Function Name  : SettingsMenu_ReturnEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SettingsMenu_ReturnEvent(void)
{
	FlashingText_Command(DISABLE);
	Down_Menu_Reset();

	LCD_ClearArea(0, 0, 479, 64, Global_BackColor);
	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);

	SetActiveMenu(&MainMenu);
	Draw_ActiveMenu();
}


/*******************************************************************************
 * Function Name  : Settings_ActiveSetDateTime_ItemEvent
 * Description    : Установки даты и времени
 * Input          : None
 * Return         : None
*******************************************************************************/
void Settings_ActiveSetDateTime_ItemEvent(void)
{ 
	Check_SavedDate(RESET);
	SettingsResetButtons();

	Settings_SetDate_Item.Status = SET;
	Settings_SetTime_Item.Status = SET;

	Draw_All_Items_Menu(9, 11);
} 
 
 
/*******************************************************************************
 * Function Name  : Settings_ActiveSetLCD_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Settings_ActiveSetLCD_ItemEvent(void)
{ 
	uint16_t Xtxt = 0;

	Check_SavedDate(RESET);
	SettingsResetButtons();

	Settings_SetBL_Item.Status = SET;
	Settings_SetBL_OFFTime_Item.Status = SET;

	LCD_SetFont(&microsoftSansSerif_10ptFontInfo);
	LCD_SetTextColor(Orange1);
	LCD_SetGraphicsColor(GreenLine);

	Xtxt = LCD_PutStrig(216, 235, 1, "Яркость");
	LCD_DrawLine(211, 234, Xtxt + 2, 234);

	Xtxt = LCD_PutStrig(301, 235, 1, "Отключение");
	LCD_DrawLine(297, 234, Xtxt + 2, 234);

	SettActiveButtonIndx = 12;
	Settings_BL_AutoOff.Item[0].Param = BL_AutoOff_Seconds / 3600;
	Settings_BL_AutoOff.Item[1].Param = (BL_AutoOff_Seconds % 3600) / 60;

	SettingsUpdateSetParametr(Settings_BL_AutoOff.Item[0].Param, 0, Settings_BL_AutoOff.Item[0].SymNum);
	SettingsUpdateSetParametr(Settings_BL_AutoOff.Item[1].Param, 1, Settings_BL_AutoOff.Item[1].SymNum);

	SettActiveButtonIndx = 11;
	Settings_BL_Precent.Item[0].Param = BL_Precent;
	SettingsUpdateSetParametr(Settings_BL_Precent.Item[0].Param, 0, Settings_BL_Precent.Item[0].SymNum);

//	Draw_All_Items_Menu(11, 13);
} 
 
 
/*******************************************************************************
 * Function Name  : Settings_ActiveUpdateFirmware_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Settings_ActiveFan_ItemEvent(void)
{ 
	Check_SavedDate(RESET);
	SettingsResetButtons();

	Settings_SetFanPWM_Item.Status = SET;

	SettActiveButtonIndx = 13;
	Settings_FAN_Precent.Item[0].Param = FAN_Precent;;
	SettingsUpdateSetParametr(Settings_FAN_Precent.Item[0].Param, 0, Settings_FAN_Precent.Item[0].SymNum);
} 
 
 
/*******************************************************************************
 * Function Name  : Settings_ActiveSetEraseEEPROM_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Settings_ActiveSetEraseEEPROM_ItemEvent(void)
{ 
	Check_SavedDate(RESET);
	SettingsResetButtons();

	Settings_CancelEraseEEPROM_Item.Status = SET;
	Settings_YesEraseEEPROM_Item.Status = SET;

	Draw_All_Items_Menu(14, 16);
} 
 


/*******************************************************************************
 * Function Name  : Settings_ActiveSetEraseEEPROM_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Settings_TouchCalibrate_ItemEvent(void)
{
	matrix.Divider = 0;
	Settings_ReCalibrateTouch = TRUE;
}




/* ------------------------------------------------------------------------------------------------------------- */

/*******************************************************************************
 * Function Name  : Settings_SetDate_ItemEvent
 * Description    : нажатие на дату
 * Input          : None
 * Return         : None
*******************************************************************************/
void Settings_SetDate_ItemEvent(void)
{ 
	SettActiveButtonIndx = TouchObject;
	Settings_SettParametr = &Settings_Date;			// Активный параметр - Дата
	Settings_SettParametr->Position = 0;
	SettingsMenu_Enable_Contol_Icons();
} 
 
 
/*******************************************************************************
 * Function Name  : Settings_SetTime_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Settings_SetTime_ItemEvent(void)
{ 
	SettActiveButtonIndx = TouchObject;
	Settings_SettParametr = &Settings_Time;			// Активный параметр - Время
	Settings_SettParametr->Position = 0;
	SettingsMenu_Enable_Contol_Icons();
} 
 
 
/*******************************************************************************
 * Function Name  : Settings_SetBL_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Settings_SetBL_ItemEvent(void)
{ 
	SettActiveButtonIndx = TouchObject;
	Settings_SettParametr = &Settings_BL_Precent;			// Активный параметр - яркость подсведки
	SettingsMenu_Enable_Contol_Icons();
} 
 
 
/*******************************************************************************
 * Function Name  : Settings_SetBL_OFFTime_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Settings_SetBL_OFFTime_ItemEvent(void)
{ 
	SettActiveButtonIndx = TouchObject;
	Settings_SettParametr = &Settings_BL_AutoOff;			// Активный параметр - автоотключение подсведки
	Settings_SettParametr->Position = 0;
	SettingsMenu_Enable_Contol_Icons();
} 
 

/*******************************************************************************
 * Function Name  : Settings_GoToUpdateFirmware_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Settings_SetFanPWM_ItemEvent(void)
{ 
	SettActiveButtonIndx = TouchObject;
	Settings_SettParametr = &Settings_FAN_Precent;			// Активный параметр - яркость подсведки
	Settings_SettParametr->Position = 0;
	SettingsMenu_Enable_Contol_Icons();
} 
 
 
/*******************************************************************************
 * Function Name  : Settings_CancelEraseEEPROM_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Settings_CancelEraseEEPROM_ItemEvent(void)
{ 
	SettingsResetButtons();
} 
 
 
/*******************************************************************************
 * Function Name  : Settings_YesEraseEEPROM_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Settings_YesEraseEEPROM_ItemEvent(void)
{ 
     Erase_Blocks_EEPROM(0, 390);
     HAL_Delay(300);

     NVIC_SystemReset();
} 
 
 


/*---------------------------------------------------- Стрелки ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : OK_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void OK_ItemEvent(void)
{
	SettingsMenu.MaxItems = 17;

	FlashingText_Command(DISABLE);
	SlashLine(SlashLineClear, 0, 0);
	LCD_ClearArea(420, 66, 479, 285, Global_BackColor);

	// рисуем все кнопки меню
	Draw_All_Items_Menu(0, SettingsMenu.MaxItems);


	if(Settings_SettParametr == &Settings_Date)
	{
		Graphics_Update_Save_Date();
		SetDate(Settings_SettParametr->Item[0].Param, Settings_SettParametr->Item[1].Param, Settings_SettParametr->Item[2].Param + 2000);
		Display_Date_and_Time(0);		// обновляем время на экране
	}
	else if(Settings_SettParametr == &Settings_Time)
	{
		SetTime(Settings_SettParametr->Item[0].Param, Settings_SettParametr->Item[1].Param, 0);
		Display_Date_and_Time(0);		// обновляем время на экране
	}
	else if(Settings_SettParametr == &Settings_BL_Precent)
	{
		BL_Precent = Settings_SettParametr->Item[0].Param;
		LCD_Set_BackLightPWM(BL_Precent);
		SaveBackLightSettings(BL_Precent, BL_AutoOff_Seconds);
	}
	else if(Settings_SettParametr == &Settings_BL_AutoOff)
	{
		if((Settings_SettParametr->Item[0].Param | Settings_SettParametr->Item[1].Param) != 0)
		{
			BL_AutoOFF_Status = TRUE;
			BL_AutoOff_Seconds = ((Settings_SettParametr->Item[0].Param * 60) + Settings_SettParametr->Item[1].Param) * 60;
			LCD_Verify_BL_AutoOFF(RTC_GetCounter(), SET);
		}
		else
		{
			BL_AutoOff_Seconds = 0;
			BL_AutoOFF_Status = FALSE;
		}

		SaveBackLightSettings(BL_Precent, BL_AutoOff_Seconds);
	}
	else if(Settings_SettParametr == &Settings_FAN_Precent)
	{
		FAN_Precent = Settings_SettParametr->Item[0].Param;

		uint32_t PWM_Val = (TIM3->ARR * FAN_Precent) / 100;
		TIM3->CCR2 = (uint16_t)PWM_Val;
		Save_FAN_PWM_Settings(FAN_Precent);
	}

	Settings_SettParametr = (void*)0;
}



/*******************************************************************************
 * Function Name  : Up_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Up_ItemEvent(void)
{
	UP_DOWN_Event(+1);
}


/*******************************************************************************
 * Function Name  : Down_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Down_ItemEvent(void)
{
	UP_DOWN_Event(-1);
}


/*******************************************************************************
 * Function Name  : UP_DOWN_Event
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void UP_DOWN_Event(int8_t sign)
{
	int8_t position = Settings_SettParametr->Position;

	// изменение параметра на +1 или -1
	Settings_SettParametr->Item[position].Param += sign;

	// проверка на выход за пределы
	if(Settings_SettParametr->Item[position].Param < Settings_SettParametr->Item[position].MIN )
	{
		Settings_SettParametr->Item[position].Param = Settings_SettParametr->Item[position].MAX;
	}
	else if(Settings_SettParametr->Item[position].Param > Settings_SettParametr->Item[position].MAX)
	{
		Settings_SettParametr->Item[position].Param = Settings_SettParametr->Item[position].MIN;
	}

	SettingsUpdateSetParametr(Settings_SettParametr->Item[position].Param, position, Settings_SettParametr->Item[position].SymNum);
}


/*******************************************************************************
 * Function Name  : Right_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Right_ItemEvent(void)
{
	RIGHT_LEFT_Event(+1);
}


/*******************************************************************************
 * Function Name  : Left_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Left_ItemEvent(void)
{
	RIGHT_LEFT_Event(-1);
}


/*******************************************************************************
 * Function Name  : UP_DOWN_Event
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void RIGHT_LEFT_Event(int8_t sign)
{
	Settings_SettParametr->Position += sign;

	if(Settings_SettParametr->Position >= Settings_SettParametr->MaxParametrs) Settings_SettParametr->Position = Settings_SettParametr->MaxParametrs - 1;
	else if(Settings_SettParametr->Position < 0) Settings_SettParametr->Position = 0;

	// рисуем подчеркивание
	SlashLine(SlashLineDraw, SettActiveButtonIndx, Settings_SettParametr->Position);
}



/* ------------------------------------------------------------------------------------------------------------- */

/*******************************************************************************
 * Function Name  : SettingsResetButtons
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void SettingsResetButtons(void)
{
	uint8_t i;

	FlashingText_Command(DISABLE);

	for(i = 9; i < 16; i++ ) SettingsMenu.Items[i]->Status = RESET;
	LCD_ClearArea(186, 66, 410, 285, Global_BackColor);
}




/*******************************************************************************
* Function Name  : TimerMenu_Draw_Contol_Icons
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
static void SettingsMenu_Enable_Contol_Icons(void)
{
	SettingsMenu.MaxItems = 22;

	/* запрещаем мигание */
	FlashingText_Command(DISABLE);

	// рисуем стрелки и кнопку 'ОК'
	Draw_All_Items_Menu(0, SettingsMenu.MaxItems);

	// рисуем подчеркивание
	if((SettActiveButtonIndx != 11) && (SettActiveButtonIndx != 13) ) SlashLine(SlashLineDraw, SettActiveButtonIndx, 0);

	/* рарешаем мигание */
	FlashingText_Enable(0, SettingsMenu.Items[TouchObject]);
}


/*******************************************************************************
* Function Name  : SettingsUpdateSetParametr
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
static void SettingsUpdateSetParametr(uint32_t NewData, uint8_t txtShift, uint8_t charNum)
{
	// очищаем старый текст параметра программы таймера
	SettingsMenu.Items[SettActiveButtonIndx]->TextColor = Global_BackColor;
	Draw_Item_Menu(SettingsMenu.Items[SettActiveButtonIndx]);

	// обновляем текст кнопки и устанавливаем цвет текста - белый
	ConvertToString(NewData, &SettingsMenu.Items[SettActiveButtonIndx]->Text[txtShift*3], charNum);
	SettingsMenu.Items[SettActiveButtonIndx]->TextColor = White;

	// копируем текст в мигающую кнопку и отрисовываем кнопку
	memcpy((char*)FlashingButton.Text, (char*)SettingsMenu.Items[SettActiveButtonIndx]->Text, 9);
	Draw_Item_Menu(SettingsMenu.Items[SettActiveButtonIndx]);
}


/*******************************************************************************
* Function Name  : UpdateSetParametr
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Check_SavedDate(FlagStatus InitStatus)
{
	/* запрещаем мигание */
	FlashingText_Command(DISABLE);

	if(((Settings_SettParametr == &Settings_Date) || (Settings_SettParametr == &Settings_Time)) || (InitStatus == SET))
	{
		Settings_Date.Item[0].Param = s_DateStructVar.Day;
		Settings_Date.Item[1].Param = s_DateStructVar.Month;
		Settings_Date.Item[2].Param = s_DateStructVar.Year - 2000;

		Settings_Time.Item[0].Param = (s_TimeStructVar.HourHigh * 10) + s_TimeStructVar.HourLow;
		Settings_Time.Item[1].Param = (s_TimeStructVar.MinHigh * 10) + s_TimeStructVar.MinLow;

		// Обновляем текст кнопки установки даты
		ConvertToString(Settings_Date.Item[0].Param, &pMenu->Items[9]->Text[0], Settings_Date.Item[0].SymNum);	// год
		ConvertToString(Settings_Date.Item[1].Param, &pMenu->Items[9]->Text[3], Settings_Date.Item[0].SymNum);	// месяц
		ConvertToString(Settings_Date.Item[2].Param, &pMenu->Items[9]->Text[6], Settings_Date.Item[0].SymNum);	// день
		pMenu->Items[9]->Text[8] = 0;
		Draw_Item_Menu(pMenu->Items[9]);

		// Обновляем текст кнопки установки времени
		ConvertToString(Settings_Time.Item[0].Param, &pMenu->Items[10]->Text[0], Settings_Time.Item[0].SymNum);	// Часы
		ConvertToString(Settings_Time.Item[1].Param, &pMenu->Items[10]->Text[3], Settings_Time.Item[1].SymNum);	// Минуты
		pMenu->Items[10]->Text[5] = 0;
		Draw_Item_Menu(pMenu->Items[10]);

		if(Settings_SettParametr != (void*)0) Settings_SettParametr->Position = 0;
		Settings_SettParametr = (void*)0;
	}
}


/*******************************************************************************
* Function Name  : SettingsMenu_Init_Buttons_and_Values
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
static void SettingsMenu_Init_Buttons_and_Values(void)
{
	Check_SavedDate(SET);
}
