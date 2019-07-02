/**
  ******************************************************************************
  * @file    HOST.h
  * @author
  * @version V1.0.0
  * @date
  * @brief
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GRAPH__H
#define __GRAPH__H

/* Includes ------------------------------------------------------------------*/
#include "Timers.h"
#include "clock_calendar.h"

/* Exported types ------------------------------------------------------------*/
typedef enum { DHTType, LM75Type, SHTType } SensType;
typedef enum { Week = 0, Month = !Week } GraphMode;

typedef struct
{
	uint8_t WeekData[24];
	int8_t WeekData_MAX;
	uint8_t WeekData_MIN;

	uint8_t MonthData[31];
	uint8_t MonthData_MAX;
	uint8_t MonthData_MIN;

	uint8_t Sensor_Index;
	SensType Type;

	const char Name[2];
	uint16_t Color;

} Graphics_object;


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define _Day_							0
#define _Month_							1

#define GRAPHICS_SENSORS_COUNT			10

/* Exported variables --------------------------------------------------------*/
extern GraphMode Graphics_Mode;
extern Date_s Graphic_PrintData;
extern Date_s Graphics_Save_Date;

extern volatile uint8_t Graphics_Save_DayIndex;		// текщий день дл€ графика
extern volatile uint8_t Graphics_Save_MonthIndex;	// текщий мес€ц дл€ графика

extern volatile uint8_t Graphics_Print_DayIndex;		// текщий отображаемый день дл€ графика
extern volatile uint8_t Graphics_Print_MonthIndex;		// текщий отображаемый мес€ц дл€ графика

extern Graphics_object *pGraphic;
extern Graphics_object SensGraphics[GRAPHICS_SENSORS_COUNT];

extern Bool Save_Hours_Point;
extern Bool Save_Month_Point;

/* Extern variables ----------------------------------------------------------*/
/* Extern functions ----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void InitGraphicsObjects(void);
void Graphics_Day_MIN_MAX(void);
void Graphics_Month_MIN_MAX(void);

void DrawGrid(SensType Type, uint16_t TextColor, uint16_t GridColor);
void Clear_Graphics(uint8_t IndexToClear, uint8_t IndexToRedraw);
void Draw_Graphics(uint8_t Index);

void Graphics_Update_Save_Date(RTC_DateTypeDef* date);

#endif  /*__GRAPH__H*/
