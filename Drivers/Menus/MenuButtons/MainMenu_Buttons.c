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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LowerBtn		5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* кнопка ----------------------------*/
menuItem_INFO Main_Timers = {

		11,
		LowerBtn,
		85,
		65,
		(BITMAP_INFO_16*)&Clock_ICOInfo,
		RESET,
		Black,
		1,
		(FONT_INFO*)&sansation_10ptFontInfo,
		"Таймеры",
		0,
		ButtonTextColor,
		SET
};

/* кнопка ----------------------------*/
menuItem_INFO Main_Outs = {

		96,
		LowerBtn,
		173,
		65,
		(BITMAP_INFO_16*)&Outs_ICOInfo,
		RESET,
		Black,
		1,
		(FONT_INFO*)&sansation_10ptFontInfo,
		"Выходы",
		0,
		ButtonTextColor,
		SET
};

/* кнопка ----------------------------*/
menuItem_INFO Main_Regulate = {

		174,
		LowerBtn,
		253,
		65,
		(BITMAP_INFO_16*)&Regulate_ICOInfo,
		RESET,
		Black,
		1,
		(FONT_INFO*)&sansation_10ptFontInfo,
		"Регулировки",
		0,
		ButtonTextColor,
		SET
};


/* кнопка ----------------------------*/
menuItem_INFO Main_Graphics = {

		295,
		LowerBtn,
		376,
		65,
		(BITMAP_INFO_16*)&Graphics_ICOInfo,
		RESET,
		Black,
		1,
		(FONT_INFO*)&sansation_10ptFontInfo,
		"Графики",
		0,
		ButtonTextColor,
		SET
};


/* кнопка ----------------------------*/
menuItem_INFO Main_Settings = {

		380,
		LowerBtn,
		470,
		65,
		(BITMAP_INFO_16*)&Settings_InfoICO,
		RESET,
		Black,
		1,
		(FONT_INFO*)&sansation_10ptFontInfo,
		"Настройки",
		0,
		ButtonTextColor,
		SET
};
