2018/12/26
CSL\CMSIS\DeviceSupport\SWM260.h文件中 BTIRM0_IRQn 改正为 BTIMR0_IRQn

2018/12/28
CSL\SWM260_StdPeriph_Driver\SWM260_uart.c文件中 UART_IsTXBusy() 函数代码由
return (UARTx->CTRL & UART_CTRL_TXIDLE_Msk) ? 0 : 1;
改正为：
if(UARTx->FIFO & UART_FIFO_TXLVL_Msk)
{
	return 1;
}
else
{
	if(((UARTx->BAUD & UART_BAUD_TXD_Msk) == 0) || ((UARTx->CTRL & UART_CTRL_TXIDLE_Msk) == 0))
		return 1;
	else
		return 0;
}

2019/02/13
CSL\SWM260_StdPeriph_Driver\SWM260_exti.c 文件中 EXTI_RawState() 中，将
return (GPIOx->INTRAWSTAT >> 1) & 0x01;
修正为：
return (GPIOx->INTRAWSTAT >> n) & 0x01;

2019/02/13
CSL\SWM260_StdPeriph_Driver\SWM260_gpio.c 文件中，将
bits = 0xFFFFFF >> (24 - w);
修正为：
bits = 0xFFFF >> (16 - w);

2019/02/14
CSL\SWM260_StdPeriph_Driver\SWM260_uart.c文件中，UART_SetBaudrate()修正UART4波特率设置

2019/03/04
CSL\SWM260_StdPeriph_Driver\SWM260_i2c.c 文件中，I2C_Init()中将：
if(initStruct->MstIEn)  I2Cx->MSTCR &= ~(1 << I2C_MSTCR_IE_Pos);
else                    I2Cx->MSTCR |=  (1 << I2C_MSTCR_IE_Pos);
修正为：
if(initStruct->MstIEn)  I2Cx->MSTCR |=  (1 << I2C_MSTCR_IE_Pos);
else                    I2Cx->MSTCR &= ~(1 << I2C_MSTCR_IE_Pos);

CSL\SWM260_StdPeriph_Driver\SWM260_timr.c 文件中，TIMR_INTClr()中将：
TIMRx->IF |= (1 << TIMR_IF_TO_Pos);
修正为：
TIMRx->IF = (1 << TIMR_IF_TO_Pos);

CSL\SWM260_StdPeriph_Driver\SWM260_uart.c 文件中，UART_GetBaudrate()中将：
return (UARTx->BAUD & UART_BAUD_BAUD_Msk);
修正为：
if((UARTx == UART0) || (UARTx == UART1) || (UARTx == UART2) || (UARTx == UART3))
	return SystemCoreClock/16/(((UARTx->BAUD & UART_BAUD_BAUD_Msk) >> UART_BAUD_BAUD_Pos) + 1);
else
	return (SystemCoreClock/2)/16/(((UARTx->BAUD & UART_BAUD_BAUD_Msk) >> UART_BAUD_BAUD_Pos) + 1);

2019/03/08
CSL\CMSIS\DeviceSupport\SWM260.h 文件中，CLKSEL寄存器的bit5名称由LPR改为RTC

CSL\SWM260_StdPeriph_Driver\SWM260_rtc.c 文件中，RTC_Init()中添加时钟源选择

2019/05/15
CSL\SWM260_StdPeriph_Driver\SWM260_rtc.c 文件中，修正：Date取值1--31，Month取值1--12

2019/05/20
CSL\CMSIS\DeviceSupport\SWM260.h 文件中 PWMG->FORCEH 寄存器定义更正

2019/06/04
CSL\CMSIS\DeviceSupport\SWM260.h 文件中 SYS->BODCR.LVL 位拆分成 RSTLVL和INTLVL

2019/07/31
CSL\SWM260_StdPeriph_Driver\SWM260_uart.c 文件中 UART_LINIsDetected() 函数中，将：
return (UARTx->LINCR & UART_LINCR_BRKDETIE_Msk) ? 1 : 0;
修正为：
return (UARTx->LINCR & UART_LINCR_BRKDETIF_Msk) ? 1 : 0;

2019/08/21
CSL\SWM260_StdPeriph_Driver\SWM260_flash.c 文件中 FLASH_Write() 函数中，将：
if(addr/2048 != (addr+cnt*4)/2048) return FLASH_RES_ERR;	// 跨页
修正为：
if(addr/2048 != (addr+(cnt-1)*4)/2048) return FLASH_RES_ERR;	// 跨页
解决写入整页时误判跨页导致不执行写操作

ADC开启硬件校准

2019/10/18
CSL\CMSIS\DeviceSupport\SWM260.h 文件中，ADC_TypeDef 定义中将 TRIG 重命名为 TRGMSK

2019/11/25
CSL\CMSIS\DeviceSupport\SWM260.h 文件中，SYS_TypeDef 定义中添加 PRSTEN、PRSTR1、PRSTR2三个寄存器定义

2020/01/15
CSL\SWM260_StdPeriph_Driver\SWM260_adc.c 文件中 ADC_Init() 函数中，将：
ADCx->CTRL3 &= ~(ADC_CTRL3_EXTREF_Msk | ADC_CTRL3_CLKDIV_Msk);
ADCx->CTRL3 |= (ADC_REFP_EXTERNAL << ADC_CTRL3_EXTREF_Pos) | (2 << ADC_CTRL3_CLKDIV_Pos);
修正为：
ADCx->CTRL3 &= ~(ADC_CTRL3_EXTREF_Msk | ADC_CTRL3_CLKDIV1_Msk | ADC_CTRL3_CLKDIV2_Msk);
ADCx->CTRL3 |= (ADC_REFP_EXTERNAL   << ADC_CTRL3_EXTREF_Pos)  |
	           (1                   << ADC_CTRL3_CLKDIV1_Pos) |
			   (initStruct->clk_div << ADC_CTRL3_CLKDIV2_Pos);

2020/03/03
CSL\SWM260_StdPeriph_Driver\SWM260_i2c.c 文件中，添加 I2C_Start、I2C_Stop、I2C_Write、I2C_Read 函数定义

2020/05/12
CSL\SWM260_StdPeriph_Driver\SWM260_flash.c 文件中，FLASH_Erase、FLASH_Write 中执行操作前关闭全局中断

2020/08/19
CSL\SWM260_StdPeriph_Driver\SWM260_wdt.c 文件中，WDT_Init()中将：
if(rst_period == 0)
{
	WDTx->CR &= ~(1 << WDT_CR_RSTEN_Pos);
}
修正为：
if(rst_period == 0)
{
	WDTx->CR &= ~(1 << WDT_CR_RSTEN_Pos);
	
	WDTx->RSTVAL = int_period;
}

2020/12/25
CSL\CMSIS\DeviceSupport\SWM260.h 文件中，优化 PORT_TypeDef 定义，从而简化 PORT_Init()、GPIO_Init() 函数定义

CSL\SWM260_StdPeriph_Driver\SWM260_timr.c 文件中，优化 TIMR_Init() 定义，将预分频从period中分离出来，提高易用性

2021/5/21
CSL\CMSIS\DeviceSupport\SWM260.h 文件中，SYS_TypeDef 定义中，将PRSTR1、PRSTR2重命名为PRSTR0、PRSTR1

2021/6/17
CSL\SWM260_StdPeriph_Driver\SWM260_timr.c 文件中，IM固定为0、IE开关中断，改为：IE固定为0x1F、IM开关中断
因为IE为0时IF无法置位，因此无法使用轮询方式查询中断标志

注意：BTIMR没有IM寄存器，因此只能通过NVIC_EnableIRQ()、NVIC_DisableIRQ开关BTIMR中断。
BTIMR2的中断向量BTIMR2_GPIOB12_Handler与GPIOB12共用，因此开关GPIOB12中断时会影响到BTIMR2中断。
BTIMR3的中断向量BTIMR3_RTC_Handler与RTC共用，因此开关RTC中断时会影响到BTIMR3中断。

2021/08/10
CSL\SWM260_StdPeriph_Driver\SWM260_rtc.c 文件中
RTC_Init() 函数中
RTCx->YEAR = initStruct->Year - 1901;
修正为：
RTCx->YEAR = initStruct->Year;

RTC_GetDateTime() 函数中
dateTime->Year = RTCx->YEAR + 1901;
修正为：
dateTime->Year = RTCx->YEAR;

2022/08/16
CSL\CMSIS\DeviceSupport\SWM260.h 文件中，SPI_TypeDef 定义中，CTRL 寄存器添加 NSYNC 位定义

CSL\SWM260_StdPeriph_Driver\SWM260_spi.c 文件中，SPI_Init() 函数中，修正 4 分频数据传输错误问题

2022/10/20
CSL\SWM260_StdPeriph_Driver\SWM260_uart.c 文件中，将：
void UART_INTRXThresholdEn(UART_TypeDef * UARTx);
void UART_INTRXThresholdDis(UART_TypeDef * UARTx);
uint32_t UART_INTRXThresholdStat(UART_TypeDef * UARTx);
void UART_INTTXThresholdEn(UART_TypeDef * UARTx);
void UART_INTTXThresholdDis(UART_TypeDef * UARTx);
uint32_t UART_INTTXThresholdStat(UART_TypeDef * UARTx);
void UART_INTTimeoutEn(UART_TypeDef * UARTx);
void UART_INTTimeoutDis(UART_TypeDef * UARTx);
uint32_t UART_INTTimeoutStat(UART_TypeDef * UARTx);
void UART_INTTXDoneEn(UART_TypeDef * UARTx);
void UART_INTTXDoneDis(UART_TypeDef * UARTx);
uint32_t UART_INTTXDoneStat(UART_TypeDef * UARTx);
简化为：
void UART_INTEn(UART_TypeDef * UARTx, uint32_t it);
void UART_INTDis(UART_TypeDef * UARTx, uint32_t it);
uint32_t UART_INTStat(UART_TypeDef * UARTx, uint32_t it);

修改 CSL\SWM260_StdPeriph_Driver\SWM260_dma.c 和 SWM260_dma.h 文件，使用 Scatter-Gather 模式实现 ping-pong 功能

2022/10/21
CSL\SWM260_StdPeriph_Driver\SWM260_spi.c 文件中，将：
void SPI_INTRXHalfFullEn(SPI_TypeDef * SPIx);
void SPI_INTRXHalfFullDis(SPI_TypeDef * SPIx);
void SPI_INTRXHalfFullClr(SPI_TypeDef * SPIx);
uint32_t SPI_INTRXHalfFullStat(SPI_TypeDef * SPIx);
void SPI_INTRXFullEn(SPI_TypeDef * SPIx);
void SPI_INTRXFullDis(SPI_TypeDef * SPIx);
void SPI_INTRXFullClr(SPI_TypeDef * SPIx);
uint32_t SPI_INTRXFullStat(SPI_TypeDef * SPIx);
void SPI_INTRXOverflowEn(SPI_TypeDef * SPIx);
void SPI_INTRXOverflowDis(SPI_TypeDef * SPIx);
void SPI_INTRXOverflowClr(SPI_TypeDef * SPIx);
uint32_t SPI_INTRXOverflowStat(SPI_TypeDef * SPIx);
void SPI_INTTXEmptyEn(SPI_TypeDef * SPIx);
void SPI_INTTXEmptyDis(SPI_TypeDef * SPIx);
void SPI_INTTXEmptyClr(SPI_TypeDef * SPIx);
uint32_t SPI_INTTXEmptyStat(SPI_TypeDef * SPIx);
void SPI_INTTXCompleteEn(SPI_TypeDef * SPIx);
void SPI_INTTXCompleteDis(SPI_TypeDef * SPIx);
void SPI_INTTXCompleteClr(SPI_TypeDef * SPIx);
uint32_t SPI_INTTXCompleteStat(SPI_TypeDef * SPIx);
void SPI_INTRXThresholdEn(SPI_TypeDef * SPIx);
void SPI_INTRXThresholdDis(SPI_TypeDef * SPIx);
void SPI_INTRXThresholdClr(SPI_TypeDef * SPIx);
uint32_t SPI_INTRXThresholdStat(SPI_TypeDef * SPIx);
void SPI_INTTXThresholdEn(SPI_TypeDef * SPIx);
void SPI_INTTXThresholdDis(SPI_TypeDef * SPIx);
void SPI_INTTXThresholdClr(SPI_TypeDef * SPIx);
uint32_t SPI_INTTXThresholdStat(SPI_TypeDef * SPIx);
简化为：
void SPI_INTEn(SPI_TypeDef * SPIx, uint32_t it);
void SPI_INTDis(SPI_TypeDef * SPIx, uint32_t it);
void SPI_INTClr(SPI_TypeDef * SPIx, uint32_t it);
uint32_t SPI_INTStat(SPI_TypeDef * SPIx, uint32_t it);
