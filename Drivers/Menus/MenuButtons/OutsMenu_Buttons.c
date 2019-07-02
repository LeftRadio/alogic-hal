/**
   ******************************************************************************
   * @file      OutsMenu_Buttons.c 
   * @author		LeftRadio
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

 
/* кнопка OUT_1 ----------------- "Выход 1" */ 
menuItem_INFO OUT_1_Item = { 
       6,                            // X0 
       248,                             // Y0 
       86,                            // X0 
       288,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 1",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_2 ----------------- "Выход 2" */ 
menuItem_INFO OUT_2_Item = { 
       6,                            // X0 
       204,                             // Y0 
       86,                            // X0 
       244,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 2",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_3 ----------------- "Выход 3" */ 
menuItem_INFO OUT_3_Item = { 
       6,                            // X0 
       160,                             // Y0 
       86,                            // X0 
       200,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 3",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_4 ----------------- "Выход 4" */ 
menuItem_INFO OUT_4_Item = { 
       6,                            // X0 
       116,                             // Y0 
       86,                            // X0 
       156,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 4",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_5 ----------------- "Выход 5" */ 
menuItem_INFO OUT_5_Item = { 
       6,                            // X0 
       72,                             // Y0 
       86,                            // X0 
       112,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 5",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_6 ----------------- "Выход 6" */ 
menuItem_INFO OUT_6_Item = { 
       89,                            // X0 
       248,                             // Y0 
       169,                            // X0 
       288,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 6",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_7 ----------------- "Выход 7" */ 
menuItem_INFO OUT_7_Item = { 
       89,                            // X0 
       204,                             // Y0 
       169,                            // X0 
       244,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 7",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_8 ----------------- "Выход 8" */ 
menuItem_INFO OUT_8_Item = { 
       89,                            // X0 
       160,                             // Y0 
       169,                            // X0 
       200,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 8",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_9 ----------------- "Выход 9" */ 
menuItem_INFO OUT_9_Item = { 
       89,                            // X0 
       116,                             // Y0 
       169,                            // X0 
       156,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 9",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_10 ----------------- "Выход 10" */ 
menuItem_INFO OUT_10_Item = { 
       89,                            // X0 
       72,                             // Y0 
       169,                            // X0 
       112,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 10",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_11 ----------------- "Выход 11" */ 
menuItem_INFO OUT_11_Item = { 
       172,                            // X0 
       248,                             // Y0 
       252,                            // X0 
       288,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 11",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_12 ----------------- "Выход 12" */ 
menuItem_INFO OUT_12_Item = { 
       172,                            // X0 
       204,                             // Y0 
       252,                            // X0 
       244,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 12",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_13 ----------------- "Выход 13" */ 
menuItem_INFO OUT_13_Item = { 
       172,                            // X0 
       160,                             // Y0 
       252,                            // X0 
       200,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 13",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_14 ----------------- "Выход 14" */ 
menuItem_INFO OUT_14_Item = { 
       172,                            // X0 
       116,                             // Y0 
       252,                            // X0 
       156,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 14",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_15 ----------------- "Выход 15" */ 
menuItem_INFO OUT_15_Item = { 
       172,                            // X0 
       72,                             // Y0 
       252,                            // X0 
       112,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 15",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_16 ----------------- "Выход 16" */ 
menuItem_INFO OUT_16_Item = { 
       255,                            // X0 
       248,                             // Y0 
       335,                            // X0 
       288,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 16",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_17 ----------------- "Выход 17" */ 
menuItem_INFO OUT_17_Item = { 
       255,                            // X0 
       204,                             // Y0 
       335,                            // X0 
       244,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 17",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_18 ----------------- "Выход 18" */ 
menuItem_INFO OUT_18_Item = { 
       255,                            // X0 
       160,                             // Y0 
       335,                            // X0 
       200,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 18",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_19 ----------------- "Выход 19" */ 
menuItem_INFO OUT_19_Item = { 
       255,                            // X0 
       116,                             // Y0 
       335,                            // X0 
       156,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 19",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_20 ----------------- "Выход 20" */ 
menuItem_INFO OUT_20_Item = { 
       255,                            // X0 
       72,                             // Y0 
       335,                            // X0 
       112,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выход 20",                      // text 
       19,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_To_Timer ----------------- "Таймер" */ 
menuItem_INFO OUT_To_Timer_Item = { 
       352,                            // X0 
       233,                             // Y0 
       472,                            // X0 
       273,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x10a4,                        // button color
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Таймер",                      // text 
       11,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_To_Regulate ----------------- "Регулировки" */ 
menuItem_INFO OUT_To_Regulate_Item = { 
       352,                            // X0 
       189,                             // Y0 
       472,                            // X0 
       229,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x10a4,                        // button color
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Регулировки",                      // text 
       11,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_Switch_ON ----------------- "Включить" */ 
menuItem_INFO OUT_Switch_ON_Item = { 
       352,                            // X0 
       115,                             // Y0 
       472,                            // X0 
       155,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x180,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Включить",                      // text 
       11,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUT_Switch_OFF ----------------- "Выключить" */ 
menuItem_INFO OUT_Switch_OFF_Item = { 
       352,                            // X0 
       71,                             // Y0 
       472,                            // X0 
       111,                            // Y1 
       (void*)0,                      // ICO 
       SET,                           // button preset 
       DarkRed,                        // button color
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_12ptFontInfo,                      // font 
       "Выключить",                      // text 
       11,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status 
};

 
/* кнопка OUTS_ResetAll ----------------- "Сброс" */ 
menuItem_INFO OUTS_ResetAll_Item = { 
       405,                            // X0
       5,                             // Y0 
       475,                            // X0
       65,                            // Y1 
       (BITMAP_INFO_16*)&ResetAll_ICOInfo,                      // ICO
       RESET,                           // button preset 
       0xffff,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font 
       "Сброс",                      // text 
       0,                            // text shift Y 
       0xb6db,                        // text color 
       SET,                         // presed status
};

 
/* кнопка OUTS_Exit ----------------- "Выход" */ 
menuItem_INFO OUTS_Exit_Item = { 
		15,                            // X0
		5,                             // Y0
		85,                            // X0
		65,
		// Y1
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

 
/* кнопка OUTS_Save ----------------- "Сохранить" */ 
menuItem_INFO OUTS_Save_Item = { 
		100,                            // X0
		5,                             // Y0
		170,                            // X0
		65,                           // Y1
       (BITMAP_INFO_16*)&Save_ICOInfo,                      // ICO
       RESET,                           // button preset 
       0xffff,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font 
       "Сохранить",                      // text 
       0,                            // text shift Y 
       0xb6db,                        // text color 
       SET,                         // presed status
};

 
/* кнопка OUTS_Enable_Disable ----------------- "Разрешить все выходы" */ 
menuItem_INFO OUTS_Enable_Disable_Item = { 
       280,                            // X0 
       5,                             // Y0 
       450,                            // X0 
       65,                            // Y1 
       (BITMAP_INFO_16*)&OUTS_Enable_Disable_ICOInfo,                      // ICO 
       RESET,                           // button preset 
       0xffff,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font 
       "Разрешить все выходы",                      // text 
       0,                            // text shift Y 
       0xb6db,                        // text color 
       RESET,                         // presed status 
};


