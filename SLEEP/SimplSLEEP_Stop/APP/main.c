#include "SWM260.h"


void SerialInit(void);

int main(void)
{
	uint32_t i, j;
	
	SystemInit();
	
	SerialInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);				//接 LED，指示程序执行状态
	GPIO_ClrBit(GPIOA, PIN5);
	for(i = 0; i < 4; i++)
	{
		GPIO_InvBit(GPIOA, PIN5);
		for(j = 0; j < 5000000; j++) __NOP();		//不要注释掉，防止上电后立即进入Stop模式没法JLink更新程序
	}
	
	EnterStopMode();								//进入深度休眠，只能由Wakeup引脚（PC5）下降沿唤醒
	
	while(1==1)
	{
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
