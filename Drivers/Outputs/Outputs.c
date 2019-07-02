/**
  ******************************************************************************
  * @file
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Outputs.h"
#include "User_Interface.h"
#include "Pparam.h"
#include "Regulates.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void Update_ONE_OutButtonState(uint8_t Indx);
//static BitAction OUT_Read(uint8_t indxOUT);

/* Private variables ---------------------------------------------------------*/
OutStruct_Typedef StructOuts[20] = {

		 {{GPIOB, GPIO_Pin_12}, "", NotAssigned, RESET}, {{GPIOB, GPIO_Pin_13}, "", NotAssigned, RESET}, {{GPIOB, GPIO_Pin_15}, "", NotAssigned, RESET}, {{GPIOD, GPIO_Pin_12}, "", NotAssigned, RESET}, {{GPIOD, GPIO_Pin_13}, "", NotAssigned, RESET},
		 {{GPIOC, GPIO_Pin_6}, "", NotAssigned, RESET}, {{GPIOC, GPIO_Pin_7}, "", NotAssigned, RESET}, {{GPIOC, GPIO_Pin_8}, "", NotAssigned, RESET}, {{GPIOC, GPIO_Pin_9}, "", NotAssigned, RESET}, {{GPIOA, GPIO_Pin_8}, "", NotAssigned, RESET},
		 {{GPIOC, GPIO_Pin_3}, "", NotAssigned, RESET}, {{GPIOC, GPIO_Pin_2}, "", NotAssigned, RESET}, {{GPIOC, GPIO_Pin_1}, "", NotAssigned, RESET}, {{GPIOC, GPIO_Pin_0}, "", NotAssigned, RESET}, {{GPIOC, GPIO_Pin_13}, "", NotAssigned, RESET},
		 {{GPIOE, GPIO_Pin_6}, "", NotAssigned, RESET}, {{GPIOE, GPIO_Pin_5}, "", NotAssigned, RESET}, {{GPIOE, GPIO_Pin_4}, "", NotAssigned, RESET}, {{GPIOE, GPIO_Pin_2}, "", NotAssigned, RESET}, {{GPIOE, GPIO_Pin_3}, "", NotAssigned, RESET}
};

volatile FunctionalState Outs_ENABLE[20] =
{
		ENABLE, ENABLE, ENABLE, ENABLE, ENABLE, ENABLE, ENABLE, ENABLE, ENABLE, ENABLE,
		ENABLE, ENABLE, ENABLE, ENABLE, ENABLE, ENABLE, ENABLE, ENABLE, ENABLE, ENABLE
};


/* Functions ----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Timers_Default_Init
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Outputs_Init(void)
{
	ReadOutputsSettings();
}


/*******************************************************************************
* Function Name  : OUT_ON
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void OUT_ON(uint8_t indxOUT)
{
	if((Outs_ENABLE[indxOUT] == ENABLE) && (OUT_SateRead(indxOUT) != Bit_SET))
	{
		StructOuts[indxOUT].Out.OUT_Port->BRR = StructOuts[indxOUT].Out.GPIO_PIN;
		StructOuts[indxOUT].ManualState = SET;
		Update_Out_Flag[indxOUT] = TRUE;
	}
}


/*******************************************************************************
* Function Name  : OUT_ON
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void OUT_PWM_ON(uint8_t indxOUT)
{
	if(Outs_ENABLE[indxOUT] == ENABLE)
	{
		StructOuts[indxOUT].Out.OUT_Port->BRR = StructOuts[indxOUT].Out.GPIO_PIN;
	}
}


/*******************************************************************************
* Function Name  : OUT_ON
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void OUT_OFF(uint8_t indxOUT)
{
	if(OUT_SateRead(indxOUT) != Bit_RESET)
	{
		StructOuts[indxOUT].Out.OUT_Port->BSRR = StructOuts[indxOUT].Out.GPIO_PIN;
		StructOuts[indxOUT].ManualState = RESET;
		Update_Out_Flag[indxOUT] = TRUE;
	}
}


/*******************************************************************************
* Function Name  : OUT_ON
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void OUT_PWM_OFF(uint8_t indxOUT)
{
	StructOuts[indxOUT].Out.OUT_Port->BSRR = StructOuts[indxOUT].Out.GPIO_PIN;
}



/*******************************************************************************
* Function Name  : OUT_ON
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void OUT_NewState(uint8_t indxOUT, BitAction NewState)
{
	if((Outs_ENABLE[indxOUT] == ENABLE) && (NewState == Bit_SET))
	{
		HAL_GPIO_WritePin(StructOuts[indxOUT].Out.OUT_Port, StructOuts[indxOUT].Out.GPIO_PIN, GPIO_PIN_RESET);
		StructOuts[indxOUT].ManualState = SET;
		Update_Out_Flag[indxOUT] = TRUE;
	}
	else
	{
		HAL_GPIO_WritePin(StructOuts[indxOUT].Out.OUT_Port, StructOuts[indxOUT].Out.GPIO_PIN, GPIO_PIN_SET);
		StructOuts[indxOUT].ManualState = RESET;
		Update_Out_Flag[indxOUT] = TRUE;
	}
}

/*******************************************************************************
* Function Name  : OUT_SateRead
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
BitAction OUT_SateRead(uint8_t indxOUT)
{
	if(StructOuts[indxOUT].ManualState == SET) return Bit_SET;
	else return Bit_RESET;
}


///*******************************************************************************
//* Function Name  : OUT_Read
//* Description    :
//* Input          : indxOUT
//* Return         : BitAction
//*******************************************************************************/
//static BitAction OUT_Read(uint8_t indxOUT)
//{
//	return GPIO_ReadOutputDataBit(StructOuts[indxOUT].Out.OUT_Port, StructOuts[indxOUT].Out.GPIO_PIN);
//}


/*******************************************************************************
* Function Name  : VerifyRegulatesProgram
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void Control_Outs_From_Internal_Temperature(ErrorStatus Status, uint8_t Data, uint8_t Group)
{
	uint8_t i;
	uint8_t Start = 0, Stop = 9;
	FunctionalState State = ENABLE;

	if(Status == ERROR) return;

	if(Group == 1){ Start = 10; Stop = 19; }

	if(Data > 80) State = DISABLE;
	else State = ENABLE;

	for(i = Start; i <= Stop; i++)
	{
		Outs_ENABLE[i] = State;
		if(State == DISABLE) OUT_OFF(i);
	}
}
