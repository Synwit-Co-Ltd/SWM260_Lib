#include "SWM260.h"


/* 注意：SWM260_Flash.c中的函数必须在RAM中执行，Keil下实现方法有：
   方法一、Scatter file
   方法二、SWM260_Flash.c上右键 =》Options for File "SWM260_Flash.c" =》Properties =》Memory Assignment =》Code/Conts 选择 IRAM1
*/


#define EEPROM_ADDR	  0x10000
#define EEPROM_Ptr	  ((volatile uint32_t *) EEPROM_ADDR)

uint32_t WrBuff[20] = {0x14141414, 0x15151515, 0x16161616, 0x17171717, 0x18181818,
				       0x19191919, 0x1A1A1A1A, 0x1B1B1B1B, 0x1C1C1C1C, 0x1D1D1D1D,
				       0x1E1E1E1E, 0x1F1F1F1F, 0x20202020, 0x21212121, 0x22222222,
				       0x23232323, 0x24242424, 0x25252525, 0x26262626, 0x27272727};


void SerialInit(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
		
	printf("\r\nAfter Erase: \r\n");
	
	FLASH_Erase(EEPROM_ADDR);
	for(i = 0; i < 20; i++) printf("0x%08X, ", EEPROM_Ptr[i]);
	
	printf("\r\nAfter Write: \r\n");
   	
	FLASH_Write(EEPROM_ADDR, WrBuff, 20);
	for(i = 0; i < 20; i++) printf("0x%08X, ", EEPROM_Ptr[i]);
   	
	while(1==1)
	{
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
