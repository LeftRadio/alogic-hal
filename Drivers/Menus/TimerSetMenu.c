/**
  ******************************************************************************
  * @file		TimerSetMenu.c
  * @author		LeftRadio
  * @version 	V1.0.0
  * @date
  * @brief		Timer Settings Source
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "User_Interface.h"
#include "TimerNormalSetMenu.h"
#include "Timers.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void TimerNormalSetMenu_Draw(void);


/* Private variables ---------------------------------------------------------*/

Menu_Struct_TypeDef TimerNormalSetMenu = {

		0,
		{  // ������ ����
				
		},
		{ // ������� ������ ����

		},
		TRUE,
		TimerNormalSetMenu_Draw,
		0,
		0,
};


/* Functions ----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormalSetMenu_Draw(void)
{
	
//	������ ��� ������ ����
//	Draw_All_Items_Menu();


}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void TimerNormalSetMenu_Return_Event(void)
{

}

