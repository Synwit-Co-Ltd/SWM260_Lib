#include "SWM260.h"


int main(void)
{
	PWM_InitStructure  PWM_initStruct;
	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN8, 1, 0, 0, 0);			//反转PA8，指示PWM新周期开始中断位置
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);			//反转PA9，指示PWM高电平结束中断位置
	
	PWM_initStruct.PWMnXN = 0;					//只输出PWM0A信号、不输出PWM0AN信号
	PWM_initStruct.clkdiv = PWM_CLKDIV_8;		//F_PWM = 24M/8 = 3M	
	PWM_initStruct.cycle = 10000;				//3M/10000 = 300Hz，PWMnXN = 1时频率降低到150Hz
	PWM_initStruct.hduty =  2500;				//2500/10000 = 25%
	PWM_initStruct.deadzone = 00;
	PWM_initStruct.initLevel = 1;
	PWM_initStruct.HEndIE = 1;
	PWM_initStruct.NCycleIE = 1;
	PWM_initStruct.HCycleIE = 0;	
	PWM_Init(PWM0A, &PWM_initStruct);
	
	PORT_Init(PORTA, PIN11, PORTA_PIN11_PWM0A,  0);
	PORT_Init(PORTC, PIN10, PORTC_PIN10_PWM0AN, 0);
	PORT_Init(PORTA, PIN12, PORTA_PIN12_PWM0B,  0);
	PORT_Init(PORTC, PIN11, PORTC_PIN11_PWM0BN, 0);
	PORT_Init(PORTC, PIN4,  PORTC_PIN4_PWM1A,   0);
	PORT_Init(PORTB, PIN10, PORTB_PIN10_PWM1AN, 0);
	PORT_Init(PORTC, PIN3,  PORTC_PIN3_PWM1B,   0);
	PORT_Init(PORTB, PIN9,  PORTB_PIN9_PWM1BN,  0);
	
 	PWM_Start(PWM0A);

	while(1==1)
	{
	}
}

void PWM0_Handler(void)
{
	if(PWM_IntNCycleStat(PWM0A))
	{
		PWM_IntNCycleClr(PWM0A);
		
		GPIO_InvBit(GPIOA, PIN8);
	}
	else if(PWM_IntHEndStat(PWM0A))
	{
		PWM_IntHEndClr(PWM0A);
		
		GPIO_InvBit(GPIOA, PIN9);
	}
}
