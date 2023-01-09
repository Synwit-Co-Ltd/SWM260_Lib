#include "SWM260.h"

#include "SEGGER_RTT.h"

void SerialInit(void);

int main(void)
{
 	uint32_t i;
 	
 	SystemInit();
	
	SerialInit();
	
	SEGGER_RTT_Init();
 	
	GPIO_Init(GPIOA, PIN7, 1, 0, 0, 0);				//GPIOA.7����Ϊ������ţ��������
	GPIO_Init(GPIOA, PIN8, 1, 0, 0, 0);				//GPIOA.8����Ϊ������ţ��������
	
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
	
	PORT_Init(PORTC, PIN13, PORTC_PIN13_UART0_TX, 0);	//GPIOC.13����ΪUART0 TXD
	PORT_Init(PORTC, PIN14, PORTC_PIN14_UART0_RX, 1);	//GPIOC.14����ΪUART0 RXD
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/****************************************************************************************************************************************** 
* ��������: fputc()
* ����˵��: printf()ʹ�ô˺������ʵ�ʵĴ��ڴ�ӡ����
* ��    ��: int ch		Ҫ��ӡ���ַ�
*			FILE *f		�ļ����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
 	while(UART_IsTXFIFOFull(UART0));
	
	UART_WriteByte(UART0, ch);
 	
	return ch;
}
