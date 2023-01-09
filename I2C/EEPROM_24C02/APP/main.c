#include "SWM260.h"

#define SLV_ADDR  0x50

#define MEM_ADDR  0x10

char txbuff[4] = {0x37, 0x55, 0xAA, 0x78};
char rxbuff[4] = {0};

void SerialInit(void);
void I2CMstInit(void);

int main(void)
{
	uint32_t i;
	uint8_t ack;
	
	SystemInit();
	
	SerialInit();
	
	I2CMstInit();
	
	while(1==1)
	{
		/*************** EEPROM Write ***************/		
		ack = I2C_Start(I2C4, (SLV_ADDR << 1) | 0);
		if(ack == 0)
		{
			printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		
		ack = I2C_Write(I2C4, MEM_ADDR);
		if(ack == 0)
		{
			printf("Slave send NACK for memory address\r\n");
			goto nextloop;
		}
		
		for(i = 0; i < 4; i++)
		{
			ack = I2C_Write(I2C4, txbuff[i]);
			if(ack == 0)
			{
				printf("Slave send NACK for data\r\n");
				goto nextloop;
			}
		}
		
		I2C_Stop(I2C4);
		
		printf("Master Write %X %X %X %X @ %X\r\n", txbuff[0], txbuff[1], txbuff[2], txbuff[3], MEM_ADDR);
		
		
		for(i = 0; i < 1000000; i++) __NOP();	// 延时等待内部写入操作完成
		
		
		/*************** EEPROM Read ***************/
		ack = I2C_Start(I2C4, (SLV_ADDR << 1) | 0);
		if(ack == 0)
		{
			printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		
		ack = I2C_Write(I2C4, MEM_ADDR);
		if(ack == 0)
		{
			printf("Slave send NACK for memory address\r\n");
			goto nextloop;
		}
		
		ack = I2C_Start(I2C4, (SLV_ADDR << 1) | 1);
		if(ack == 0)
		{
			printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		
		for(i = 0; i < 3; i++)
		{
			rxbuff[i] = I2C_Read(I2C4, 1);
		}
		rxbuff[i] = I2C_Read(I2C4, 0);
		
		printf("Master Read %X %X %X %X @ %X\r\n", rxbuff[0], rxbuff[1], rxbuff[2], rxbuff[3], MEM_ADDR);
		
		if((txbuff[0] == rxbuff[0]) && (txbuff[1] == rxbuff[1]) && (txbuff[2] == rxbuff[2]) && (txbuff[3] == rxbuff[3]))
			printf("Success\r\n");
		else
			printf("Fail\r\n");

nextloop:
		I2C_Stop(I2C4);
		for(i = 0; i < 10000000; i++) ;
	}
}


void I2CMstInit(void)
{
	I2C_InitStructure I2C_initStruct;
	
	PORT_Init(PORTB, PIN11, PORTB_PIN11_I2C4_SCL, 1);	//GPIOB.11配置为I2C4 SCL引脚
	PORTB->PULLU |= (1 << PIN11);						//必须使能上拉，用于模拟开漏
	PORT_Init(PORTB, PIN10, PORTB_PIN10_I2C4_SDA, 1);	//GPIOB.10配置为I2C4 SDA引脚
	PORTB->PULLU |= (1 << PIN10);						//必须使能上拉，用于模拟开漏
	
	I2C_initStruct.Master = 1;
	I2C_initStruct.Addr7b = 1;
	I2C_initStruct.MstClk = 10000;
	I2C_initStruct.MstIEn = 0;
	I2C_Init(I2C4, &I2C_initStruct);
	
	I2C_Open(I2C4);
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
