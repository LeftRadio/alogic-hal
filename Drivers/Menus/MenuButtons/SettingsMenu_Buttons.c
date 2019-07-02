/**
   ******************************************************************************
   * @file      SettingsMenu_Buttons.c 
   * @author		LeftRadio
   * @version 	V1.0.0
   * @date      15.04.2013 1:37:05
   * @brief
   ******************************************************************************
**/ 

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h" 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/



 
/* ������ Settings_ActiveSetDateTime ----------------- "���� � �����" */ 
menuItem_INFO Settings_ActiveSetDateTime_Item = { 
       1,                            // X0
       250,                             // Y0
       185,                            // X0
       290,                            // Y1
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x923,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font 
       "���� � �����",                      // text 
       11,                            // text shift Y 
       0xffff,                        // text color 
       SET,                         // presed status
};

 
/* ������ Settings_ActiveSetLCD ----------------- "��������� �������" */ 
menuItem_INFO Settings_ActiveSetLCD_Item = { 
       1,                            // X0
       205,                             // Y0
       185,                            // X0
       245,                            // Y1
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x923,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font 
       "��������� �������",                      // text
       11,                            // text shift Y 
       0xffff,                        // text color 
       SET,                         // presed status
};

 
/* ������ Settings_ActiveUpdateFirmware ----------------- "���������� ��������" */ 
menuItem_INFO Settings_ActiveFan_Item = { 
       1,                            // X0
       160,                             // Y0
       185,                            // X0
       200,                            // Y1
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x923,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font 
       "�����. ����������",                      // text
       11,                            // text shift Y 
       0xffff,                        // text color 
       SET,                         // presed status
};

 
/* ������ Settings_ActiveSetEraseEEPROM ----------------- "������� ������ EEPROM" */ 
menuItem_INFO Settings_ActiveSetEraseEEPROM_Item = { 
       1,                            // X0
       115,                             // Y0
       185,                            // X0
       155,                            // Y1
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x923,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font 
       "�������� ���������",                      // text
       11,                            // text shift Y 
       0xffff,                        // text color 
       SET,                         // presed status
};


/* ������ Settings_TouchCalibrate */
menuItem_INFO Settings_TouchCalibrate_Item = {
       1,                            // X0
       70,                             // Y0
       185,                            // X0
       110,                            // Y1
       (void*)0,                      // ICO
       SET,                           // button preset
       0x923,                        // button color
       1,                             // button color shift
       (FONT_INFO*)&microsoftSansSerif_10ptFontInfo,                      // font
       "���������� Touch",                      // text
       11,                            // text shift Y
       0xffff,                        // text color
       SET,                         // presed status
};

/* ������ Settings_SetDate ----------------- "21/12/2012" */ 
menuItem_INFO Settings_SetDate_Item = { 
       195,                            // X0
       250,                             // Y0
       310,                            // X0
       290,                            // Y1
       (void*)0,                      // ICO 
       RESET,                           // button preset
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS22ptFontInfo,                      // font
       "21/12/12",                      // text
       4,                            // text shift Y
       0xffff,                        // text color 
       RESET,                         // presed status
};

 
/* ������ Settings_SetTime ----------------- "00:00" */ 
menuItem_INFO Settings_SetTime_Item = { 
       330,                            // X0
       250,                             // Y0
       410,                            // X0
       290,                            // Y1
       (void*)0,                      // ICO 
       RESET,                           // button preset
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS22ptFontInfo,                      // font
       "00:00",                      // text 
       4,                            // text shift Y
       0xffff,                        // text color 
       RESET,                         // presed status
};

 
/* ������ Settings_SetBL ----------------- "100 %" */ 
menuItem_INFO Settings_SetBL_Item = { 
       212,                            // X0
       203,                             // Y0
       297,                            // X0
       243,                            // Y1
       (void*)0,                      // ICO 
       RESET,                           // button preset
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS22ptFontInfo,                      // font
       "100 %",                      // text 
       0,                            // text shift Y
       0xffff,                        // text color 
       RESET,                         // presed status
};

 
/* ������ Settings_SetBL_OFFTime ----------------- "00:00" */ 
menuItem_INFO Settings_SetBL_OFFTime_Item = { 
       313,                            // X0
       203,                             // Y0
       410,                            // X0
       243,                            // Y1
       (void*)0,                      // ICO 
       RESET,                           // button preset
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS22ptFontInfo,                      // font
       "00:00",                      // text 
       0,                            // text shift Y
       0xffff,                        // text color 
       RESET,                         // presed status
};

 
/* ������ Settings_GoToUpdateFirmware ----------------- "������ � ����������" */ 
menuItem_INFO Settings_SetFanPWM_Item = { 
       275,                            // X0
       160,                             // Y0
       400,                            // X0
       200,                            // Y1
       (void*)0,                      // ICO 
       RESET,                           // button preset
       0x3186,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS22ptFontInfo,                      // font
       "100 %",                      // text
       0,                            // text shift Y
       0xffff,                        // text color 
       RESET,                         // presed status
};

 
/* ������ Settings_CancelEraseEEPROM ----------------- "���" */ 
menuItem_INFO Settings_CancelEraseEEPROM_Item = { 
       202,                            // X0
       115,                             // Y0
       300,                            // X0
       155,                            // Y1
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x10a6,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font 
       "���",                      // text 
       8,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status
};

 
/* ������ Settings_YesEraseEEPROM ----------------- "��" */ 
menuItem_INFO Settings_YesEraseEEPROM_Item = { 
       304,                            // X0
       115,                             // Y0
       402,                            // X0
       155,                            // Y1
       (void*)0,                      // ICO 
       SET,                           // button preset 
       0x5924,                        // button color 
       1,                             // button color shift 
       (FONT_INFO*)&arialUnicodeMS_16ptFontInfo,                      // font 
       "��",                      // text 
       8,                            // text shift Y 
       0xffff,                        // text color 
       RESET,                         // presed status
};


