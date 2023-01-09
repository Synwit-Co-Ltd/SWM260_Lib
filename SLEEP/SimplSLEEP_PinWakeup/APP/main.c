#include "SWM260.h"


/* ע�⣺
 *	оƬ�� ISP��SWD ����Ĭ�Ͽ������������裬���������߹��ģ�������������߹��ģ�����ǰ��ر��������ŵ���������������
 */
 

int main(void)
{
	uint32_t i;
	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);				//�� LED��ָʾ����ִ��״̬
	GPIO_SetBit(GPIOA, PIN5);						//����LED
	for(i = 0; i < SystemCoreClock/2; i++);			//��ʱ����ֹ�ϵ��SWD�����е��޷����س���
	
	GPIO_Init(GPIOA, PIN9, 0, 1, 0, 0);				//�Ӱ���������ʹ��
	SYS->PAWKEN |= (1 << PIN9);						//����PA9���ŵ͵�ƽ����
	
	SYS->LRCCR |= 0x01;								//����ģʽʹ�õ�Ƶʱ��
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);					//����LED
		for(i = 0; i < SystemCoreClock/8; i++);
		GPIO_ClrBit(GPIOA, PIN5);					//Ϩ��LED
		
		EnterSleepMode();
		
		while((SYS->PAWKSR & (1 << PIN9)) == 0);	//�ȴ���������
		SYS->PAWKSR |= (1 << PIN9);					//�������״̬
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
