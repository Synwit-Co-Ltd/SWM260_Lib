#include "SWM260.h"

#include <string.h>

#define SLV_ADDR  0x6C

char slv_rxbuff[5] = {0};	//第一个数据是地址
char slv_txbuff[4] = {0};

volatile uint32_t slv_rxindx = 0;
volatile uint32_t slv_txindx = 0;

void SerialInit(void);
void I2C_Slv_Init(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	I2C_Slv_Init();
	
	while(1==1)
	{
	}
}


void I2C_Slv_Init(void)
{
	I2C_InitStructure I2C_initStruct;
	
	PORT_Init(PORTB, PIN11, PORTB_PIN11_I2C4_SCL, 1);	//GPIOB.11配置为I2C4 SCL引脚
	PORTB->PULLU |= (1 << PIN11);						//必须使能上拉，用于模拟开漏
	PORT_Init(PORTB, PIN10, PORTB_PIN10_I2C4_SDA, 1);	//GPIOB.10配置为I2C4 SDA引脚
	PORTB->PULLU |= (1 << PIN10);						//必须使能上拉，用于模拟开漏
	
	I2C_initStruct.Master = 0;
	I2C_initStruct.Addr7b = 1;
	I2C_initStruct.SlvAddr = SLV_ADDR;
	I2C_initStruct.SlvAddrMask = 0xFF;
	I2C_initStruct.SlvSTADetIEn = 1;
	I2C_initStruct.SlvRdReqIEn = 0;
	I2C_initStruct.SlvWrReqIEn = 1;
	I2C_initStruct.SlvTxEndIEn = 1;
	I2C_initStruct.SlvRxEndIEn = 1;
	I2C_initStruct.SlvSTODetIEn = 1;
	I2C_Init(I2C4, &I2C_initStruct);
	
	I2C_Open(I2C4);
}


void I2C4_Handler(void)
{
	if(I2C4->SLVIF & I2C_SLVIF_STADET_Msk)				//收到起始位
	{
		I2C4->SLVIF = (1 << I2C_SLVIF_STADET_Pos);
		
		slv_rxindx = 0;
		slv_txindx = 0;
		
		I2C4->SLVTX = slv_txbuff[slv_txindx++];
	}
	else if(I2C4->SLVIF & I2C_SLVIF_STODET_Msk)			//收到停止位
	{
		I2C4->SLVIF = (1 << I2C_SLVIF_STODET_Pos);
		
		memcpy(slv_txbuff, &slv_rxbuff[1], 4);
	}
	else if(I2C4->SLVIF & I2C_SLVIF_WRREQ_Msk)			//收到写请求
	{
		I2C4->SLVIF = (1 << I2C_SLVIF_WRREQ_Pos);
		
		I2C4->SLVCR |= (1 << I2C_SLVCR_ACK_Pos);
	}
	else if(I2C4->SLVIF & I2C_SLVIF_RXEND_Msk)			//接收完成
	{
		I2C4->SLVIF = (1 << I2C_SLVIF_RXEND_Pos);
		
		if(slv_rxindx < sizeof(slv_rxbuff)) slv_rxbuff[slv_rxindx++] = I2C4->SLVRX;
	}
	else if(I2C4->SLVIF & I2C_SLVIF_RDREQ_Msk)			//收到读请求
	{		
		I2C4->SLVIF = (1 << I2C_SLVIF_RDREQ_Pos);
		
		//收到读请求后会立即把SLVTX中的值发送出去，而不是等待软件写入后再发送；所以将第一个数据写入SLVTX只能提前到“收到起始位”
	}
	else if(I2C4->SLVIF & I2C_SLVIF_TXEND_Msk)			//发送完成
	{
		I2C4->SLVIF = (1 << I2C_SLVIF_TXEND_Pos);
		
		if(slv_txindx < sizeof(slv_txbuff)) I2C4->SLVTX = slv_txbuff[slv_txindx++];
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
