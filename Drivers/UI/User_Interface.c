/**
  ******************************************************************************
  * @file	 User_Interface.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h"
#include "TouchScreen.h"
#include "GlobalInit.h"
#include "clock_calendar.h"
#include "Outputs.h"
#include "Timers.h"
#include "Regulates.h"
#include "hello.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define Separator0_X		182
#define Separator1_X		315
#define Separator2_X		400

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t Global_BackColor = Black;
Menu_Struct_TypeDef *pMenu;
volatile uint8_t TouchObject = Menu_NoObject;

volatile menuItem_INFO FlashingButton;
Bool FlashingButtonUpdate = FALSE;
Bool Update_Out_Flag[20] = {FALSE};

extern volatile Bool UpdateOUT_Enable;		// Разрешение обновления выходов в меню выходов, запрещено когда назначаем

/* Extern function ---------------------------------------------------------*/
extern void AllTimersMenu__Init_Buttons(void);
extern void Update_Timer_OUT(uint8_t Indx);
extern void Update_Sensor_OUT(uint8_t Indx);

/* Private function prototypes -----------------------------------------------*/
static void JumpToEvent(void);



/* Functions ----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Change_Menu_Indx
* Description    : Функция смены индекса меню
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Interface_Init(void)
{
	Bool temp = BL_AutoOFF_Status;
	BL_AutoOFF_Status = FALSE;
	LCD_Set_BackLightPWM(0);
	LCD_Draw_16_IMG(0, 0, &hello_copyInfo, hello_copyInfo.RLE);
	LCD_SetFont(&DigitSansSerif11ptFontInfo);
	LCD_SetTextColor(0x0000);
	LCD_PutStrig(5, 5, 1, "ver 1.8.3");

	int k;
	for(k = 0; k < 100; k++)
	{
		LCD_Set_BackLightPWM(k);
		HAL_Delay(10);
		/* Reload IWDG counter */
		HAL_IWDG_Refresh(&hiwdg);
	}

	LCD_Set_BackLightPWM(100);
	HAL_Delay(500);

	for(k = 100; k > 0; k--)
	{
		LCD_Set_BackLightPWM(k);
		HAL_Delay(10);
		/* Reload IWDG counter */
		HAL_IWDG_Refresh(&hiwdg);
	}
	BL_AutoOFF_Status = temp;

	LCD_Set_BackLightPWM(70);
	//__enable_irq();

	LCD_FillScreen(Global_BackColor);
	//LCD_ClearArea(0, 0, 479, 319, Black);

	LCD_SetGraphicsColor(0x4410);
	LCD_DrawLine(0, 295, 479, 295);

	LCD_DrawLine(Separator0_X, 301, Separator0_X, 314);
	LCD_DrawLine(Separator0_X + 1, 301, Separator0_X + 1, 314);

	LCD_DrawLine(Separator1_X, 301, Separator1_X, 314);
	LCD_DrawLine(Separator1_X + 1, 301, Separator1_X + 1, 314);

	LCD_DrawLine(Separator2_X, 301, Separator2_X, 314);
	LCD_DrawLine(Separator2_X + 1, 301, Separator2_X + 1, 314);

	LCD_SetFont(&DigitSansSerif11ptFontInfo);
	LCD_SetTextColor(0xb7fb);
	LCD_DrawChar(344, 302, 1, ':');
	LCD_DrawChar(368, 302, 1, ':');

	LCD_Draw_16_IMG(461, 300, &BT_ICOInfo, 0);
}


/*******************************************************************************
* Function Name  : Display_Date_and_Time
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Display_Date_and_Time(uint8_t Reset)
{
	// переменные и текст под дату
	static uint8_t Old_Day = 1, Old_Month = 1;
	static uint16_t Old_Year = 1;
	static char Date_Text[30] = {""};
	static uint16_t WeekDayText_Coordinate_X; //, OLD_WeekDayText_Coordinate_X = WeekDayText_Coordinate_X;
	uint16_t tmp = 0;

	// переменные и текст под время
	static volatile uint8_t Old_TimeVar[6] = {244,244,244,244,244,244};

	if(Reset == 1)
	{
		Old_Day = 1; Old_Month = 1; Old_Year = 1;
		Old_TimeVar[0] = 244; Old_TimeVar[1] = 244; Old_TimeVar[2] = 244;
		Old_TimeVar[3] = 244; Old_TimeVar[4] = 244; Old_TimeVar[5] = 244;
	}

	static char Time_Hour_Text[2] = " ";
	static char Time_Minute_Text[2] = " ";
	static char Time_Second_Text[2] = " ";

	char tmp_Text[9];

	LCD_SetFont(&microsoftSansSerif_12ptFontInfo);

	Calculate_RTC_All();

	if((Old_Day != s_WeekDay) || (Old_Month != s_DateStructVar.Month) || (Old_Year != s_DateStructVar.Year))
	{
		// очиаем старые надписи
		LCD_SetTextColor(Global_BackColor);
		LCD_PutStrig(10, 297, 1, Date_Text);
		LCD_PutStrig(WeekDayText_Coordinate_X + 5, 297, 1, WeekDays_Names[Old_Day]);

		memset(Date_Text, 0, 30);

		Date_Text[0] = (s_DateStructVar.Day / 10) + 48;
		Date_Text[1] = (s_DateStructVar.Day % 10) + 48;
		memcpy(&Date_Text[2], " - ", 3);

		strcat(Date_Text, Months_Names[s_DateStructVar.Month - 1]);
		strcat(Date_Text, " - ");

		sprintf(tmp_Text, "%d", s_DateStructVar.Year);
		strcat(Date_Text, tmp_Text);

		// выводим новые дату и день недели
		LCD_Strig_PointsWidht(&tmp, WeekDays_Names[s_WeekDay]);
		WeekDayText_Coordinate_X = ((Separator0_X + Separator1_X) - tmp) / 2;

		LCD_SetTextColor(0xb7fb);
		LCD_PutStrig(10, 297, 1, Date_Text);
		LCD_PutStrig(WeekDayText_Coordinate_X + 5, 297, 1, WeekDays_Names[s_WeekDay]);

		Old_Day = s_WeekDay;
		Old_Month = s_DateStructVar.Month;
		Old_Year = s_DateStructVar.Year;
	}


	LCD_SetFont(&DigitSansSerif11ptFontInfo);

	// обновляем часы -------------------------------------------------
	if((Old_TimeVar[0] != s_TimeStructVar.HourHigh) || (Old_TimeVar[1] != s_TimeStructVar.HourLow))
	{
		// очиаем старые надписи
		LCD_PutColorStrig(326, 301, 1, Time_Hour_Text, Global_BackColor);

		Time_Hour_Text[0] = s_TimeStructVar.HourHigh + 48;
		Time_Hour_Text[1] = s_TimeStructVar.HourLow + 48;

		LCD_PutColorStrig(326, 301, 1, Time_Hour_Text, 0xb7fb);

		Old_TimeVar[0] = s_TimeStructVar.HourHigh;
		Old_TimeVar[1] = s_TimeStructVar.HourLow;
	}

	// обновляем минуты --------------------------------------------------------------
	if((Old_TimeVar[2] != s_TimeStructVar.MinHigh) || (Old_TimeVar[3] != s_TimeStructVar.MinLow))
	{
		// очиаем старые надписи
		LCD_PutColorStrig(350, 301, 1, Time_Minute_Text, Global_BackColor);

		Time_Minute_Text[0] = s_TimeStructVar.MinHigh + 48;
		Time_Minute_Text[1] = s_TimeStructVar.MinLow + 48;

		LCD_PutColorStrig(350, 301, 1, Time_Minute_Text, 0xb7fb);

		Old_TimeVar[2] = s_TimeStructVar.MinHigh;
		Old_TimeVar[3] = s_TimeStructVar.MinLow;
	}

	// обновляем секунды --------------------------------------------------------------
	if((Old_TimeVar[4] != s_TimeStructVar.SecHigh) || (Old_TimeVar[5] != s_TimeStructVar.SecLow))
	{
		// очиаем старые надписи
		LCD_PutColorStrig(374, 301, 1, Time_Second_Text, Global_BackColor);

		Time_Second_Text[0] = s_TimeStructVar.SecHigh + 48;
		Time_Second_Text[1] = s_TimeStructVar.SecLow + 48;

		LCD_PutColorStrig(374, 301, 1, Time_Second_Text, 0xb7fb);

		Old_TimeVar[4] = s_TimeStructVar.SecHigh;
		Old_TimeVar[5] = s_TimeStructVar.SecLow;
	}
}


/*******************************************************************************
* Function Name  : SetActiveMenu
* Description    : Установить активное меню
* Input          : Menu_Struct_TypeDef *NewActiveMenu
* Output         : None
* Return         : None
*******************************************************************************/
void SetActiveMenu(Menu_Struct_TypeDef *NewActiveMenu)
{
	pMenu = NewActiveMenu;
}


/*******************************************************************************
* Function Name  : Active_Item_Menu
* Description    : Функция выбора активного пункта меню
* Input          : uint8_t Item_Index
* Output         : None
* Return         : None
*******************************************************************************/
void Draw_ActiveMenu(void)
{
	pMenu->MenuDrawFunc();
}


/*******************************************************************************
* Function Name  : Draw_Item_Menu
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Draw_All_Items_Menu(uint8_t StartItem, uint8_t EndItem)
{
	uint8_t i;

	for(i = StartItem; i < EndItem; i++)
	{
		Draw_Item_Menu(pMenu->Items[i]);
	}
}


/*******************************************************************************
* Function Name  : Display_Date_and_Time
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Draw_Item_Menu(menuItem_INFO *Menu_Item)
{
	uint16_t X_ICO = 0, tmpX_ICO = 0, Y_ICO = Menu_Item->Y0;
	uint16_t Text_X0 = 0;

	if(Menu_Item->Status != SET) return;

	if(Menu_Item->ButtonPreset == SET)
	{
		if(Menu_Item->ICO != 0)
		{
			LCD_Draw_16_IMG(Menu_Item->X0, Menu_Item->Y0, Menu_Item->ICO, Menu_Item->ICO->RLE);

			LCD_Strig_PointsWidht(&tmpX_ICO, (char*)Menu_Item->Text);
			Text_X0 = Menu_Item->X0 + (((Menu_Item->X1 - Menu_Item->X0) - tmpX_ICO) / 2);

			LCD_SetFont(Menu_Item->Font);
			LCD_SetTextColor(Menu_Item->TextColor);
			LCD_PutStrig(Text_X0, Menu_Item->Y0 + Menu_Item->Text_Y_Coordinate, 1, (char*)Menu_Item->Text);
		}
		else
		{
			LCD_SetFont(Menu_Item->Font);
			LCD_DrawButton(Menu_Item->X0, Menu_Item->Y0, Menu_Item->X1, Menu_Item->Y1, Menu_Item->Text_Y_Coordinate,
			(char*)Menu_Item->Text, 0, Menu_Item->ButtonColor, Menu_Item->TextColor, Menu_Item->ButtonColorShift);
		}
	}
	else
	{
		if((Menu_Item->Font != 0) && (Menu_Item->Text != 0))
		{
			LCD_SetFont(Menu_Item->Font);
			LCD_SetTextColor(Menu_Item->TextColor);
			LCD_PutStrig(Menu_Item->X0,Menu_Item->Y0 + Menu_Item->Text_Y_Coordinate, 1, (char*)Menu_Item->Text);
		}

		if(Menu_Item->ICO != 0)
		{
			if((Menu_Item->Font != 0) && (Menu_Item->Text != 0))
			{
				LCD_Strig_PointsWidht(&tmpX_ICO, (char*)Menu_Item->Text);
				Menu_Item->X1 = (Menu_Item->X0 + tmpX_ICO) - 4;

				X_ICO = Menu_Item->X0 + (((Menu_Item->X1 - Menu_Item->X0) - Menu_Item->ICO->Width) / 2);
				if(Menu_Item->Font != 0) Y_ICO = (Menu_Item->Y0 + Menu_Item->Font->Height) + 3;
			}
			else
			{
				X_ICO = Menu_Item->X0;
				Y_ICO = Menu_Item->Y0;
			}

			LCD_Draw_16_IMG(X_ICO, Y_ICO, Menu_Item->ICO, Menu_Item->ICO->RLE);
		}
	}
}


/*******************************************************************************
* Function Name  : Active_Item_Menu
* Description    : Функция выбора активного пункта меню
* Input          : uint8_t Item_Index
* Output         : None
* Return         : None
*******************************************************************************/
void Return_ActiveMenu(void)
{
	if(pMenu->MenuSpecialFunc != (void*)0) pMenu->MenuSpecialFunc();
}



/*******************************************************************************
* Function Name  : Menu_GetTouchObject
* Description    : Функция поиска нажатого объекта меню
* Input          : координаты нажатия тачскрина
* Output         : None
* Return         : индекс объекта в меню
*******************************************************************************/
static uint8_t Menu_GetTouchObject(uint16_t X, uint16_t Y)
{
	uint8_t i;

	for(i = 0; i < pMenu->MaxItems; i++)
	{
		if(pMenu->Items[i]->Status == SET)
		{
			if((X > pMenu->Items[i]->X0) && (X < pMenu->Items[i]->X1))
			{
				if((Y > pMenu->Items[i]->Y0) && (Y < pMenu->Items[i]->Y1))
				{
					return i;
				}
			}
		}
	}
	return Menu_NoObject;
}


/*******************************************************************************
* Function Name  : Menu_Func
* Description    : функция поиска нажатого объекта меню
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Menu_Func(void)
{
	static uint8_t Old_TouchObject = Menu_NoObject;
	static uint16_t TouchCounter = 0;

    TouchObject = Menu_GetTouchObject(TouchCoordinate.x, TouchCoordinate.y);

	if(TouchObject != Menu_NoObject)
	{
		if(TouchObject == Old_TouchObject)
		{
			TouchCounter++;

			if(TouchCounter >= 10)
			{
				TouchCounter--;
				JumpToEvent();
			}
		}
		else
		{
			Old_TouchObject = TouchObject;
			TouchCounter = 0;
			JumpToEvent();
		}

		ADC_Configuration(Touch_X);
		HAL_Delay(1);
		Touch_Start();
	}
	else
	{
		Old_TouchObject = Menu_NoObject;
		TouchCounter = 0;

		Touch_Stop();
		ADC_Configuration(Touch_INT);
	}
}


/*******************************************************************************
* Function Name  : JumpToEvent
* Description    : функция перехода на событие нажатого объекта меню
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void JumpToEvent(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	//LCD_Set_BackLightPWM(LCD_Get_BackLightPWM() + 20);
	HAL_Delay(50);
	pMenu->MenuItemEvent[TouchObject]();
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	//LCD_Set_BackLightPWM(LCD_Get_BackLightPWM() - 20);
}





/*******************************************************************************
* Function Name  : Menu_Flashing_Text
* Description    : функция мигания текста
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FlashingText_Enable(int16_t X_swPosX, menuItem_INFO *NewItem)
{
	if(NewItem == (void*)0)
	{
		FlashingText_Command(DISABLE);
		return;
	}

	FlashingButton.X0 = NewItem->X0 + X_swPosX;
	FlashingButton.Y0 = NewItem->Y0;
	FlashingButton.X1 = NewItem->X1;
	FlashingButton.Y1 = NewItem->Y1;
	FlashingButton.ICO = NewItem->ICO;
	FlashingButton.ButtonPreset = RESET;
	FlashingButton.ButtonColor = 0;
	FlashingButton.ButtonColorShift = 0;
	FlashingButton.Font = NewItem->Font;
	memcpy((char*)FlashingButton.Text, (char*)NewItem->Text, 30);
	FlashingButton.Text_Y_Coordinate = NewItem->Text_Y_Coordinate;			// смещение текста по Y
	FlashingButton.TextColor = NewItem->TextColor;


	FlashingButton.Status = SET;
	NVIC_EnableIRQ(TIM7_IRQn);
}


/*******************************************************************************
* Function Name  : FlashingText_Command
* Description    : функция включения мигания текста
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FlashingText_Command(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		FlashingButton.Status = SET;
		FlashingButtonUpdate = FALSE;
		NVIC_EnableIRQ(TIM7_IRQn);
	}
	else
	{
		NVIC_DisableIRQ(TIM7_IRQn);
		FlashingButton.Status = RESET;
		FlashingButtonUpdate = FALSE;
	}
}


/*******************************************************************************
* Function Name  : FlashingText_GetState
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
FunctionalState FlashingText_GetState(void)
{
	if(FlashingButton.Status == SET) return ENABLE;
	else return DISABLE;
}



/*******************************************************************************
* Function Name  : Update_SlashLine
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void SlashLine(uint8_t Init, uint8_t ButtonSlashIndx, uint8_t Position)
{
	static uint16_t X = 0, Y = 0;
	static uint8_t OLD_Shift = 0;

	const uint8_t Height = 25;
	uint8_t Shift = Position * 46;


	if(Init == SlashLineInit)
	{
		X = pMenu->Items[ButtonSlashIndx]->X0;
		Y = pMenu->Items[ButtonSlashIndx]->Y0 + 4;
		OLD_Shift = Shift;
	}
	else if((Init == SlashLineDraw) || (Init == SlashLineClear))
	{
		LCD_SetGraphicsColor(Global_BackColor);
		LCD_DrawLine(X + OLD_Shift, Y, X + OLD_Shift + Height, Y);

		X = pMenu->Items[ButtonSlashIndx]->X0;
		Y = pMenu->Items[ButtonSlashIndx]->Y0 + 4;
		OLD_Shift = Shift;

		if(Init == SlashLineClear) return;

		LCD_SetGraphicsColor(LighGreen);
		LCD_DrawLine(X + Shift, Y, X + Shift + Height, Y);
	}
}


/*******************************************************************************
* Function Name  : UpdateSensorsOnScreen
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Update_TempSensors_OnScreen(uint16_t SensorsData, uint8_t SensorNum, ErrorStatus OLD_SensorStatus, ErrorStatus SensorStatus)
{
	char Sens_DataText[3] = {'0', 0, 0};
	static char OLD_Text[8][3] = {{0}};
	static uint16_t OLD_Data[8];

	uint16_t ColorName = DarkGray;
	uint16_t ColorData = DarkGray;

	uint16_t ScreenPos_Y = 250 - (SensorNum * 25);
	uint16_t i, j;

	Sens_DataText[0] = (SensorNum + 1) + 48;

	if(SensorStatus != ERROR)
	{
		if(OLD_SensorStatus == ERROR) LCD_ClearArea(17, ScreenPos_Y, 64, ScreenPos_Y + 25, Global_BackColor);
		ColorName = 0x6a9c;
		ColorData = 0x9ff3;
	}

	LCD_SetFont(&sansation_22ptFontInfo);
	LCD_SetTextColor(ColorName);
	i = LCD_PutStrig(17, ScreenPos_Y, 1, "TEMP ");
	i = LCD_PutStrig(i, ScreenPos_Y, 1, Sens_DataText);

//	if(SensorsData > 38)
//	{
//		while(SensorStatus != ERROR);
//	}

	if(SensorStatus != ERROR)
	{
		Sens_DataText[0] = ((SensorsData) / 10) + 48;
		Sens_DataText[1] = ((SensorsData) % 10) + 48;
	}
	else
	{
		Sens_DataText[0] = '-';
		Sens_DataText[1] = '-';
	}

	if(OLD_Data[SensorNum] != SensorsData)
	{
		LCD_SetTextColor(Global_BackColor);
		j = LCD_PutStrig(i + 15, ScreenPos_Y, 1, &OLD_Text[SensorNum][0]);
		LCD_PutStrig(j + 4, ScreenPos_Y, 1, "C");

		memcpy(&OLD_Text[SensorNum][0], &Sens_DataText[0], 3);
		OLD_Data[SensorNum] = SensorsData;
	}

	LCD_SetTextColor(ColorData);
	i = LCD_PutStrig(i + 15, ScreenPos_Y, 1, Sens_DataText);
	LCD_PutStrig(i + 4, ScreenPos_Y, 1, "C");
}



/*******************************************************************************
* Function Name  : UpdateSensorsOnScreen
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Update_TempSensors_OnScreen_AllSensos(uint16_t SensorsData, uint8_t SensorNum, ErrorStatus OLD_SensorStatus, ErrorStatus SensorStatus)
{
	char Sens_DataText[3] = {'0', 0, 0};
	static volatile char OLD_Text[8][3] = {{0}};
	static uint16_t OLD_Data[8];

	uint16_t ColorData = DarkGray;

	uint16_t ScreenPos_X = (160 * (SensorNum/3)) + 110;
	uint16_t ScreenPos_Y = 0;


	if(SensorNum < 3) ScreenPos_Y = 235 - (SensorNum * 75);
	else if((SensorNum >= 3) && (SensorNum < 6)) ScreenPos_Y = 235 - ((SensorNum - 3) * 75);
	else if(SensorNum >= 6) ScreenPos_Y = 235 - ((SensorNum - 6) * 75);


	if(SensorStatus != ERROR)
	{
		if(OLD_SensorStatus == ERROR) LCD_ClearArea(ScreenPos_X, ScreenPos_Y, ScreenPos_X + 30, ScreenPos_Y + 25, Global_BackColor);
		ColorData = 0x9ff3;

		Sens_DataText[0] = ((SensorsData) / 10) + 48;
		Sens_DataText[1] = ((SensorsData) % 10) + 48;
	}
	else
	{
		Sens_DataText[0] = '-';
		Sens_DataText[1] = '-';
	}

	LCD_SetFont(&sansation_22ptFontInfo);

	if(OLD_Data[SensorNum] != SensorsData)
	{
		LCD_SetTextColor(Global_BackColor);
		LCD_PutStrig(ScreenPos_X, ScreenPos_Y, 1, (char*)&OLD_Text[SensorNum][0]);

		memcpy((char*)&OLD_Text[SensorNum][0], &Sens_DataText[0], 3);
		OLD_Data[SensorNum] = SensorsData;
	}

	LCD_SetTextColor(ColorData);
	LCD_PutStrig(ScreenPos_X, ScreenPos_Y, 1, Sens_DataText);
}



/*******************************************************************************
* Function Name  : Update_DHT_Sensor_OnScreen
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Update_HTSensor_OnScreen(uint8_t* Data, uint8_t DHT_SHT)
{
	char Sens_DataText[2][5] = {{0}};

	static char OLD_Text_0[5] = {0}, OLD_Text_1[5] = {0};
	static char OLD_Text_2[5] = {0}, OLD_Text_3[5] = {0};
	static uint16_t OLD_Data_0[2] = {255, 255};
	static uint16_t OLD_Data_1[2] = {255, 255};

	char *OLD_Text_U, *OLD_Text_D;
	uint16_t *OLD_Data;

	uint16_t ColorName = DarkGray;
	uint16_t ColorData = DarkGray;

	const uint16_t ScreenPos_X = 235;
	uint16_t ScreenPos_Y = 180;
	uint16_t i, j, k = ScreenPos_X;

	char SensorText_0[10];
	char SensorText_1[10];

if(DHT_SHT == 0)
{
	strcpy(SensorText_0, "DHT RH ");
	strcpy(SensorText_1, "DHT TEMP ");

	OLD_Text_U = OLD_Text_0;
	OLD_Text_D = OLD_Text_1;
	OLD_Data = OLD_Data_0;
	ScreenPos_Y = 250;
}
else
{
	strcpy(SensorText_0, "SHT RH ");
	strcpy(SensorText_1, "SHT TEMP ");
	OLD_Text_U = OLD_Text_2;
	OLD_Text_D = OLD_Text_3;
	OLD_Data = OLD_Data_1;
}


	ColorName = 0x6a9c;
	ColorData = 0x9ff3;

	LCD_SetFont(&sansation_22ptFontInfo);
	LCD_SetTextColor(ColorName);

	i = LCD_PutStrig(ScreenPos_X, ScreenPos_Y, 1, (char*)SensorText_0);
	LCD_Strig_PointsWidht(&k, (char*)SensorText_1);
	ConvertToString((*Data), &Sens_DataText[0][0], 2);

	if(*OLD_Data != *Data)
	{
		LCD_SetTextColor(Global_BackColor);
		j = LCD_PutStrig(k + 10, ScreenPos_Y, 1, OLD_Text_U);
		LCD_PutStrig(j + 4, ScreenPos_Y, 1, "%");

		memcpy(OLD_Text_U, &Sens_DataText[0][0], 3);
		*OLD_Data = *Data;
	}

	LCD_SetTextColor(ColorData);
	i = LCD_PutStrig(k + 10, ScreenPos_Y, 1, &Sens_DataText[0][0]);
	LCD_PutStrig(i + 4, ScreenPos_Y, 1, "%");


	Data += 2;

	ScreenPos_Y -= 30;
	LCD_SetTextColor(ColorName);
	i = LCD_PutStrig(ScreenPos_X, ScreenPos_Y, 1, (char*)SensorText_1);

	Sens_DataText[1][0] = ((*Data) / 10) + 48;
	Sens_DataText[1][1] = ((*Data) % 10) + 48;

	if(*(OLD_Data + 1) != *Data)
	{
		LCD_SetTextColor(Global_BackColor);
		j = LCD_PutStrig(i + 10, ScreenPos_Y, 1, OLD_Text_D);
		LCD_PutStrig(j + 4, ScreenPos_Y, 1, "C");

		memcpy(OLD_Text_D, &Sens_DataText[1][0], 3);
		*(OLD_Data + 1) = (*Data);
	}

	LCD_SetTextColor(ColorData);
	i = LCD_PutStrig(i + 10, ScreenPos_Y, 1, &Sens_DataText[1][0]);
	LCD_PutStrig(i + 4, ScreenPos_Y, 1, "C");
}


/*******************************************************************************
* Function Name  : Update_DHT_Sensor_OnScreen
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Update_HTSensor_OnScreenAllSensors(uint8_t Data, ErrorStatus Status)
{
	char Sens_DataText[3] = {'0', 0, 0};
	static char OLD_Text[3] = {0};
	ErrorStatus OLD_SensorStatus = ERROR;
	static uint8_t OLD_Data = 0;
	uint16_t ColorData = DarkGray;

	uint16_t ScreenPos_X = 430;
	uint16_t ScreenPos_Y = 85;

	if(Status != ERROR)
	{
		if(OLD_SensorStatus == ERROR) LCD_ClearArea(ScreenPos_X, ScreenPos_Y, ScreenPos_X + 30, ScreenPos_Y + 25, Global_BackColor);
		ColorData = 0x9ff3;

		Sens_DataText[0] = ((Data) / 10) + 48;
		Sens_DataText[1] = ((Data) % 10) + 48;
	}
	else
	{
		Sens_DataText[0] = '-';
		Sens_DataText[1] = '-';
	}

	LCD_SetFont(&sansation_22ptFontInfo);

	if(OLD_Data != Data)
	{
		LCD_SetTextColor(Global_BackColor);
		LCD_PutStrig(ScreenPos_X, ScreenPos_Y, 1, &OLD_Text[0]);

		memcpy(&OLD_Text[0], &Sens_DataText[0], 3);
		OLD_Data = Data;
	}

	LCD_SetTextColor(ColorData);
	LCD_PutStrig(ScreenPos_X, ScreenPos_Y, 1, Sens_DataText);

	OLD_SensorStatus = Status;
}





/*******************************************************************************
* Function Name  : UpdateSensorsOnScreen
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Update_InternalTempSensors_OnScreen(uint16_t SensorsData, uint8_t SensorNum, ErrorStatus SensorStatus)
{
	char Sens_DataText[3] = {'0', 0, 0};
	static char OLD_Text[2][3] = {{0}};
	static uint16_t OLD_Data[2];

	uint16_t ColorName = DarkGray;
	uint16_t ColorData = DarkGray;

	uint16_t ScreenPos_Y = 100 - (SensorNum * 25);
	uint16_t i, j;
	const uint16_t X0 = 420;

	if(SensorStatus != ERROR)
	{
		ColorName = 0x6a9c;
		if(SensorsData >= 70) ColorData = Red;
		else ColorData = Color_ChangeBrightness(GreenBlue, 4);
	}

	LCD_SetFont(&arialUnicodeMS_16ptFontInfo);
	LCD_SetTextColor(ColorName);

	if(SensorStatus != ERROR)
	{
		ConvertToString(SensorsData, &Sens_DataText[0], 2);
	}
	else
	{
		Sens_DataText[0] = '-';
		Sens_DataText[1] = '-';
	}


	if(OLD_Data[SensorNum] != SensorsData)
	{
		LCD_SetTextColor(Global_BackColor);
		j = LCD_PutStrig(X0, ScreenPos_Y, 1, &OLD_Text[SensorNum][0]);
		LCD_PutStrig(j + 4, ScreenPos_Y, 1, "C");

		memcpy(&OLD_Text[SensorNum][0], &Sens_DataText[0], 3);
		OLD_Data[SensorNum] = SensorsData;
	}

	LCD_SetTextColor(ColorData);
	i = LCD_PutStrig(X0, ScreenPos_Y, 1, Sens_DataText);
	LCD_PutStrig(i + 4, ScreenPos_Y, 1, "C");
}


/*******************************************************************************
* Function Name  : Update_DHT_Sensor_OnScreen
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void ConvertToString(uint16_t Num, char* Str, uint8_t NumSymbol)
{
	uint8_t A[5] = { 0 };
	int8_t i;

//	if(Num < 0) Num *= -1;

	while(Num >= 10000){ Num = Num - 10000; A[4]++; }
	while(Num >= 1000){ Num = Num - 1000; A[3]++; }
	while(Num >= 100){ Num = Num - 100;	A[2]++; }
	while(Num >= 10){ Num = Num - 10; A[1]++; }
	while(Num >= 1){ Num = Num - 1; A[0]++; }

	for(i = NumSymbol - 1; i >= 0; i--)
	{
		(*Str) = A[i] + 48;
		Str++;
	}
}


/*******************************************************************************
* Function Name  : Power_ON_OFF_ICO_Update
* Description    : обновление иконки выхода в правом верхнем углу
* Input          : None
* Return         : None
*******************************************************************************/
void Power_ON_OFF_ICO_Update(FunctionalState ICO_State, uint8_t OUT_Index)
{
	BITMAP_INFO_16 *ON_OFF_ICO = (void*)0;

	if(ICO_State == DISABLE)
	{
		LCD_ClearArea(440, 298, 461, 319, Global_BackColor);
		return;
	}

	if(OUT_SateRead(OUT_Index) == Bit_SET) ON_OFF_ICO = &PowerON_16_ICOInfo;
	else ON_OFF_ICO = &PowerOFF_16_ICOInfo;

	LCD_Draw_16_IMG(440, 299, ON_OFF_ICO, ON_OFF_ICO->RLE);
}


/*******************************************************************************
* Function Name  : Power_ON_OFF_Squares_Update
* Description    : обновление состояний выходов
* Input          : None
* Return         : None
*******************************************************************************/
void Power_ON_OFF_Squares_Update(void)
{
	uint8_t i = 0, j = 0;

	// проверки нужно ли обновлять состояние выходов
	for(i = 0; i < 20; i++)
	{
		if(Update_Out_Flag[i] == TRUE)
		{
			Update_Out_Flag[i] = FALSE;

			if((pMenu == &OutsMenu) && (UpdateOUT_Enable == TRUE))
			{
				Update_ONE_OutButtonState(i);
			}

			// проверяем обновление иконки состояния выхода если находимся в меню таймера/регулировки
			// если меню всех таймеров
			if(pMenu == &AllTimersMenu)
			{
				for(j = 0; j < 15; j++)
				{
					if(Timers_Struct[j].Out_Index == i)
					{
						// обновить
						Update_Timer_OUT(j);
						break;
					}
				}
			}
			// если меню таймера
			else if((pMenu == &TimerDailySetMenu) || (pMenu == &TimerNormalSetMenu))
			{
				if(i == Timer_Temp_Struct.Out_Index)
				{
					// обновить иконку
					Power_ON_OFF_ICO_Update(ENABLE,  i);
				}
			}
			// если меню всех регулировок
			else if(pMenu == &AllSensorsMenu)
			{
				for(j = 0; j < 9; j++)
				{
					if(Regulates_Struct[j].Out_Index == i)
					{
						// обновить
						Update_Sensor_OUT(j);
//						break;
					}
				}
			}
			// если меню регулировки
			else if(pMenu == &SensorMenu)
			{
				if(i == Regul_Temp_Struct.Out_Index)
				{
					// обновить иконку
					Power_ON_OFF_ICO_Update(ENABLE,  i);
				}
			}
		}
	}
}



/*******************************************************************************
* Function Name  : LCD_Configuration
* Description    : Configure the LCD Control pins and FSMC Parallel interface
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Verify_BL_AutoOFF(uint32_t CurrSeconds, FlagStatus Init)
{
	static uint32_t StartSeconds = 0;

	if((BL_AutoOFF_Status != TRUE) || (Init == SET))
	{
		StartSeconds = CurrSeconds;
		//LCD_Set_BackLightPWM(BL_Precent);
		return;
	}

	if((CurrSeconds - StartSeconds) >= BL_AutoOff_Seconds)
	{
		LCD_Set_BackLightPWM(5);
	}
	else LCD_Set_BackLightPWM(BL_Precent);
}



