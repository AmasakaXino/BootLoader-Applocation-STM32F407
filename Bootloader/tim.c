#include "stm32f4xx_tim.h"

#define RCC_BASE_TIM		RCC_APB1Periph_TIM6
#define BASE_TIM				TIM6

static uint32_t systicks;

/* APB1 84M */
void TimeBaseInit(void)
{
	RCC_APB1PeriphClockCmd(RCC_BASE_TIM,ENABLE);
	
	TIM_TimeBaseInitTypeDef tim;
	
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Period = 10000-1;
	tim.TIM_Prescaler = 8400-1;
	
	TIM_TimeBaseInit(BASE_TIM,&tim);
	
	TIM_ITConfig(BASE_TIM,TIM_IT_Update,ENABLE);
	TIM_Cmd(BASE_TIM,ENABLE);
}

void _delay_ms(uint32_t ms)
{
	uint32_t start = systicks;
	/* us to ms */
	ms *= 1000;
	while((systicks - start) < ms) ;
}

void _delay_us(uint32_t us)
{
	uint32_t start = systicks;
	while((systicks - start) < us) ;
}

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(BASE_TIM,TIM_IT_Update) != SET)
	{
		TIM_ClearITPendingBit(BASE_TIM,TIM_IT_Update);
		systicks++;
	}
}

/*****************************************************************/
