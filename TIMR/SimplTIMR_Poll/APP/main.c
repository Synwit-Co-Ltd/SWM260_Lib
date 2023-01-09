#include "SWM260.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			// ‰≥ˆ£¨Ω”LED
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, 1, SystemCoreClock/2, 0);	//0.5√Î÷”≥¨ ±
	TIMR_Start(TIMR0);
	
	while(1==1)
	{
		if(TIMR_INTStat(TIMR0))
		{
			TIMR_INTClr(TIMR0);
			
			GPIO_InvBit(GPIOA, PIN5);
		}
	}
}
