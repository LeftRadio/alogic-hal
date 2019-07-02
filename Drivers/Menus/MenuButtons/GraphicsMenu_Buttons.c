/**
   ******************************************************************************
   * @file      GraphicsMenu_Buttons.c
   * @author	LeftRadio
   * @version 	V1.0.0
   * @date      14.04.2013 21:42:01
   * @brief
   ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern const BITMAP_INFO_16 Down_ICOInfo, Up_ICOInfo, Right_ICOInfo, Left_ICOInfo;

/* кнопка Graphics_Exit ----------------- "Выход" */
menuItem_INFO Graphics_Exit_Item = {
       15,                            // X0
       5,                             // Y0
       85,                            // X0
       65,                            // Y1
       (BITMAP_INFO_16*)&Exit_ICOInfo,                      // ICO
       RESET,                           // button preset
       0xffff,                        // button color
       1,                             // button color shift
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font
       "Выход",                      // text
       0,                            // text shift Y
       0xb6db,                        // text color
       SET,                         // presed status
};


/* кнопка Graphics_Mode_Item ----------------- */
menuItem_INFO Graphics_Mode_Item = {
	   85,                            // X0
       5,                             // Y0
       170,                            // X0
       65,                            // Y1
       (BITMAP_INFO_16*)&Graphics_Mode_ICOInfo,                      // ICO
       RESET,                           // button preset
       0xffff,                        // button color
       1,                             // button color shift
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font
       "Интервал",                      // text
       0,                            // text shift Y
       0xb6db,                        // text color
       SET,                         // presed status
};






/* кнопка Down ----------------- "" */
menuItem_INFO Graphics_Down_Item = {
       212,                            // X0
       23,                             // Y0 	120
       254,                            // X0
       65,                            // Y1
       (BITMAP_INFO_16*)&Down_ICOInfo,                      // ICO
       RESET,                           // button preset
      0x294a,                        // button color
       1,                             // button color shift
       (void*)0,                       // font
       "",                      // text
       10,                            // text shift Y
      0x0000,                        // text color
      SET,                         // presed status
};


/* кнопка Up ----------------- "" */
menuItem_INFO Graphics_Up_Item = {
       274,                            // X0
       23,                             // Y0
       316,                            // X0
       65,                            // Y1
       (BITMAP_INFO_16*)&Up_ICOInfo,                      // ICO
       RESET,                           // button preset
      0x294a,                        // button color
       1,                             // button color shift
       (void*)0,                      // font
       "",                      // text
       10,                            // text shift Y
      0x0000,                        // text color
      SET,                         // presed status
};



/* кнопка Left ----------------- "" */
menuItem_INFO Graphics_Left_Item = {
	   357,                            // X0
	   23,                             // Y0
       399,                            // X0
       65,                            // Y1
       (BITMAP_INFO_16*)&Left_ICOInfo,                      // ICO
       RESET,                           // button preset
      0x294a,                        // button color
       1,                             // button color shift
       (void*)0,                      // font
       "",                      // text
       10,                            // text shift Y
      0x0000,                        // text color
      SET,                         // presed status
};



/* кнопка Right ----------------- "" */
menuItem_INFO Graphics_Right_Item = {
	   419,                            // X0
	   23,                             // Y0
       461,                            // X0
       65,                            // Y1
       (BITMAP_INFO_16*)&Right_ICOInfo,                      // ICO
       RESET,                           // button preset
      0x294a,                        // button color
       1,                             // button color shift
       (void*)0,                       // font
       "",                      // text
       10,                            // text shift Y
      0x0000,                        // text color
      SET,                         // presed status
};


/* кнопка Graphics_Reset ----------------- "" */
menuItem_INFO Graphics_Reset_Item = {
	   407,                            // X0
	   298,                             // Y0
       460,                            // X0
       316,                            // Y1
       (void*)0,                      // ICO
       SET,                           // button preset
       Red,                        // button color
       1,                             // button color shift
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font
       "Сброс",                      // text
       0,                            // text shift Y
       White,                        // text color
       SET,                         // presed status
};
