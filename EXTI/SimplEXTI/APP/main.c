#include "SWM260.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//输出，接LED
	
	GPIO_Init(GPIOA, PIN9, 0, 1, 0, 0);			//输入，上拉使能，接KEY
	
	EXTI_Init(GPIOA, PIN9, EXTI_FALL_EDGE);		//下降沿触发中断
	
	NVIC_EnableIRQ(GPIOA9_GPIOC6_IRQn);
	
	EXTI_Open(GPIOA, PIN9);
	
	while(1==1)
	{
	}
}

void GPIOA9_GPIOC6_Handler(void)
{
	EXTI_Clear(GPIOA, PIN9);
	
	GPIO_InvBit(GPIOA, PIN5);
}
