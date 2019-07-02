/**
  ******************************************************************************
  * @file		TimerNormalSetMenu.c
  * @author		LeftRadio
  * @version 	V1.0.0
  * @date
  * @brief		Timer Settings Source
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h"
#include "TimerNormalSetMenu.h"
#include "Timers.h"
#include "Pparam.h"
#include "Outputs.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define btnSW					4
#define btnSW_X					4
#define LowerBtn				67
#define btnHigh					40
#define btnWidht				90
#define btnLeft					5

#define Btn_Y0(i)				(LowerBtn + (btnHigh + btnSW) * (i))
#define Btn_X0(i)				(btnLeft + (btnWidht + btnSW_X) * (i))

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void TimerNormalSetMenu_Draw(void);
void TimerNormalSetMenu_Init_Buttons_and_Values(void);

static void TimerMenu_Enable_Contol_Icons(void);
static void UpdateSetParametr(uint32_t NewData, uint8_t txtShift);


/* События кнопок меню*/
void Timer_Program_ON_OFF(void);
void TimerNormal_Set_StartData_Event(void);
void TimerNormal_Set_StartTime_Event(void);
void TimerNormal_Set_StopData_Event(void);
void TimerNormal_Set_StopTime_Event(void);

void TimerNormal_Left_Event(void);
void TimerNormal_Right_Event(void);
void TimerNormal_Ok_Event(void);
void TimerNormal_Up_Event(void);
void TimerNormal_Down_Event(void);

void TimerNormal_Reset_AllSetting_Event(void);
void TimerNormal_Menu_Return_Event(void);
void TimerNormal_Save_Event(void);
void TimerNormal_DailyMode_Event(void);
void TimerNormal_NormalMode_Event(void);


/* Private variables ---------------------------------------------------------*/
menuItem_INFO Timer_ON_OFF_1, Timer_ON_OFF_2, Timer_ON_OFF_3, Timer_ON_OFF_4, Timer_ON_OFF_5;
menuItem_INFO Timer_DateON_1, Timer_DateON_2, Timer_DateON_3, Timer_DateON_4, Timer_DateON_5;
menuItem_INFO Timer_TimeON_1, Timer_TimeON_2, Timer_TimeON_3, Timer_TimeON_4, Timer_TimeON_5;
menuItem_INFO Timer_DateOFF_1, Timer_DateOFF_2, Timer_DateOFF_3, Timer_DateOFF_4, Timer_DateOFF_5;
menuItem_INFO Timer_TimeOFF_1, Timer_TimeOFF_2, Timer_TimeOFF_3, Timer_TimeOFF_4, Timer_TimeOFF_5;
menuItem_INFO Timer_Reset_BTN, Timer_ExitWithoutSave__BTN, Timer_Save__BTN, Timer_Timer_to_Daily__BTN, Timer_Timer_to_Normal__BTN;
extern menuItem_INFO OK_Item, Down_Item, Up_Item, Right_Item, Left_Item;

const char TimerMenu_TextNames[5][15] = {
		{"Выйти"}, {"Сохранить"}, {"Суточный"}, {"Простой"}, {"Сбросить все"}
};


const uint16_t BtnSettings_X0[5] = {11, 80, 183, 282, 362};
const uint16_t BtnSettings_X1[5] = {65, 137, 262, 361, 480};

Menu_Struct_TypeDef TimerNormalSetMenu = {

		30,
		{  // кнопки меню
			&Timer_DateON_1, &Timer_DateON_2, &Timer_DateON_3, &Timer_DateON_4, &Timer_DateON_5,
			&Timer_TimeON_1, &Timer_TimeON_2, &Timer_TimeON_3, &Timer_TimeON_4, &Timer_TimeON_5,
			&Timer_DateOFF_1, &Timer_DateOFF_2, &Timer_DateOFF_3, &Timer_DateOFF_4, &Timer_DateOFF_5,
			&Timer_TimeOFF_1, &Timer_TimeOFF_2, &Timer_TimeOFF_3, &Timer_TimeOFF_4, &Timer_TimeOFF_5,
			&Timer_ON_OFF_1, &Timer_ON_OFF_2, &Timer_ON_OFF_3, &Timer_ON_OFF_4, &Timer_ON_OFF_5,
			&Timer_ExitWithoutSave__BTN, &Timer_Save__BTN, &Timer_Timer_to_Daily__BTN, &Timer_Timer_to_Normal__BTN, &Timer_Reset_BTN,
			&OK_Item, &Down_Item, &Up_Item, &Right_Item, &Left_Item,
		},
		{ // функции кнопок меню
				&TimerNormal_Set_StartData_Event, &TimerNormal_Set_StartData_Event, &TimerNormal_Set_StartData_Event, &TimerNormal_Set_StartData_Event, &TimerNormal_Set_StartData_Event,
				&TimerNormal_Set_StartTime_Event, &TimerNormal_Set_StartTime_Event, &TimerNormal_Set_StartTime_Event, &TimerNormal_Set_StartTime_Event, &TimerNormal_Set_StartTime_Event,
				&TimerNormal_Set_StopData_Event, &TimerNormal_Set_StopData_Event, &TimerNormal_Set_StopData_Event, &TimerNormal_Set_StopData_Event, &TimerNormal_Set_StopData_Event,
				&TimerNormal_Set_StopTime_Event, &TimerNormal_Set_StopTime_Event, &TimerNormal_Set_StopTime_Event, &TimerNormal_Set_StopTime_Event, &TimerNormal_Set_StopTime_Event,
				&Timer_Program_ON_OFF, &Timer_Program_ON_OFF, &Timer_Program_ON_OFF, &Timer_Program_ON_OFF, &Timer_Program_ON_OFF,
				&TimerNormal_Menu_Return_Event, &TimerNormal_Save_Event, &TimerNormal_DailyMode_Event, &TimerNormal_NormalMode_Event, &TimerNormal_Reset_AllSetting_Event,
				&TimerNormal_Ok_Event, &TimerNormal_Down_Event, &TimerNormal_Up_Event, &TimerNormal_Right_Event, &TimerNormal_Left_Event,
		},
		TRUE,
		TimerNormalSetMenu_Draw,
		0,
		0,
};

volatile char DayTimeText[30][9];

// индекс активного параметра программы таймера для выставления
volatile uint8_t ProgramIndex = 0;

// индекс активной кнопки параметра программы таймера для выставления
volatile uint8_t TimerActiveButton = 0;

// указатели на параметры программы таймера
volatile uint8_t *ActiveParametr_Day = (void*)0;
volatile uint8_t *ActiveParametr_Month = (void*)0;
volatile int32_t *ActiveParametr_Time_Hours = (void*)0;
volatile int32_t *ActiveParametr_Time_Minutes = (void*)0;

/* Functions ----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormalSetMenu_Draw(void)
{
	// выводим номер таймера в который зашли в вверхнем правом углу
	LCD_ClearArea(410, 298, 431, 314, Red);

	LCD_SetFont(&DigitSansSerif11ptFontInfo);
	LCD_PutColorStrig(412, 301, 1, (char*)Timer_Temp_Struct.Name, White);

	// обовляем иконку выхода
	if(Timer_Temp_Struct.Out_Index != Timer_OUT_NotAssigned)
	{
		// обновить иконку
		Power_ON_OFF_ICO_Update(ENABLE, Timer_Temp_Struct.Out_Index);
	}

	// инициализируем кнопки
	TimerNormalSetMenu_Init_Buttons_and_Values();

	// отрисовываем все кнопки меню
	Draw_All_Items_Menu(0, TimerNormalSetMenu.MaxItems);

	SlashLine(SlashLineInit, 0, 0);
}





// кнопки выставления программ ------------------------------------------------------------------------- //

/*******************************************************************************
* Function Name  : Timer_Program_ON_OFF
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Timer_Program_ON_OFF(void)
{
	uint8_t i, j = 0;
	FunctionalState tmpFlashingStatus = FlashingText_GetState();

	/* если сейчас мигает текст, то запрещаем мигание */
	if(tmpFlashingStatus == ENABLE)	FlashingText_Command(DISABLE);

	if(Timer_Temp_Struct.Normal_Property.TimeWork_Enable[TouchObject - 20] == DISABLE)
	{
		TimerNormalSetMenu.Items[TouchObject]->ButtonColor = Green2;
		memcpy(TimerNormalSetMenu.Items[TouchObject]->Text, "ON", 2);
		TimerNormalSetMenu.Items[TouchObject]->Text[2] = 0;

		Timer_Temp_Struct.Normal_Property.TimeWork_Enable[TouchObject - 20] = ENABLE;
		Timer_Temp_Struct.State = ENABLE;

		TimerNormalSetMenu.Items[TouchObject - 20]->TextColor = LighGreen;
		TimerNormalSetMenu.Items[TouchObject - 15]->TextColor = LighGreen;
		TimerNormalSetMenu.Items[TouchObject - 10]->TextColor = LighGreen;
		TimerNormalSetMenu.Items[TouchObject - 5]->TextColor = LighGreen;
	}
	else
	{
		TimerNormalSetMenu.Items[TouchObject]->ButtonColor = LightBlack2;
		memcpy(TimerNormalSetMenu.Items[TouchObject]->Text, "OFF", 3);
		TimerNormalSetMenu.Items[TouchObject]->Text[4] = 0;

		Timer_Temp_Struct.Normal_Property.TimeWork_Enable[TouchObject - 20] = DISABLE;

		TimerNormalSetMenu.Items[TouchObject - 20]->TextColor = White;
		TimerNormalSetMenu.Items[TouchObject - 15]->TextColor = White;
		TimerNormalSetMenu.Items[TouchObject - 10]->TextColor = White;
		TimerNormalSetMenu.Items[TouchObject - 5]->TextColor = White;

		for(i = 0; i < 5; i++)
		{
			if(Timer_Temp_Struct.Normal_Property.TimeWork_Enable[i] == DISABLE) j++;
		}

		if(j >= 5) Timer_Temp_Struct.State = DISABLE;
	}

	Draw_Item_Menu(TimerNormalSetMenu.Items[TouchObject]);
	Draw_Item_Menu(TimerNormalSetMenu.Items[TouchObject - 20]);
	Draw_Item_Menu(TimerNormalSetMenu.Items[TouchObject - 15]);
	Draw_Item_Menu(TimerNormalSetMenu.Items[TouchObject - 10]);
	Draw_Item_Menu(TimerNormalSetMenu.Items[TouchObject - 5]);

	/* если текст мигал, то разрешаем опять мигание */
	if(tmpFlashingStatus == ENABLE)	FlashingText_Command(ENABLE);
}



/*******************************************************************************
* Function Name  : TimerNormal_Set_StartData_Event
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_Set_StartData_Event(void)
{
	ProgramIndex = TouchObject;
	TimerActiveButton = TouchObject;

	ActiveParametr_Day = &Timer_Temp_Struct.Normal_Property.Start_Day_Index[ProgramIndex];

	ActiveParametr_Month = (void*)0;
	ActiveParametr_Time_Hours = (void*)0;
	ActiveParametr_Time_Minutes = (void*)0;

	TimerMenu_Enable_Contol_Icons();
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_Set_StartTime_Event(void)
{
	ProgramIndex = TouchObject - 5;
	TimerActiveButton = TouchObject;

	ActiveParametr_Time_Hours = &Timer_Temp_Struct.Normal_Property.ON_Time[ProgramIndex];

	ActiveParametr_Time_Minutes = (void*)0;
	ActiveParametr_Month = (void*)0;
	ActiveParametr_Day = (void*)0;

	TimerMenu_Enable_Contol_Icons();
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_Set_StopData_Event(void)
{
	ProgramIndex = TouchObject - 10;
	TimerActiveButton = TouchObject;

	ActiveParametr_Day = &Timer_Temp_Struct.Normal_Property.Stop_Day_Index[ProgramIndex];

	ActiveParametr_Month = (void*)0;
	ActiveParametr_Time_Hours = (void*)0;
	ActiveParametr_Time_Minutes = (void*)0;

	TimerMenu_Enable_Contol_Icons();
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_Set_StopTime_Event(void)
{
	ProgramIndex = TouchObject - 15;
	TimerActiveButton = TouchObject;

	ActiveParametr_Time_Hours = &Timer_Temp_Struct.Normal_Property.OFF_Time[ProgramIndex];
	ActiveParametr_Time_Minutes = (void*)0;

	ActiveParametr_Month = (void*)0;
	ActiveParametr_Day = (void*)0;

	TimerMenu_Enable_Contol_Icons();
}



/*******************************************************************************
* Function Name  : TimerMenu_Draw_Contol_Icons
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
static void TimerMenu_Enable_Contol_Icons(void)
{
	TimerNormalSetMenu.MaxItems = 35;

	FlashingText_Command(DISABLE);

	// рисуем все кнопки меню
	Draw_All_Items_Menu(0, TimerNormalSetMenu.MaxItems);

	// рисуем подчеркивание
	SlashLine(SlashLineDraw, TimerActiveButton, 0);

	FlashingText_Enable(0, TimerNormalSetMenu.Items[TouchObject]);
}






/* Меню управления стрелками --------------------------------------------------------------------------------------------------------- */

/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_Left_Event(void)
{
//	if(ActiveParametr_Month != (void*)0)
//	{
//		// если активный параметр стартовый то перекидываем также на стартовый
//		if(ActiveParametr_Month == &Timer_Temp_Struct.Normal_Property.Start_Day_Index[ProgramIndex])
//		{
//			ActiveParametr_Day = &Timer_Temp_Struct.Normal_Property.Start_Day_Index[ProgramIndex];
//		}
//		else	// иначе перекидываем на стоповый
//		{
//			ActiveParametr_Day = &Timer_Temp_Struct.Normal_Property.Stop_Day_Index[ProgramIndex];
//		}
//
//		ActiveParametr_Month = (void*)0;
//
//		// рисуем подчеркивание
//		SlashLine(SlashLineDraw, TimerActiveButton, 0);
//	}
	if(ActiveParametr_Time_Minutes != (void*)0)
	{
		// если активный параметр стартовый то перекидываем также на стартовый
		if(ActiveParametr_Time_Minutes == &Timer_Temp_Struct.Normal_Property.ON_Time[ProgramIndex])
		{
			ActiveParametr_Time_Hours = &Timer_Temp_Struct.Normal_Property.ON_Time[ProgramIndex];
		}
		else	// иначе перекидываем на стоповый
		{
			ActiveParametr_Time_Hours = &Timer_Temp_Struct.Normal_Property.OFF_Time[ProgramIndex];
		}

		ActiveParametr_Time_Minutes = (void*)0;

		// рисуем подчеркивание
		SlashLine(SlashLineDraw, TimerActiveButton, 0);
	}
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_Right_Event(void)
{
//	if(ActiveParametr_Day != (void*)0)
//	{
//		if(ActiveParametr_Day == &Timer_Temp_Struct.Normal_Property.Start_Day_Index[ProgramIndex])
//		{
//			ActiveParametr_Month = &Timer_Temp_Struct.Normal_Property.Start_Month_Index[ProgramIndex];
//		}
//		else
//		{
//			ActiveParametr_Month = &Timer_Temp_Struct.Normal_Property.Stop_Month_Index[ProgramIndex];
//		}
//
//		ActiveParametr_Day = (void*)0;
//
//		// рисуем подчеркивание
//		SlashLine(SlashLineDraw, TimerActiveButton, 1);
//	}
	if(ActiveParametr_Time_Hours != (void*)0)
	{
		if(ActiveParametr_Time_Hours == &Timer_Temp_Struct.Normal_Property.ON_Time[ProgramIndex])
		{
			ActiveParametr_Time_Minutes = &Timer_Temp_Struct.Normal_Property.ON_Time[ProgramIndex];
		}
		else
		{
			ActiveParametr_Time_Minutes = &Timer_Temp_Struct.Normal_Property.OFF_Time[ProgramIndex];
		}

		ActiveParametr_Time_Hours = (void*)0;

		// рисуем подчеркивание
		SlashLine(SlashLineDraw, TimerActiveButton, 1);
	}
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_Ok_Event(void)
{
	TimerNormalSetMenu.MaxItems = 30;
	FlashingText_Command(DISABLE);
	SlashLine(SlashLineClear, 0, 0);

	LCD_ClearArea(420, 66, 479, 285, Global_BackColor);

	// рисуем все кнопки меню
	Draw_All_Items_Menu(0, TimerNormalSetMenu.MaxItems);
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_Up_Event(void)
{
	uint32_t tmpData = 0;
	uint8_t TextShift = 0;

	// если активный параметр день то
	if(ActiveParametr_Day != (void*)0)
	{
		(*ActiveParametr_Day) += 1;

		if((*ActiveParametr_Day) > 31) (*ActiveParametr_Day) = 1;

		TextShift = 0;
		tmpData = *ActiveParametr_Day;
	}

//	// иначе если активный параметр месяц
//	else if(ActiveParametr_Month != (void*)0)
//	{
//		(*ActiveParametr_Month) += 1;
//
//		if((*ActiveParametr_Month) > 12) (*ActiveParametr_Month) = 1;
//
//		TextShift = 3;
//		tmpData = *ActiveParametr_Month;
//	}

	// иначе если активный параметр часы
	else if(ActiveParametr_Time_Hours != (void*)0)
	{
		(*ActiveParametr_Time_Hours) += 3600;

		if((*ActiveParametr_Time_Hours) >= 86400) (*ActiveParametr_Time_Hours) = 0;

		TextShift = 0;
		tmpData = (*ActiveParametr_Time_Hours) / 3600;
	}

	// иначе если активный параметр минуты
	else if(ActiveParametr_Time_Minutes != (void*)0)
	{
		(*ActiveParametr_Time_Minutes) += 60;

		if((((*ActiveParametr_Time_Minutes) % 3600) / 60) >= 60) (*ActiveParametr_Time_Minutes) = 0;

		TextShift = 3;
		tmpData = ((*ActiveParametr_Time_Minutes) % 3600) / 60;
	}
	else return;	// иначе ошибка - выход

	UpdateSetParametr(tmpData, TextShift);
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_Down_Event(void)
{
	uint32_t tmpData = 0;
	uint8_t TextShift = 0;

	// если активный параметр день то
	if(ActiveParametr_Day != (void*)0)
	{
		(*ActiveParametr_Day) -= 1;

		if((*ActiveParametr_Day) < 1) (*ActiveParametr_Day) = 31;

		TextShift = 0;
		tmpData = *ActiveParametr_Day;
	}

//	// иначе если активный параметр месяц
//	else if(ActiveParametr_Month != (void*)0)
//	{
//		(*ActiveParametr_Month) -= 1;
//
//		if((*ActiveParametr_Month) < 1) (*ActiveParametr_Month) = 12;
//
//		TextShift = 3;
//		tmpData = *ActiveParametr_Month;
//	}

	// иначе если активный параметр часы
	else if(ActiveParametr_Time_Hours != (void*)0)
	{
		(*ActiveParametr_Time_Hours) -= 3600;

		if((*ActiveParametr_Time_Hours) < 0) (*ActiveParametr_Time_Hours) = 23 * 3600;

		TextShift = 0;
		tmpData = (*ActiveParametr_Time_Hours) / 3600;
	}

	// иначе если активный параметр минуты
	else if(ActiveParametr_Time_Minutes != (void*)0)
	{
		(*ActiveParametr_Time_Minutes) -= 60;

		if((((*ActiveParametr_Time_Minutes) % 3600) / 60) < 0) (*ActiveParametr_Time_Minutes) = 59 * 60;

		TextShift = 3;
		tmpData = ((*ActiveParametr_Time_Minutes) % 3600) / 60;
	}
	else return;	// иначе ошибка - выход

	UpdateSetParametr(tmpData, TextShift);
}



/*******************************************************************************
* Function Name  : UpdateSetParametr
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
static void UpdateSetParametr(uint32_t NewData, uint8_t txtShift)
{
	TimerNormalSetMenu.Items[TimerActiveButton]->TextColor = Global_BackColor;
	Draw_Item_Menu(TimerNormalSetMenu.Items[TimerActiveButton]);

	// обновляем текст кнопки
	TimerNormalSetMenu.Items[TimerActiveButton]->Text[txtShift] = (NewData / 10) + 48;
	TimerNormalSetMenu.Items[TimerActiveButton]->Text[txtShift + 1] = (NewData % 10) + 48;

	// копируем текст в мигающую кнопку
	memcpy((char*)FlashingButton.Text, (char*)TimerNormalSetMenu.Items[TimerActiveButton]->Text, 6);

	if(Timer_Temp_Struct.Normal_Property.TimeWork_Enable[ProgramIndex] == ENABLE)
	{
		TimerNormalSetMenu.Items[TimerActiveButton]->TextColor = LighGreen;
	}
	else TimerNormalSetMenu.Items[TimerActiveButton]->TextColor = White;

	Draw_Item_Menu(TimerNormalSetMenu.Items[TimerActiveButton]);
}






/* Нижнее меню --------------------------------------------------------------------------------------------------------- */

/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_Reset_AllSetting_Event(void)
{
	uint8_t Data0 = 0, Data1 = 0;

	FlashingText_Command(DISABLE);

	// вычисляем номер таймера
	Data0 = (Timer_Temp_Struct.Name[0] - 48) * 10;
	Data0 = (Timer_Temp_Struct.Name[1] - 48);

	// сбрасываем таймер
	One_Timer_Default_Init((Timer_INFO*)&Timer_Temp_Struct, Timer_Temp_Struct.Out_Index, Data0 + Data1);

	// очистка и перерисовка меню
	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);

	TimerNormalSetMenu_Init_Buttons_and_Values();

	//отрисовываем все кнопки меню
	Draw_All_Items_Menu(0, TimerNormalSetMenu.MaxItems - 5);

	LCD_SetGraphicsColor(GreenLine);
	LCD_DrawLine(223, 75, 223, 285);
	LCD_DrawLine(410, 75, 410, 285);
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_Menu_Return_Event(void)
{
	FlashingText_Command(DISABLE);
	SlashLine(SlashLineClear, 0, 0);
//	Delay_ms(100);

	SetActiveMenu(&AllTimersMenu);
	LCD_ClearArea(0, 5, 479, 294, Global_BackColor);
	LCD_ClearArea(410, 298, 460, 315, Global_BackColor);
	Draw_ActiveMenu();
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_Save_Event(void)
{
	FlashingText_Command(DISABLE);

	Timer_Temp_Struct.Mode = Timer_NormalMode;
	Draw_Item_Menu(TimerNormalSetMenu.Items[TimerActiveButton]);

	Timer_CopyStruct_To_Timer((Timer_INFO*)&Timers_Struct[Timer_ActiveIndx], (Timer_INFO*)&Timer_Temp_Struct);

	SaveTimerSettings(Timer_ActiveIndx);
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_DailyMode_Event(void)
{
	FlashingText_Command(DISABLE);
	SetActiveMenu(&TimerDailySetMenu);

	LCD_ClearArea(0, 0, 479, 294, Global_BackColor);
	Draw_ActiveMenu();
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormal_NormalMode_Event(void)
{
//	StopFlashing_and_Update();
}










/* Инициализация кнопок ----------------------------------------------------------------------------------------------- */

/*******************************************************************************
* Function Name  : TimerNormalSetMenu_Init_Buttons_and_Values
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormalSetMenu_Init_Buttons_and_Values(void)
{
	uint8_t i, j;
	int16_t k = 0;
	uint8_t Data_0 = 0, Data_1 = 0;
	uint32_t DayData_ONTimeSec = 0;


	/* инициализируем кнопки вкл/выкл программ таймера */
	for(i = 20; i < 25; i++)
	{
		TimerNormalSetMenu.Items[i]->X0 = 1;
		TimerNormalSetMenu.Items[i]->Y0 = (Btn_Y0(i-20));
		TimerNormalSetMenu.Items[i]->X1 = 45;
		TimerNormalSetMenu.Items[i]->Y1 = Btn_Y0(i-20) + 34;
		TimerNormalSetMenu.Items[i]->ICO = (void*)0;
		TimerNormalSetMenu.Items[i]->ButtonPreset = SET;
		TimerNormalSetMenu.Items[i]->Font = (FONT_INFO*)&arialUnicodeMS_16ptFontInfo;
		TimerNormalSetMenu.Items[i]->Text_Y_Coordinate = 4;
		TimerNormalSetMenu.Items[i]->TextColor = White;
		TimerNormalSetMenu.Items[i]->Status = SET;
		TimerNormalSetMenu.Items[i]->ButtonColorShift = 1;

		// проставляем цвет вкл или выкл
		if(Timer_Temp_Struct.Normal_Property.TimeWork_Enable[i-20] == ENABLE)
		{
			TimerNormalSetMenu.Items[i]->ButtonColor = Green2;
			memcpy(TimerNormalSetMenu.Items[i]->Text, "ON", 2);
			TimerNormalSetMenu.Items[i]->Text[2] = 0;
		}
		else
		{
			TimerNormalSetMenu.Items[i]->ButtonColor = LightBlack2;
			memcpy(TimerNormalSetMenu.Items[i]->Text, "OFF", 3);
			TimerNormalSetMenu.Items[i]->Text[3] = 0;
		}
	}


	/* инициализируем кнопки - даты включения и даты выключения */
	for(j = 0; j < 4; j++)
	{
		if(j != 0)
		{
			if(j == 1) k = -30;
			else if(j == 2) k = 12;
			else if(j == 3) k = -15;
		}

		for(i = 0; i < 5; i++)
		{
			TimerNormalSetMenu.Items[i+(j*5)]->X0 = 74 + (j*85) + k;
			TimerNormalSetMenu.Items[i+(j*5)]->Y0 = (Btn_Y0(i));
			TimerNormalSetMenu.Items[i+(j*5)]->X1 = 130 + (j*85) + k;
			TimerNormalSetMenu.Items[i+(j*5)]->Y1 = Btn_Y0(i) + 40;
			TimerNormalSetMenu.Items[i+(j*5)]->ICO = (void*)0;
			TimerNormalSetMenu.Items[i+(j*5)]->ButtonPreset = RESET;
			TimerNormalSetMenu.Items[i+(j*5)]->ButtonColor = 0;
			TimerNormalSetMenu.Items[i+(j*5)]->ButtonColorShift = 0;
			TimerNormalSetMenu.Items[i+(j*5)]->Font = (FONT_INFO*)&arialUnicodeMS22ptFontInfo;
			TimerNormalSetMenu.Items[i+(j*5)]->Text_Y_Coordinate = 0;
			TimerNormalSetMenu.Items[i+(j*5)]->Status = SET;

			// цвет надписи в зависимости от вкл/выкл
			if(Timer_Temp_Struct.Normal_Property.TimeWork_Enable[i] == ENABLE)
			{
				TimerNormalSetMenu.Items[i+(j*5)]->TextColor = LighGreen;
			}
			else TimerNormalSetMenu.Items[i+(j*5)]->TextColor = White;


			/*  текст в зависимости от значений программы таймера  */
			if((j == 0) || (j == 2))
			{
				if(j == 0)			// если первый столбец то даты включения
				{
					Data_0 = Timer_Temp_Struct.Normal_Property.Start_Day_Index[i];
				}
				else if(j == 2)		// если третий столбец даты вЫключения
				{
					Data_0 = Timer_Temp_Struct.Normal_Property.Stop_Day_Index[i];
				}

				ConvertToString(Data_0, TimerNormalSetMenu.Items[i+(j*5)]->Text, 2);
				TimerNormalSetMenu.Items[i+(j*5)]->Text[3] = 0;
			}
			else
			{
				if(j == 1)		// если второй столбец то времена включения
				{
					DayData_ONTimeSec = Timer_Temp_Struct.Normal_Property.ON_Time[i];

					Data_0 = DayData_ONTimeSec / 3600;
					Data_1 = (DayData_ONTimeSec % 3600) / 60;
//					DayTimeText[i+(j*5)][2] = ':';
				}
				else if(j == 3)		// если четвертый столбец то времена вЫключения
				{
					DayData_ONTimeSec = Timer_Temp_Struct.Normal_Property.OFF_Time[i];

					Data_0 = DayData_ONTimeSec / 3600;
					Data_1 = (DayData_ONTimeSec % 3600) / 60;
//					DayTimeText[i+(j*5)][2] = ':';
				}

				ConvertToString(Data_0, TimerNormalSetMenu.Items[i+(j*5)]->Text, 2);
				ConvertToString(Data_1, &TimerNormalSetMenu.Items[i+(j*5)]->Text[3], 2);
				TimerNormalSetMenu.Items[i+(j*5)]->Text[2] = ':';
				TimerNormalSetMenu.Items[i+(j*5)]->Text[5] = 0;

				//			DayTimeText[i+(j*5)][0] = (Data_0 / 10) + 48;
				//			DayTimeText[i+(j*5)][1] = (Data_0 % 10) + 48;

				//			DayTimeText[i+(j*5)][3] = (Data_1 / 10) + 48;
				//			DayTimeText[i+(j*5)][4] = (Data_1 % 10) + 48;
			}

//			memcpy((char*), (char*)DayTimeText[i+(j*5)], 30);
		}
	}

	// рисуем линии
	LCD_SetGraphicsColor(GreenLine);
	LCD_DrawLine(0, 65, 479, 65);
	LCD_DrawLine(223, 75, 223, 285);
	LCD_DrawLine(410, 75, 410, 285);

	// отрисовываем вверхние линии и надписи
	LCD_SetFont(&microsoftSansSerif_10ptFontInfo);
	LCD_SetTextColor(Orange1);
	LCD_PutStrig(63, 275, 1, "Дата и Время вкл.");
	LCD_PutStrig(239, 275, 1, "Дата и Время выкл.");
	LCD_DrawLine(61, 275, 203, 275);
	LCD_DrawLine(238, 275, 392, 275);

	ButtonsDownMenuInit();
}


/*******************************************************************************
* Function Name  : ButtonsDownMenuInit
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void ButtonsDownMenuInit(void)
{
	uint8_t i;

	/* инициализируем кнопок сохранения, выхода и т.д */
	for(i = 25; i < 30; i++)
	{
		TimerNormalSetMenu.Items[i]->X0 = BtnSettings_X0[i-25];
		TimerNormalSetMenu.Items[i]->Y0 = 5;
		TimerNormalSetMenu.Items[i]->X1 = BtnSettings_X1[i-25];
		TimerNormalSetMenu.Items[i]->Y1 = 65;
		TimerNormalSetMenu.Items[i]->ButtonPreset = RESET;
		TimerNormalSetMenu.Items[i]->ButtonColorShift = 0;
		TimerNormalSetMenu.Items[i]->Font = (FONT_INFO*)&microsoftSansSerif_10ptFontInfo;
		memcpy((char*)TimerNormalSetMenu.Items[i]->Text, (char*)(TimerMenu_TextNames[i-25]), 30);
		TimerNormalSetMenu.Items[i]->Text_Y_Coordinate = 5;
		TimerNormalSetMenu.Items[i]->TextColor = ButtonTextColor;
		TimerNormalSetMenu.Items[i]->Status = SET;
	}

	TimerNormalSetMenu.Items[25]->ICO = (BITMAP_INFO_16*)&Exit_ICOInfo;
	TimerNormalSetMenu.Items[26]->ICO = (BITMAP_INFO_16*)&Save_ICOInfo;
	TimerNormalSetMenu.Items[27]->ICO = (BITMAP_INFO_16*)&DailyTimer_ICOInfo;
	TimerNormalSetMenu.Items[28]->ICO = (BITMAP_INFO_16*)&NormalTimer_ICOInfo;
	TimerNormalSetMenu.Items[29]->ICO = (BITMAP_INFO_16*)&ResetAll_ICOInfo;
}


