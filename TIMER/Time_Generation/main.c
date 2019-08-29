#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "main.h"
#include "misc.h"

static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

GPIO_InitTypeDef GPIO_InitStruct;
//int counter = 0;

/* Private functions ---------------------------------------------------------*/


void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update))
	{
		GPIO_ToggleBits(GPIOD, 0x1000);
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}




void timer__init_fun()
{
  // Enable clock for Timer-2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
  // Timer -2 has a clock of 84MHz
  // so update event is 1ms
  // Update Event (Hz) = timer_clock / ((Prescaler + 1) * 
  // (Period + 1))
  // Event (Hz) = 84MHz / ((8399 + 1) * (9 + 1)) = 40 Hz or 1 ms
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;		//Timer structure pointer
  TIM_TimeBaseInitStruct.TIM_Prescaler = 8399;
  TIM_TimeBaseInitStruct.TIM_Period = 999;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
  // TIM2 initialize
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	//Interrupt vector table structure initialization
	
	NVIC_InitTypeDef NVIC_Init_structure;
	NVIC_Init_structure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_Init_structure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init_structure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init_structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Init_structure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
  // Start TIM2
  TIM_Cmd(TIM2, ENABLE);
	
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{ 
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  // GPIOD initialization green LED (GPIOD12)
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOD, &GPIO_InitStruct);

	timer__init_fun();

  while (1)
  {
/*		if(counter == 500)
		{
			GPIO_ToggleBits(GPIOD, 0x1000);
			counter = 0;
		}*/
		//nothing
	}
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

	
  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}
