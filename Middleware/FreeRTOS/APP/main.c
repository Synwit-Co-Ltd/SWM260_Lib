#include "SWM260.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

QueueHandle_t queueADC;

void TaskADC(void *arg);
void TaskPWM(void *arg);
void SerialInit(void);

int main(void)
{ 	
 	SystemInit();
	
	SerialInit();
	
	xTaskCreate(TaskADC, (const char *)"ADC", 128, NULL, 2, NULL);
	xTaskCreate(TaskPWM, (const char *)"PWM", 128, NULL, 3, NULL);
	
	queueADC = xQueueCreate(16, 2);
	
	vTaskStartScheduler();
}

/****************************************************************************************************************************************** 
* ��������:	TaskADC()
* ����˵��: ����ADC�ɼ�����
* ��    ��: void *arg
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void TaskADC(void *arg)
{
	ADC_InitStructure ADC_initStruct;
	
	PORT_Init(PORTA, PIN14, PORTA_PIN14_ADC0_IN0,  0);		//PA.14 => ADC0.CH0
	PORT_Init(PORTA, PIN13, PORTA_PIN13_ADC0_IN1, 0);		//PA.13 => ADC0.CH1
	PORT_Init(PORTA, PIN12, PORTA_PIN12_ADC0_IN2, 0);		//PA.12 => ADC0.CH2
	PORT_Init(PORTA, PIN11, PORTA_PIN11_ADC0_IN3, 0);		//PA.11 => ADC0.CH3
	PORT_Init(PORTA, PIN10, PORTA_PIN10_ADC0_IN4, 0);		//PA.10 => ADC0.CH4
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_ADC0_IN5,  0);		//PA.9  => ADC0.CH5
	PORT_Init(PORTA, PIN8,  PORTA_PIN8_ADC0_IN6,  0);		//PA.8  => ADC0.CH6
	PORT_Init(PORTA, PIN7,  PORTA_PIN7_ADC0_IN7,  0);		//PA.7  => ADC0.CH7
	
	ADC_initStruct.clk_src = ADC_CLKSRC_HRC;
	ADC_initStruct.clk_div = 12;
	ADC_initStruct.channels = ADC_CH3;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE1;
	ADC_initStruct.trig_src = ADC_TRIGGER_SW;
	ADC_initStruct.Continue = 0;						//������ģʽ��������ģʽ
	ADC_initStruct.EOC_IEn = ADC_CH3;	
	ADC_initStruct.OVF_IEn = 0;
	ADC_Init(ADC0, &ADC_initStruct);					//����ADC
	
	NVIC_SetPriority(ADC0_IRQn, 2);
	
	ADC_Open(ADC0);										//ʹ��ADC
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);					//����ָʾ�ź�
	
	while(1)
	{
		ADC_Start(ADC0);
		
		vTaskDelay(200);
	}
}

void ADC0_Handler(void)
{
	uint16_t val;
	
	ADC0->IF = (1 << ADC_IF_CH3EOC_Pos);
	
	val = ADC_Read(ADC0, ADC_CH3);
	
	xQueueSendFromISR(queueADC, &val, 0);
	
	GPIO_InvBit(GPIOA, PIN5);
}


/****************************************************************************************************************************************** 
* ��������:	TaskPWM()
* ����˵��: �ȴ�ADCת�����������ADCת���������PWMռ�ձ�
* ��    ��: void *arg
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void TaskPWM(void *arg)
{
	uint16_t duty;
	PWM_InitStructure  PWM_initStruct;
	
	PWM_initStruct.PWMnXN = 1;					//ֻ���PWM0A�źš������PWM0AN�ź�
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
	
// 	PORT_Init(PORTA, PIN11, PORTA_PIN11_PWM0A,  0);		//ADC��������
	PORT_Init(PORTC, PIN10, PORTC_PIN10_PWM0AN, 0);
// 	PORT_Init(PORTA, PIN12, PORTA_PIN12_PWM0B,  0);		//ADC��������
	PORT_Init(PORTC, PIN11, PORTC_PIN11_PWM0BN, 0);
	
	PWM_Start(PWM0A);
	PWM_Start(PWM0B);
	
	while(1)
	{
		if(xQueueReceive(queueADC, &duty, 10) == pdTRUE)
		{
			printf("%d,", duty);
			if(duty <  100) duty =  100;
			if(duty > 4000) duty = 4000;
			
			PWM_SetHDuty(PWM0A, 10000 * duty / 4095);
			PWM_SetHDuty(PWM0B, 10000 - PWM_GetHDuty(PWM0A));
		}
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
