/**
  ******************************************************************************
  * @file    init.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief   Main Init Peripherals header file.
  ******************************************************************************
**/

#ifndef __LCD___H
#define __LCD___H

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"
#include "colors 5-6-5.h"

/* Exported typedef ----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern volatile uint32_t BL_AutoOff_Seconds;
extern uint8_t BL_Precent;
extern Bool BL_AutoOFF_Status;

extern uint8_t FAN_Precent;

/* Установленные картинки/иконки ---------------------------------------------*/
// иконки нижнего меню
extern const BITMAP_INFO_16 ResetAll_ICOInfo, Exit_ICOInfo, Save_ICOInfo, DailyTimer_ICOInfo, NormalTimer_ICOInfo;
extern const BITMAP_INFO_16 Home_ICOInfo;
extern const BITMAP_INFO_16 Outs_ICOInfo;
extern const BITMAP_INFO_16 Clock_ICOInfo;
extern const BITMAP_INFO_16 Graphics_ICOInfo;
extern const BITMAP_INFO_16 Graphics_Mode_ICOInfo;
extern const BITMAP_INFO_16 Regulate_ICOInfo;
extern const BITMAP_INFO_16 Regul_Mode_ICOInfo;
extern const BITMAP_INFO_16 Settings_InfoICO;
extern const BITMAP_INFO_16 OUTS_Enable_Disable_ICOInfo;
extern BITMAP_INFO_16 SensRegul_ON_ICOInfo;
extern BITMAP_INFO_16 SensRegul_OFF_ICOInfo;
extern BITMAP_INFO_16 InvertOUT_ICOInfo;

extern BITMAP_INFO_16 PowerON_16_ICOInfo;
extern BITMAP_INFO_16 PowerOFF_16_ICOInfo;

// иконки стрелок
extern const BITMAP_INFO_16 Up_ICOInfo, Down_ICOInfo, OK_ICOInfo, Left_ICOInfo, Right_ICOInfo;

// разные
extern const BITMAP_INFO_16 BT_ICOInfo;


/* Установленные шрифты ------------------------------------------------------*/
extern const FONT_INFO arialUnicodeMS_16ptFontInfo;
extern const FONT_INFO microsoftSansSerif_10ptFontInfo;
extern const FONT_INFO microsoftSansSerif_12ptFontInfo;
extern const FONT_INFO DigitSansSerif11ptFontInfo;
extern const FONT_INFO arial_16Digit_FontInfo;
extern const FONT_INFO arialUnicodeMS22ptFontInfo;
extern const FONT_INFO sansation_10ptFontInfo;
extern const FONT_INFO sansation_22ptFontInfo;
extern const FONT_INFO microsoftSansSerif_8ptFontInfo;

/* Exported function ---------------------------------------------------------*/
void LCD_Configuration(void);
void LCD_Set_BackLightPWM(uint8_t BL_Precent);
uint8_t LCD_Get_BackLightPWM(void);

#endif /* __LCD___H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

