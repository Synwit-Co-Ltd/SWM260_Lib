#include "SWM260.h"

RTC_AlarmStructure RTC_AlarmStruct;

int main(void)
{
	uint32_t i;
	RTC_InitStructure RTC_initStruct;
	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN6, 1, 0, 0, 0);
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);				//�� LED��ָʾ����ִ��״̬
	GPIO_SetBit(GPIOA, PIN5);						//����LED
	for(i = 0; i < SystemCoreClock/2; i++);			//��ʱ����ֹ�ϵ��SWD�����е��޷����س���
	
	RTC_initStruct.clksrc = RTC_CLKSRC_LRC32K;
	RTC_initStruct.Year = 2016;
	RTC_initStruct.Month = 5;
	RTC_initStruct.Date = 5;
	RTC_initStruct.Hour = 15;
	RTC_initStruct.Minute = 5;
	RTC_initStruct.Second = 5;
	RTC_initStruct.SecondIEn = 0;
	RTC_initStruct.MinuteIEn = 0;
	RTC_Init(RTC, &RTC_initStruct);
	
	RTC_AlarmStruct.Days = RTC_SUN | RTC_MON | RTC_TUE | RTC_WED | RTC_THU | RTC_FRI | RTC_SAT;
	RTC_AlarmStruct.Hour = RTC_initStruct.Hour;
	RTC_AlarmStruct.Minute = RTC_initStruct.Minute;
	RTC_AlarmStruct.Second = RTC_initStruct.Second + 5;
	RTC_AlarmStruct.AlarmIEn = 1;
	RTC_AlarmSetup(RTC, &RTC_AlarmStruct);
	
	RTC_Start(RTC);
	
	SYS->RTCWKCR = (1 << SYS_RTCWKCR_EN_Pos);
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);					//����LED
		for(i = 0; i < SystemCoreClock/8; i++);
		GPIO_ClrBit(GPIOA, PIN5);					//Ϩ��LED
		
		EnterSleepMode();
		
		while((SYS->RTCWKSR & (1 << SYS_RTCWKSR_FLAG_Pos)) == 0);	//�ȴ���������
		SYS->RTCWKSR |= (1 << SYS_RTCWKSR_FLAG_Pos);				//�������״̬
	}
}


void BTIMR3_RTC_Handler(void)
{
	RTC_DateTime dateTime;
	
	if(RTC_IntSecondStat(RTC))
	{
		RTC_IntSecondClr(RTC);
	}
	
	if(RTC_IntMinuteStat(RTC))
	{
		RTC_IntMinuteClr(RTC);
	}
	
	if(RTC_IntAlarmStat(RTC))
	{
		RTC_IntAlarmClr(RTC);
		
		RTC_GetDateTime(RTC, &dateTime);
		if(dateTime.Second < 55)
		{
			RTC_AlarmStruct.Second = dateTime.Second + 5;
		}
		else
		{
			RTC_AlarmStruct.Second = 0;
			RTC_AlarmStruct.Minute = dateTime.Minute + 1;
		}
		RTC_AlarmSetup(RTC, &RTC_AlarmStruct);
		
		GPIO_InvBit(GPIOA, PIN6);
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
