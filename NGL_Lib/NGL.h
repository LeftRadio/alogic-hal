/*************************************************************************************
*
Project : R61581 Graphics Library :: API
Version :
Date    :
Author  : Left Radio
Comments:
*
**************************************************************************************/

#ifndef __NGL_H
#define __NGL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported typedef -----------------------------------------------------------*/
typedef enum {CLEAR = 0, DRAW = !CLEAR} DrawState;
//typedef enum {IN_OBJECT = 0, OUT_OBJECT = !IN_OBJECT} ClipMode;


/** Структура доступа к массиву индексов символов **/
typedef struct FontTable {
	uint8_t		width;           // Ширина символа
	uint16_t	start;           // Стартовый индекс на первый байт символа в массиве данных символов
} FONT_CHAR_INFO;

/** Структура для доступа к параметрам используемого шрифта **/
typedef struct
{
	uint8_t Height;		              // Высота символов
	uint8_t HeightBytes;			  // Высота символов в байтах
	uint8_t FirstChar;	              // Индекс первого символа
	uint8_t LastChar;		          // Индекс последнего символа
	uint8_t FontSpace;                // Пробел между символами
	const FONT_CHAR_INFO *FontTable;  // Таблица индексов символов
	const uint8_t *FontBitmaps;       // Указатель на массив с данными о символах
} FONT_INFO;


/** Bitmap information for Scheme_pict **/
typedef struct
{
	uint16_t Width;                 // Picture Width
	uint16_t Height;                // Picture Height
	uint8_t RLE;                    // RLE Copressed flag, if yes value = 1
	uint32_t massiveSize;           // Last index of bitmap massive
	const uint8_t *ptrBitmap;       // Bitmap point array
}	BITMAP_INFO_8;

typedef struct
{
	uint16_t Width;                 // Picture Width
	uint16_t Height;                // Picture Height
	uint8_t RLE;                    // RLE Copressed flag, if yes value = 1
	uint32_t massiveSize;           // Last index of bitmap massive
	const uint16_t *ptrBitmap;      // Bitmap point array
}	BITMAP_INFO_16;


/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function --------------------------------------------------------*/

/*************************************************************************************
*
*  Установка и считывание цветов
*
**************************************************************************************/
extern void LCD_SetTextColor(uint16_t color);			/* Установить цвет текста 	*/
extern uint16_t LCD_GetTextColor(void);					/* Получить цвета текста 	*/
extern void LCD_SetGraphicsColor(uint16_t color);		/* Установить цвет графики 	*/
extern uint16_t LCD_GetGraphicsColor(void);				/* Получить цвет графики 	*/
extern void LCD_SetBackColor(uint16_t color);			/* Установить цвет фона 	*/
extern uint16_t LCD_GetBackColor(void); 				/* Получить цвет фона 		*/

extern uint16_t Color_ChangeBrightness(uint16_t ColorIn, int8_t BrightLevel);

/** -----------------------------------    ------------------------------------------------------------------- **/
extern void LCD_ReadRAM_Prepare(void);

extern void (*LCD_PutPixel)(uint16_t Color);												/* Нарисовать точку */
extern void LCD_GetPixel(uint8_t *ReadData, uint16_t NumPoints);

extern void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);									/* Установить курсор */
extern void LCD_SetArea(uint16_t X0pos, uint16_t Y0pos, uint16_t X1pos, uint16_t Y1pos);	/* Установить область вывода */
extern void LCD_SetFullScreen(void);														/* Установить область вывода на весь экран */

extern void LCD_Init(void);																/* Инициализация LCD */
extern void LCD_FillScreen(uint16_t Color);												/* Полностью залить экран одним цветом */
extern void LCD_ClearArea(uint16_t X0pos, uint16_t Y0pos, uint16_t X1pos, uint16_t Y1pos, uint16_t Color);		/* Очистка или закраска области экрана одним цветом */

extern void LCD_Set_TE_InputPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/** --------------------------------  Работа с выводом bitmaps  -------------------------------- **/
extern void LCD_Draw_1_8_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_8 *IN_bitmap, uint8_t RLE);
extern void LCD_Draw_1_16_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_16 *IN_bitmap, uint8_t RLE);
extern void LCD_Draw_8_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_8 *IN_bitmap, uint8_t RLE);
extern void LCD_Draw_16_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_16 *IN_bitmap, uint8_t RLE);


/** --------------------------------  Работа с выводом символов и текста  -------------------------------- **/
extern void LCD_SetFont(const FONT_INFO *selectFont);
extern uint8_t LCD_DrawChar(uint16_t x, uint16_t y, uint8_t trspr, char c);
extern uint16_t LCD_PutStrig(uint16_t XPos, uint16_t YPos, uint8_t trspr, char *str);
extern uint16_t LCD_PutColorStrig(uint16_t XPos, uint16_t YPos, uint8_t trspr, char *str, uint16_t Color);
extern char* LCD_Strig_PointsWidht(uint16_t *XPos, char *str);	// Вычисление длинны строки в пикселях


/** --------------------------------  Графические примитивы  -------------------------------- **/
extern void DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t Color);
//extern void DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t Color);
extern void LCD_DrawLine(uint16_t X0pos, uint16_t Y0pos, uint16_t X1pos, uint16_t Y1pos);
extern void LCD_DrawRect(uint16_t X0pos, uint16_t Y0pos, uint16_t X1pos, uint16_t Y1pos);
extern void LCD_DrawFillRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint8_t border, uint16_t borderColor);
//extern void LCD_DrawFillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width, DrawState border, uint16_t borderColor);
//extern void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
extern void DrawBeveledRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Radius);
extern void DrawFillBeveledRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Radius, uint8_t border, uint16_t borderColor);
//extern void LCD_DrawTriangle(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t X3, uint16_t Y3, uint16_t Color);
//extern void LCD_DrawFillTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t Color);

extern void LCD_DrawButton(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Text_Y_Coordinate, char *Text, uint8_t PushState, uint16_t Color, uint16_t TextColor, uint8_t ButtonSwColor);
extern void LCD_ButtonShiftColorSelect(uint8_t ShiftPixelNum);

#endif /* __NGL_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

