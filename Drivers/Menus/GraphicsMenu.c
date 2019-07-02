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
#include "STLM75.h"
#include "DHT11.h"
#include "clock_calendar.h"
#include "Graphics.h"
#include "STLM75.h"
//#include "Regulates.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private const -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void GraphicsMenu_Draw(void);
static void Graphics_Exit_ItemEvent(void);
static void Graphics_Mode_ItemEvent(void);

static void Graphics_Reset_ItemEvent(void);

/* Смена Датчика для вывода на график */
static void Graphics_Up_ItemEvent(void);
static void Graphics_Down_ItemEvent(void);
static void Change_Sensor(int8_t Sign);

/* Прокрутка по дням/месяцам для вывода на график */
static void Graphics_Right_ItemEvent(void);
static void Graphics_Left_ItemEvent(void);

static void Change_Day(int8_t Sign);
static void Change_Month(int8_t Sign);
void (*Change_Range)(int8_t Sign) = Change_Day;


/* Private variables ---------------------------------------------------------*/
extern menuItem_INFO Graphics_Reset_Item, Graphics_Exit_Item, Graphics_Mode_Item, Graphics_Down_Item, Graphics_Up_Item, Graphics_Right_Item, Graphics_Left_Item;

Menu_Struct_TypeDef GraphicsMenu = {
		7,
		{ // кнопки меню
				&Graphics_Exit_Item, &Graphics_Mode_Item, &Graphics_Down_Item, &Graphics_Up_Item, &Graphics_Right_Item, &Graphics_Left_Item,
				&Graphics_Reset_Item
		},
		{ // функции кнопок меню
				&Graphics_Exit_ItemEvent, &Graphics_Mode_ItemEvent, &Graphics_Down_ItemEvent, &Graphics_Up_ItemEvent, &Graphics_Right_ItemEvent, &Graphics_Left_ItemEvent,
				&Graphics_Reset_ItemEvent
		},
		TRUE,
		&GraphicsMenu_Draw,
		0,
		0,
};



/* Functions ----------------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : GraphicsMenu_Draw
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void GraphicsMenu_Draw(void)
{
	static Bool InitState = FALSE;
//	static Bool Point = FALSE;
//	uint8_t tDay, tMonth;

	Change_Sensor(0);


	// рисуем линии
	LCD_SetGraphicsColor(GreenLine);
	LCD_DrawLine(0, 65, 479, 65);
	LCD_DrawLine(172, 5, 172, 60);

	/* рисуем' все кнопки меню */
	LCD_SetBackColor(LightBlack3);		// цвет тени у кнопок
	Draw_All_Items_Menu(0, GraphicsMenu.MaxItems);

	LCD_SetFont(&microsoftSansSerif_10ptFontInfo);
	LCD_SetTextColor(0xb6db);
	LCD_PutStrig(Graphics_Down_Item.X0 - 15, 5, 1, "Выбор датчика");
	LCD_PutStrig(Graphics_Left_Item.X0 + 4, 5, 1, "Прокрутка");


	if(InitState == FALSE)
	{
		/* ищем метку по текущей дате */
		Graphics_Print_DayIndex = Find_Last_Save_Index(s_DateStructVar, Graphics_Hours_Varibles_Start, Hours_VarNum, 7, _Day_, (void*)0);
		Graphics_Print_MonthIndex = Find_Last_Save_Index(s_DateStructVar, Graphics_Month_Varibles_Start, Month_VarNum, 12, _Month_, (void*)0);

		InitState = TRUE;
	}

	/* отрисовываем */
	if(Graphics_Mode == Week) Draw_Graphics(Graphics_Print_DayIndex);
	else Draw_Graphics(Graphics_Print_MonthIndex);
}


/*******************************************************************************
 * Function Name  : Graphics_Exit_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void Graphics_Exit_ItemEvent(void)
{
	LCD_ClearArea(406, 297, 460, 316, Global_BackColor);
	STLM75_LedEnableState(RESET, pGraphic->Sensor_Index);

	Down_Menu_Reset();

	SetActiveMenu(&MainMenu);
	LCD_ClearArea(0, 5, 479, 294, Global_BackColor);
	Draw_ActiveMenu();
}





/*---------------------------------------------------- Стрелки и режим ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : Up_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Graphics_Up_ItemEvent(void)
{
	Change_Sensor(+1);
}


/*******************************************************************************
 * Function Name  : Down_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Graphics_Down_ItemEvent(void)
{
	Change_Sensor(-1);
}


/*******************************************************************************
 * Function Name  : Change_Sensor
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Change_Sensor(int8_t Sign)
{
	static int8_t Index = 0;
	STLM75_LedEnableState(RESET, Index);

	Index = Index + Sign;

	if(Index < 0) Index = GRAPHICS_SENSORS_COUNT - 1;
	else if(Index > GRAPHICS_SENSORS_COUNT - 1) Index = 0;

	pGraphic = &SensGraphics[Index];
	Clear_Graphics(Graphics_Print_DayIndex, Graphics_Print_DayIndex);

	STLM75_LedEnableState(SET, Index);
}



/*******************************************************************************
 * Function Name  : Right_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Graphics_Right_ItemEvent(void)
{
	Change_Range(+1);
}


/*******************************************************************************
 * Function Name  : Left_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Graphics_Left_ItemEvent(void)
{
	Change_Range(-1);
}


/*******************************************************************************
 * Function Name  : Change_Day
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Change_Day(int8_t Sign)
{
	int8_t New_Day = Graphics_Print_DayIndex;
	int8_t Old_Day = Graphics_Print_DayIndex;
//	Bool SuccessFlag = FALSE;
	uint8_t cnt = 0;

	/* Переключаем день */
	do
	{
		New_Day += Sign;
		if(New_Day < 0) New_Day = 6;
		else if(New_Day > 6) New_Day = 0;

		/* Ищем день для переключения */
		if(Find_Day_From_Index(New_Day) == SUCCESS)
		{
			if(New_Day != Graphics_Print_DayIndex)
			{
				/* Если нашли то перерисовать и закончить */
				Graphics_Print_DayIndex = New_Day;
				Clear_Graphics(Old_Day, New_Day);
			}
			return;
		}
		else
		{
			/* Ищем дальше, если уже искали 7 раз то выход */
			cnt++;
			if(cnt > 7) return;
		}
	}
	while(1);
}


/*******************************************************************************
 * Function Name  : Change_Month
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void Change_Month(int8_t Sign)
{
	int8_t New_Month = Graphics_Print_MonthIndex;
	int8_t Old_Month = Graphics_Print_MonthIndex;
//	Bool SuccessFlag = FALSE;
	uint8_t cnt = 0;

	/* Переключаем день */
	do
	{
		New_Month += Sign;
		if(New_Month < 0) New_Month = 11;
		else if(New_Month > 11) New_Month = 0;

		/* Ищем день для переключения */
		if(Find_Month_From_Index(New_Month) == SUCCESS)
		{
			if(New_Month != Graphics_Print_MonthIndex)
			{
				/* Если нашли то перерисовать и закончить */
				Graphics_Print_MonthIndex = New_Month;
				Clear_Graphics(Old_Month, New_Month);
			}
			return;
		}
		else
		{
			/* Ищем дальше, если уже искали 12 раз то выход */
			cnt++;
			if(cnt > 12) return;
		}
	}
	while(1);
}



/*******************************************************************************
 * Function Name  : Graphics_Mode_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void Graphics_Mode_ItemEvent(void)
{
	if(Graphics_Mode == Week)
	{
		Clear_Graphics(Graphics_Print_DayIndex, 255);

		Graphics_Mode = Month;
		Change_Range = Change_Month;

		if(Graphics_Print_MonthIndex == 255) Graphics_Print_MonthIndex = Find_Last_Save_Index(s_DateStructVar, Graphics_Month_Varibles_Start, Month_VarNum, 12, _Month_, (void*)0);

		Draw_Graphics(Graphics_Print_MonthIndex);
	}
	else
	{
		Clear_Graphics(Graphics_Print_MonthIndex, 255);

		Graphics_Mode = Week;
		Change_Range = Change_Day;

		if(Graphics_Print_DayIndex == 255) Graphics_Print_DayIndex = Find_Last_Save_Index(s_DateStructVar, Graphics_Hours_Varibles_Start, Hours_VarNum, 7, _Day_, (void*)0);

		Draw_Graphics(Graphics_Print_DayIndex);
	}
}





/*******************************************************************************
 * Function Name  : Graphics_Reset_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void Graphics_Reset_ItemEvent(void)
{
	Erase_Data_EEPROM(Graphics_Hours_Varibles_Start, Hours_VarNum * 7);
	Erase_Data_EEPROM(Graphics_Month_Varibles_Start, Month_VarNum * 12);

	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);

	Graphics_Mode = Week;
	Change_Range = Change_Day;
	Graphics_Print_DayIndex = Find_Last_Save_Index(s_DateStructVar, Graphics_Hours_Varibles_Start, Hours_VarNum, 7, _Day_, (void*)0);
	Draw_Graphics(Graphics_Print_DayIndex);
}














