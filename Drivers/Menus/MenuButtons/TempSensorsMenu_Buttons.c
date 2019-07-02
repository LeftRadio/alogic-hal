/**
   ******************************************************************************
   * @file      TempSensorsMenu_Buttons.c 
   * @author		LeftRadio
   * @version 	V1.0.0
   * @date      02.05.2013 21:33:54
   * @brief
   ******************************************************************************
**/ 

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h" 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* кнопка SensorExit ----------------- "Выход" */
menuItem_INFO SensorExit_Item = {
       10,                            // X0
       5,                             // Y0 
       80,                            // X0
       60,                            // Y1 
       (BITMAP_INFO_16*)&Exit_ICOInfo,                      // ICO
       RESET,                           // button preset 
      0xf800,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font 
       "Выход",                      // text
       0,                            // text shift Y 
      0xb6db,                        // text color 
      SET,                         // presed status
};

/* кнопка SensorSave ----------------- "Сохранить" */
menuItem_INFO SensorSave_Item = {
       78,                            // X0
       5,                             // Y0 
       138,                            // X0
       60,                            // Y1 
       (BITMAP_INFO_16*)&Save_ICOInfo,                      // ICO
       RESET,                           // button preset 
      0xf800,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font 
       "Сохранить",                      // text
       0,                            // text shift Y 
      0xb6db,                        // text color 
      SET,                         // presed status
};


/* кнопка SensRegul_ON_OFF ----------------- "Включить" */
menuItem_INFO SensRegul_ON_OFF_Item = {
	   165,                            // X0
       5,                             // Y0 
       235,                            // X0
       60,                            // Y1 
       (BITMAP_INFO_16*)&SensRegul_ON_ICOInfo,                      // ICO
       RESET,                           // button preset 
      0xf800,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font 
       "Включить",                      // text
       0,                            // text shift Y 
      0xb6db,                        // text color 
       SET,                         // presed status
};

 
/* кнопка SensorReset ----------------- "Сбросить" */
menuItem_INFO SensorRegulType_Item = {
	   275,                            // X0
       5,                             // Y0 
       345,                            // X0
       60,                            // Y1 
       (BITMAP_INFO_16*)&Regul_Mode_ICOInfo,              // ICO
       RESET,                           // button preset 
       0xf800,                        // button color
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font 
       "Режим",                      // text
       0,                            // text shift Y 
       0xb6db,                        // text color
       SET,                         // presed status
};

 
/* кнопка InvertOUT ----------------- "Инвертировать" */
menuItem_INFO InvertOUT_Item = {
       350,                            // X0
       5,                             // Y0 
       470,                            // X0
       60,                            // Y1 
       (BITMAP_INFO_16*)&InvertOUT_ICOInfo,                      // ICO
       RESET,                           // button preset 
      0xf800,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font 
       "Инвертировать",                      // text
       0,                            // text shift Y 
      0xb6db,                        // text color 
      SET,                         // presed status
};

 


/* кнопка SensorThresold ----------------- "00" */
menuItem_INFO SensorIndex_Item = {
       275,                            // X0
       230,                             // Y0
       375,                            // X0
       260,                            // Y1
       (void*)0,                      // ICO
       RESET,                           // button preset
      0xf800,                        // button color
       1,                             // button color shift
       (FONT_INFO*)&sansation_22ptFontInfo,                      // font
       "25",                      // text
       0,                            // text shift Y
       DarkOrange3,                        // text color
      SET,                         // presed status
};



/* кнопка SensorThresold ----------------- "00" */ 
menuItem_INFO SensorThresold_Item = { 
       275,                            // X0
       190,                             // Y0
       325,                            // X0
       230,                            // Y1
       (void*)0,                      // ICO 
       RESET,                           // button preset 
      0xf800,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&sansation_22ptFontInfo,                      // font
       "25",                      // text
       0,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

/* кнопка SensorThresold ----------------- "00" */
menuItem_INFO SensorThresold2_Item = {
       275,                            // X0
       150,                             // Y0
       325,                            // X0
       190,                            // Y1
       (void*)0,                      // ICO
       RESET,                           // button preset
      0xf800,                        // button color
       1,                             // button color shift
       (FONT_INFO*)&sansation_22ptFontInfo,                      // font
       "25",                      // text
       0,                            // text shift Y
      0xffff,                        // text color
      SET,                         // presed status
};
 
/* кнопка SensorHysteresis ----------------- "00" */ 
menuItem_INFO SensorHysteresis_Item = { 
       275,                            // X0
       110,                             // Y0
       325,                            // X0
       150,                            // Y1
       (void*)0,                      // ICO 
       RESET,                           // button preset 
      0xf800,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&sansation_22ptFontInfo,                      // font
       "00",                      // text 
       0,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};


/* кнопка SensorHysteresis ----------------- "00" */
menuItem_INFO SensorPWM_Item = {
       275,                            // X0
       70,                             // Y0
       325,                            // X0
       110,                            // Y1
       (void*)0,                      // ICO
       RESET,                           // button preset
      0xf800,                        // button color
       1,                             // button color shift
       (FONT_INFO*)&sansation_22ptFontInfo,                      // font
       "100",                      // text
       0,                            // text shift Y
      0xffff,                        // text color
      SET,                         // presed status
};

