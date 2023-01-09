#include "SWM260.h"

void SerialInit(void);

int main(void)
{
	ADC_InitStructure ADC_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTA, PIN14, PORTA_PIN14_ADC0_IN0, 0);		//PA.14 => ADC0.CH0
	PORT_Init(PORTA, PIN13, PORTA_PIN13_ADC0_IN1, 0);		//PA.13 => ADC0.CH1
	PORT_Init(PORTA, PIN12, PORTA_PIN12_ADC0_IN2, 0);		//PA.12 => ADC0.CH2
	PORT_Init(PORTA, PIN11, PORTA_PIN11_ADC0_IN3, 0);		//PA.11 => ADC0.CH3
	PORT_Init(PORTA, PIN10, PORTA_PIN10_ADC0_IN4, 0);		//PA.10 => ADC0.CH4
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_ADC0_IN5,  0);		//PA.9  => ADC0.CH5
	PORT_Init(PORTA, PIN8,  PORTA_PIN8_ADC0_IN6,  0);		//PA.8  => ADC0.CH6
	PORT_Init(PORTA, PIN7,  PORTA_PIN7_ADC0_IN7,  0);		//PA.7  => ADC0.CH7
	
	ADC_initStruct.clk_src = ADC_CLKSRC_HRC;
	ADC_initStruct.clk_div = 12;
	ADC_initStruct.channels = ADC_CH3;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE1;
	ADC_initStruct.trig_src = ADC_TRIGGER_TIMR3;
	ADC_initStruct.Continue = 0;						//非连续模式，即单次模式
	ADC_initStruct.EOC_IEn = 0;	
	ADC_initStruct.OVF_IEn = 0;
	ADC_Init(ADC0, &ADC_initStruct);					//配置ADC
	
	ADC_Open(ADC0);										//使能ADC
	
	TIMR_Init(TIMR3, TIMR_MODE_TIMER, 1, SystemCoreClock/200, 0);
	TIMR_Start(TIMR3);
	
	while(1==1)
	{
		while((ADC0->CH[3].STAT & 0x01) == 0);
		printf("%4d,", ADC_Read(ADC0, ADC_CH3));
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
