#include "SWM260.h"

void SerialInit(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTB, PIN4,  PORTB_PIN4_ACMP0_INN,  0);
 	PORT_Init(PORTB, PIN5,  PORTB_PIN5_ACMP0_INP,  0);
	
	SYS->ACMPCR |= (1 << SYS_ACMPCR_CMP0ON_Pos) | (1 << SYS_ACMPCR_CMP0HYS_Pos) | (1 << SYS_ACMPCR_CMP0IE_Pos);	//�������͡�ʹ���ж�
	
	NVIC_EnableIRQ(PWM3_GPIOB_ACMP_IRQn);
	
	while(1==1)
	{
		printf("ACMP0->CMP0OUT = %d\r\n", (SYS->ACMPSR & SYS_ACMPSR_CMP0OUT_Msk) ? 1 : 0);
		
		for(i = 0; i < SystemCoreClock/10; i++) __NOP();
	}
}


void PWM3_GPIOB_ACMP_Handler(void)
{
	if(SYS->ACMPSR & SYS_ACMPSR_CMP0IF_Msk)
	{
		SYS->ACMPSR = (1 << SYS_ACMPSR_CMP0IF_Pos);
		
		printf("ACMP0 Interrupt Happened\r\n");
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
