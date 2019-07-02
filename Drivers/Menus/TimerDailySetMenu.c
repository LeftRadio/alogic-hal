/**
   ******************************************************************************
   * @file      TimerDailySetMenu.c 
   * @author		LeftRadio
   * @version 	V1.0.0
   * @date      15.04.2013 23:08:21
   * @brief
   ******************************************************************************
**/ 

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h" 
#include "Timers.h"
#include "Pparam.h"
#include "Outputs.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define defHours		0
#define defMinutes		0
#define defSeconds		0

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void TimerDailySetMenu_Draw(void);
void TimerDailySetMenu_ReturnEvent(void);

void TimerDailyON_OFF_Program_ItemEvent(void);
void TimerDailyON_Times_ItemEvent(void);
void TimerDailyTimeWork_ItemEvent(void);

static void TimerMenu_Enable_Contol_Icons(void);
static void Daily_Update_SetParametr(uint32_t NewData, uint8_t txtShift, uint8_t charNum);
static void TimerDailySetMenu_Init_ButtonsColors_and_Values(void);

static void TimerDaily_Reset_AllSetting_Event(void);
static void TimerDaily_Menu_Return_Event(void);
static void TimerDaily_Save_Event(void);
static void TimerDaily_DailyMode_Event(void);
static void TimerDaily_NormalMode_Event(void);

static void OK_ItemEvent(void);
static void Down_ItemEvent(void);
static void Up_ItemEvent(void);
static void Right_ItemEvent(void);
static void Left_ItemEvent(void);

static void UP_DOWN_Event(int8_t sign);
static void RIGHT_LEFT_Event(int8_t sign);

/* Private variables ---------------------------------------------------------*/
extern menuItem_INFO TimerDaily_ON_OFF_0_Item, TimerDaily_ON_OFF_1_Item, TimerDaily_ON_OFF_2_Item, TimerDaily_ON_OFF_3_Item, TimerDaily_ON_OFF_4_Item;
extern menuItem_INFO TimerDailyTimeON_4_Item, TimerDailyTimeON_3_Item, TimerDailyTimeON_2_Item, TimerDailyTimeON_1_Item, TimerDailyTimeON_0_Item;
extern menuItem_INFO TimerDailyTimeWork_Item;
extern menuItem_INFO Timer_Reset_BTN, Timer_ExitWithoutSave__BTN, Timer_Save__BTN, Timer_Timer_to_Daily__BTN, Timer_Timer_to_Normal__BTN;
extern menuItem_INFO OK_Item, Down_Item, Up_Item, Right_Item, Left_Item;

Menu_Struct_TypeDef TimerDailySetMenu = {
		16,
		{ // кнопки меню
				&TimerDaily_ON_OFF_0_Item, &TimerDaily_ON_OFF_1_Item, &TimerDaily_ON_OFF_2_Item, &TimerDaily_ON_OFF_3_Item, &TimerDaily_ON_OFF_4_Item,
				&TimerDailyTimeON_0_Item, &TimerDailyTimeON_1_Item, &TimerDailyTimeON_2_Item, &TimerDailyTimeON_3_Item, &TimerDailyTimeON_4_Item,
				&TimerDailyTimeWork_Item,
				&Timer_ExitWithoutSave__BTN, &Timer_Save__BTN, &Timer_Timer_to_Daily__BTN, &Timer_Timer_to_Normal__BTN, &Timer_Reset_BTN,
				&OK_Item, &Down_Item, &Up_Item, &Right_Item, &Left_Item,
		},
		{ // функции кнопок меню
				&TimerDailyON_OFF_Program_ItemEvent, &TimerDailyON_OFF_Program_ItemEvent, &TimerDailyON_OFF_Program_ItemEvent, &TimerDailyON_OFF_Program_ItemEvent, &TimerDailyON_OFF_Program_ItemEvent,
				&TimerDailyON_Times_ItemEvent, &TimerDailyON_Times_ItemEvent, &TimerDailyON_Times_ItemEvent, &TimerDailyON_Times_ItemEvent, &TimerDailyON_Times_ItemEvent,
				&TimerDailyTimeWork_ItemEvent,
				&TimerDaily_Menu_Return_Event, &TimerDaily_Save_Event, &TimerDaily_DailyMode_Event, &TimerDaily_NormalMode_Event, &TimerDaily_Reset_AllSetting_Event,
				&OK_ItemEvent, &Down_ItemEvent, &Up_ItemEvent, &Right_ItemEvent, &Left_ItemEvent,
		},
		TRUE,
		&TimerDailySetMenu_Draw,
		0,
		0,
}; 


/* --------------------------- Параметры меню --------------------------- */
// время включения, 5 программ
ParamStruct_TypeDef Daily_Time_ON[5] = {
		{ 2, { {defHours, 23, 0, 2}, {defMinutes, 59, 0, 2} }, 0 },
		{ 2, { {defHours, 23, 0, 2}, {defMinutes, 59, 0, 2} }, 0 },
		{ 2, { {defHours, 23, 0, 2}, {defMinutes, 59, 0, 2} }, 0 },
		{ 2, { {defHours, 23, 0, 2}, {defMinutes, 59, 0, 2} }, 0 },
		{ 2, { {defHours, 23, 0, 2}, {defMinutes, 59, 0, 2} }, 0 },
};

// время работы
ParamStruct_TypeDef Daily_Time_Work = { 3, { {defHours, 23, 0, 2}, {defMinutes, 59, 0, 2}, {defSeconds, 59, 0, 2} }, 0 };

// указатель на активный параметр
ParamStruct_TypeDef *Daily_SettParametr = (void*)0;

// индекс активного параметра программы таймера для выставления
volatile uint8_t Daily_ProgramIndex = 0;

// индекс активной кнопки параметра программы таймера для выставления
volatile uint8_t DailyActiveButtonIndx = 0;




/* Functions ----------------------------------------------------------------*/
 
/*******************************************************************************
 * Function Name  : TimerDailySetMenu_Draw
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void TimerDailySetMenu_Draw(void)
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

	TimerDailySetMenu_Init_ButtonsColors_and_Values();

	// рисуем все кнопки меню
	Draw_All_Items_Menu(0, TimerDailySetMenu.MaxItems);

	SlashLine(SlashLineInit, 5, 0);
}
 
 
 
 
/*******************************************************************************
 * Function Name  : TimerDailyON_OFF_Program_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void TimerDailyON_OFF_Program_ItemEvent(void)
{ 
	uint8_t i, j = 0;
	FunctionalState tmpFlashingStatus = FlashingText_GetState();

	/* запрещаем мигание */
	FlashingText_Command(DISABLE);

	// если программа была запрещена
	if(Timer_Temp_Struct.DailyProperty.TimeWork_Enable[TouchObject] == DISABLE)
	{
		// обновляем кнопку включения/выключения программы
		TimerDailySetMenu.Items[TouchObject]->ButtonColor = Green2;
		memcpy(TimerDailySetMenu.Items[TouchObject]->Text, "ON", 2);
		TimerDailySetMenu.Items[TouchObject]->Text[2] = 0;

		// включаем программу
		Timer_Temp_Struct.DailyProperty.TimeWork_Enable[TouchObject] = ENABLE;
		Timer_Temp_Struct.State = ENABLE;

		// обновляем зеленым текст установки программы
		TimerDailySetMenu.Items[TouchObject+5]->TextColor = LighGreen;
	}
	// иначе если программа была включена
	else
	{
		// обновляем кнопку включения/выключения программы
		TimerDailySetMenu.Items[TouchObject]->ButtonColor = LightBlack2;
		memcpy(TimerDailySetMenu.Items[TouchObject]->Text, "OFF", 3);
		TimerDailySetMenu.Items[TouchObject]->Text[4] = 0;

		// выключаем программу
		Timer_Temp_Struct.DailyProperty.TimeWork_Enable[TouchObject] = DISABLE;

		// обновляем белым текст установки программы
		TimerDailySetMenu.Items[TouchObject+5]->TextColor = White;

		// проверяем есть ли еще включенные программы
		for(i = 0; i < 5; i++)
		{
			if(Timer_Temp_Struct.DailyProperty.TimeWork_Enable[i] == DISABLE) j++;
		}

		// если больше включенных програм не нашли, то выключаем полностью таймер
		if(j >= 5) Timer_Temp_Struct.State = DISABLE;
	}

	// перерисовывем кнопки
	Draw_Item_Menu(TimerDailySetMenu.Items[TouchObject]);
	Draw_Item_Menu(TimerDailySetMenu.Items[TouchObject+5]);


	/* возвращаем мигание в исходное состояние */
	FlashingText_Command(tmpFlashingStatus);
} 
 
 
 
/*******************************************************************************
 * Function Name  : TimerDailyON_Times_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void TimerDailyON_Times_ItemEvent(void)
{ 
	Daily_ProgramIndex = TouchObject - 5;
	DailyActiveButtonIndx = TouchObject;

	Daily_SettParametr = &Daily_Time_ON[Daily_ProgramIndex];			// Активный параметр
	Daily_SettParametr->Position = 0;

	TimerMenu_Enable_Contol_Icons();
} 
 
 

/*******************************************************************************
 * Function Name  : TimerDailyTimeWork_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void TimerDailyTimeWork_ItemEvent(void)
{ 
	DailyActiveButtonIndx = TouchObject;

	Daily_SettParametr = &Daily_Time_Work;			// Активный параметр
	Daily_SettParametr->Position = 0;

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
	TimerDailySetMenu.MaxItems = 21;

	/* запрещаем мигание */
	FlashingText_Command(DISABLE);

	// рисуем стрелки и кнопку 'ОК'
	Draw_All_Items_Menu(0, TimerDailySetMenu.MaxItems);

	// рисуем подчеркивание
	SlashLine(SlashLineDraw, DailyActiveButtonIndx, 0);

	/* рарешаем мигание */
	FlashingText_Enable(0, TimerDailySetMenu.Items[TouchObject]);
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
	TimerDailySetMenu.MaxItems = 16;

	FlashingText_Command(DISABLE);
	SlashLine(SlashLineClear, 0, 0);
	LCD_ClearArea(420, 66, 479, 285, Global_BackColor);

	// рисуем все кнопки меню
	Draw_All_Items_Menu(0, TimerDailySetMenu.MaxItems);

	Daily_SettParametr = (void*)0;
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
	int8_t position = Daily_SettParametr->Position;

	// изменение параметра на +1 или -1
	Daily_SettParametr->Item[position].Param += sign;

	// проверка на выход за пределы
	if(Daily_SettParametr->Item[position].Param < Daily_SettParametr->Item[position].MIN )
	{
		Daily_SettParametr->Item[position].Param = Daily_SettParametr->Item[position].MAX;
	}
	else if(Daily_SettParametr->Item[position].Param > Daily_SettParametr->Item[position].MAX)
	{
		Daily_SettParametr->Item[position].Param = Daily_SettParametr->Item[position].MIN;
	}

	Daily_Update_SetParametr(Daily_SettParametr->Item[position].Param, position, Daily_SettParametr->Item[position].SymNum);
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
	Daily_SettParametr->Position += sign;

	if(Daily_SettParametr->Position >= Daily_SettParametr->MaxParametrs) Daily_SettParametr->Position = Daily_SettParametr->MaxParametrs - 1;
	else if(Daily_SettParametr->Position < 0) Daily_SettParametr->Position = 0;

	// рисуем подчеркивание
	SlashLine(SlashLineDraw, DailyActiveButtonIndx, Daily_SettParametr->Position);
}







/*----------------------------------------------- нижнее меню --------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : TimerDaily_Reset_AllSetting_Event
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void TimerDaily_Reset_AllSetting_Event(void)
{
	uint8_t Data0 = 0, Data1 = 0;

	FlashingText_Command(DISABLE);

	// вычисляем номер таймера
	Data0 = (Timer_Temp_Struct.Name[0] - 48) * 10;
	Data0 = (Timer_Temp_Struct.Name[1] - 48);

	// сбрасываем таймер
	One_Timer_Default_Init((Timer_INFO*)&Timer_Temp_Struct, Timer_Temp_Struct.Out_Index, Data0 + Data1);
	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);

	TimerDailySetMenu_Init_ButtonsColors_and_Values();

	// рисуем все кнопки меню
	if(TimerDailySetMenu.MaxItems > 16) TimerDailySetMenu.MaxItems = 16;
	Draw_All_Items_Menu(0, TimerDailySetMenu.MaxItems);

	SlashLine(SlashLineInit, 5, 0);
}


/*******************************************************************************
 * Function Name  : TimerDaily_Menu_Return_Event
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void TimerDaily_Menu_Return_Event(void)
{
	FlashingText_Command(DISABLE);

	SetActiveMenu(&AllTimersMenu);

	/* очистка старого и отрисовка нового активного меню */
	LCD_ClearArea(0, 5, 479, 294, Global_BackColor);

	LCD_ClearArea(410, 298, 460, 315, Global_BackColor);

	Draw_ActiveMenu();
}



/*******************************************************************************
 * Function Name  : TimerDaily_Reset_AllSetting_Event
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void TimerDaily_Save_Event(void)
{
	FlashingText_Command(DISABLE);

	Timer_Temp_Struct.Mode = Timer_DailyMode;
	Draw_Item_Menu(TimerDailySetMenu.Items[DailyActiveButtonIndx]);

	Timer_CopyStruct_To_Timer((Timer_INFO*)&Timers_Struct[Timer_ActiveIndx], (Timer_INFO*)&Timer_Temp_Struct);

	SaveTimerSettings(Timer_ActiveIndx);
}


/*******************************************************************************
 * Function Name  : TimerDaily_Reset_AllSetting_Event
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void TimerDaily_DailyMode_Event(void)
{

}


/*******************************************************************************
 * Function Name  : TimerDaily_Reset_AllSetting_Event
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void TimerDaily_NormalMode_Event(void)
{
	FlashingText_Command(DISABLE);
	SetActiveMenu(&TimerNormalSetMenu);

	LCD_ClearArea(0, 0, 479, 294, Global_BackColor);
	Draw_ActiveMenu();
}




 
 


/*--------------------------------------------- особые функции меню --------------------------------------------------*/

/*******************************************************************************
* Function Name  : Daily_Update_SetParametr
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
static void Daily_Update_SetParametr(uint32_t NewData, uint8_t txtShift, uint8_t charNum)
{
	uint16_t textColor = pMenu->Items[DailyActiveButtonIndx]->TextColor;

	// очищаем старый текст параметра программы таймера
	pMenu->Items[DailyActiveButtonIndx]->TextColor = Global_BackColor;
	Draw_Item_Menu(pMenu->Items[DailyActiveButtonIndx]);

	// обновляем текст кнопки и устанавливаем цвет текста
	ConvertToString(NewData, &pMenu->Items[DailyActiveButtonIndx]->Text[txtShift*3], charNum);
	pMenu->Items[DailyActiveButtonIndx]->TextColor = textColor;

	// обновляем значения во временном таймере
	if(Daily_SettParametr != &Daily_Time_Work)
	{
		Timer_Temp_Struct.DailyProperty.ON_Times[Daily_ProgramIndex] = \
				(Daily_SettParametr->Item[0].Param * 3600) + (Daily_SettParametr->Item[1].Param * 60);
	}
	else
	{
		Timer_Temp_Struct.DailyProperty.Workings_Time = \
				(Daily_SettParametr->Item[0].Param * 3600) + (Daily_SettParametr->Item[1].Param * 60) + Daily_SettParametr->Item[2].Param;
	}

	// копируем текст в мигающую кнопку и отрисовываем кнопку
	memcpy((char*)FlashingButton.Text, (char*)pMenu->Items[DailyActiveButtonIndx]->Text, 9);
	Draw_Item_Menu(pMenu->Items[DailyActiveButtonIndx]);
}


/*******************************************************************************
* Function Name  : TimerDailySetMenu_Init_ButtonsColors_and_Values
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
static void TimerDailySetMenu_Init_ButtonsColors_and_Values(void)
{
	uint8_t i;
	uint32_t DayData_ONTimeSec = 0;

	/* инициализируем кнопки вкл/выкл программ таймера */
	for(i = 0; i < 5; i++)
	{
		// проставляем цвет вкл или выкл
		if(Timer_Temp_Struct.DailyProperty.TimeWork_Enable[i] == ENABLE)
		{
			TimerDailySetMenu.Items[i]->ButtonColor = Green2;
			memcpy(TimerDailySetMenu.Items[i]->Text, "ON", 2);
			TimerDailySetMenu.Items[i]->Text[2] = 0;
		}
		else
		{
			TimerDailySetMenu.Items[i]->ButtonColor = LightBlack2;
			memcpy(TimerDailySetMenu.Items[i]->Text, "OFF", 3);
			TimerDailySetMenu.Items[i]->Text[3] = 0;
		}

		// цвет текста кнопки параметра программы таймера в зависимости от вкл/выкл
		if(Timer_Temp_Struct.DailyProperty.TimeWork_Enable[i] == ENABLE) TimerDailySetMenu.Items[i+5]->TextColor = LighGreen;
		else TimerDailySetMenu.Items[i+5]->TextColor = White;

		/* значение текста в зависимости от значений программы таймера  */
		DayData_ONTimeSec = Timer_Temp_Struct.DailyProperty.ON_Times[i];

		Daily_Time_ON[i].Item[0].Param = DayData_ONTimeSec / 3600;			// часы
		Daily_Time_ON[i].Item[1].Param = (DayData_ONTimeSec % 3600) / 60;	// минуты

		ConvertToString(Daily_Time_ON[i].Item[0].Param, (char*)&TimerDailySetMenu.Items[i+5]->Text, 2);
		ConvertToString(Daily_Time_ON[i].Item[1].Param, (char*)&TimerDailySetMenu.Items[i+5]->Text[3], 2);

		TimerDailySetMenu.Items[i+5]->Text[2] = ':';
		TimerDailySetMenu.Items[i+5]->Text[5] = 0;
	}

	/* текст в зависимости от значения времени работы  */
	DayData_ONTimeSec = Timer_Temp_Struct.DailyProperty.Workings_Time;

	Daily_Time_Work.Item[0].Param = DayData_ONTimeSec / 3600;				// часы
	Daily_Time_Work.Item[1].Param = (DayData_ONTimeSec % 3600) / 60;		// минуты
	Daily_Time_Work.Item[2].Param = ((DayData_ONTimeSec % 3600) % 60);		// секунды

	ConvertToString(Daily_Time_Work.Item[0].Param, (char*)&TimerDailySetMenu.Items[i+5]->Text, 2);
	ConvertToString(Daily_Time_Work.Item[1].Param, (char*)&TimerDailySetMenu.Items[i+5]->Text[3], 2);
	ConvertToString(Daily_Time_Work.Item[2].Param, (char*)&TimerDailySetMenu.Items[i+5]->Text[6], 2);

	TimerDailySetMenu.Items[i+5]->Text[2] = ':';
	TimerDailySetMenu.Items[i+5]->Text[5] = ':';
	TimerDailySetMenu.Items[i+5]->Text[8] = 0;


	// рисуем линии
	LCD_SetGraphicsColor(GreenLine);
	LCD_DrawLine(0, 65, 479, 65);
	LCD_DrawLine(410, 75, 410, 285);

	// отрисовываем надписи
	LCD_SetFont(&microsoftSansSerif_10ptFontInfo);
	LCD_SetTextColor(Orange1);
	LCD_PutStrig(75, 280, 1, "Время вкл.");
	LCD_DrawLine(74, 278, 161, 278);

	LCD_SetFont(&microsoftSansSerif_12ptFontInfo);
	LCD_PutStrig(227, 243, 1, "Время работы");
	LCD_DrawLine(222, 241, 348, 241);

	ButtonsDownMenuInit();
}


