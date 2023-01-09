#include "SWM260.h"


void SerialInit(void);
void TestSignal(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	TestSignal();	//产生测试信号供Input Capture功能测量
	
	PORT_Init(PORTA, PIN8, PORTA_PIN8_TIMR0_IN, 1);
	
	TIMR_Init(TIMR0, TIMR_MODE_IC, 1, 0xFFFFFFFF, 0);
	
	TIMR_IC_Init(TIMR0, 1, 1);
	
	TIMR_Start(TIMR0);
	
	while(1==1)
	{
	}
}

void TestSignal(void)
{
	PWM_InitStructure  PWM_initStruct;
	
	PWM_initStruct.PWMnXN = 1;
	PWM_initStruct.clkdiv = PWM_CLKDIV_8;		//F_PWM = 24M/8 = 3M	
	PWM_initStruct.cycle = 10000;				//3M/10000 = 300Hz			
	PWM_initStruct.hduty =  2500;				//2500/10000 = 25%
	PWM_initStruct.initLevel = 1;
	PWM_initStruct.HEndIE = 0;
	PWM_initStruct.NCycleIE = 0;
	PWM_initStruct.HCycleIE = 0;	
	PWM_Init(PWM0A, &PWM_initStruct);
	PWM_Init(PWM0B, &PWM_initStruct);
	PWM_Init(PWM1A, &PWM_initStruct);
	PWM_Init(PWM1B, &PWM_initStruct);
	
	PORT_Init(PORTA, PIN11, PORTA_PIN11_PWM0A,  0);
	PORT_Init(PORTC, PIN10, PORTC_PIN10_PWM0AN, 0);

	PWM_Start(PWM0A);
}

void TIMR0_Handler(void)
{
	if(TIMR_IC_CaptureH_INTStat(TIMR0))
	{
		TIMR_IC_CaptureH_INTClr(TIMR0);
		
		printf("H: %d\r\n", TIMR_IC_GetCaptureH(TIMR0));
	}
	else if(TIMR_IC_CaptureL_INTStat(TIMR0))
	{
		TIMR_IC_CaptureL_INTClr(TIMR0);
		
		printf("L: %d\r\n", TIMR_IC_GetCaptureL(TIMR0));
	}
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
