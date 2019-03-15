#include "stm32f4xx_usart.h"
#include <stdio.h>

#pragma import(__use_no_semihosting)   
                          
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;  
        
void _sys_exit(int x) 
{ 
	x = x; 
}

int fputc(int ch, FILE *f)	//输出重定向  printf
{      
	while(USART_GetFlagStatus(USART1 , USART_FLAG_TC) !=SET );
	USART1->DR = (uint8_t)ch;
	return ch;
}

int fgetc(FILE *f)				//输入重定向  scanf
{
	while(USART_GetFlagStatus(USART1 , USART_FLAG_RXNE) !=SET );
	return (int)USART1->DR;
}


void UartInit(uint32_t baud)
{
	USART_InitTypeDef uart;
	GPIO_InitTypeDef gpio;
	NVIC_InitTypeDef nvic;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
	
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(GPIOA , &gpio);
	

	GPIO_PinAFConfig(GPIOA , GPIO_PinSource9 ,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA , GPIO_PinSource10 ,GPIO_AF_USART1);
	
	uart.USART_BaudRate = baud;
	uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	uart.USART_Parity = USART_Parity_No;
	uart.USART_StopBits = USART_StopBits_1;
	uart.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1 , &uart);

	nvic.NVIC_IRQChannel = USART1_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvic);



	USART_ITConfig(USART1 , USART_IT_RXNE , ENABLE);
	
	USART_Cmd(USART1 , ENABLE);
	
	USART_ClearFlag(USART1 , USART_FLAG_TC);
}


//中断服务函数    中断服务句柄函数 (╯‵□′)╯︵┻━┻
void USART1_IRQHandler(void) 
{
	if(USART_GetITStatus(USART1 , USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART1 , USART_IT_RXNE);
	}
}
/*****************************************************************/
