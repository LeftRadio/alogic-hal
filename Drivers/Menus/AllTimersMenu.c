/**
  ******************************************************************************
  * @file		All Timers Menu
  * @author		LeftRadio
  * @version 	V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/


/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h"
#include "Outputs.h"
#include "Timers.h"

extern menuItem_INFO Main_Settings, Main_Timers, Main_Outs, Main_Regulate, Main_Graphics;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define btnSW					3
#define UpperBtn				290
#define btnHigh					40

#define Btn_Y1(i)				(UpperBtn - (btnHigh + btnSW) * (i+1))


/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void AllTimersMenu_Draw(void);
void AllTimersMenu__Init_Buttons(void);
void AllTimersMenu_Return_Event(void);
void AllTimersMenuButtons_Event(void);

extern void Main_Outs_Event(void);
extern void Main_Regulate_Event(void);
extern void Main_Graphics_Event(void);
extern void Main_Settings_Event(void);

void Update_Timer_OUT(uint8_t Indx);

/* Private variables ---------------------------------------------------------*/
//menuItem_INFO Timer_Button_Home;
menuItem_INFO Timer_Button_1, Timer_Button_2, Timer_Button_3, Timer_Button_4, Timer_Button_5;
menuItem_INFO Timer_Button_6, Timer_Button_7, Timer_Button_8, Timer_Button_9, Timer_Button_10;
menuItem_INFO Timer_Button_11, Timer_Button_12, Timer_Button_13, Timer_Button_14, Timer_Button_15;

static const char *TimersButtonText[15] = {
		"ТАЙМЕР 1", "ТАЙМЕР 2", "ТАЙМЕР 3", "ТАЙМЕР 4", "ТАЙМЕР 5",
		"ТАЙМЕР 6", "ТАЙМЕР 7", "ТАЙМЕР 8", "ТАЙМЕР 9", "ТАЙМЕР 10",
		"ТАЙМЕР 11", "ТАЙМЕР 12", "ТАЙМЕР 13", "ТАЙМЕР 14", "ТАЙМЕР 15",
};

Menu_Struct_TypeDef AllTimersMenu = {

		20,
		{  // кнопки меню
				&Timer_Button_1, &Timer_Button_2, &Timer_Button_3, &Timer_Button_4, &Timer_Button_5,
				&Timer_Button_6, &Timer_Button_7, &Timer_Button_8, &Timer_Button_9, &Timer_Button_10,
				&Timer_Button_11, &Timer_Button_12, &Timer_Button_13, &Timer_Button_14, &Timer_Button_15,
				&Main_Timers, &Main_Outs, &Main_Regulate, &Main_Graphics, &Main_Settings,
		},
		{ // функции кнопок меню
				&AllTimersMenuButtons_Event, &AllTimersMenuButtons_Event, &AllTimersMenuButtons_Event, &AllTimersMenuButtons_Event,
				&AllTimersMenuButtons_Event, &AllTimersMenuButtons_Event, &AllTimersMenuButtons_Event, &AllTimersMenuButtons_Event,
				&AllTimersMenuButtons_Event, &AllTimersMenuButtons_Event, &AllTimersMenuButtons_Event, &AllTimersMenuButtons_Event,
				&AllTimersMenuButtons_Event, &AllTimersMenuButtons_Event, &AllTimersMenuButtons_Event,
				&AllTimersMenu_Return_Event, &Main_Outs_Event, &Main_Regulate_Event, &Main_Graphics_Event, &Main_Settings_Event,
		},
		TRUE,
		AllTimersMenu_Draw,
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
void AllTimersMenu_Draw(void)
{
	uint8_t i = 0;
	char *TimerModeString;

	// рисуем линии
	LCD_SetGraphicsColor(0x4410);
	LCD_DrawLine(0, 65, 479, 65);

	Down_Menu_Reset();
	AllTimersMenu__Init_Buttons();

	/* заменяем иконку у кнопки таймера, теперь кнопка выхода */
	Main_Timers.ICO = (BITMAP_INFO_16*)&Home_ICOInfo;
	memcpy(Main_Timers.Text, "Главная", 7);

	LCD_SetBackColor(LightBlack3);		// цвет тени у кнопок

	// рисуем все кнопки меню
	Draw_All_Items_Menu(0, AllTimersMenu.MaxItems);

	LCD_SetTextColor(LightSky);
	LCD_SetFont(&microsoftSansSerif_10ptFontInfo);

	for(i = 0; i < 15; i++)
	{
		if(Timers_Struct[i].Mode == Timer_NormalMode) TimerModeString = "Простой";
		else TimerModeString = "Суточный";
		LCD_PutStrig(AllTimersMenu.Items[i]->X0 + 25, AllTimersMenu.Items[i]->Y0 + (AllTimersMenu.Items[i]->Text_Y_Coordinate - 15), 1, TimerModeString);
	}
}


/*******************************************************************************
* Function Name  : AllTimersMenu_Return_Event
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void AllTimersMenu_Return_Event(void)
{
	Down_Menu_Reset();

	LCD_ClearArea(Main_Timers.X0, Main_Timers.Y0 - 4, Main_Timers.X1 + 5, Main_Timers.Y1 - 10, Black);
	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);

	SetActiveMenu(&MainMenu);
	Draw_ActiveMenu();
}


/*******************************************************************************
* Function Name  : AllTimersMenuButtons_Event
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void AllTimersMenuButtons_Event(void)
{
	Timer_ActiveIndx = TouchObject;

	/* Копируем таймер во времменый таймер */
	Timer_CopyStruct_To_Timer((Timer_INFO*)&Timer_Temp_Struct, (Timer_INFO*)&Timers_Struct[Timer_ActiveIndx]);

	/* если режим таймера нормальный то сделать активным нормальное меню таймера */
	if(Timer_Temp_Struct.Mode == Timer_NormalMode)
	{
		SetActiveMenu(&TimerNormalSetMenu);
	}
	else	// иначе сделать активным суточное меню таймера
	{
		SetActiveMenu(&TimerDailySetMenu);
	}

	/* очистка старого и отрисовка нового активного меню */
	LCD_ClearArea(0, 5, 479, 294, Global_BackColor);
	Draw_ActiveMenu();
}



/*******************************************************************************
* Function Name  : AllTimersMenu_Return_Event
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void AllTimersMenu__Init_Buttons(void)
{
	uint8_t i, j;

	/* инициализируем кнопок таймеров */
	for(j = 0; j < 3; j++)					// три столбца
	{
		for(i = 0; i < 5; i++)
		{
			AllTimersMenu.Items[i+(j*5)]->X0 = 8 + (j * 160);					// 160 - расстояние между столбцами
			AllTimersMenu.Items[i+(j*5)]->Y0 = Btn_Y1(i);
			AllTimersMenu.Items[i+(j*5)]->X1 = 128 + (j * 160);
			AllTimersMenu.Items[i+(j*5)]->Y1 = Btn_Y1(i) + 40;
			AllTimersMenu.Items[i+(j*5)]->ICO = (void*)0;
			AllTimersMenu.Items[i+(j*5)]->ButtonPreset = SET;
			AllTimersMenu.Items[i+(j*5)]->ButtonColor = LightBlack2;  //DarkRed2;
			AllTimersMenu.Items[i+(j*5)]->ButtonColorShift = SET;
			AllTimersMenu.Items[i+(j*5)]->Font = (FONT_INFO*)&microsoftSansSerif_12ptFontInfo;
			memcpy(AllTimersMenu.Items[i+(j*5)]->Text, TimersButtonText[i+(j*5)], 30);
			AllTimersMenu.Items[i+(j*5)]->Text_Y_Coordinate = 17;			// смещение текста по Y

			AllTimersMenu.Items[i+(j*5)]->Status = SET;

			if(Timers_Struct[i+(j*5)].State == ENABLE) AllTimersMenu.Items[i+(j*5)]->TextColor = LighGreen;
			else AllTimersMenu.Items[i+(j*5)]->TextColor = White;

			if(pMenu == &AllTimersMenu) Update_Timer_OUT(i+(j*5));
		}
	}
}


/*******************************************************************************
* Function Name  : Update_Timer_OUT
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Update_Timer_OUT(uint8_t Indx)
{
	volatile BitAction BIT_State = Bit_RESET;

	if(Timers_Struct[Indx].Out_Index > 19) return;
	else if(Timers_Struct[Indx].Out_Index != Timer_OUT_NotAssigned)
	{
		BIT_State = OUT_SateRead(Timers_Struct[Indx].Out_Index);

		if(BIT_State == Bit_SET) LCD_SetGraphicsColor(LighGreen);
		else if(BIT_State == Bit_RESET)	LCD_SetGraphicsColor(Red);

		LCD_DrawFillRect(AllTimersMenu.Items[Indx]->X1 + 1, AllTimersMenu.Items[Indx]->Y0 - 1, AllTimersMenu.Items[Indx]->X1 + 11, AllTimersMenu.Items[Indx]->Y1 - 1, 0, 0);
	}
}

