#include "SWM260.h"

#include "SEGGER_RTT.h"

void SerialInit(void);

int main(void)
{
 	uint32_t i;
 	
 	SystemInit();
	
	SerialInit();
	
	SEGGER_RTT_Init();
 	
	GPIO_Init(GPIOA, PIN7, 1, 0, 0, 0);				//GPIOA.7配置为输出引脚，推挽输出
	GPIO_Init(GPIOA, PIN8, 1, 0, 0, 0);				//GPIOA.8配置为输出引脚，推挽输出
	
 	while(1==1)
 	{
 		GPIO_InvBit(GPIOA, PIN7);
 		GPIO_InvBit(GPIOA, PIN8);
 		for(i=0; i<2000000; i++) i=i;
		
   		printf("Hi, World!\r\n");
		
		SEGGER_RTT_printf(0, "Hi, World!\r\n");
 	}
}

void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTC, PIN13, PORTC_PIN13_UART0_TX, 0);	//GPIOC.13配置为UART0 TXD
	PORT_Init(PORTC, PIN14, PORTC_PIN14_UART0_RX, 1);	//GPIOC.14配置为UART0 RXD
 	
 	UART_initStruct.Baudrate = 57600;
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
 	while(UART_IsTXFIFOFull(UART0));
	
	UART_WriteByte(UART0, ch);
 	
	return ch;
}
