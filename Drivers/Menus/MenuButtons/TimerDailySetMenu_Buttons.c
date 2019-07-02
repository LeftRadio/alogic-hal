/**
   ******************************************************************************
   * @file      TimerDailySetMenu_Buttons.c 
   * @author		LeftRadio
   * @version 	V1.0.0
   * @date      15.04.2013 23:32:16
   * @brief
   ******************************************************************************
**/ 

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h" 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

 extern const BITMAP_INFO_16 OK_ICOInfo, Down_ICOInfo, Up_ICOInfo, Right_ICOInfo, Left_ICOInfo;


 
/* кнопка TimerDaily_ON_OFF_0 ----------------- "OFF" */ 
menuItem_INFO TimerDaily_ON_OFF_0_Item = { 
       1,                            // X0 
       67,                             // Y0 
       45,                            // X0 
       107,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
      0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font 
       "OFF",                      // text 
       6,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TimerDaily_ON_OFF_1 ----------------- "OFF" */ 
menuItem_INFO TimerDaily_ON_OFF_1_Item = { 
       1,                            // X0 
       111,                             // Y0 
       45,                            // X0 
       151,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
      0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font 
       "OFF",                      // text 
       6,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TimerDaily_ON_OFF_2 ----------------- "OFF" */ 
menuItem_INFO TimerDaily_ON_OFF_2_Item = { 
       1,                            // X0 
       155,                             // Y0 
       45,                            // X0 
       195,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
      0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font 
       "OFF",                      // text 
       6,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TimerDaily_ON_OFF_3 ----------------- "OFF" */ 
menuItem_INFO TimerDaily_ON_OFF_3_Item = { 
       1,                            // X0 
       199,                             // Y0 
       45,                            // X0 
       239,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
      0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font 
       "OFF",                      // text 
       6,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TimerDaily_ON_OFF_4 ----------------- "OFF" */ 
menuItem_INFO TimerDaily_ON_OFF_4_Item = { 
       1,                            // X0 
       243,                             // Y0 
       45,                            // X0 
       283,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
      0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font 
       "OFF",                      // text 
       6,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TimerDailyTimeON_4 ----------------- "00:00" */ 
menuItem_INFO TimerDailyTimeON_4_Item = { 
       77,                            // X0 
       243,                             // Y0 
       147,                            // X0 
       283,                            // Y1 
       (void*)0,                      // ICO 
       RESET,                           // button preset 
      0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&sansation_22ptFontInfo,                      // font
       "00:00",                      // text 
       3,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TimerDailyTimeON_3 ----------------- "00:00" */ 
menuItem_INFO TimerDailyTimeON_3_Item = { 
       77,                            // X0 
       199,                             // Y0 
       147,                            // X0 
       239,                            // Y1 
       (void*)0,                      // ICO 
       RESET,                           // button preset 
      0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&sansation_22ptFontInfo,                      // font
       "00:00",                      // text 
       3,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TimerDailyTimeON_2 ----------------- "00:00" */ 
menuItem_INFO TimerDailyTimeON_2_Item = { 
       77,                            // X0 
       155,                             // Y0 
       147,                            // X0 
       195,                            // Y1 
       (void*)0,                      // ICO 
       RESET,                           // button preset 
      0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&sansation_22ptFontInfo,                      // font
       "00:00",                      // text 
       3,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TimerDailyTimeON_1 ----------------- "00:00" */ 
menuItem_INFO TimerDailyTimeON_1_Item = { 
       77,                            // X0 
       111,                             // Y0 
       147,                            // X0 
       151,                            // Y1 
       (void*)0,                      // ICO 
       RESET,                           // button preset 
      0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&sansation_22ptFontInfo,                      // font
       "00:00",                      // text 
       3,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TimerDailyTimeON_0 ----------------- "00:00" */ 
menuItem_INFO TimerDailyTimeON_0_Item = { 
       77,                            // X0 
       67,                             // Y0 
       147,                            // X0 
       107,                            // Y1 
       (void*)0,                      // ICO 
       RESET,                           // button preset 
      0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&sansation_22ptFontInfo,                      // font
       "00:00",                      // text 
       3,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};

 
/* кнопка TimerDailyTimeWork ----------------- "00:00:00" */ 
menuItem_INFO TimerDailyTimeWork_Item = { 
       223,                            // X0 
       199,                             // Y0 
       348,                            // X1
       239,                            // Y1 
       (void*)0,                      // ICO 
       RESET,                           // button preset 
      0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&sansation_22ptFontInfo,                      // font
       "00:00:00",                      // text 
       3,                            // text shift Y
      0xffff,                        // text color 
      SET,                         // presed status
};


///* кнопка TimerDailyDayMon ----------------- "Пн" */
//menuItem_INFO TimerDailyDayMon_Item = {
//       175,                            // X0
//       111,                             // Y0
//       225,                            // X0
//       151,                            // Y1
//       (void*)0,                      // ICO
//       SET,                           // button preset
//      0x294a,                        // button color
//       1,                             // button color shift
//       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
//       "Пн",                      // text
//       10,                            // text shift Y
//      0xffff,                        // text color
//      SET,                         // presed status
//};
//
//
///* кнопка TimerDailyDayTue ----------------- "Вт" */
//menuItem_INFO TimerDailyDayTue_Item = {
//       232,                            // X0
//       111,                             // Y0
//       282,                            // X0
//       151,                            // Y1
//       (void*)0,                      // ICO
//       SET,                           // button preset
//      0x294a,                        // button color
//       1,                             // button color shift
//       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
//       "Вт",                      // text
//       10,                            // text shift Y
//      0xffff,                        // text color
//      SET,                         // presed status
//};
//
//
///* кнопка TimerDailyDayWed ----------------- "Ср" */
//menuItem_INFO TimerDailyDayWed_Item = {
//       289,                            // X0
//       111,                             // Y0
//       339,                            // X0
//       151,                            // Y1
//       (void*)0,                      // ICO
//       SET,                           // button preset
//      0x294a,                        // button color
//       1,                             // button color shift
//       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
//       "Ср",                      // text
//       10,                            // text shift Y
//      0xffff,                        // text color
//      SET,                         // presed status
//};
//
//
///* кнопка TimerDailyDayThru ----------------- "Чт" */
//menuItem_INFO TimerDailyDayThru_Item = {
//       346,                            // X0
//       111,                             // Y0
//       396,                            // X0
//       151,                            // Y1
//       (void*)0,                      // ICO
//       SET,                           // button preset
//      0x294a,                        // button color
//       1,                             // button color shift
//       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
//       "Чт",                      // text
//       10,                            // text shift Y
//      0xffff,                        // text color
//      SET,                         // presed status
//};
//
//
///* кнопка TimerDailyDayFri ----------------- "Пт" */
//menuItem_INFO TimerDailyDayFri_Item = {
//       175,                            // X0
//       67,                             // Y0
//       225,                            // X0
//       107,                            // Y1
//       (void*)0,                      // ICO
//       SET,                           // button preset
//      0x294a,                        // button color
//       1,                             // button color shift
//       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
//       "Пт",                      // text
//       10,                            // text shift Y
//      0xffff,                        // text color
//      SET,                         // presed status
//};
//
//
///* кнопка TimerDailyDaySat ----------------- "Сб" */
//menuItem_INFO TimerDailyDaySat_Item = {
//       232,                            // X0
//       67,                             // Y0
//       282,                            // X0
//       107,                            // Y1
//       (void*)0,                      // ICO
//       SET,                           // button preset
//      0x294a,                        // button color
//       1,                             // button color shift
//       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
//       "Сб",                      // text
//       10,                            // text shift Y
//      0xffff,                        // text color
//      SET,                         // presed status
//};
//
//
///* кнопка TimerDailyDaySan ----------------- "Вс" */
//menuItem_INFO TimerDailyDaySan_Item = {
//       289,                            // X0
//       67,                             // Y0
//       339,                            // X0
//       107,                            // Y1
//       (void*)0,                      // ICO
//       SET,                           // button preset
//      0x294a,                        // button color
//       1,                             // button color shift
//       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
//       "Вс",                      // text
//       10,                            // text shift Y
//      0xffff,                        // text color
//      SET,                         // presed status
//};
//
//
///* кнопка TimerDailyDaysAll ----------------- "Все" */
//menuItem_INFO TimerDailyDaysAll_Item = {
//       346,                            // X0
//       67,                             // Y0
//       396,                            // X0
//       107,                            // Y1
//       (void*)0,                      // ICO
//       SET,                           // button preset
//      0x294a,                        // button color
//       1,                             // button color shift
//       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font
//       "Все",                      // text
//       10,                            // text shift Y
//      0xffff,                        // text color
//      SET,                         // presed status
//};

 
/* кнопка OK ----------------- "" */ 
menuItem_INFO OK_Item = { 
       430,                            // X0 	425
       75,                             // Y0  75
       472,                            // X0 	457
       117,                            // Y1  107
       (BITMAP_INFO_16*)&OK_ICOInfo,                      // ICO 
       RESET,                           // button preset 
      0x294a,                        // button color 
       1,                             // button color shift 
       (void*)0,                      // font
       "",                      // text 
       10,                            // text shift Y 
      0x0000,                        // text color 
       SET,                         // presed status
};

 
/* кнопка Down ----------------- "" */ 
menuItem_INFO Down_Item = { 
       430,                            // X0
       120,                             // Y0 	120
       472,                            // X0
       162,                            // Y1
       (BITMAP_INFO_16*)&Down_ICOInfo,                      // ICO 
       RESET,                           // button preset 
      0x294a,                        // button color 
       1,                             // button color shift 
       (void*)0,                      // font
       "",                      // text 
       10,                            // text shift Y 
      0x0000,                        // text color 
      SET,                         // presed status
};

 
/* кнопка Up ----------------- "" */ 
menuItem_INFO Up_Item = { 
       430,                            // X0
       165,                             // Y0 
       472,                            // X0
       207,                            // Y1
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

 
/* кнопка Right ----------------- "" */ 
menuItem_INFO Right_Item = { 
       430,                            // X0
       210,                             // Y0 
       472,                            // X0
       252,                            // Y1
       (BITMAP_INFO_16*)&Right_ICOInfo,                      // ICO 
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
menuItem_INFO Left_Item = { 
       430,                            // X0
       254,                             // Y0 
       472,                            // X0
       296,                            // Y1
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


