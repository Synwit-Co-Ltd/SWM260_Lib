#include "SWM260.h"


int main(void)
{	
	SystemInit();
		
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//����� ��LED
	
	SYS->BODCR = (1 << SYS_BODCR_IE_Pos) |
				 (0 << SYS_BODCR_RSTLVL_Pos) |	//2.0V��λ
				 (0 << SYS_BODCR_INTLVL_Pos);	//2.7V�����ж�
	NVIC_EnableIRQ(BOD_PWMHALT_IRQn);
	
	while(1==1)
	{
		GPIO_ClrBit(GPIOA, PIN5);
	}
}

void BOD_PWMHALT_Handler(void)
{
	SYS->BODSR = (1 << SYS_BODSR_IF_Pos);		//����жϱ�־
	
	GPIO_SetBit(GPIOA, PIN5);
}
