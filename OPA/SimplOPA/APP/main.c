#include "SWM260.h"

void SerialInit(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTC, PIN12, PORTC_PIN12_OPA0_INN, 0);
	PORT_Init(PORTC, PIN13, PORTC_PIN13_OPA0_INP, 0);
	PORT_Init(PORTC, PIN14, PORTC_PIN14_OPA0_OUT, 0);
	PORT_Init(PORTC, PIN6,  PORTC_PIN6_OPA1_INN,  0);
	PORT_Init(PORTC, PIN7,  PORTC_PIN7_OPA1_INP,  0);
	PORT_Init(PORTC, PIN8,  PORTC_PIN8_OPA1_OUT,  0);
	
	SYS->OPACR  |= (1 << SYS_OPACR_OPA0ON_Pos);
	SYS->OPACR  |= (1 << SYS_OPACR_OPA1ON_Pos);
	
	while(1==1)
	{
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTC, PIN13, PORTC_PIN13_UART0_TX, 0);	//GPIOC.13����ΪUART0 TXD
	PORT_Init(PORTC, PIN14, PORTC_PIN14_UART0_RX, 1);	//GPIOC.14����ΪUART0 RXD
 	
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
* ��������: fputc()
* ����˵��: printf()ʹ�ô˺������ʵ�ʵĴ��ڴ�ӡ����
* ��    ��: int ch		Ҫ��ӡ���ַ�
*			FILE *f		�ļ����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
