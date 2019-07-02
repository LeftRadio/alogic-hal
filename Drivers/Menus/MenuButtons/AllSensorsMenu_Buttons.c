/**
   ******************************************************************************
   * @file      AllSensorsMenu_Buttons.c 
   * @author		LeftRadio
   * @version 	V1.0.0
   * @date      02.05.2013 16:03:09
   * @brief
   ******************************************************************************
**/ 

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h" 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/



 
/* кнопка TempSensor1 ----------------- "TEMP 1" */ 
menuItem_INFO TempSensor1_Item = { 
       10,                            // X0 
       220,                             // Y0 
       100,                            // X0
       285,                            // Y1
       (void*)0,                      // ICO 
       SET,                           // button preset 
       LightBlack2,                        // button color
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
       "TEMP 1",                      // text 
       40,                            // text shift Y
       0xffff,                        // text color
       SET,                         // presed status
};

 
/* кнопка TempSensor2 ----------------- "TEMP 2" */ 
menuItem_INFO TempSensor2_Item = { 
       10,                            // X0 
       145,                             // Y0 
       100,                            // X0
       210,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       LightBlack2,                        // button color
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
       "TEMP 2",                      // text 
       40,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TempSensor3 ----------------- "TEMP 3" */ 
menuItem_INFO TempSensor3_Item = { 
       10,                            // X0 
       70,                             // Y0 
       100,                            // X0
       135,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       LightBlack2,                        // button color
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
       "TEMP 3",                      // text 
       40,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TempSensor4 ----------------- "TEMP 4" */ 
menuItem_INFO TempSensor4_Item = { 
       170,                            // X0 
       220,                             // Y0 
       260,                            // X0
       285,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       LightBlack2,                        // button color
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
       "TEMP 4",                      // text 
       40,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TempSensor5 ----------------- "TEMP 5" */ 
menuItem_INFO TempSensor5_Item = { 
       170,                            // X0 
       145,                             // Y0 
       260,                            // X0
       210,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       LightBlack2,                        // button color
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
       "TEMP 5",                      // text 
       40,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TempSensor6 ----------------- "TEMP 6" */ 
menuItem_INFO TempSensor6_Item = { 
       170,                            // X0 
       70,                             // Y0 
       260,                            // X0
       135,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       LightBlack2,                        // button color
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
       "TEMP 6",                      // text 
       40,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TempSensor7 ----------------- "TEMP 7" */ 
menuItem_INFO TempSensor7_Item = { 
       330,                            // X0 
       220,                             // Y0 
       420,                            // X0
       285,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       LightBlack2,                        // button color
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
       "TEMP 7",                      // text 
       40,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TempSensor8 ----------------- "TEMP 8" */ 
menuItem_INFO TempSensor8_Item = { 
       330,                            // X0 
       145,                             // Y0 
       420,                            // X0
       210,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       LightBlack2,                        // button color
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
       "TEMP 8",                      // text 
       40,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка HumiditySensor ----------------- "RH" */ 
menuItem_INFO HumiditySensor_Item = { 
       330,                            // X0 
       70,                             // Y0 
       420,                            // X0
       135,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       LightBlack2,                        // button color
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
       "RH",                      // text 
       40,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};


