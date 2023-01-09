#include "SWM260.h"


int main(void)
{	
	uint32_t i;
	SystemInit();
	
	GPIO_Init(GPIOA, PIN3, 1, 0, 0, 0);			//�Ӽ��������룬����GPIOA.3������
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//GPIOA.5 => LED
	
	PORT_Init(PORTA, PIN6, PORTA_PIN6_TIMR2_IN, 1);
	PORTA->PULLU |= (1 << PIN6);
	
	TIMR_Init(TIMR2, TIMR_MODE_COUNTER, 1, 3, 1);	//ÿ��3�������ؽ����ж�
	
	TIMR_Start(TIMR2);
	
	while(1==1)
	{
		GPIO_InvBit(GPIOA, PIN3);
		
		for(i = 0; i < 10000; i++) __NOP();
	}
}

void TIMR2_Handler(void)
{
	TIMR_INTClr(TIMR2);
	
	GPIO_InvBit(GPIOA, PIN5);
}
