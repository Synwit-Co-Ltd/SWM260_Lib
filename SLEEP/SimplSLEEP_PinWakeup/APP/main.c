#include "SWM260.h"


/* 注意：
 *	芯片的 ISP、SWD 引脚默认开启了上拉电阻，会增加休眠功耗，若想获得最低休眠功耗，休眠前请关闭所有引脚的上拉和下拉电阻
 */
 

int main(void)
{
	uint32_t i;
	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);				//接 LED，指示程序执行状态
	GPIO_SetBit(GPIOA, PIN5);						//点亮LED
	for(i = 0; i < SystemCoreClock/2; i++);			//延时，防止上电后SWD立即切掉无法下载程序
	
	GPIO_Init(GPIOA, PIN9, 0, 1, 0, 0);				//接按键，上拉使能
	SYS->PAWKEN |= (1 << PIN9);						//开启PA9引脚低电平唤醒
	
	SYS->LRCCR |= 0x01;								//休眠模式使用低频时钟
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);					//点亮LED
		for(i = 0; i < SystemCoreClock/8; i++);
		GPIO_ClrBit(GPIOA, PIN5);					//熄灭LED
		
		EnterSleepMode();
		
		while((SYS->PAWKSR & (1 << PIN9)) == 0);	//等待唤醒条件
		SYS->PAWKSR |= (1 << PIN9);					//清除唤醒状态
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
