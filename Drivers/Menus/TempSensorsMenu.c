/**
   ******************************************************************************
   * @file      TempSensorsMenu.c 
   * @author		LeftRadio
   * @version 	V1.0.0
   * @date      02.05.2013 17:07:53
   * @brief
   ******************************************************************************
**/ 

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h" 
#include "Regulates.h"
#include "Outputs.h"
#include "STLM75.h"
#include "Pparam.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LeftValX		330

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SensorMenu_Draw(void);

static void SensorMenu_ReturnEvent(void);
static void SensorMenu_SaveEvent(void);
static void SensRegul_ON_OFF_ItemEvent(void);
static void SensorRegulType_ItemEvent(void);
static void InvertOUT_ItemEvent(void);

static void SensorIndex_ItemEvent(void);
static void SensorThresold_ItemEvent(void);
static void SensorThresold2_ItemEvent(void);
static void SensorHysteresis_ItemEvent(void);
static void SensorPWM_ItemEvent(void);


static void SensorUpdateSetParametr(uint32_t NewData);
static void SensorMenu_InitValues(void);

static void OK_ItemEvent(void);
static void Down_ItemEvent(void);
static void Up_ItemEvent(void);
static void Right_ItemEvent(void);
static void Left_ItemEvent(void);

/* Private variables ---------------------------------------------------------*/
extern menuItem_INFO SensorExit_Item, SensorSave_Item, SensRegul_ON_OFF_Item, SensorRegulType_Item, InvertOUT_Item;
extern menuItem_INFO SensorIndex_Item, SensorThresold_Item, SensorThresold2_Item, SensorHysteresis_Item, SensorPWM_Item;
extern menuItem_INFO OK_Item, Down_Item, Up_Item, Right_Item, Left_Item;

 Menu_Struct_TypeDef SensorMenu = { 
       15,
       { // ������ ����
    		   &SensorIndex_Item, &SensorThresold_Item, &SensorThresold2_Item, &SensorHysteresis_Item, &SensorPWM_Item,
    		   &SensorExit_Item, &SensorSave_Item, &SensRegul_ON_OFF_Item, &SensorRegulType_Item, &InvertOUT_Item,
    		   &OK_Item, &Down_Item, &Up_Item, &Right_Item, &Left_Item,
       },
       { // ������� ������ ����

    		   SensorIndex_ItemEvent, SensorThresold_ItemEvent, SensorThresold2_ItemEvent, SensorHysteresis_ItemEvent, SensorPWM_ItemEvent,
    		   SensorMenu_ReturnEvent, SensorMenu_SaveEvent, SensRegul_ON_OFF_ItemEvent, SensorRegulType_ItemEvent, InvertOUT_ItemEvent,
    		   OK_ItemEvent, Down_ItemEvent, Up_ItemEvent, Right_ItemEvent, Left_ItemEvent,
       },
       TRUE,
       &SensorMenu_Draw,
       0,
       0,
}; 
 
 
uint8_t *pSensorActiveParametr = (void*)0;
uint8_t SensorActiveButton = 0;
volatile Regulates_INFO Regul_Temp_Struct;
extern volatile uint8_t SelectedRegulate;

static char *OLD_InfoText = " ";
static char *OLD_Mode_Name = " ";

/* Functions ----------------------------------------------------------------*/
 
/*******************************************************************************
 * Function Name  : TempSensorsMenu_Draw
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SensorMenu_Draw(void)
{ 
	SensorMenu.MaxItems = 10;

	STLM75_LedEnableState(SET, Regul_Temp_Struct.SensorIndex);

	LCD_ButtonShiftColorSelect(2);		// 3 ����� �� �������� �������
	LCD_SetBackColor(LightBlack3);		// ���� ���� � ������

	// ������� ����� ����������� � ������� ����� � �������� ������ ����
	LCD_ClearArea(410, 298, 431, 314, Red);
	LCD_SetFont(&DigitSansSerif11ptFontInfo);
	LCD_PutColorStrig(412, 301, 1, (char*)Regul_Temp_Struct.Name, White);

	// �������� ������ ������
	if(Regul_Temp_Struct.Out_Index != Regulates_OUT_NotAssigned)
	{
		// �������� ������
		Power_ON_OFF_ICO_Update(ENABLE, Regul_Temp_Struct.Out_Index);
	}


	Down_Menu_Reset();

	// ������ �����
	LCD_SetGraphicsColor(0x5cb);
	LCD_DrawLine(0, 65, 479, 65);
	LCD_DrawLine(410, 75, 410, 285);

	// ������ �����
//	LCD_SetGraphicsColor(0x5cb);
//	LCD_DrawLine(20, SensorThresold_Item.Y0 - 5, 355, SensorThresold_Item.Y0 - 5);
//	LCD_DrawLine(20, SensorHysteresis_Item.Y0 - 5, 355, SensorHysteresis_Item.Y0 - 5);
//	LCD_DrawLine(20, SensorPWM_Item.Y0 - 5, 355, SensorPWM_Item.Y0 - 5);

	OLD_InfoText = " ";
	OLD_Mode_Name = " ";

	/* ����������� ����� �� ������� � ����������� �� ��������� */
	SensorMenu_InitValues();

	// ������ ��� ������ ����
    Draw_All_Items_Menu(0, SensorMenu.MaxItems);
}
 



/*******************************************************************************
 * Function Name  : TempSensorsMenu_ReturnEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SensorMenu_ReturnEvent(void)
{
	FlashingText_Command(DISABLE);
	STLM75_LedEnableState(RESET, Regul_Temp_Struct.SensorIndex);

	LCD_ClearArea(0, 5, 479, 294, Global_BackColor);
	LCD_ClearArea(410, 298, 460, 315, Global_BackColor);

	Down_Menu_Reset();
	SetActiveMenu(&AllSensorsMenu);
	Draw_ActiveMenu();
}


/*******************************************************************************
 * Function Name  : TempSensorsMenu_SaveEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SensorMenu_SaveEvent(void)
{
	FlashingText_Command(DISABLE);

	// ������� � �����������
	LCD_ClearArea(SensorMenu.Items[SensorActiveButton]->X0, SensorMenu.Items[SensorActiveButton]->Y0,
			SensorMenu.Items[SensorActiveButton]->X1, SensorMenu.Items[SensorActiveButton]->Y1,	Global_BackColor);

	Draw_Item_Menu(SensorMenu.Items[SensorActiveButton]);

	Regulate_CopyStruct((Regulates_INFO*)&Regulates_Struct[SelectedRegulate], (Regulates_INFO*)&Regul_Temp_Struct);

	SaveRegulsSettings(SelectedRegulate);
}


/*******************************************************************************
 * Function Name  : SensRegul_ON_OFF_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SensRegul_ON_OFF_ItemEvent(void)
{
	FlashingText_Command(DISABLE);
	Draw_Item_Menu(pMenu->Items[SensorActiveButton]);

	if(Regul_Temp_Struct.State == ENABLE)
	{
		Regul_Temp_Struct.State = DISABLE;
	}
	else if(Regul_Temp_Struct.State == DISABLE)
	{
		Regul_Temp_Struct.State = ENABLE;
	}

	/* ������� ������ ��������/��������� */
	LCD_ClearArea(SensRegul_ON_OFF_Item.X0 - 2, SensRegul_ON_OFF_Item.Y0 - 1, \
			SensRegul_ON_OFF_Item.X1 + 2, SensRegul_ON_OFF_Item.Y1 + 1, Global_BackColor);

	SensorMenu_InitValues();
	Draw_All_Items_Menu(0, 9);
}


/*******************************************************************************
 * Function Name  : InvertOUT_ICO_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SensorRegulType_ItemEvent(void)
{
	FlashingText_Command(DISABLE);
	Draw_Item_Menu(pMenu->Items[SensorActiveButton]);

	if(Regul_Temp_Struct.RegulType == Thresold)
	{
		Regul_Temp_Struct.RegulType = Proportional;
	}
	else if(Regul_Temp_Struct.RegulType == Proportional)
	{
		Regul_Temp_Struct.RegulType = Thresold;
	}

	SensorMenu_InitValues();
	Draw_All_Items_Menu(0, SensorMenu.MaxItems);
}
 

/*******************************************************************************
 * Function Name  : InvertOUT_ICO_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void InvertOUT_ItemEvent(void)
{
	FlashingText_Command(DISABLE);
	Draw_Item_Menu(pMenu->Items[SensorActiveButton]);

	if(Regul_Temp_Struct.INVERT == ENABLE)
	{
		Regul_Temp_Struct.INVERT = DISABLE;
	}
	else if(Regul_Temp_Struct.INVERT == DISABLE)
	{
		Regul_Temp_Struct.INVERT = ENABLE;
	}

	SensorMenu_InitValues();

}






// ------------------------------------------ ��������� ������ � ����������� ------------------------------------------ //

/*******************************************************************************
 * Function Name  : SensorThresold_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SensorIndex_ItemEvent(void)
{
//	if(SelectedRegulate != 8)
//	{
		SensorMenu.MaxItems = 15;
		pSensorActiveParametr = (uint8_t*)&Regul_Temp_Struct.SensorIndex;
		SensorActiveButton = TouchObject;

		/* ��������� ������� */
		FlashingText_Command(DISABLE);

		// ������ ������� � ������ '��'
		Draw_All_Items_Menu(0, SensorMenu.MaxItems);

		/* �������� ������� */
		FlashingText_Enable(0, SensorMenu.Items[TouchObject]);
//	}
}


/*******************************************************************************
 * Function Name  : SensorThresold_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SensorThresold_ItemEvent(void)
{
	SensorMenu.MaxItems = 15;
	pSensorActiveParametr = (uint8_t*)&Regul_Temp_Struct.Thresold;
	SensorActiveButton = TouchObject;

	/* ��������� ������� */
	FlashingText_Command(DISABLE);

	// ������ ������� � ������ '��'
	Draw_All_Items_Menu(0, SensorMenu.MaxItems);

	/* �������� ������� */
	FlashingText_Enable(0, SensorMenu.Items[TouchObject]);
}


/*******************************************************************************
 * Function Name  : SensorThresold_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SensorThresold2_ItemEvent(void)
{
	SensorMenu.MaxItems = 15;
	pSensorActiveParametr = (uint8_t*)&Regul_Temp_Struct.Thresold_2;
	SensorActiveButton = TouchObject;

	/* ��������� ������� */
	FlashingText_Command(DISABLE);

	// ������ ������� � ������ '��'
	Draw_All_Items_Menu(0, SensorMenu.MaxItems);

	/* �������� ������� */
	FlashingText_Enable(0, SensorMenu.Items[TouchObject]);
}


/*******************************************************************************
 * Function Name  : SensorThresold_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void SensorHysteresis_ItemEvent(void)
{
	SensorMenu.MaxItems = 15;
	SensorActiveButton = TouchObject;

	pSensorActiveParametr = (uint8_t*)&Regul_Temp_Struct.Hysteresis;

	/* ��������� ������� */
	FlashingText_Command(DISABLE);

	// ������ ������� � ������ '��'
	Draw_All_Items_Menu(0, SensorMenu.MaxItems);

	/* �������� ������� */
	FlashingText_Enable(0, SensorMenu.Items[TouchObject]);
}


/*******************************************************************************
 * Function Name  : SensorThresold_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void SensorPWM_ItemEvent(void)
{
	SensorMenu.MaxItems = 15;
	SensorActiveButton = TouchObject;

	if(Regul_Temp_Struct.RegulType == Thresold) pSensorActiveParametr = (uint8_t*)&Regul_Temp_Struct.PWM;
	else pSensorActiveParametr = (uint8_t*)&Regul_Temp_Struct.PWM_ON_Parametr;

	/* ��������� ������� */
	FlashingText_Command(DISABLE);

	// ������ ������� � ������ '��'
	Draw_All_Items_Menu(0, SensorMenu.MaxItems);

	/* �������� ������� */
	FlashingText_Enable(0, SensorMenu.Items[TouchObject]);
}



/*---------------------------------------------------- ������� ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : OK_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void OK_ItemEvent(void)
{
	SensorMenu.MaxItems = 10;

	FlashingText_Command(DISABLE);

	LCD_ClearArea(420, 66, 479, 285, Global_BackColor);

	// ������ ��� ������ ����
	Draw_All_Items_Menu(0, SensorMenu.MaxItems);
}


/*******************************************************************************
 * Function Name  : Down_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Down_ItemEvent(void)
{
	(*pSensorActiveParametr)--;

	if(pSensorActiveParametr == &Regul_Temp_Struct.SensorIndex)
	{
		if((*pSensorActiveParametr < Regul_Temp_Struct.MIN_SensorIndex) || (*pSensorActiveParametr >= 254))
		{
			*pSensorActiveParametr = Regul_Temp_Struct.MIN_SensorIndex;
		}
	}
	//  ���� �������� �������� Thresold
	else if(pSensorActiveParametr == &Regul_Temp_Struct.Thresold)
	{
		if((*pSensorActiveParametr) >= 250) (*pSensorActiveParametr) = 99;
		else if((*pSensorActiveParametr) >= 100) (*pSensorActiveParametr) = 0;
	}

	else if(pSensorActiveParametr == &Regul_Temp_Struct.Thresold_2)
	{
		if((*pSensorActiveParametr) < Regul_Temp_Struct.Thresold + 5)
		{
			(*pSensorActiveParametr) = Regul_Temp_Struct.Thresold + 5;
		}
	}

	// ����� ���� �������� �������� Hysteresis
	else if(pSensorActiveParametr == &Regul_Temp_Struct.Hysteresis)
	{
		if((*pSensorActiveParametr) >= 250) (*pSensorActiveParametr) = 10;
		else if((*pSensorActiveParametr) >= 11) (*pSensorActiveParametr) = 0;
	}

	// ����� ���� �������� ��������
	else if(pSensorActiveParametr == &Regul_Temp_Struct.PWM_ON_Parametr)
	{
		(*pSensorActiveParametr) = (*pSensorActiveParametr) - 9;
		if((*pSensorActiveParametr) >= 200) (*pSensorActiveParametr) = 100;
		else if((*pSensorActiveParametr) > 100) (*pSensorActiveParametr) = 0;
	}

	// ����� ���� �������� �������� PWM
	else if(pSensorActiveParametr == (uint8_t*)&Regul_Temp_Struct.PWM)
	{
		(*pSensorActiveParametr) = (*pSensorActiveParametr) - 9;
		if((*pSensorActiveParametr) >= 200) (*pSensorActiveParametr) = 100;
		else if((*pSensorActiveParametr) > 100) (*pSensorActiveParametr) = 0;
	}


	SensorUpdateSetParametr((*pSensorActiveParametr));
}


/*******************************************************************************
 * Function Name  : Up_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Up_ItemEvent(void)
{
	(*pSensorActiveParametr)++;


	if(pSensorActiveParametr == &Regul_Temp_Struct.SensorIndex)
	{
		if(*pSensorActiveParametr > Regul_Temp_Struct.MAX_SensorIndex)
		{
			*pSensorActiveParametr = Regul_Temp_Struct.MAX_SensorIndex;
		}
	}
	//  ���� �������� �������� Thresold
	else if(pSensorActiveParametr == &Regul_Temp_Struct.Thresold)
	{
		if((*pSensorActiveParametr) >= 250) (*pSensorActiveParametr) = 99;
		else if((*pSensorActiveParametr) >= 100) (*pSensorActiveParametr) = 0;
	}

	else if(pSensorActiveParametr == &Regul_Temp_Struct.Thresold_2)
	{
		if((*pSensorActiveParametr) > 100)
		{
			(*pSensorActiveParametr) = 100;
		}
	}

	// ����� ���� �������� �������� Hysteresis
	else if(pSensorActiveParametr == &Regul_Temp_Struct.Hysteresis)
	{
		if((*pSensorActiveParametr) >= 250) (*pSensorActiveParametr) = 10;
		else if((*pSensorActiveParametr) >= 11) (*pSensorActiveParametr) = 0;
	}

	// ����� ���� �������� ��������
	else if(pSensorActiveParametr == &Regul_Temp_Struct.PWM_ON_Parametr)
	{
		(*pSensorActiveParametr) = (*pSensorActiveParametr) + 9;
		if((*pSensorActiveParametr) >= 200) (*pSensorActiveParametr) = 100;
		else if((*pSensorActiveParametr) > 100) (*pSensorActiveParametr) = 0;
	}

	// ����� ���� �������� �������� PWM
	else if(pSensorActiveParametr == (uint8_t*)&Regul_Temp_Struct.PWM)
	{
		(*pSensorActiveParametr) = (*pSensorActiveParametr) + 9;
		if((*pSensorActiveParametr) >= 200) (*pSensorActiveParametr) = 100;
		else if((*pSensorActiveParametr) > 100) (*pSensorActiveParametr) = 0;
	}

	SensorUpdateSetParametr((*pSensorActiveParametr));
}


/*******************************************************************************
 * Function Name  : Right_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Right_ItemEvent(void)
{

}


/*******************************************************************************
 * Function Name  : Left_ItemEvent
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Left_ItemEvent(void)
{

}





/*--------------------------------------------- ������ ������� ���� --------------------------------------------------*/

/*******************************************************************************
* Function Name  : UpdateSetParametr
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
static void SensorUpdateSetParametr(uint32_t NewData)
{
	// ������� ������ ����� ��������� ��������� �������
	SensorMenu.Items[SensorActiveButton]->TextColor = Global_BackColor;
	Draw_Item_Menu(SensorMenu.Items[SensorActiveButton]);

	// ��������� ����� ������ � ������������� ���� ������ - �����
	if(SensorMenu.Items[SensorActiveButton] == &SensorIndex_Item)	//
	{
		STLM75_LedDisableAll();

		if(NewData == 8)
		{
			strcpy(&SensorIndex_Item.Text[0], "DHT11");
		}
		else if(NewData == 9)
		{
			strcpy(&SensorIndex_Item.Text[0], "SHT21");
		}
		else
		{
			ConvertToString(NewData + 1, &SensorIndex_Item.Text[0], 2);
		}

		STLM75_LedEnableState(SET, NewData);
	}
	else if(SensorMenu.Items[SensorActiveButton] == &SensorPWM_Item)	// ������ PWM
	{
		ConvertToString(NewData, &SensorPWM_Item.Text[0], 3);
	}
	else if(SensorMenu.Items[SensorActiveButton] == &SensorThresold2_Item)	// ������ � ����������
	{
		if(NewData >= 100) memcpy(&SensorThresold2_Item.Text[0], "--", 2);
		else
		{
			ConvertToString(NewData, &SensorThresold2_Item.Text[0], 2);
		}
	}
	else
	{
		ConvertToString(NewData, &SensorMenu.Items[SensorActiveButton]->Text[0], 2);
	}
	SensorMenu.Items[SensorActiveButton]->TextColor = White;

	//
	if(Regul_Temp_Struct.State == ENABLE)
	{
		SensorMenu.Items[SensorActiveButton]->TextColor = LighGreen;
	}

	// �������� ����� � �������� ������ � ������������ ������
	memcpy((char*)FlashingButton.Text, (char*)SensorMenu.Items[SensorActiveButton]->Text, 4);
	Draw_Item_Menu(SensorMenu.Items[SensorActiveButton]);
}

uint8_t ColorI = 8;

/*******************************************************************************
 * Function Name  : TempSensorsMenu_InitValues
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void SensorMenu_InitValues(void)
{
	uint16_t TxtColor, btnTxtColor;
	char *NormalName = (Regul_Temp_Struct.SensorType == STLM75)? "������" : "�� ������.";
	char *InvertName = (Regul_Temp_Struct.SensorType == STLM75)? "����������" : "������.";
	char *PrefixName = (Regul_Temp_Struct.SensorType == STLM75)? "C" : "%";
	char *RegTypeName = "";

	static char *InfoText = (void*)0;
	static char *ModeName = (void*)0;
	uint16_t PWM_PrefixColor = Global_BackColor;

	static uint8_t *Parametr;

	static uint16_t Regulate_INVERT_Text_Coordinate_X = 0, Regulate_TypeMODE_Text_Coordinate_X = 0;
	static uint16_t tmp_0 = 0, tmp_1 = 0;



	// ��� �����������, ���������/�������.
	if(Regul_Temp_Struct.RegulType == Thresold)
	{
		Parametr = (uint8_t*)&Regul_Temp_Struct.PWM;
		RegTypeName = "���������";
		ModeName = "������";
		if(Regul_Temp_Struct.State == ENABLE) PWM_PrefixColor = LighGreen;
		else PWM_PrefixColor = LightGray4;
	}
	else if(Regul_Temp_Struct.RegulType == Proportional)
	{
		Parametr = (uint8_t*)&Regul_Temp_Struct.PWM_ON_Parametr;
		RegTypeName = "����������������";
		if(Regul_Temp_Struct.SensorType == STLM75) ModeName = "��� �� ������";
		else ModeName = "��� �� �������";
		PWM_PrefixColor = Global_BackColor;
	}


	if(ModeName != OLD_Mode_Name)
	{
		LCD_SetFont(&sansation_22ptFontInfo);
		LCD_ClearArea(SensorPWM_Item.X0, SensorPWM_Item.Y0, SensorPWM_Item.X1 + 2, SensorPWM_Item.Y1, Global_BackColor);
		LCD_PutColorStrig(30, SensorPWM_Item.Y0, 1, OLD_Mode_Name, Global_BackColor);

		LCD_ClearArea(Regulate_TypeMODE_Text_Coordinate_X, 264, Regulate_TypeMODE_Text_Coordinate_X + tmp_1, 294, Global_BackColor);

		tmp_1 = 0;
		LCD_SetFont(&arialUnicodeMS_16ptFontInfo);
		LCD_Strig_PointsWidht(&tmp_1, RegTypeName);
		Regulate_TypeMODE_Text_Coordinate_X = (240 - tmp_1) / 2;
		LCD_PutColorStrig(Regulate_TypeMODE_Text_Coordinate_X, 264, 1, RegTypeName, BlueGray);
		OLD_Mode_Name = ModeName;
	}

	if(Regul_Temp_Struct.INVERT != ENABLE)
	{
		LCD_SetTextColor(Red);
		InfoText = (char*)NormalName;
	}
	else
	{
		LCD_SetTextColor(Blue1);
		InfoText = (char*)InvertName;
	}

	if(InfoText != OLD_InfoText)
	{
		LCD_SetFont(&arialUnicodeMS_16ptFontInfo);
		LCD_ClearArea(Regulate_INVERT_Text_Coordinate_X, 264, Regulate_INVERT_Text_Coordinate_X + tmp_0, 294, Global_BackColor);

		tmp_0 = 0;
		LCD_Strig_PointsWidht(&tmp_0, InfoText);
		Regulate_INVERT_Text_Coordinate_X = 240 + (((410 - 240) - tmp_0) / 2);
		LCD_PutStrig(Regulate_INVERT_Text_Coordinate_X, 264, 1, InfoText);
		OLD_InfoText = InfoText;
	}

	if(Regul_Temp_Struct.State == ENABLE)
	{
		TxtColor = LighGreen;
		btnTxtColor = LighGreen;

		memcpy(SensRegul_ON_OFF_Item.Text, "���������", 9);
		SensRegul_ON_OFF_Item.ICO = (BITMAP_INFO_16*)&SensRegul_OFF_ICOInfo;
		SensRegul_ON_OFF_Item.X0 = 180 - 5;
		SensRegul_ON_OFF_Item.X1 = 250 - 5;
	}
	else
	{
		TxtColor = LightGray4;
		btnTxtColor = White;

		memcpy(SensRegul_ON_OFF_Item.Text, "�������� ", 9);
		SensRegul_ON_OFF_Item.ICO = (BITMAP_INFO_16*)&SensRegul_ON_ICOInfo;
		SensRegul_ON_OFF_Item.X0 = 180;
		SensRegul_ON_OFF_Item.X1 = 250;
	}

//	Draw_Item_Menu(&SensRegul_ON_OFF_Item);
	SensorIndex_Item.TextColor = White;
	SensorThresold_Item.TextColor = btnTxtColor;
	SensorThresold2_Item.TextColor = btnTxtColor;
	SensorHysteresis_Item.TextColor = btnTxtColor;
	SensorPWM_Item.TextColor = btnTxtColor;

	LCD_SetTextColor(TxtColor);
	LCD_SetFont(&sansation_22ptFontInfo);

	if(Regul_Temp_Struct.SensorIndex == 8)
	{
		strcpy(&SensorIndex_Item.Text[0], "DHT11");
	}
	else if(Regul_Temp_Struct.SensorIndex == 9)
	{
		strcpy(&SensorIndex_Item.Text[0], "SHT21");
	}
	else
	{
		memset(SensorIndex_Item.Text, 0, 10);
		ConvertToString(Regul_Temp_Struct.SensorIndex + 1, &SensorIndex_Item.Text[0], 2);
	}

	LCD_PutStrig(30, SensorIndex_Item.Y0, 1, "������");

	ConvertToString(Regul_Temp_Struct.Thresold, &SensorThresold_Item.Text[0], 2);
	if(Regul_Temp_Struct.SensorType == STLM75) LCD_PutStrig(30, SensorThresold_Item.Y0, 1, "T ��������");
	else
	{
		LCD_PutStrig(30, SensorThresold_Item.Y0, 1, "RH ��������");
	}

	if(Regul_Temp_Struct.Thresold_2 >= 100)
	{
		Regul_Temp_Struct.Thresold_2 = 100;
		memcpy(&SensorThresold2_Item.Text[0], "--", 2);
	}
	else
	{
		ConvertToString(Regul_Temp_Struct.Thresold_2, &SensorThresold2_Item.Text[0], 2);
	}
	if(Regul_Temp_Struct.SensorType == STLM75) LCD_PutStrig(30, SensorThresold2_Item.Y0, 1, "T ����������");
	else
	{
		LCD_PutStrig(30, SensorThresold2_Item.Y0, 1, "RH ����������");
	}


	ConvertToString(Regul_Temp_Struct.Hysteresis, &SensorHysteresis_Item.Text[0], 2);
	LCD_PutStrig(30, SensorHysteresis_Item.Y0, 1, "����������");

	ConvertToString((*Parametr), &SensorPWM_Item.Text[0], 3);
	LCD_PutStrig(30, SensorPWM_Item.Y0, 1, ModeName);

	// ������ ��������
	LCD_PutColorStrig(LeftValX, SensorPWM_Item.Y0, 1, "%", PWM_PrefixColor);		// ������ - ���
	LCD_PutColorStrig(LeftValX, SensorThresold_Item.Y0, 1, PrefixName, TxtColor);		// �����
	LCD_PutColorStrig(LeftValX, SensorThresold2_Item.Y0, 1, PrefixName, TxtColor);		// �����
	LCD_PutColorStrig(LeftValX, SensorHysteresis_Item.Y0, 1, PrefixName, TxtColor);		// ����������


//	LCD_SetFont(&sansation_22ptFontInfo);







}



