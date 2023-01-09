#ifndef __SWM260_DMA_H__
#define __SWM260_DMA_H__


typedef struct {
	uint8_t  Mode;			//DMA_MODE_SINGLE、DMA_MODE_CIRCLE
	
	uint8_t  Unit;			//DMA_UNIT_BYTE、DMA_UNIT_HALFWORD、DMA_UNIT_WORD
	
	uint16_t Count;			//传输 Unit 个数，最大取值4096
	
	uint32_t SrcAddr;
	
	uint8_t  SrcAddrInc;	//0 地址固定    1 地址递增
	
	uint32_t DstAddr;
	
	uint8_t  DstAddrInc;
	
	uint8_t  Trigger;		//软件启动：DMA_TRG_SW    硬件触发：DMA_CH0_UART0TX、DMA_CH0_SPI0TX、... ...
	
	uint8_t  Priority;		//DMA_PRI_LOW、DMA_PRI_HIGH
	
	uint32_t  INTEn;		//中断使能，有效值有 DMA_IT_DONE、DMA_IT_DSTSG_HALF、DMA_IT_DSTSG_DONE、DMA_IT_SRCSG_HALF、DMA_IT_SRCSG_DONE 及其“或”
} DMA_InitStructure;


#define DMA_CH0		0
#define DMA_CH1		1
#define DMA_CH2		2
#define DMA_CH3		3

#define DMA_MODE_SINGLE		0		//单次模式，传输完成后停止
#define DMA_MODE_CIRCLE		1		//环形模式，传输完成后从头执行下一轮传输

#define DMA_UNIT_BYTE		0
#define DMA_UNIT_HALFWORD	1
#define DMA_UNIT_WORD		2

#define DMA_PRI_LOW			0
#define DMA_PRI_HIGH		1


#define DMA_TRG_M0			(0 << 4)
#define DMA_TRG_M1			(1 << 4)
#define DMA_TRG_SW			(2 << 4)
#define DMA_TRG_MSK			(3 << 4)

#define DMA_DIR_TX			(0 << 7)	// M0 --> M1
#define DMA_DIR_RX			(1 << 7)	// M1 --> M0
#define DMA_DIR_MSK			(1 << 7)

// M0 上外设
#define DMA_CH0_UART0TX		(0 | DMA_TRG_M0 | DMA_DIR_RX)
#define DMA_CH0_SPI0TX		(1 | DMA_TRG_M0 | DMA_DIR_RX)
#define DMA_CH0_UART3TX		(2 | DMA_TRG_M0 | DMA_DIR_RX)
#define DMA_CH0_UART4TX		(3 | DMA_TRG_M0 | DMA_DIR_RX)
#define DMA_CH1_UART1TX		(0 | DMA_TRG_M0 | DMA_DIR_RX)
#define DMA_CH1_SPI1TX		(1 | DMA_TRG_M0 | DMA_DIR_RX)
#define DMA_CH1_UART2TX		(2 | DMA_TRG_M0 | DMA_DIR_RX)
#define DMA_CH2_UART2TX		(0 | DMA_TRG_M0 | DMA_DIR_RX)
#define DMA_CH2_SPI0TX		(1 | DMA_TRG_M0 | DMA_DIR_RX)
#define DMA_CH2_UART1TX		(2 | DMA_TRG_M0 | DMA_DIR_RX)
#define DMA_CH2_UART4TX		(3 | DMA_TRG_M0 | DMA_DIR_RX)
#define DMA_CH3_UART3TX		(0 | DMA_TRG_M0 | DMA_DIR_RX)
#define DMA_CH3_SPI1TX		(1 | DMA_TRG_M0 | DMA_DIR_RX)
#define DMA_CH3_UART0TX		(2 | DMA_TRG_M0 | DMA_DIR_RX)

// M1 上外设
#define DMA_CH0_UART1RX		(0 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH0_SPI1RX		(1 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH0_ADC0		(2 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH0_UART2RX		(3 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH1_UART0RX		(0 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH1_SPI0RX		(1 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH1_UART4RX		(2 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH1_UART3RX		(3 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH2_UART3RX		(0 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH2_SPI1RX		(1 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH2_ADC0		(2 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH2_UART0RX		(3 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH3_UART2RX		(0 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH3_SPI0RX		(1 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH3_UART4		(2 | DMA_TRG_M1 | DMA_DIR_RX)
#define DMA_CH3_UART1RX		(3 | DMA_TRG_M1 | DMA_DIR_RX)


/* Interrupt Type */
#define DMA_IT_DONE			(1 <<  0)	//Transfer Done
#define DMA_IT_DSTSG_HALF	(1 <<  8)	//Destination Scatter-Gather Transfer Half
#define DMA_IT_DSTSG_DONE	(1 <<  9)	//Destination Scatter-Gather Transfer Done
#define DMA_IT_SRCSG_HALF	(1 << 16)	//Source      Scatter-Gather Transfer Half
#define DMA_IT_SRCSG_DONE	(1 << 17)	//Source      Scatter-Gather Transfer Done



void DMA_CH_Init(uint32_t chn, DMA_InitStructure * initStruct);	//DMA通道配置
void DMA_CH_Open(uint32_t chn);
void DMA_CH_Close(uint32_t chn);

void DMA_CH_SetCount(uint32_t chn, uint32_t count);
void DMA_CH_SetSrcAddress(uint32_t chn, uint32_t address);
void DMA_CH_SetDstAddress(uint32_t chn, uint32_t address);
uint32_t DMA_CH_GetRemaining(uint32_t chn);

void DMA_CH_INTEn(uint32_t chn, uint32_t it);					//DMA中断使能
void DMA_CH_INTDis(uint32_t chn, uint32_t it);					//DMA中断禁止
void DMA_CH_INTClr(uint32_t chn, uint32_t it);					//DMA中断标志清除
uint32_t DMA_CH_INTStat(uint32_t chn, uint32_t it);				//DMA中断状态查询


#endif //__SWM260_DMA_H__
