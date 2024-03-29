#include "SWM260.h"

void SerialInit(void);

int main(void)
{
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	if(SYS->RSTSR & SYS_RSTSR_WDT_Msk)
	{
		SYS->RSTSR = (1 << SYS_RSTSR_WDT_Pos);
		
		printf("WDT Reset Happened\r\n");
	}
	
	WDT_Init(WDT, 0, 32768);
	WDT_Start(WDT);
	
	while(1==1)
	{
		WDT_Feed(WDT);
		
		/* 注意：
		 *	1、两次喂狗之间至少间隔 5 个 WDT 时钟周期，即 1000000 / 32768 * 5 = 150us；又考虑到 WDT 时钟误差很大，建议间隔不小于 300us
		 *	2、WDT 停止状态下，不要执行 WDT_Feed()
		 *	3、执行 WDT_Feed() 后，不能立即执行 WDT_Stop()，必须间隔 5 个 WDT 时钟周期再执行 WDT_Stop()
		*/
		
		for(i = 0; i < CyclesPerUs * 300 / 4; i++) __NOP();
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
