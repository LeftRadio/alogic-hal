/**
   ******************************************************************************
   * @file      AllSensorsMenu.c 
   * @author		LeftRadio
   * @version 	V1.0.0
   * @date      02.05.2013 16:03:09
   * @brief
   ******************************************************************************
**/ 

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h" 
#include "Regulates.h"
#include "Outputs.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern menuItem_INFO Main_Settings, Main_Timers, Main_Outs, Main_Regulate, Main_Graphics;

/* Private function prototypes -----------------------------------------------*/
void AllSensorsMenu_Draw(void);
void AllSensorsMenu_ReturnEvent(void);

extern void Main_Timers_Event(void);
extern void Main_Outs_Event(void);
extern void Main_Graphics_Event(void);
extern void Main_Settings_Event(void);


void SetTempSensor_ItemEvent(void);
void SetHumiditySensor_ItemEvent(void);

/* Private variables ---------------------------------------------------------*/

 extern menuItem_INFO TempSensor1_Item, TempSensor2_Item, TempSensor3_Item, TempSensor4_Item, TempSensor5_Item;
 extern menuItem_INFO TempSensor6_Item, TempSensor7_Item, TempSensor8_Item, HumiditySensor_Item;

 Menu_Struct_TypeDef AllSensorsMenu = { 
       14,
       { // кнопки меню
            &TempSensor1_Item, &TempSensor2_Item, &TempSensor3_Item, &TempSensor4_Item, &TempSensor5_Item,
            &TempSensor6_Item, &TempSensor7_Item, &TempSensor8_Item, &HumiditySensor_Item,
            &Main_Timers, &Main_Outs, &Main_Regulate, &Main_Graphics, &Main_Settings
       },
       { // функции кнопок меню
            &SetTempSensor_ItemEvent, &SetTempSensor_ItemEvent, &SetTempSensor_ItemEvent, &SetTempSensor_ItemEvent, &SetTempSensor_ItemEvent,
            &SetTempSensor_ItemEvent, &SetTempSensor_ItemEvent, &SetTempSensor_ItemEvent, &SetHumiditySensor_ItemEvent,
            &Main_Timers_Event, &Main_Outs_Event, &AllSensorsMenu_ReturnEvent, &Main_Graphics_Event, &Main_Settings_Event,
       },
       TRUE,
       &AllSensorsMenu_Draw,
       0,
       0,
}; 
 

volatile uint8_t SelectedRegulate = 255;
extern volatile Regulates_INFO Regul_Temp_Struct;

 /* Functions ----------------------------------------------------------------*/

 /*******************************************************************************
  * Function Name  : AllSensorsMenu_Draw
  * Description    :
  * Input          : None
  * Return         : None
  *******************************************************************************/
 void AllSensorsMenu_Draw(void)
 {
	 uint8_t i;
	 char str[10] = {0};
	 uint16_t X = 0;
	 uint16_t OutTextColor = Color_ChangeBrightness(LightSky, 8);
	 uint16_t LineColor = Color_ChangeBrightness(LightBlack2, 8);

	 LCD_ButtonShiftColorSelect(3);		// 3 линии на цветовой переход
	 LCD_SetBackColor(LightBlack3);		// цвет тени у кнопок

	 for(i = 0; i < 9; i++) AllSensorsMenu.Items[i]->Status = SET;
	 Down_Menu_Reset();

	 /* заменяем иконку у кнопки таймера, теперь кнопка выхода */
	 Main_Regulate.ICO = (BITMAP_INFO_16*)&Home_ICOInfo;
	 memset(Main_Regulate.Text, 0, 12);
	 memcpy(Main_Regulate.Text, "Главная", 7);

	 Main_Regulate.X0 = 191;
	 Main_Regulate.X1 = 270;

	 //
	 Init_Regulates_Buttons();

	 // рисуем все кнопки меню
	 Draw_All_Items_Menu(0, AllSensorsMenu.MaxItems);

	 LCD_SetGraphicsColor(LineColor);

	 for(i = 0; i < 9; i++)
	 {
		 LCD_DrawLine(AllSensorsMenu.Items[i]->X1, AllSensorsMenu.Items[i]->Y0 - 1, AllSensorsMenu.Items[i]->X1 + 49, AllSensorsMenu.Items[i]->Y0 - 1);
		 LCD_DrawLine(AllSensorsMenu.Items[i]->X1, AllSensorsMenu.Items[i]->Y1 - 1, AllSensorsMenu.Items[i]->X1 + 49, AllSensorsMenu.Items[i]->Y1 - 1);
		 LCD_DrawLine(AllSensorsMenu.Items[i]->X1 + 49, AllSensorsMenu.Items[i]->Y0 - 1, AllSensorsMenu.Items[i]->X1 + 49, AllSensorsMenu.Items[i]->Y1 - 1);

		 /* текст номера датчика */
		 ConvertToString(Regulates_Struct[i].SensorIndex + 1, str, 2);
		 X = LCD_PutColorStrig(AllSensorsMenu.Items[i]->X0 + 10, AllSensorsMenu.Items[i]->Y0 + 20, 1, "Дат-", OutTextColor);
		 LCD_PutColorStrig(X + 5, AllSensorsMenu.Items[i]->Y0 + 20, 1, str, OutTextColor);

		 /* текст номера выхода если назначен */
		 if(Regulates_Struct[i].Out_Index != 255)
		 {
			 ConvertToString(Regulates_Struct[i].Out_Index + 1, str, 2);
			 X = LCD_PutColorStrig(AllSensorsMenu.Items[i]->X0 + 10, AllSensorsMenu.Items[i]->Y0, 1, "Вых-", OutTextColor);
			 LCD_PutColorStrig(X + 5, AllSensorsMenu.Items[i]->Y0, 1, str, OutTextColor);
		 }
	 }



//	 LCD_SetFont(&sansation_22ptFontInfo);
//	 LCD_PutColorStrig(AllSensorsMenu.Items[0]->X1 + 3, AllSensorsMenu.Items[0]->Y0 + 25, 1, "25", LighGreen);
//	 LCD_DrawFillRect(AllSensorsMenu.Items[Indx]->X1 + 51, AllSensorsMenu.Items[Indx]->Y0 - 1, AllSensorsMenu.Items[Indx]->X1 + 56, AllSensorsMenu.Items[Indx]->Y1 - 1, 0, 0);

	 // рисуем линии
	 LCD_SetGraphicsColor(0x4410);
	 LCD_DrawLine(0, 65, 479, 65);
 }




 /*******************************************************************************
  * Function Name  : AllSensorsMenu_ReturnEvent
  * Description    :
  * Input          : None
  * Return         : None
  *******************************************************************************/
 void AllSensorsMenu_ReturnEvent(void)
 {
	 LCD_ButtonShiftColorSelect(2);		// 3 линии на цветовой переход

	 LCD_ClearArea(Main_Regulate.X0, Main_Regulate.Y0 - 4, Main_Regulate.X1 + 5, Main_Regulate.Y1 + 1, Global_BackColor);
	 LCD_ClearArea(0, 66, 479, 294, Global_BackColor);

	 Down_Menu_Reset();
	 SetActiveMenu(&MainMenu);
	 Draw_ActiveMenu();
 }
 
 



/*******************************************************************************
 * Function Name  : SetTempSensor_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SetTempSensor_ItemEvent(void)
{
	SelectedRegulate = TouchObject;

	/* Копируем  во времменый  */
	Regulate_CopyStruct((Regulates_INFO*)&Regul_Temp_Struct, (Regulates_INFO*)&Regulates_Struct[SelectedRegulate]);

	/* очистка старого и отрисовка нового активного меню */
	LCD_ClearArea(0, 5, 479, 294, Global_BackColor);
	SetActiveMenu(&SensorMenu);
	Draw_ActiveMenu();
}


/*******************************************************************************
 * Function Name  : SetHumiditySensor_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SetHumiditySensor_ItemEvent(void)
{
	SetTempSensor_ItemEvent();
}
 
 

/*******************************************************************************
* Function Name  : Update_Timer_OUT
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Update_Sensor_OUT(uint8_t Indx)
{
	volatile BitAction BIT_State = Bit_RESET;

	if(Regulates_Struct[Indx].Out_Index > 19) return;
	else if(Regulates_Struct[Indx].Out_Index != Regulates_OUT_NotAssigned)
	{
		BIT_State = OUT_SateRead(Regulates_Struct[Indx].Out_Index);

		if(BIT_State == Bit_SET) LCD_SetGraphicsColor(LighGreen);
		else if(BIT_State == Bit_RESET)	LCD_SetGraphicsColor(Red);

		LCD_DrawFillRect(AllSensorsMenu.Items[Indx]->X1 + 51, AllSensorsMenu.Items[Indx]->Y0 - 1, AllSensorsMenu.Items[Indx]->X1 + 56, AllSensorsMenu.Items[Indx]->Y1 - 1, 0, 0);
	}
}


