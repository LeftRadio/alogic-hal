/**
   ******************************************************************************
   * @file      Graphics.c
   * @author	LeftRadio
   * @version 	V1.0.0
   * @date      14.04.2013 20:27:59
   * @brief
   ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h"
#include "Graphics.h"
#include "Pparam.h"
#include "STLM75.h"
#include "DHT11.h"
#include "clock_calendar.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define GraphicsMonthColor				LighGreen
#define GraphicsMonthLablesColor		LighGreen

#define GraphicsDayColor				LighGreen
#define GraphicsDayLablesColor			LighGreen

/* Private macro -------------------------------------------------------------*/
/* Private const -------------------------------------------------------------*/
/* Private const -------------------------------------------------------------*/
const char Y_text_Temp[10][3] = {" 0", "10", "20", "25", "30", "35", "40", "50", "70", "90"};

/* Private variables ---------------------------------------------------------*/
GraphMode Graphics_Mode = Week;
Date_s Graphic_PrintData;

volatile uint8_t Graphics_Save_DayIndex = 255;		// текщий день для графика
volatile uint8_t Graphics_Save_MonthIndex = 255;		// текщий месяц для графика

volatile uint8_t Graphics_Print_DayIndex = 255;		// текщий отображаемый день для графика
volatile uint8_t Graphics_Print_MonthIndex = 255;		// текщий отображаемый месяц для графика

Graphics_object SensGraphics[GRAPHICS_SENSORS_COUNT];
Graphics_object *pGraphic = &SensGraphics[0];			// текущий выводимый датчик

Bool Save_Hours_Point = FALSE;
Bool Save_Month_Point = FALSE;

Date_s Graphics_Save_Date;

/* Private function prototypes -----------------------------------------------*/

/* Отрисовки */

void Draw_Graph(uint8_t StartPoint, uint8_t EndPoind, uint8_t *InData, uint16_t Color);
static void DrawGraphicsLables(uint16_t Color);
//static void DrawNoDataLabel(uint16_t Color);
ErrorStatus (*ReadPoint)(int8_t Day_Index, int8_t Sens_Index) = Read_Graphics_HoursPoint;

/* Functions ----------------------------------------------------------------*/
/*******************************************************************************
 * Function Name  : InitGraphicsObjects
 * Description    : Инициализация графиков
 * Input          : None
 * Return         : None
*******************************************************************************/
void InitGraphicsObjects(void)
{
	uint8_t i;
	Calculate_Date();

	// обнуляем все данные по всем датчикам
	for(i = 0; i < GRAPHICS_SENSORS_COUNT; i++)
	{
		memset(&SensGraphics[i].WeekData[0], 255, 24);
		SensGraphics[i].WeekData_MIN = 255;
		SensGraphics[i].WeekData_MAX = -1;

		memset(&SensGraphics[i].MonthData[0], 255, 31);
		SensGraphics[i].MonthData_MIN = 255;
		SensGraphics[i].MonthData_MAX = -1;

		SensGraphics[i].Type = LM75Type;
		SensGraphics[i].Sensor_Index = i;
		SensGraphics[i].Color = White;

		ConvertToString(i + 1, (char*)&SensGraphics[i].Name[0], 2);
	}

	SensGraphics[8].Type = DHTType;
	SensGraphics[9].Type = SHTType;

	Graphics_Save_Date.Day = s_DateStructVar.Day;
	Graphics_Save_Date.Month = s_DateStructVar.Month;
	Graphics_Save_Date.Year = s_DateStructVar.Year;
}


/*******************************************************************************
 * Function Name  : DrawGraphicsLables
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Draw_Graphics(uint8_t Index)
{
	uint8_t End_DrawPoint = (Graphics_Mode == Week)? 23 : 31;
	uint8_t *Data = (Graphics_Mode == Week)? pGraphic->WeekData : pGraphic->MonthData;

	ReadPoint = (Graphics_Mode == Week)? Read_Graphics_HoursPoint : Read_Graphics_MonthPoint;

	// отрисовываем сетку
	DrawGrid(pGraphic->Type, LightGray4, Gray);

	//
	if(Index == 255) return;
	else
	{
		// если нашли, то читаем по этой метке и по индексу текущего датчика
		if(ReadPoint(Index, pGraphic->Sensor_Index) == SUCCESS)
		{
			if(Graphics_Mode == Week)
			{
				// проверяем текущий ли день отображать
				if( ((Graphic_PrintData.Day == s_DateStructVar.Day) && (Graphic_PrintData.Month = s_DateStructVar.Month)) && (Graphic_PrintData.Year == s_DateStructVar.Year))
				{
					End_DrawPoint = (RTC_GetCounter() / 3600);
				}
			}
			else
			{
				// проверяем текущий ли месяц отображать
				if( (Graphic_PrintData.Month == s_DateStructVar.Month) && (Graphic_PrintData.Year == s_DateStructVar.Year) )
				{
					End_DrawPoint = s_DateStructVar.Day;
				}
			}

			Draw_Graph(0, End_DrawPoint, Data, GraphicsDayColor);

			// отрисовываем дату и номер датчика
			DrawGraphicsLables(GraphicsDayLablesColor);
		}
	}
}


/*******************************************************************************
 * Function Name  : DrawGraphicsLables
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Draw_Graph(uint8_t StartPoint, uint8_t EndPoind, uint8_t *InData, uint16_t Color)
{
	uint8_t i, j, Data_0, Data_1;
	uint16_t X0 = 0, X1 = 0, Y0 = 0, Y1 = 0;
	uint8_t X_cl = (Graphics_Mode == Week)? 19 : 15;

	// отрисовываем график
	for(i = StartPoint; i <= EndPoind; i++)
	{
		// j - следующая точка, расчитываем Х1 по этой точке
		j = i + 1;

		Data_0 = *(InData + i);
		Data_1 = *(InData + j);

		// Проверяем выход за предел данных
		if(Data_0 <= 100)
		{
			// расчитываем Х0 по текущему часу
			X0 = 29 + (i * X_cl);

			// расчитываем Y0 по данным
			if(Data_0 <= 20) Y0 = (uint16_t)(85.0 + ((float)Data_0 * 2));
			else if((Data_0 > 20) && (Data_0 <= 40)) Y0 = (uint16_t)(85.0 + 40 + (4 *((float)Data_0 - 20.0)));
			else if((Data_0 > 40) && (Data_0 <= 50)) Y0 = (uint16_t)(85.0 + 120 + (2 *((float)Data_0 - 40.0)));
			else if((Data_0 > 50) && (Data_0 <= 90)) Y0 = (uint16_t)(85.0 + 140 + (1 *((float)Data_0 - 50.0)));
			else if(Data_0 > 90) Y0 = (uint16_t)(85.0 + 180 + (2 *((float)Data_0 - 90.0)));

			// если текущий час 0, или последняя точка, или следующая точка неккоректна, то отрисовать одну лишь точку
			if(((EndPoind == 0) || (i == EndPoind)) || (Data_1 > 100))
			{
				LCD_SetGraphicsColor(LighGreen);
				LCD_DrawRect(X0 - 1, Y0 - 1, X0 + 1, Y0 + 1);
			}
			else		// иначе отрисовываем линии между точками
			{
				X1 = 29 + ((i + 1) * X_cl);

				// Проверяем выход за предел данных
				if(Data_1 > 100) Data_1 = 0;

				// расчитываем Y1 по данным
				if(Data_1 <= 20) Y1 = (uint16_t)(85.0 + ((float)Data_1 * 2));
				else if((Data_1 > 20) && (Data_1 <= 40)) Y1 = (uint16_t)(85.0 + 40 + (4 *((float)Data_1 - 20.0)));
				else if((Data_1 > 40) && (Data_1 <= 50)) Y1 = (uint16_t)(85.0 + 120 + (2 *((float)Data_1 - 40.0)));
				else if((Data_1 > 50) && (Data_1 <= 90)) Y1 = (uint16_t)(85.0 + 140 + (1 *((float)Data_1 - 50.0)));
				else if(Data_1 > 90) Y1 = (uint16_t)(85.0 + 180 + (2 *((float)Data_1 - 90.0)));

				// Отрисовываем линию
				LCD_SetGraphicsColor(Color_ChangeBrightness(Color, -18));
				LCD_DrawLine(X0, Y0 + 1, X1, Y1 + 1);

				LCD_SetGraphicsColor(Color);
				LCD_DrawRect(X0 - 1, Y0 - 1, X0 + 1, Y0 + 1);
				LCD_DrawRect(X1 - 1, Y1 - 1, X1 + 1, Y1 + 1);
				LCD_DrawLine(X0, Y0, X1, Y1);
			}

			if(j == EndPoind) break;
		}
	}
}


/*******************************************************************************
 * Function Name  : Clear_Graphics
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Clear_Graphics(uint8_t IndexToClear, uint8_t IndexToRedraw)
{
	uint8_t PrintData = (Graphics_Mode == Week)? Graphics_Print_DayIndex : Graphics_Print_MonthIndex;

	// очищаем обасть графигов черным цветом
	LCD_ClearArea(0, 66, 479, 294, Global_BackColor);

	if(IndexToRedraw != 255)
	{
		Draw_Graphics(PrintData);
	}
}


/*******************************************************************************
 * Function Name  : Graphics_Day_MIN_MAX
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Graphics_Day_MIN_MAX(void)
{
	uint32_t Curent_Counter = (RTC_GetCounter() / 3600);
	static uint32_t OLD_Hour = 255;
	uint8_t i;
	int8_t Data;

	// датчики температуры
	for(i = 0; i < 8; i++)
	{
		Data = (int8_t)(TempSensor[i].Data >> 1);
		if(Data > SensGraphics[i].WeekData_MAX) SensGraphics[i].WeekData_MAX = Data;
		if(Data < SensGraphics[i].WeekData_MIN) SensGraphics[i].WeekData_MIN = Data;
	}

	// датчик влажности
	Data = DHTSensor.Data[0];
	if(Data > SensGraphics[8].WeekData_MAX) SensGraphics[8].WeekData_MAX = Data;
	if(Data < SensGraphics[8].WeekData_MIN) SensGraphics[8].WeekData_MIN = Data;

	Data = SHTSensor.Data[0];
	if(Data > SensGraphics[9].WeekData_MAX) SensGraphics[9].WeekData_MAX = Data;
	if(Data < SensGraphics[9].WeekData_MIN) SensGraphics[9].WeekData_MIN = Data;

	// Проверка нужно ли сохранять
	if(OLD_Hour == 255) OLD_Hour = Curent_Counter;
	else if(OLD_Hour != Curent_Counter)
	{
		for(i = 0; i < GRAPHICS_SENSORS_COUNT; i++)
		{
			/* если минимум или максимум не схватило, то ошибка */
			if((SensGraphics[i].WeekData_MAX == -1) || (SensGraphics[i].WeekData_MIN == 255))
			{
				SensGraphics[i].WeekData[Curent_Counter] = 255;
			}
			else	// иначе считаем усредненные данные
			{
				SensGraphics[i].WeekData[Curent_Counter] = (SensGraphics[i].WeekData_MAX + SensGraphics[i].WeekData_MIN) >> 1;
			}

			SensGraphics[i].WeekData_MAX = -1;
			SensGraphics[i].WeekData_MIN = 255;
		}

		Save_Hours_Point = TRUE;
		OLD_Hour = Curent_Counter;
	}
}


/*******************************************************************************
 * Function Name  : Graphics_Month_MIN_MAX
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Graphics_Month_MIN_MAX(void)
{
	static Date_s OLD_Date = {0, 0, 0};
	uint8_t i, j, sum_counter = 0;
	uint8_t Data[30] = {255};
	uint32_t Sum = 0;
	uint8_t Save_DayIndex = Graphics_Save_Date.Day - 1;

	if(OLD_Date.Year == 0)
	{
		OLD_Date.Day = Graphics_Save_Date.Day;
		OLD_Date.Month = Graphics_Save_Date.Month;
		OLD_Date.Year = Graphics_Save_Date.Year;
	}
	else if(Date_Compare(s_DateStructVar, OLD_Date, 0) != 0)
	{
		for(i = 0; i < GRAPHICS_SENSORS_COUNT; i++)
		{
			if(Read_All_HoursPoint(Graphics_Save_DayIndex, i, &Data[0]) == SUCCESS)
			{
				for(j = 0; j < 24; j++)
				{
					if(Data[j] != 255)
					{
						Sum = Sum + Data[j];
						sum_counter++;
					}
				}

				/* ЕСли не нашли ни одной точки, либо не работает датчик то 255 */
				if(sum_counter == 0) SensGraphics[i].MonthData[Save_DayIndex] = 255;
				else
				{
					/* иначе считаем */
					SensGraphics[i].MonthData[Save_DayIndex] = Sum / sum_counter;
				}

				Sum = 0;
				sum_counter = 0;
			}
			else
			{
				SensGraphics[i].MonthData[Save_DayIndex] = 255;
			}
		}

		OLD_Date.Day = s_DateStructVar.Day;
		OLD_Date.Month = s_DateStructVar.Month;
		OLD_Date.Year = s_DateStructVar.Year;

		Save_Month_Point = TRUE;
	}
}



/*******************************************************************************
 * Function Name  : DrawGrid
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void DrawGrid(SensType Type, uint16_t TextColor, uint16_t GridColor)
{
	float i, j, k = 2;
	float cl_X, cl_Y;
	float Max_cl_X = 0, Max_cl_Y = 0;
	uint16_t X_Shift = 0;

	uint16_t str = 0;
	char chStr[3] = {0};

	uint16_t UpCrop = 293;

	if(Graphics_Mode == Week)
	{
		cl_X = 450/23;
		Max_cl_X = 29 + (cl_X * 23);
		str = 0;
	}
	else
	{
		cl_X = 450/30;
		Max_cl_X = 29 + (cl_X * 30);
		str = 1;
	}

	LCD_SetTextColor(TextColor);
	LCD_SetFont(&microsoftSansSerif_12ptFontInfo);

	if(Type == LM75Type)
	{
		cl_Y = (UpCrop - 85)/10;
		Max_cl_Y = 85 + (cl_Y * 10);
		LCD_DrawChar(8, Max_cl_Y - cl_Y + 8, 1, 'C');
	}
	else
	{
		cl_Y = (UpCrop - 85)/10;
		Max_cl_Y =  85 + (cl_Y * 10);
		LCD_DrawChar(8, Max_cl_Y - cl_Y + 8, 1, '%');
	}

	LCD_SetGraphicsColor(GridColor);
	LCD_SetFont(&microsoftSansSerif_10ptFontInfo);

	// вертикальные линии
	for(i = 29; i <= Max_cl_X; i += cl_X)
	{
		LCD_DrawLine(i, 85, i, Max_cl_Y);
	}

	// горизонтальные линии
	for(j = 85; j <= Max_cl_Y; j += cl_Y)
	{
		LCD_DrawLine(30, j, Max_cl_X, j);
	}



	// разметка по Х
	for(i = 29; i <= Max_cl_X; i += cl_X)
	{
		k++;
		if(k >= 2)
		{
			if(str < 10) ConvertToString(str, chStr, 1);
			else ConvertToString(str, chStr, 2);

			if(str < 10) X_Shift = -2;
			else if(str < 20) X_Shift = -5;
			else if(str < 30) X_Shift = -8;
			else X_Shift = -14;

			LCD_PutStrig(i + X_Shift, 66, 1, chStr);
			str += 2;
			k = 0;
		}
	}

	str = 0;
	chStr[0] = 0; chStr[1] = 0; chStr[2] = 0;

	// разметка по Y
	for(i = 85; i < Max_cl_Y; i += cl_Y)
	{
		if(str <= 9)
		{
//			if(Type == LM75Type)
//			{
				LCD_PutStrig(5, i - 9, 1, (char*)&Y_text_Temp[str][0]);
//			}
//			else
//			{
//				LCD_PutStrig(5, i - 9, 1, (char*)&Y_text_RH[str][0]);
//			}

			str++;
		}
	}
}


/*******************************************************************************
 * Function Name  : DrawGraphicsLables
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
static void DrawGraphicsLables(uint16_t Color)
{
	char Str[15] = {0};
	uint16_t x = 0;
	const uint16_t _Y = 266;
	uint8_t weekDay;

	LCD_SetTextColor(Color_ChangeBrightness(Color, 8));
	LCD_SetFont(&microsoftSansSerif_12ptFontInfo);

	// выводим номер датчика
	x = LCD_PutStrig(35, _Y, 1, "Датчик ");
	if(pGraphic->Type == DHTType) x = LCD_PutStrig(x + 10, _Y, 0, "DHT11");
	else if(pGraphic->Type == SHTType) x = LCD_PutStrig(x + 10, _Y, 0, "SHT21");
	else x = LCD_PutStrig(x + 10, _Y, 0, (char*)&pGraphic->Name[0]);

	if(Graphics_Mode == Week)
	{
		// выводим прочитанную дату(метку)
		ConvertToString(Graphic_PrintData.Day, &Str[0], 2);
		x = LCD_PutStrig(345, _Y, 0, (char*)&Str[0]);
		x = LCD_PutStrig(x + 5, _Y, 1, "-");

		ConvertToString(Graphic_PrintData.Month, &Str[0], 2);
		x = LCD_PutStrig(x + 5, _Y, 0, (char*)&Str[0]);

		weekDay = Calculate_WeekDay(Graphic_PrintData.Year, Graphic_PrintData.Month, Graphic_PrintData.Day);
		LCD_PutStrig(345, _Y - 20, 0, WeekDays_Names[weekDay]);
	}
	else
	{
		x = LCD_PutStrig(335, _Y, 0, Months_Names[Graphic_PrintData.Month - 1]);
	}

	x = LCD_PutStrig(x + 5, _Y, 0, "-");

	ConvertToString(Graphic_PrintData.Year, &Str[0], 4);
	x = LCD_PutStrig(x + 5, _Y, 0, (char*)&Str[0]);
}



/*******************************************************************************
 * Function Name  : DrawGraphicsLables
 * Description    :
 * Input          : None
 * Return         : None
*******************************************************************************/
void Graphics_Update_Save_Date(RTC_DateTypeDef* date)
{
	// запомним дату дня для записи
	Graphics_Save_Date.Day = date->Day;
	Graphics_Save_Date.Month = date->Month;
	Graphics_Save_Date.Year = date->.Year;
}

///*******************************************************************************
// * Function Name  : DrawNoData
// * Description    :
// * Input          : None
// * Return         : None
//*******************************************************************************/
//static void DrawNoDataLabel(uint16_t Color)
//{
//	LCD_SetTextColor(Color);
//	LCD_SetFont(&arialUnicodeMS_16ptFontInfo);
//	LCD_PutStrig(200, 150, 1, "Нет Данных");
//}
