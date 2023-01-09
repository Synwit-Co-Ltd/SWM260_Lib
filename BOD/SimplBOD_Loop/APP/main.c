#include "SWM260.h"


int main(void)
{	
	SystemInit();
		
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//输出， 接LED
	
	SYS->BODCR = (1 << SYS_BODCR_IE_Pos) |
				 (0 << SYS_BODCR_RSTLVL_Pos) |	//2.0V复位
				 (0 << SYS_BODCR_RSTLVL_Pos);	//2.7V触发中断
	NVIC_DisableIRQ(BOD_PWMHALT_IRQn);			//想要查询BODSR.IF、又不想触发BOD中断，必须将BODCR.IE置位、同时关闭BOD的NVIC内核中断
	
	while(1==1)
	{
		while(SYS->BODSR & SYS_BODSR_IF_Msk)
		{
			SYS->BODSR = (1 << SYS_BODSR_IF_Pos);		//清除中断标志
			
			GPIO_SetBit(GPIOA, PIN5);
		}
		
		GPIO_ClrBit(GPIOA, PIN5);
	}
}

