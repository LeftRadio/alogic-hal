/**
  ******************************************************************************
  * @file    UserInterface.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief   header
  ******************************************************************************
**/

#ifndef __USER_INTERFACE_H
#define __USER_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "LCD.h"
#include <stdio.h>
#include <string.h>

/* Exported typedef ----------------------------------------------------------*/
//typedef enum { DrawEvent, ClearEvent, ClickEvent } Event;

//typedef struct
//{
//	uint16_t X;					// координаты
//	uint16_t Y;
//} Point;

/** Структурный тип данных параметров кннопок меню **/
typedef struct
{
	uint16_t X0;					// координаты
	uint16_t Y0;
	uint16_t X1;
	uint16_t Y1;
	BITMAP_INFO_16 *ICO;					// иконка
	FlagStatus ButtonPreset;				// кнопка
	uint16_t ButtonColor;					// цвет кнопки
	uint8_t ButtonColorShift;				// цвет кнопки с переливом или без
	FONT_INFO *Font;						// шрифт кнопок
	char Text[30];								// текст
	uint16_t Text_Y_Coordinate;
	uint16_t TextColor;						// цвет текста
	FlagStatus Status;						// нажата или нет
} menuItem_INFO;


typedef struct
{
	uint8_t MaxItems;
	menuItem_INFO *Items[55];
	void (*MenuItemEvent[55])(void);
	Bool ExitAllowed;
	void (*MenuDrawFunc)(void);
	void (*MenuCallBack)(void);
	void (*MenuSpecialFunc)(void);
} Menu_Struct_TypeDef;



/** Структурный тип данных параметров кннопок меню **/
typedef struct
{
	int32_t Param;
	int32_t MAX;					//
	int32_t MIN;					//
	uint8_t SymNum;					// количество символов, 100 это 3
} ParamItem_INFO;


typedef struct
{
	uint8_t MaxParametrs;
	ParamItem_INFO Item[5];
	uint8_t Position;
} ParamStruct_TypeDef;


/* Exported define -----------------------------------------------------------*/
#define Menu_NoObject		255

#define SlashLineInit		2
#define SlashLineDraw		1
#define SlashLineClear		0

/* Exported variables --------------------------------------------------------*/
extern uint16_t Global_BackColor;

/** указатель, тип данных menuItem_INFO **/
extern menuItem_INFO *menuItem;
extern Menu_Struct_TypeDef *pMenu;
extern volatile uint8_t TouchObject;

extern Menu_Struct_TypeDef MainMenu, AllTimersMenu, OutsMenu, AllSensorsMenu, SettingsMenu;
extern Menu_Struct_TypeDef TimerNormalSetMenu, TimerDailySetMenu;
extern Menu_Struct_TypeDef SensorMenu;
extern Menu_Struct_TypeDef GraphicsMenu;
extern Menu_Struct_TypeDef HostMenu;

extern volatile menuItem_INFO FlashingButton;
extern Bool FlashingButtonUpdate;
extern Bool Update_Out_Flag[20];

/* Exported function ---------------------------------------------------------*/
void ConvertToString(uint16_t Num, char* Str, uint8_t NumSymbol);

void Power_ON_OFF_ICO_Update(FunctionalState ICO_State, uint8_t OUT_Index);
void Power_ON_OFF_Squares_Update(void);

void Interface_Init(void);
void Display_Date_and_Time(uint8_t Reset);
void Update_TempSensors_OnScreen(uint16_t SensorsData, uint8_t SensorNum, ErrorStatus OLD_SensorStatus, ErrorStatus SensorStatus);
void Update_TempSensors_OnScreen_AllSensos(uint16_t SensorsData, uint8_t SensorNum, ErrorStatus OLD_SensorStatus, ErrorStatus SensorStatus);
void Update_HTSensor_OnScreen(uint8_t* Data, uint8_t DHT_SHT);
void Update_HTSensor_OnScreenAllSensors(uint8_t Data, ErrorStatus Status);

void Update_InternalTempSensors_OnScreen(uint16_t SensorsData, uint8_t SensorNum, ErrorStatus SensorStatus);


/* Меню */
void SetActiveMenu(Menu_Struct_TypeDef *NewActiveMenu);
void Draw_ActiveMenu(void);
void Draw_All_Items_Menu(uint8_t StartItem, uint8_t EndItem);
void Draw_Item_Menu(menuItem_INFO *Menu_Item);
void Return_ActiveMenu(void);

/* Мигание */
void FlashingText_Enable(int16_t X_swPosX, menuItem_INFO *NewItem);
void FlashingText_Command(FunctionalState NewState);
FunctionalState FlashingText_GetState(void);
void StopFlashing_and_Update(void);

/* подчеркивание */
void SlashLine(uint8_t Init, uint8_t ButtonSlashIndx, uint8_t Position);

/* сброс всех иконок нижнего меню "по умолчанию" */
extern void Down_Menu_Reset(void);
extern void ButtonsDownMenuInit(void);

void LCD_Verify_BL_AutoOFF(uint32_t CurrSeconds, FlagStatus Init);

/* Поиск объекта по нажатию тачскрина */
void Menu_Func(void);


#endif /* __USER_INTERFACE_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
