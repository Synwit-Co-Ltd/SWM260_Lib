#include <string.h>
#include "SWM260.h"


const char TX_String[8][32] = {
	"TestString\r\n",
	"TestString1\r\n",
	"TestString12\r\n",
	"TestString123\r\n",
	"TestString1234\r\n",
	"TestString12345\r\n",
	"TestString123456\r\n",
	"TestString1234567\r\n",
};

#define RX_LEN	256					// 推荐用 2 的整数次幂，从而将取余运算转换成与运算，加快运算速度
int  RX_Buffer[RX_LEN] = { 0 };		// DMA 搬运 UART 接收到的数据必须以字为单位，否则可能出现数据暂存在 DMA 内部无法被程序读出的问题
char TX_Buffer[RX_LEN] = { 0 };


volatile uint32_t RX_Start = 0;


void SerialInit(void);

int main(void)
{
	SystemInit();
	
	SerialInit();
	
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN13, PORTA_PIN13_UART1_RX, 1);	// 连接 PC13，接收 UART0 发出的数据，然后原样发出
	PORT_Init(PORTA, PIN12, PORTA_PIN12_UART1_TX, 0);
 	
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
 	UART_Init(UART1, &UART_initStruct);
	UART_Open(UART1);
	
	DMA_InitStructure DMA_initStruct;
	
	DMA_initStruct.Mode = DMA_MODE_CIRCLE;
	DMA_initStruct.Unit = DMA_UNIT_WORD;
	DMA_initStruct.Count = RX_LEN;
	DMA_initStruct.SrcAddr = (uint32_t)&UART1->DATA;
	DMA_initStruct.SrcAddrInc = 0;
	DMA_initStruct.DstAddr = (uint32_t)RX_Buffer;
	DMA_initStruct.DstAddrInc = 1;
	DMA_initStruct.Trigger = DMA_CH0_UART1RX;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = 0;
	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	DMA_CH_Open(DMA_CH0);
	
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_BYTE;
	DMA_initStruct.Count = sizeof(TX_Buffer);
	DMA_initStruct.SrcAddr = (uint32_t)TX_Buffer;
	DMA_initStruct.SrcAddrInc = 1;
	DMA_initStruct.DstAddr = (uint32_t)&UART1->DATA;
	DMA_initStruct.DstAddrInc = 0;
	DMA_initStruct.Trigger = DMA_CH1_UART1TX;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = 0;
	DMA_CH_Init(DMA_CH1, &DMA_initStruct);
	
	TIMR_Init(BTIMR0, TIMR_MODE_TIMER, CyclesPerUs, 1000, 1);
	TIMR_Start(BTIMR0);
	
	while(1==1)
	{
		for(int i = 0; i < 8; i++)
		{
			for(int j = 0; j < strlen(TX_String[i]); j++)
			{
				UART_WriteByte(UART0, TX_String[i][j]);
				
				while(UART_IsTXBusy(UART0));
			}
			
			for(int j = 0; j < SystemCoreClock/8; j++) __NOP();
		}
	}
}


void BTIMR0_Handler(void)
{
	static int remaining_last = RX_LEN;
	static int start_rx = 0;
	static int count_ms = 0;
	
	TIMR_INTClr(BTIMR0);
	
	int remaining = DMA_CH_GetRemaining(DMA_CH0);
	if(remaining_last != remaining)
	{
		remaining_last = remaining;
		
		start_rx = 1;
		
		count_ms = 0;
	}
	else if(start_rx)
	{
		count_ms++;
		if(count_ms >= 10)
		{
			count_ms = 0;
			
			start_rx = 0;
			
			/* 已经 10ms 未接收到新的数据，接收超时 */
			int str_len = (RX_LEN + RX_LEN - DMA_CH_GetRemaining(DMA_CH0) - RX_Start) % RX_LEN;
			for(int i = 0; i < str_len; i++)
				TX_Buffer[i] = RX_Buffer[(RX_Start + i) % RX_LEN];
			
			RX_Start += str_len;
			
			DMA_CH_SetSrcAddress(DMA_CH1, (uint32_t)TX_Buffer);
			DMA_CH_SetCount(DMA_CH1, str_len);
			DMA_CH_Open(DMA_CH1);
		}
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
