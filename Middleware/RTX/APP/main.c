#include "SWM260.h"

#include <RTL.h>

/* 注意：
*	为了简单，在RX_Conf_CM.c中配置所有Task都运行于特权等级，否则当Task中
* 代码写某些内核寄存器（如调用NVIC_DisableIRQ())时就会触发HardFault
************************************************************************/

os_mbx_declare(queueADC, 16);

__task void TaskINI(void);
__task void TaskADC(void);
__task void TaskPWM(void);
void SerialInit(void);

int main(void)
{ 	
 	SystemInit();
	
	SerialInit();
	
	os_sys_init(TaskINI);
}

__task void TaskINI(void)
{
	os_tsk_create(TaskADC, 1);
	os_tsk_create(TaskPWM, 2);
	
	os_mbx_init(&queueADC, sizeof(queueADC));
	
	os_tsk_delete(os_tsk_self());
}


/****************************************************************************************************************************************** 
* 函数名称:	TaskADC()
* 功能说明: 启动ADC采集任务
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
__task void TaskADC(void)
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
	ADC_initStruct.trig_src = ADC_TRIGGER_TIMR3;
	ADC_initStruct.Continue = 0;						//非连续模式，即单次模式
	ADC_initStruct.EOC_IEn = ADC_CH3;	
	ADC_initStruct.OVF_IEn = 0;
	ADC_Init(ADC0, &ADC_initStruct);					//配置ADC
	
	ADC_Open(ADC0);										//使能ADC
	
	TIMR_Init(TIMR3, TIMR_MODE_TIMER, 1, SystemCoreClock/10, 0);
	TIMR_Start(TIMR3);
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);					//调试指示信号
	
	while(1)
	{
		// do nothing
		
		os_dly_wait(500);
	}
}

void ADC0_Handler(void)
{
	uint32_t val;
	
	ADC0->IF = (1 << ADC_IF_CH3EOC_Pos);
	
	val = ADC_Read(ADC0, ADC_CH3);
	
	isr_mbx_send(&queueADC, (void *)val);
	
	GPIO_InvBit(GPIOA, PIN5);
}


/****************************************************************************************************************************************** 
* 函数名称:	TaskPWM()
* 功能说明: 等待ADC转换结果，根据ADC转换结果设置PWM占空比
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
__task void TaskPWM(void)
{
	uint32_t duty;
	PWM_InitStructure  PWM_initStruct;
	
	PWM_initStruct.PWMnXN = 1;					//只输出PWM0A信号、不输出PWM0AN信号
	PWM_initStruct.clkdiv = PWM_CLKDIV_8;		//F_PWM = 24M/8 = 3M	
	PWM_initStruct.cycle = 10000;				//3M/10000 = 300Hz，PWMnXN = 1时频率降低到150Hz
	PWM_initStruct.hduty =  2500;				//2500/10000 = 25%
	PWM_initStruct.deadzone = 00;
	PWM_initStruct.initLevel = 1;
	PWM_initStruct.HEndIE = 0;
	PWM_initStruct.NCycleIE = 0;
	PWM_initStruct.HCycleIE = 0;	
	PWM_Init(PWM0A, &PWM_initStruct);
	PWM_Init(PWM0B, &PWM_initStruct);
	
// 	PORT_Init(PORTA, PIN11, PORTA_PIN11_PWM0A,  0);		//ADC输入引脚
	PORT_Init(PORTC, PIN10, PORTC_PIN10_PWM0AN, 0);
// 	PORT_Init(PORTA, PIN12, PORTA_PIN12_PWM0B,  0);		//ADC输入引脚
	PORT_Init(PORTC, PIN11, PORTC_PIN11_PWM0BN, 0);
	
	PWM_Start(PWM0A);
	PWM_Start(PWM0B);
	
	while(1)
	{
		if(os_mbx_wait(queueADC, (void *)&duty, 10) != OS_R_TMO)
		{
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
