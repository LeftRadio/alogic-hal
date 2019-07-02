/**
   ******************************************************************************
   * @file      OutsMenu.c 
   * @author		LeftRadio
   * @version 	V1.0.0
   * @date      14.04.2013 20:27:59
   * @brief
   ******************************************************************************
**/ 

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h" 
#include "Outputs.h"
#include "Timers.h"
#include "Regulates.h"
#include "Pparam.h"
#include <string.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OUT_ON_Color 		Green1
#define OUT_OFF_Color		DarkRed2


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void OutsMenu_Draw(void);
void OutsMenu_ReturnEvent(void);

void OutPush_ItemEvent(void);

static void OUT_To_Timer_ItemEvent(void);
static void SelectTimerItemEvent(void);
void SelectRegulateItemEvent(void);
static void Set_SelectTimerButtonsStatus(FlagStatus NewStatus);

void OUT_To_Regulate_ItemEvent(void);
void OUT_Switch_ON_ItemEvent(void);
void OUT_Switch_OFF_ItemEvent(void);

void OUTS_ResetAll_ItemEvent(void);
void OUTS_Exit_ItemEvent(void);
void OUTS_Save_ItemEvent(void);
void OUTS_Enable_Disable_ItemEvent(void);

static void Update_OutsButtonsState(void);
static void Draw_SelectOUT(uint8_t OUTIndx, uint16_t Color);



/* Private variables ---------------------------------------------------------*/

extern menuItem_INFO OUT_1_Item, OUT_2_Item, OUT_3_Item, OUT_4_Item, OUT_5_Item;
extern menuItem_INFO OUT_6_Item, OUT_7_Item, OUT_8_Item, OUT_9_Item, OUT_10_Item;
extern menuItem_INFO OUT_11_Item, OUT_12_Item, OUT_13_Item, OUT_14_Item, OUT_15_Item;
extern menuItem_INFO OUT_16_Item, OUT_17_Item, OUT_18_Item, OUT_19_Item, OUT_20_Item;
extern menuItem_INFO OUT_To_Timer_Item, OUT_To_Regulate_Item, OUT_Switch_ON_Item, OUT_Switch_OFF_Item, OUTS_ResetAll_Item;
extern menuItem_INFO OUTS_Exit_Item, OUTS_Save_Item, OUTS_Enable_Disable_Item;

extern menuItem_INFO Timer_Button_1, Timer_Button_2, Timer_Button_3, Timer_Button_4, Timer_Button_5;
extern menuItem_INFO Timer_Button_6, Timer_Button_7, Timer_Button_8, Timer_Button_9, Timer_Button_10;
extern menuItem_INFO Timer_Button_11, Timer_Button_12, Timer_Button_13, Timer_Button_14, Timer_Button_15;

extern menuItem_INFO TempSensor1_Item, TempSensor2_Item, TempSensor3_Item, TempSensor4_Item, TempSensor5_Item;
extern menuItem_INFO TempSensor6_Item, TempSensor7_Item, TempSensor8_Item, HumiditySensor_Item;

Menu_Struct_TypeDef OutsMenu = {
		51,
		{ // кнопки меню
				&OUT_1_Item, &OUT_2_Item, &OUT_3_Item, &OUT_4_Item, &OUT_5_Item,
				&OUT_6_Item, &OUT_7_Item, &OUT_8_Item, &OUT_9_Item, &OUT_10_Item,
				&OUT_11_Item, &OUT_12_Item, &OUT_13_Item, &OUT_14_Item, &OUT_15_Item,
				&OUT_16_Item, &OUT_17_Item, &OUT_18_Item, &OUT_19_Item, &OUT_20_Item,
				&OUT_To_Timer_Item, &OUT_To_Regulate_Item, &OUT_Switch_ON_Item, &OUT_Switch_OFF_Item, &OUTS_ResetAll_Item,
				&OUTS_Exit_Item, &OUTS_Save_Item,

				&Timer_Button_1, &Timer_Button_2, &Timer_Button_3, &Timer_Button_4, &Timer_Button_5,
				&Timer_Button_6, &Timer_Button_7, &Timer_Button_8, &Timer_Button_9, &Timer_Button_10,
				&Timer_Button_11, &Timer_Button_12, &Timer_Button_13, &Timer_Button_14, &Timer_Button_15,

				&TempSensor1_Item, &TempSensor2_Item, &TempSensor3_Item, &TempSensor4_Item, &TempSensor5_Item,
				&TempSensor6_Item, &TempSensor7_Item, &TempSensor8_Item, &HumiditySensor_Item,
		},
		{ // функции кнопок меню
				&OutPush_ItemEvent, &OutPush_ItemEvent, &OutPush_ItemEvent, &OutPush_ItemEvent, &OutPush_ItemEvent,
				&OutPush_ItemEvent, &OutPush_ItemEvent, &OutPush_ItemEvent, &OutPush_ItemEvent, &OutPush_ItemEvent,
				&OutPush_ItemEvent, &OutPush_ItemEvent, &OutPush_ItemEvent, &OutPush_ItemEvent, &OutPush_ItemEvent,
				&OutPush_ItemEvent, &OutPush_ItemEvent, &OutPush_ItemEvent, &OutPush_ItemEvent, &OutPush_ItemEvent,
				&OUT_To_Timer_ItemEvent, &OUT_To_Regulate_ItemEvent, &OUT_Switch_ON_ItemEvent, &OUT_Switch_OFF_ItemEvent, &OUTS_ResetAll_ItemEvent,
				&OUTS_Exit_ItemEvent, &OUTS_Save_ItemEvent,

				&SelectTimerItemEvent, &SelectTimerItemEvent, &SelectTimerItemEvent, &SelectTimerItemEvent, &SelectTimerItemEvent,
				&SelectTimerItemEvent, &SelectTimerItemEvent, &SelectTimerItemEvent, &SelectTimerItemEvent, &SelectTimerItemEvent,
				&SelectTimerItemEvent, &SelectTimerItemEvent, &SelectTimerItemEvent, &SelectTimerItemEvent, &SelectTimerItemEvent,

				&SelectRegulateItemEvent, &SelectRegulateItemEvent, &SelectRegulateItemEvent, &SelectRegulateItemEvent, &SelectRegulateItemEvent,
				&SelectRegulateItemEvent, &SelectRegulateItemEvent, &SelectRegulateItemEvent, &SelectRegulateItemEvent,
		},
		TRUE,
		&OutsMenu_Draw,
		0,
		0,
}; 

volatile uint8_t OLD_Active_OUT = 0;
volatile uint8_t Active_OUT = 0;
volatile Bool UpdateOUT_Enable = TRUE;

const Timer_Regul_Num Reguls[9] = {
		Regul1,	Regul2,	Regul3,	Regul4,
		Regul5,	Regul6,	Regul7,	Regul8,	Regul9
};

const Timer_Regul_Num TimersNums[15] = {
		Timer1, Timer2, Timer3, Timer4, Timer5,
		Timer6, Timer7, Timer8, Timer9, Timer10,
		Timer11, Timer12, Timer13, Timer14, Timer15,
};

/* Functions ----------------------------------------------------------------*/
 
/*******************************************************************************
 * Function Name  : OutsMenu_Draw
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void OutsMenu_Draw(void)
{ 
	// рисуем линии
	LCD_SetGraphicsColor(0x4410);
	LCD_DrawLine(0, 65, 479, 65);
	LCD_DrawLine(344, 75, 344, 285);

	// отрисовываем вверхние линии и надписи
	LCD_SetFont(&microsoftSansSerif_10ptFontInfo);
	LCD_SetTextColor(Orange1);
	LCD_PutStrig(357, 277, 1, "Меню Выхода");
	LCD_DrawLine(351, 277, 473, 277);

	LCD_PutStrig(377, 159, 1, "Вручную");
	LCD_DrawLine(351, 159, 473, 159);

	// отключаем кнопки выбора таймеров, так как они ненужны
	Set_SelectTimerButtonsStatus(RESET);

	LCD_SetBackColor(LightBlack3);		// цвет тени у кнопок

	// рисуем все кнопки меню
	Draw_All_Items_Menu(20, OutsMenu.MaxItems - 15);

	// сбос номера активного выхода в ноль на первый выход
	Active_OUT = 0;
	Draw_SelectOUT(Active_OUT, White);

	// обновление состояния выходов в меню
	Update_OutsButtonsState();
}
 
 
/*******************************************************************************
 * Function Name  : Update_ButtonsState
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void Update_OutsButtonsState(void)
{
	uint8_t i;

	for(i = 0; i < 20; i++) Update_ONE_OutButtonState(i);
}


/*******************************************************************************
 * Function Name  : Update_OutsButtonsState
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Update_ONE_OutButtonState(uint8_t Indx)
{
	uint16_t Color = OUT_OFF_Color;

	// читаем состояние выхода
	if(OUT_SateRead(Indx) == Bit_SET) Color = OUT_ON_Color;

	// устанавливаем цвет кнопки и обновляем ее
	OutsMenu.Items[Indx]->ButtonColor = Color;
	Draw_Item_Menu(OutsMenu.Items[Indx]);

	// обновляем функциональный текст кнопки, если выход никуда не назначен то текста нет
	LCD_SetTextColor(LightGray4);
	LCD_SetFont(&microsoftSansSerif_10ptFontInfo);
	LCD_PutStrig(OutsMenu.Items[Indx]->X0 + 4, OutsMenu.Items[Indx]->Y0 + (OutsMenu.Items[Indx]->Text_Y_Coordinate - 15), 1, &StructOuts[Indx].FunctionText[0]);
}


/*******************************************************************************
 * Function Name  : Update_ButtonsState
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void Draw_SelectOUT(uint8_t OUTIndx, uint16_t Color)
{
	LCD_SetGraphicsColor(Color);
	LCD_DrawRect(OutsMenu.Items[OUTIndx]->X0 - 2, OutsMenu.Items[OUTIndx]->Y0 - 2, OutsMenu.Items[OUTIndx]->X1 + 1, OutsMenu.Items[OUTIndx]->Y1 + 1);
}

 
/*******************************************************************************
 * Function Name  : Set_SelectTimerButtonsStatus
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void Set_SelectTimerButtonsStatus(FlagStatus NewStatus)
{
	uint8_t i;
	FlagStatus tStatus = (NewStatus == RESET)? SET : RESET;

	for(i = 0; i < 27; i++) OutsMenu.Items[i]->Status = tStatus;
	for(i = 27; i < 42; i++)
	{
		if(Timers_Struct[i - 27].State == ENABLE) OutsMenu.Items[i]->TextColor = LighGreen;
		else OutsMenu.Items[i]->TextColor = White;

		OutsMenu.Items[i]->Status = NewStatus;
	}
	for(i = 42; i < 51; i++) OutsMenu.Items[i]->Status = tStatus;
}


/*******************************************************************************
 * Function Name  : Set_SelectRegulateButtonsStatus
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void Set_SelectRegulateButtonsStatus(FlagStatus NewStatus)
{
	uint8_t i;
	FlagStatus tStatus = (NewStatus == RESET)? SET : RESET;

	for(i = 0; i < 42; i++) OutsMenu.Items[i]->Status = tStatus;
	for(i = 42; i < 51; i++) OutsMenu.Items[i]->Status = NewStatus;
}





// --- События кнопок -----------------------------------------------------------------------------------------//

/*******************************************************************************
 * Function Name  : OutPush_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void OutPush_ItemEvent(void)
{ 
	Draw_SelectOUT(Active_OUT, Global_BackColor);
	Active_OUT = TouchObject;
	Draw_SelectOUT(Active_OUT, White);
} 
 
 
/*******************************************************************************
 * Function Name  : OUT_To_Timer_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void OUT_To_Timer_ItemEvent(void)
{
	UpdateOUT_Enable = FALSE;

	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);
	LCD_ClearArea(0, 0, 479, 65, Global_BackColor);
	Set_SelectTimerButtonsStatus(SET);
	Draw_All_Items_Menu(0, OutsMenu.MaxItems);
} 
 

/*******************************************************************************
 * Function Name  : OUT_To_Timer_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SelectTimerItemEvent(void)
{
	uint8_t Timer_indx = TouchObject - 27;
	uint8_t i;

	OUT_OFF(Active_OUT);

	DeAssigned_OUT_FromTimer(Active_OUT);
	DeAssigned_OUT_FromRegulate(Active_OUT);
	Timers_Struct[Timer_indx].Out_Index = Active_OUT;

	// назначаем текст кнопке и конвертируем номер таймера в текст и назначаем выходу новый номер таймера
	memcpy(&StructOuts[Active_OUT].FunctionText[0], "тайм. ", 6);
	ConvertToString((Timer_indx + 1), &StructOuts[Active_OUT].FunctionText[5], 2);
	StructOuts[Active_OUT].Num = TimersNums[Timer_indx];

	for(i = 0; i < 20; i++)
	{
		if(Active_OUT != i)
		{
			//if(strcmp(&StructOuts[Active_OUT].FunctionText[0], &StructOuts[i].FunctionText[0]) == 0)
			if(StructOuts[Active_OUT].Num == StructOuts[i].Num)
			{
				OUT_OFF(i);
				memset(&StructOuts[i].FunctionText[0], 0, 10);
				StructOuts[i].Num = NotAssigned;
			}
		}
	}

	Set_SelectTimerButtonsStatus(RESET);

	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);

	Draw_All_Items_Menu(20, OutsMenu.MaxItems - 15);
	Update_OutsButtonsState();
	Draw_SelectOUT(Active_OUT, White);

	// отрисовываем вверхние линии и надписи
	LCD_SetFont(&microsoftSansSerif_10ptFontInfo);
	LCD_SetTextColor(Orange1);
	LCD_SetGraphicsColor(GreenLine);
	LCD_PutStrig(357, 277, 1, "Меню Выхода");
	LCD_DrawLine(351, 277, 473, 277);

	LCD_PutStrig(377, 159, 1, "Вручную");
	LCD_DrawLine(351, 159, 473, 159);

	UpdateOUT_Enable = TRUE;
}



/*******************************************************************************
 * Function Name  : OUT_To_Regulate_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void OUT_To_Regulate_ItemEvent(void)
{
	UpdateOUT_Enable = FALSE;

	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);
	LCD_ClearArea(0, 0, 479, 65, Global_BackColor);
	Set_SelectRegulateButtonsStatus(SET);
	LCD_ButtonShiftColorSelect(3);
	Draw_All_Items_Menu(0, OutsMenu.MaxItems);
	LCD_ButtonShiftColorSelect(2);
} 
 

/*******************************************************************************
 * Function Name  : OUT_To_Timer_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SelectRegulateItemEvent(void)
{
	uint8_t Regul_indx = TouchObject - 42;
	uint8_t i, k;
	char NewText[6] = { "регул." };

	OUT_OFF(Active_OUT);

	DeAssigned_OUT_FromTimer(Active_OUT);
	DeAssigned_OUT_FromRegulate(Active_OUT);
	Regulates_Struct[Regul_indx].Out_Index = Active_OUT;

	for(k = 0; k < 6; k++)
	{
		StructOuts[Active_OUT].FunctionText[k] = NewText[k];
	}

	// конвертируем номер регулировки в текст и назначаем выходу новый номер регулировки
	ConvertToString((Regul_indx + 1), &StructOuts[Active_OUT].FunctionText[6], 2);
	StructOuts[Active_OUT].Num = Reguls[Regul_indx];

	// ищем не была ли эта регулировка уже назначена а другой выход
	for(i = 0; i < 20; i++)
	{
		if(Active_OUT != i)
		{
			if(StructOuts[Active_OUT].Num == StructOuts[i].Num)
			{
				// нашли совпадение, выключаем выход и очищаем текст на выходе
				OUT_OFF(i);
				memset(StructOuts[i].FunctionText, 0, 10);
				StructOuts[i].Num = NotAssigned;
			}
		}
	}

	/* Возвращамся к выходам и перерисовываем меню */
	Set_SelectRegulateButtonsStatus(RESET);
	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);
	Draw_All_Items_Menu(20, OutsMenu.MaxItems - 24);
	Update_OutsButtonsState();
	Draw_SelectOUT(Active_OUT, White);

	// отрисовываем вверхние линии и надписи
	LCD_SetFont(&microsoftSansSerif_10ptFontInfo);
	LCD_SetTextColor(Orange1);
	LCD_SetGraphicsColor(GreenLine);
	LCD_PutStrig(357, 277, 1, "Меню Выхода");
	LCD_DrawLine(351, 277, 473, 277);

	LCD_PutStrig(377, 159, 1, "Вручную");
	LCD_DrawLine(351, 159, 473, 159);

	UpdateOUT_Enable = TRUE;
}


/*******************************************************************************
 * Function Name  : OUT_Switch_ON_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void OUT_Switch_ON_ItemEvent(void)
{ 
	DeAssigned_OUT_FromTimer(Active_OUT);			// ищем не назначен ли данный выход на таймер
	DeAssigned_OUT_FromRegulate(Active_OUT);
	OUT_ON(Active_OUT);
	Update_ONE_OutButtonState(Active_OUT);
} 
 
 
/*******************************************************************************
 * Function Name  : OUT_Switch_OFF_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void OUT_Switch_OFF_ItemEvent(void)
{
	DeAssigned_OUT_FromTimer(Active_OUT);		// ищем не назначен ли данный выход на таймер
	DeAssigned_OUT_FromRegulate(Active_OUT);
	OUT_OFF(Active_OUT);						// выключаем выход
	Update_ONE_OutButtonState(Active_OUT);
} 
 

/*******************************************************************************
 * Function Name  : DeAssigned_OUT_FromTimer
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
uint8_t DeAssigned_OUT_FromTimer(uint8_t OUT_Indx)
{
	uint8_t i;
	FlagStatus Find = RESET;

	// ищем не назначен ли данный выход на таймер
	for(i = 0; i < 15; i++)
	{
		if(Timers_Struct[i].Out_Index == OUT_Indx)
		{
			Timers_Struct[i].Out_Index = Timer_OUT_NotAssigned;
			Find = SET;
			break;
		}
	}
	memset(StructOuts[OUT_Indx].FunctionText, 0, 10);

	if(Find != SET) return 255;
	else return i;
}


/*******************************************************************************
 * Function Name  : Find_OUT_FromTimer
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
uint8_t Find_OUT_OnTimer(uint8_t OUT_Indx)
{
	uint8_t i;
	FlagStatus Find = RESET;

	// ищем не назначен ли данный выход на таймер
	for(i = 0; i < 15; i++)
	{
		if(Timers_Struct[i].Out_Index == OUT_Indx)
		{
			Find = SET;
			break;
		}
	}

	if(Find != SET) return 255;
	else return i;
}


/*******************************************************************************
 * Function Name  : DeAssigned_OUT_FromTimer
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
uint8_t DeAssigned_OUT_FromRegulate(uint8_t OUT_Indx)
{
	uint8_t i;
	FlagStatus Find = RESET;

	// ищем не назначен ли данный выход на
	for(i = 0; i < 9; i++)
	{
		if(Regulates_Struct[i].Out_Index == OUT_Indx)
		{
			Regulates_Struct[i].Out_Index = Regulates_OUT_NotAssigned;
			Find = SET;
			break;
		}
	}
	memset(StructOuts[OUT_Indx].FunctionText, 0, 10);

	if(Find != SET) return 255;
	else return i;
}


/*******************************************************************************
 * Function Name  : Find_OUT_OnRegulate
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
uint8_t Find_OUT_OnRegulate(uint8_t OUT_Indx)
{
	uint8_t i;
	FlagStatus Find = RESET;

	// ищем не назначен ли данный выход на
	for(i = 0; i < 9; i++)
	{
		if(Regulates_Struct[i].Out_Index == OUT_Indx)
		{
			Find = SET;
			break;
		}
	}

	if(Find != SET) return 255;
	else return i;
}



/* Нижнее меню --------------------------------------------------------------------------------------------------------- */

/*******************************************************************************
 * Function Name  : ResetAll_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void OUTS_ResetAll_ItemEvent(void)
{ 
	static uint8_t i;

	for(i = 0; i < 20; i++)
	{
		DeAssigned_OUT_FromTimer(i);	// ищем не назначен ли данный выход на таймер
		DeAssigned_OUT_FromRegulate(i);
		OUT_OFF(i);						// выключаем выход
	}

	Update_OutsButtonsState();
} 
 
 
/*******************************************************************************
 * Function Name  : Exit_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void OUTS_Exit_ItemEvent(void)
{ 
	Down_Menu_Reset();

	SetActiveMenu(&MainMenu);
	LCD_ClearArea(0, 5, 479, 294, Global_BackColor);
	LCD_ClearArea(410, 298, 431, 315, Global_BackColor);
	Draw_ActiveMenu();
} 
 
 
/*******************************************************************************
 * Function Name  : Save_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void OUTS_Save_ItemEvent(void)
{ 
	SaveOutputsSettings();
} 
 
 
/*******************************************************************************
 * Function Name  : OUTS_Enable_Disable_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void OUTS_Enable_Disable_ItemEvent(void)
{ 
     // 
} 
 
 

