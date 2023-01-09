#include "SWM260.h"


int main(void)
{	
	SystemInit();
	
	PORT_Init(PORTA, PIN8, PORTA_PIN8_TIMR0_OUT, 0);
	
	TIMR_Init(TIMR0, TIMR_MODE_OC, 1, SystemCoreClock/100, 0);
	
	TIMR_OC_Init(TIMR0, SystemCoreClock/400, 0, 1);
	
	TIMR_Start(TIMR0);
	
	while(1==1)
	{
	}
}
