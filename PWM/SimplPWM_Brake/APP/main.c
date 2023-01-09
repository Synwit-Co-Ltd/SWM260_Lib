#include "SWM260.h"

void SerialInit(void);


int main(void)
{
	PWM_InitStructure  PWM_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PWM_initStruct.PWMnXN = 0;					//ֻ���PWM0A�źš������PWM0AN�ź�
	PWM_initStruct.clkdiv = PWM_CLKDIV_8;		//F_PWM = 24M/8 = 3M	
	PWM_initStruct.cycle = 10000;				//3M/10000 = 300Hz��PWMnXN = 1ʱƵ�ʽ��͵�150Hz
	PWM_initStruct.hduty =  2500;				//2500/10000 = 25%
	PWM_initStruct.deadzone = 00;
	PWM_initStruct.initLevel = 1;
	PWM_initStruct.HEndIE = 0;
	PWM_initStruct.NCycleIE = 0;
	PWM_initStruct.HCycleIE = 0;	
	PWM_Init(PWM0A, &PWM_initStruct);
	PWM_Init(PWM0B, &PWM_initStruct);
	
	PORT_Init(PORTA, PIN11, PORTA_PIN11_PWM0A,  0);
	PORT_Init(PORTC, PIN10, PORTC_PIN10_PWM0AN, 0);
	PORT_Init(PORTA, PIN12, PORTA_PIN12_PWM0B,  0);
	PORT_Init(PORTC, PIN11, PORTC_PIN11_PWM0BN, 0);
	
	PWMG->CHEN |= (1 << PWMG_CHEN_PWM0A_Pos) | (1 << PWMG_CHEN_PWM0B_Pos);	//��·ͬʱ����
	
	
	PORT_Init(PORTA, PIN9, PORTA_PIN9_PWM_BRK0, 1);	// PA9�½���ɲ��
	PORT->PORTA_PULLU |= (1 << PIN9);
	
	PWMG->BRKCR = (0 << PWMG_BRKCR_S0INPOL_Pos);	// PWM_BRK0�͵�ƽɲ��
	PWMG->BRKIRS = 7;
	PWMG->BRKIE = (1 << PWMG_BRKIE_S0_Pos);
	NVIC_EnableIRQ(BOD_PWMHALT_IRQn);
	
	PWM0->BRKEN = (1 << PWM_BRKEN_EN_Pos) |
				  (1 << PWM_BRKEN_S0_Pos) |
				  (0 << PWM_BRKEN_OUT_Pos);			// ɲ��ʱ����͵�ƽ
	
	while(1==1)
	{
	}
}


void BOD_PWMHALT_Handler(void)
{
	if(PWMG->BRKIF & PWMG_BRKIF_S0_Msk)
	{
		PWMG->BRKCR |= (1 << PWMG_BRKCR_S0STCLR_Pos);
		PWMG->BRKIRS = (1 << PWMG_BRKIRS_S0_Pos);
		
		printf("PWM Brake happen\r\n");
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
