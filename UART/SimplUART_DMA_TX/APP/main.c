#include "SWM260.h"

char TX_Buffer[] = "Hello From Synwit\r\n";

void SerialInit(void);

int main(void)
{	
	DMA_InitStructure DMA_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_BYTE;
	DMA_initStruct.Count = sizeof(TX_Buffer);
	DMA_initStruct.SrcAddr = (uint32_t)TX_Buffer;
	DMA_initStruct.SrcAddrInc = 1;
	DMA_initStruct.DstAddr = (uint32_t)&UART0->DATA;
	DMA_initStruct.DstAddrInc = 0;
	DMA_initStruct.Trigger = DMA_CH0_UART0TX;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = DMA_IT_DONE;
	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	
	DMA_CH_Open(DMA_CH0);
   	
	while(1==1)
	{
		
	}
}

void DMA_Handler(void)
{
	uint32_t i;
	
	if(DMA_CH_INTStat(DMA_CH0, DMA_IT_DONE))
	{
		DMA_CH_INTClr(DMA_CH0, DMA_IT_DONE);
		
		for(i = 0; i < SystemCoreClock/20; i++)  __NOP();	//延时一会儿
		
		DMA_CH_Open(DMA_CH0);	// 重新开始，再次搬运
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
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
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
