/**
  ******************************************************************************
  * @file	 	User_Interface.c
  * @author  	AL Team
  * @version 	1.5.6
  * @date
  * @brief		AutoLogic Main Sourse
  ******************************************************************************
**/


/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "GlobalInit.h"
#include "User_Interface.h"
#include "TouchScreen.h"
#include "Timers.h"
#include "Outputs.h"
#include "STLM75.h"
#include "DHT11.h"
#include "Regulates.h"
#include "Host.h"
#include "Pparam.h"
#include "Graphics.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
Bool SecondEvent = FALSE;
FunctionalState GLOBAL_WORK = ENABLE;
uint8_t i;
volatile uint8_t DHT_Cnt = 0;
volatile HAL_StatusTypeDef global_init_state = HAL_ERROR;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  user_main
  * @param  None
  * @retval None
  */
void user_main(void) {
	/* Start peripheral, LCD and other */
	global_init_state = Global_Init();
	
    /* init interface and draw menu's */    
	Interface_Init();
	SetActiveMenu(&MainMenu);
	Draw_ActiveMenu();
	HAL_Delay(50);
    /* Read and set backlight PWM value */
	ReadBackLightSettings();

    /* main cycle */
	while(1) {
		/* Reload IWDG counter */
		HAL_IWDG_Refresh(&hiwdg);

		// Если пришли данные с хоста
    	if(SendHostData == TRUE)
    	{
    		SendHostData = FALSE;
    		Host_To_MCU_Send_Data(HostIN_Data, HostIN_Nb_bytes);
    	}

    	// приостановка прибора по команде с хоста 0х30
    	while(Host_Pause == TRUE)
    	{
    		// Если пришли данные с хоста
    		if(SendHostData == TRUE){ SendHostData = FALSE;	Host_To_MCU_Send_Data(HostIN_Data, HostIN_Nb_bytes); }
    		HAL_IWDG_Refresh(&hiwdg);
    	}


    	/* секундное событие */
    	if((SecondEvent == TRUE) && (GLOBAL_WORK == ENABLE))
    	{
    		SecondEvent = FALSE;

    		/* обновляем время на экране */
    		Display_Date_and_Time(0);

    		/* Опрашиваем все датчики температуры */
    		ReadAll_LM75();
    		Read_Internal_LM75();

    		/* Считываем датчик влажности */
    		DHT_Cnt++;
    		if(DHT_Cnt == 1)
    		{
    			if(SHT_Read(SHTSensor.Data, RH_measurement_NO_Hold) == ERROR) memset(SHTSensor.Data, 0, 5);
    			if(pMenu == &MainMenu) Update_HTSensor_OnScreen(SHTSensor.Data, 1);
    			else if(pMenu == &AllSensorsMenu) Update_HTSensor_OnScreenAllSensors(HTSensor->Data[0], HTSensor->Status);
    		}
    		else if(DHT_Cnt == 2)
    		{
    			if(SHT_Read(&SHTSensor.Data[2], T_measurement_NO_Hold) == ERROR) memset(SHTSensor.Data, 0, 5);
    			if(pMenu == &MainMenu) Update_HTSensor_OnScreen(SHTSensor.Data, 1);
    			else if(pMenu == &AllSensorsMenu) Update_HTSensor_OnScreenAllSensors(HTSensor->Data, HTSensor->Status);

    			if(DHT_Read(DHTSensor.Data, RH_measurement_NO_Hold) == ERROR) memset(DHTSensor.Data, 0, 5);
    			else DHTSensor.Data[0] += 9;

    			if(pMenu == &MainMenu) Update_HTSensor_OnScreen(DHTSensor.Data, 0);
    			else if(pMenu == &AllSensorsMenu) Update_HTSensor_OnScreenAllSensors(HTSensor->Data[0], HTSensor->Status);


    			DHT_Cnt = 0;
    			Graphics_Day_MIN_MAX();
    			Graphics_Month_MIN_MAX();
    		}

    		// проверки нужно ли обновлять состояние выходов
    		Power_ON_OFF_Squares_Update();
    	}

    	// иначе если обновление мигающейй кнопки
    	else if(FlashingButtonUpdate == TRUE)
    	{
    		FlashingButtonUpdate = FALSE;
    		Draw_Item_Menu((menuItem_INFO*)&FlashingButton);
    	}

    	// если прикосновение к тачу
    	if(Touch_Interrup == TRUE)
    	{
    		// сбросить время автоотключения подсведки
    		LCD_Verify_BL_AutoOFF(RTC_GetCounter(), SET);

    		Touch_Interrup = FALSE;
    		Menu_Func();

    		if(Settings_ReCalibrateTouch == TRUE)
    		{
    			Settings_ReCalibrateTouch = FALSE;

    			GLOBAL_WORK = DISABLE;
    			TouchPanel_EE(1);
    			GLOBAL_WORK = ENABLE;

    			Interface_Init();
    			Down_Menu_Reset();
    			SetActiveMenu(&MainMenu);
    			Draw_ActiveMenu();

    			Display_Date_and_Time(1);
    			ReadBackLightSettings();
    		}
    	}

    	// Сохраняем данные за день пока активный день не поменялся в Save_Graphics_HoursPoint()
    	if(Save_Month_Point == TRUE)
    	{
    		Save_Month_Point = FALSE;
    		Save_Graphics_MonthPoint();
    	}

    	// Сохраняем данные за час
    	if(Save_Hours_Point == TRUE)
    	{
    		Save_Hours_Point = FALSE;
    		Save_Graphics_HoursPoint();
    	}
    }
}


/**
  * @brief  user_main
  * @param  None
  * @retval None
  */
void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc) {
    static uint8_t TempResetSeconds = 0;
    static RTC_DateTypeDef date = {0, 0, 0};
    
    /* */
    if (global_init_state != HAL_OK) {
        return;
    }
    /* */
    RTC_DateTypeDef new_date;
    HAL_RTC_GetDate(&hrtc, &new_date, RTC_FORMAT_BCD);
    if (date.Day != new_date.Day || date.Month != new_date.Month || date.Year != new_date.Year) {
        /* Save date for write in graphics */
        Graphics_Update_Save_Date(&date);
        /* update static date fields */
        date.Day = new_date.Day;
        date.Month = new_date.Month;
        date.Year = new_date.Year;
        /* */
        _save_date_to_backup(&date);
    }
  
    uint32_t rtc_cnt = RTC_ReadTimeCounter(&hrtc);

    // проверка автоотключения подсведки
    LCD_Verify_BL_AutoOFF(rtc_cnt, RESET);

    // Проверяем таймеры на совпадения времени включения/выключения
    VerifyTimersProgram(rtc_cnt);

    // Проверяем регулировки
    VerifyRegulatesProgram(rtc_cnt);

    // переинициализируем датчики температуры
    if(TempResetSeconds++ == 10)
    {
      TempResetSeconds = 0;
      TemeratureSensors_InitDefault();
    }

    SecondEvent = TRUE;
}


/*******************************************************************************
      END FILE
*******************************************************************************/