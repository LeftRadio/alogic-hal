/**
  ******************************************************************************
  * @file	 	AutoLogic Touch Screen Sourse
  * @author  	LeftRadio
  * @version 	V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "f_eeprom.h"
#include "TouchScreen.h"
#include "GlobalInit.h"
#include "LCD.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile Bool Touch_Interrup = FALSE;
volatile Bool Settings_ReCalibrateTouch = FALSE;

uint8_t TouchCurentMode = Touch_X;

/* DisplaySamples */
Matrix matrix;
Coordinate  display;
Coordinate TouchCoordinate = {0, 0};
Coordinate ScreenSample[3];
Coordinate DisplaySample[3] = { { 45, 45 }, {45, 270}, {430,160} } ;

/* Private function prototypes -----------------------------------------------*/
/* Exported function ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
FunctionalState getDisplayPoint(Coordinate * displayPtr, Coordinate * screenPtr, Matrix * matrixPtr);


/*******************************************************************************
* Function Name  : TouchPanel_EE
* Description    :
* Attention		 : None
*******************************************************************************/
static void CalibrateTouchPanel(void)
{
	uint8_t i;

	FLASH_Lock();
	__enable_irq();

	TouchPanel_Calibrate();

	__disable_irq();
	FLASH_Unlock();

	EE_WriteVariable(1, 0xA5FD);

	for(i = 0; i < 3; i++)
	{
		EE_WriteVariable((i*2) + 2, ScreenSample[i].x);
		EE_WriteVariable((i*2) + 3, ScreenSample[i].y);
	}
}

/*******************************************************************************
* Function Name  : TouchPanel_EE
* Description    :
* Attention		 : None
*******************************************************************************/
void TouchPanel_EE(uint8_t re_calibration)
{
	uint8_t i;
	static uint16_t ControlByte = 0;

	__disable_irq();	// çàïðåòèòü âñå ïðåðûâàíèÿ

	/* Unlock the Flash Program Erase controller */
  	FLASH_Unlock();

  	/* EEPROM Init */
  	EE_Init();

	if(re_calibration == 0)
	{
		EE_ReadVariable(1, &ControlByte);

		if(ControlByte != 0xA5FD)
		{
			CalibrateTouchPanel();
		}
		else
		{
			for(i = 0; i < 3; i++)
			{
				EE_ReadVariable((i*2) + 2, &ScreenSample[i].x);
				EE_ReadVariable((i*2) + 3, &ScreenSample[i].y);
			}
			setCalibrationMatrix(&DisplaySample[0], &ScreenSample[0], &matrix);
		}
  	}
  else
  {
	  CalibrateTouchPanel();
  }

	FLASH_Lock();
	__enable_irq();	// ðàçðåøèòü âñå ïðåðûâàíèÿ
}



/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Touch_Get_Coordinate(uint16_t ADC_Value)
{
//	float X_Coeff = 3375.0, Y_Coeff = 3250.0;

	if(TouchCurentMode == Touch_X)
	{
		TouchCurentMode = Touch_Y;
		TouchCoordinate.x = ADC_Value;

		ADC_Configuration(Touch_Y);
		HAL_Delay(1);
		Touch_Start();

		Touch_Interrup = FALSE;
	}
	else if(TouchCurentMode == Touch_Y)
	{
		TouchCurentMode = Touch_X;
		TouchCoordinate.y = ADC_Value;

		if(getDisplayPoint(&display, &TouchCoordinate, &matrix) == ENABLE)
		{
			TouchCoordinate.x = display.x;
			TouchCoordinate.y = display.y;
		}
		else
		{
			Touch_Stop();
		}

		Touch_Interrup = TRUE;
	}
	else return;
}


/*******************************************************************************
* Function Name  : Touch_Start
* Description    : Touch_Start
* Input          : None
* Return         : None
*******************************************************************************/
void Touch_Start(void)
{
	TIM_Cmd(TIM1, ENABLE);
}


/*******************************************************************************
* Function Name  : Touch_Start
* Description    : Touch_Start
* Input          : None
* Return         : None
*******************************************************************************/
void Touch_Stop(void)
{
	TIM_Cmd(TIM1, DISABLE);
}




/*******************************************************************************
* Function Name  : setCalibrationMatrix
* Description    : ¼ÆËã³ö K A B C D E F
* Input          : None
* Output         : None
* Return         : ·µ»Ø1±íÊ¾³É¹¦ 0Ê§°Ü
* Attention		 : None
*******************************************************************************/
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,
                          Coordinate * screenPtr,
                          Matrix * matrixPtr)
{

  FunctionalState retTHRESHOLD = ENABLE ;
  /* K£½(X0£­X2) (Y1£­Y2)£­(X1£­X2) (Y0£­Y2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = DISABLE;
  }
  else
  {
    /* A£½((XD0£­XD2) (Y1£­Y2)£­(XD1£­XD2) (Y0£­Y2))£¯K	*/
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	/* B£½((X0£­X2) (XD1£­XD2)£­(XD0£­XD2) (X1£­X2))£¯K	*/
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) -
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* C£½(Y0(X2XD1£­X1XD2)+Y1(X0XD2£­X2XD0)+Y2(X1XD0£­X0XD1))£¯K */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* D£½((YD0£­YD2) (Y1£­Y2)£­(YD1£­YD2) (Y0£­Y2))£¯K	*/
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) -
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* E£½((X0£­X2) (YD1£­YD2)£­(YD0£­YD2) (X1£­X2))£¯K	*/
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) -
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* F£½(Y0(X2YD1£­X1YD2)+Y1(X0YD2£­X2YD0)+Y2(X1YD0£­X0YD1))£¯K */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
  }
  return( retTHRESHOLD ) ;
}

/*******************************************************************************
* Function Name  : getDisplayPoint
* Description    :
* Input          : None
* Output         : None
* Return         :
* Attention		 : None
*******************************************************************************/
FunctionalState getDisplayPoint(Coordinate * displayPtr, Coordinate * screenPtr, Matrix * matrixPtr )
{
	FunctionalState retTHRESHOLD = ENABLE ;

	if( matrixPtr->Divider != 0 )
	{
		/* XD = AX+BY+C */
		displayPtr->x = ( (matrixPtr->An * screenPtr->x) + (matrixPtr->Bn * screenPtr->y) + matrixPtr->Cn ) / matrixPtr->Divider ;

		/* YD = DX+EY+F */
		displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + (matrixPtr->En * screenPtr->y) +	matrixPtr->Fn ) / matrixPtr->Divider ;
	}
	else
	{
		retTHRESHOLD = DISABLE;
	}
	return(retTHRESHOLD);
}


/*******************************************************************************
* Function Name  : TP_DrawPoint
* Description    : ÔÚÖ¸¶¨×ù±ê»­µã
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate
* Return         : None
*******************************************************************************/
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos)
{
	DrawPixel(Xpos, Ypos, 0xf800);
	DrawPixel(Xpos+1,Ypos,0xf800);
	DrawPixel(Xpos,Ypos+1,0xf800);
	DrawPixel(Xpos+1,Ypos+1,0xf800);
}


/*******************************************************************************
* Function Name  : DrawCross
* Description    : ÔÚÖ¸¶¨×ù±ê»­Ê®×Ö×¼ÐÇ
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate
* Return         : None
*******************************************************************************/
void DrawCross(uint16_t Xpos,uint16_t Ypos)
{
	LCD_SetGraphicsColor(0xFFFF);

	LCD_DrawLine(Xpos-15, Ypos, Xpos-2, Ypos);
	LCD_DrawLine(Xpos+2, Ypos, Xpos+15, Ypos);
	LCD_DrawLine(Xpos, Ypos-15, Xpos, Ypos-2);
	LCD_DrawLine(Xpos, Ypos+2, Xpos, Ypos+15);

	LCD_DrawLine(Xpos-15, Ypos+15, Xpos-7, Ypos+15);
	LCD_DrawLine(Xpos-15, Ypos+7, Xpos-15, Ypos+15);

	LCD_DrawLine(Xpos-15,Ypos-15,Xpos-7,Ypos-15);
	LCD_DrawLine(Xpos-15,Ypos-15, Xpos-15, Ypos-7);

	LCD_DrawLine(Xpos+7,Ypos+15,Xpos+15,Ypos+15);
	LCD_DrawLine(Xpos+15,Ypos+7,Xpos+15,Ypos+15);

	LCD_DrawLine(Xpos+7,Ypos-15,Xpos+15,Ypos-15);
	LCD_DrawLine(Xpos+15,Ypos-15,Xpos+15,Ypos-7);
}



/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    : Ð£×¼´¥ÃþÆÁ
* Input          : None
* Return         : None
*******************************************************************************/
void TouchPanel_Calibrate(void)
{
	volatile static uint8_t i = 0;
	Touch_Interrup = FALSE;
	Bool EndCalibration = FALSE;
	uint16_t tempPWM = TIM5->CCR1;
	TIM5->CCR1 = 71;

	while( Touch_Interrup == FALSE) { ; }
	Touch_Interrup = FALSE;
	ADC_Configuration(Touch_INT);
	HAL_Delay(1);

	while(EndCalibration != TRUE)
	{
		for(i = 0; i < 3; i++)
		{
			/* Reload IWDG counter */
			HAL_IWDG_Refresh(&hiwdg);

			LCD_FillScreen(Black);
			LCD_SetFont(&microsoftSansSerif_10ptFontInfo);
			LCD_PutColorStrig(10, 10, 1, "Touch crosshair to calibrate", White);
			DrawCross(DisplaySample[i].x,DisplaySample[i].y);


			while( Touch_Interrup == FALSE)
			{ /* Reload IWDG counter */
				HAL_IWDG_Refresh(&hiwdg);
			}

			GPIO_SetBits(GPIOB, GPIO_Pin_1);
			HAL_Delay(50);
			GPIO_ResetBits(GPIOB, GPIO_Pin_1);
			HAL_Delay(700);

			ScreenSample[i].x = TouchCoordinate.x;
			ScreenSample[i].y = TouchCoordinate.y;

			Touch_Interrup = FALSE;
			ADC_Configuration(Touch_INT);
			HAL_Delay(1);
		}

		setCalibrationMatrix( &DisplaySample[0], &ScreenSample[0], &matrix );
		LCD_FillScreen(Black);

		LCD_SetGraphicsColor(Red);
		LCD_DrawFillRect(225, 145, 255, 175, 0, 0);
		DrawCross(240, 160);
		LCD_PutColorStrig(10, 10, 1, "Êîñíèòåñü êðàñíîãî êâàäðàòà", White);

		uint32_t strTime = RTC_GetCounter();
		uint32_t curTime = strTime;
		while((Touch_Interrup == FALSE) && (curTime - strTime < 10))
		{
			curTime = RTC_GetCounter();
		}
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
		HAL_Delay(50);
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		HAL_Delay(500);

		if(Touch_Interrup == TRUE)
		{
			if((TouchCoordinate.x > 225) && (TouchCoordinate.x < 255))
			{
				if((TouchCoordinate.y > 145) && (TouchCoordinate.y < 175))
				{
					EndCalibration = TRUE;
				}
			}
			else
			{
				matrix.Divider = 0;
			}

			Touch_Interrup = FALSE;
			ADC_Configuration(Touch_INT);
			HAL_Delay(1);
		}
	}

//	setCalibrationMatrix( &DisplaySample[0], &ScreenSample[0], &matrix );
	LCD_FillScreen(Black);
	TIM5->CCR1 = tempPWM;
	return;
}
