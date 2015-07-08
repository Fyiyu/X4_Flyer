#include "stm32f10x_it.h"
#include <rc.h>

void TIM1_CC_IRQHandler(void)
{	
	ppm_isr(); 
}

unsigned char TIM2_UPDATED = 0;
void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{     
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); 
		TIM2_UPDATED = 1;
	}	
}

extern void Uart1_IRQ(void);
void USART1_IRQHandler(void)
{
	Uart1_IRQ();
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{ 
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}

	//溢出-如果发生溢出需要先读SR,再读DR寄存器 则可清除不断入中断的问题
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE); //读SR
		USART_ReceiveData(USART2); //读DR
	}
}

void USART2_IRQHandler(void)
{

}

