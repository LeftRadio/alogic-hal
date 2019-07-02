/**
  ******************************************************************************
  * @file    Item_struct.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief   header
  ******************************************************************************
**/

#ifndef __TOUCH_SCREEN_H
#define __TOUCH_SCREEN_H

/* Includes ------------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
typedef	struct POINT
{
   uint16_t x;
   uint16_t y;
} Coordinate;


typedef struct Matrix
{
long double An,
            Bn,
            Cn,
            Dn,
            En,
            Fn,
            Divider;
} Matrix;

/* Exported define -----------------------------------------------------------*/
#define Touch_X					0
#define Touch_Y					1
#define Touch_INT				2

/* Exported variables --------------------------------------------------------*/
extern volatile Bool Touch_Interrup;
extern volatile Bool Settings_ReCalibrateTouch;


/* Private variables ---------------------------------------------------------*/
extern Coordinate ScreenSample[3];
extern Coordinate DisplaySample[3];
extern Matrix matrix ;
extern Coordinate display ;
extern Coordinate TouchCoordinate;

/* Exported function ---------------------------------------------------------*/
void Touch_Get_Coordinate(uint16_t ADC_Value);
void Touch_Start(void);
void Touch_Stop(void);

void TouchPanel_EE(uint8_t re_calibration);
FunctionalState setCalibrationMatrix( Coordinate *displayPtr, Coordinate *screenPtr, Matrix *matrixPtr);
void TouchPanel_Calibrate(void);

#endif /* __TOUCH_SCREEN_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
