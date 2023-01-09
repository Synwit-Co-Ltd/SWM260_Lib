#include "SWM260.h"

#include <string.h>

#define SLV_ADDR  0x6C

char slv_rxbuff[5] = {0};	//��һ�������ǵ�ַ
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
	
	PORT_Init(PORTB, PIN11, PORTB_PIN11_I2C4_SCL, 1);	//GPIOB.11����ΪI2C4 SCL����
	PORTB->PULLU |= (1 << PIN11);						//����ʹ������������ģ�⿪©
	PORT_Init(PORTB, PIN10, PORTB_PIN10_I2C4_SDA, 1);	//GPIOB.10����ΪI2C4 SDA����
	PORTB->PULLU |= (1 << PIN10);						//����ʹ������������ģ�⿪©
	
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
	if(I2C4->SLVIF & I2C_SLVIF_STADET_Msk)				//�յ���ʼλ
	{
		I2C4->SLVIF = (1 << I2C_SLVIF_STADET_Pos);
		
		slv_rxindx = 0;
		slv_txindx = 0;
		
		I2C4->SLVTX = slv_txbuff[slv_txindx++];
	}
	else if(I2C4->SLVIF & I2C_SLVIF_STODET_Msk)			//�յ�ֹͣλ
	{
		I2C4->SLVIF = (1 << I2C_SLVIF_STODET_Pos);
		
		memcpy(slv_txbuff, &slv_rxbuff[1], 4);
	}
	else if(I2C4->SLVIF & I2C_SLVIF_WRREQ_Msk)			//�յ�д����
	{
		I2C4->SLVIF = (1 << I2C_SLVIF_WRREQ_Pos);
		
		I2C4->SLVCR |= (1 << I2C_SLVCR_ACK_Pos);
	}
	else if(I2C4->SLVIF & I2C_SLVIF_RXEND_Msk)			//�������
	{
		I2C4->SLVIF = (1 << I2C_SLVIF_RXEND_Pos);
		
		if(slv_rxindx < sizeof(slv_rxbuff)) slv_rxbuff[slv_rxindx++] = I2C4->SLVRX;
	}
	else if(I2C4->SLVIF & I2C_SLVIF_RDREQ_Msk)			//�յ�������
	{		
		I2C4->SLVIF = (1 << I2C_SLVIF_RDREQ_Pos);
		
		//�յ���������������SLVTX�е�ֵ���ͳ�ȥ�������ǵȴ����д����ٷ��ͣ����Խ���һ������д��SLVTXֻ����ǰ�����յ���ʼλ��
	}
	else if(I2C4->SLVIF & I2C_SLVIF_TXEND_Msk)			//�������
	{
		I2C4->SLVIF = (1 << I2C_SLVIF_TXEND_Pos);
		
		if(slv_txindx < sizeof(slv_txbuff)) I2C4->SLVTX = slv_txbuff[slv_txindx++];
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
