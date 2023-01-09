#include "SWM260.h"

volatile uint32_t HALLV0, HALLV1, HALLV2;

void SerialInit(void);
void SignalTest(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	SignalTest();
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, 1, 0xFFFFFFFF, 0);
	
	PORT_Init(PORTB, PIN11, PORTB_PIN11_HALL_IN0, 1);
	PORT_Init(PORTB, PIN12, PORTB_PIN12_HALL_IN1, 1);
	PORT_Init(PORTB, PIN13, PORTB_PIN13_HALL_IN2, 1);
	
	TIMRG->HALLMD = (3 << TIMRG_HALLMD_IN0_Pos) |
					(3 << TIMRG_HALLMD_IN1_Pos) |
					(3 << TIMRG_HALLMD_IN2_Pos);	//三个通道都是双边沿触发
	TIMRG->HALLIE = 1;
	TIMRG->HALLIM = 0;
	NVIC_EnableIRQ(SPI1_HALL_GPIOD_IRQn);
	
	TIMR_Start(TIMR0);
	
	while(1==1)
	{
		printf("%08X\t%08X\t%08X\r\n\r\n", HALLV0, HALLV1, HALLV2);
	}
}


void SPI1_HALL_GPIOD_Handler(void)
{
	if(TIMRG->HALLIF & TIMRG_HALLIF_IN0_Msk)
	{
		TIMRG->HALLIF = (1 << TIMRG_HALLIF_IN0_Pos);
		
		HALLV0 = TIMRG->HALLV0;
	}
	else if(TIMRG->HALLIF & TIMRG_HALLIF_IN1_Msk)
	{
		TIMRG->HALLIF = (1 << TIMRG_HALLIF_IN1_Pos);
		
		HALLV1 = TIMRG->HALLV1;
	}
	else if(TIMRG->HALLIF & TIMRG_HALLIF_IN2_Msk)
	{
		TIMRG->HALLIF = (1 << TIMRG_HALLIF_IN2_Pos);
		
		HALLV2 = TIMRG->HALLV2;
	}
}


void SignalTest(void)
{
	PWM_InitStructure PWM_initStruct;
	
	PWM_initStruct.PWMnXN = 1;					//同时输出PWM0A和PWM0AN
	PWM_initStruct.clkdiv = PWM_CLKDIV_8;		//F_PWM = 24M/8 = 3M
	PWM_initStruct.cycle = 10000;				//3M/10000 = 300Hz，PWMnXN = 1时频率降低到150Hz
	PWM_initStruct.hduty =  2500;				//2500/10000 = 25%
	PWM_initStruct.initLevel = 1;
	PWM_initStruct.HEndIE = 0;
	PWM_initStruct.NCycleIE = 0;
	PWM_initStruct.HCycleIE = 0;	
	PWM_Init(PWM0B, &PWM_initStruct);
	PWM_Init(PWM1A, &PWM_initStruct);
	
	PORT_Init(PORTA, PIN11, PORTA_PIN11_PWM0A,  0);
	PORT_Init(PORTC, PIN10, PORTC_PIN10_PWM0AN, 0);
	PORT_Init(PORTA, PIN12, PORTA_PIN12_PWM0B,  0);
	PORT_Init(PORTC, PIN11, PORTC_PIN11_PWM0BN, 0);
	PORT_Init(PORTC, PIN4,  PORTC_PIN4_PWM1A,   0);
	PORT_Init(PORTB, PIN10, PORTB_PIN10_PWM1AN, 0);
	PORT_Init(PORTC, PIN3,  PORTC_PIN3_PWM1B,   0);
	PORT_Init(PORTB, PIN9,  PORTB_PIN9_PWM1BN,  0);
	
	PWMG->CHEN |= (1 << PWMG_CHEN_PWM0B_Pos) | (1 << PWMG_CHEN_PWM1A_Pos);	//多路同时启动
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTC, PIN13, PORTC_PIN13_UART0_TX, 0);	//GPIOC.13配置为UART0 TXD
	PORT_Init(PORTC, PIN14, PORTC_PIN14_UART0_RX, 1);	//GPIOC.14配置为UART0 RXD
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/****************************************************************************************************************************************** 
* 函数名称: fputc()
* 功能说明: printf()使用此函数完成实际的串口打印动作
* 输    入: int ch		要打印的字符
*			FILE *f		文件句柄
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
