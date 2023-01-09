/****************************************************************************************************************************************** 
* 文件名称:	SWM260_pwm.c
* 功能说明:	SWM260单片机的PWM功能驱动库
* 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* 注意事项:
* 版本日期:	V1.0.0		2016年1月30日
* 升级记录: 
*
*
*******************************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology 
*******************************************************************************************************************************************/
#include "SWM260.h"
#include "SWM260_pwm.h"


PWM_TypeDef * PWMX[8] = {PWM0, PWM0, PWM1, PWM1, PWM2, PWM2, PWM3, PWM3};


/****************************************************************************************************************************************** 
* 函数名称:	PWM_Init()
* 功能说明:	PWM初始化
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
*			PWM_InitStructure * initStruct	包含PWM相关设定值的结构体
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_Init(uint32_t pwmx, PWM_InitStructure * initStruct)
{
	PWM_TypeDef * PWMx = PWMX[pwmx];
	
	SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_PWM_Pos);
	
	PWMG->CLKDIV = initStruct->clkdiv;
	
	PWM_Stop(pwmx);		//一些关键寄存器只能在PWM停止时设置
	
	PWMx->MODE = initStruct->PWMnXN ? 5 : 0;
	
	if((pwmx%2) == 0)
	{
		PWMx->PERA = initStruct->cycle;
		PWMx->HIGHA = initStruct->hduty << 1;
		PWMx->DZA = initStruct->deadzone;
		
		if(initStruct->initLevel) PWMx->OUTCR |=  (1 << PWM_OUTCR_INIA_Pos);
		else                      PWMx->OUTCR &= ~(1 << PWM_OUTCR_INIA_Pos);
		
		PWMG->IM = 0x00000000;
		PWMG->HCIM = 0x000000;
		switch((uint32_t)PWMx)
		{
		case((uint32_t)PWM0):
			PWMG->NCIRS = (1 << PWMG_NCIRS_PWM0A_Pos);
			PWMG->HEIRS = (1 << PWMG_HEIRS_PWM0A_Pos);
			if(initStruct->NCycleIE) PWMG->IE |=  (1 << PWMG_IE_NC0A_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_NC0A_Pos);
			if(initStruct->HEndIE)   PWMG->IE |=  (1 << PWMG_IE_HE0A_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_HE0A_Pos);
			if(initStruct->HCycleIE) PWMG->HCIE |=  (1 << PWMG_HCIE_PWM0A_Pos);
			else                     PWMG->HCIE &= ~(1 << PWMG_HCIE_PWM0A_Pos);
			if(initStruct->HEndIE | initStruct->NCycleIE | initStruct->HCycleIE) NVIC_EnableIRQ(PWM0_IRQn);
			break;

		case((uint32_t)PWM1):
			PWMG->NCIRS = (1 << PWMG_NCIRS_PWM1A_Pos);
			PWMG->HEIRS = (1 << PWMG_HEIRS_PWM1A_Pos);
			if(initStruct->NCycleIE) PWMG->IE |=  (1 << PWMG_IE_NC1A_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_NC1A_Pos);
			if(initStruct->HEndIE)   PWMG->IE |=  (1 << PWMG_IE_HE1A_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_HE1A_Pos);
			if(initStruct->HCycleIE) PWMG->HCIE |=  (1 << PWMG_HCIE_PWM1A_Pos);
			else                     PWMG->HCIE &= ~(1 << PWMG_HCIE_PWM1A_Pos);
			if(initStruct->HEndIE | initStruct->NCycleIE | initStruct->HCycleIE) NVIC_EnableIRQ(PWM1_GPIOA1_IRQn);
			break;

		case((uint32_t)PWM2):
			PWMG->NCIRS = (1 << PWMG_NCIRS_PWM2A_Pos);
			PWMG->HEIRS = (1 << PWMG_HEIRS_PWM2A_Pos);
			if(initStruct->NCycleIE) PWMG->IE |=  (1 << PWMG_IE_NC2A_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_NC2A_Pos);
			if(initStruct->HEndIE)   PWMG->IE |=  (1 << PWMG_IE_HE2A_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_HE2A_Pos);
			if(initStruct->HCycleIE) PWMG->HCIE |=  (1 << PWMG_HCIE_PWM2A_Pos);
			else                     PWMG->HCIE &= ~(1 << PWMG_HCIE_PWM2A_Pos);
			if(initStruct->HEndIE | initStruct->NCycleIE | initStruct->HCycleIE) NVIC_EnableIRQ(PWM2_UART4_IRQn);
			break;
		
		case((uint32_t)PWM3):
			PWMG->NCIRS = (1 << PWMG_NCIRS_PWM3A_Pos);
			PWMG->HEIRS = (1 << PWMG_HEIRS_PWM3A_Pos);
			if(initStruct->NCycleIE) PWMG->IE |=  (1 << PWMG_IE_NC3A_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_NC3A_Pos);
			if(initStruct->HEndIE)   PWMG->IE |=  (1 << PWMG_IE_HE3A_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_HE3A_Pos);
			if(initStruct->HCycleIE) PWMG->HCIE |=  (1 << PWMG_HCIE_PWM3A_Pos);
			else                     PWMG->HCIE &= ~(1 << PWMG_HCIE_PWM3A_Pos);
			if(initStruct->HEndIE | initStruct->NCycleIE | initStruct->HCycleIE) NVIC_EnableIRQ(PWM3_GPIOB_ACMP_IRQn);
			break;
		}
	}
	else
	{
		PWMx->PERB = initStruct->cycle;
		PWMx->HIGHB = initStruct->hduty << 1;
		PWMx->DZB = initStruct->deadzone;
		
		if(initStruct->initLevel) PWMx->OUTCR |=  (1 << PWM_OUTCR_INIB_Pos);
		else                      PWMx->OUTCR &= ~(1 << PWM_OUTCR_INIB_Pos);
		
		PWMG->IM = 0x00000000;
		PWMG->HCIM = 0x000000;
		switch((uint32_t)PWMx)
		{
		case((uint32_t)PWM0):
			PWMG->NCIRS = (1 << PWMG_NCIRS_PWM0B_Pos);
			PWMG->HEIRS = (1 << PWMG_HEIRS_PWM0B_Pos);
			if(initStruct->NCycleIE) PWMG->IE |=  (1 << PWMG_IE_NC0B_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_NC0B_Pos);
			if(initStruct->HEndIE)   PWMG->IE |=  (1 << PWMG_IE_HE0B_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_HE0B_Pos);
			if(initStruct->HCycleIE) PWMG->HCIE |=  (1 << PWMG_HCIE_PWM0B_Pos);
			else                     PWMG->HCIE &= ~(1 << PWMG_HCIE_PWM0B_Pos);
			if(initStruct->HEndIE | initStruct->NCycleIE | initStruct->HCycleIE) NVIC_EnableIRQ(PWM0_IRQn);
			break;

		case((uint32_t)PWM1):
			PWMG->NCIRS = (1 << PWMG_NCIRS_PWM1B_Pos);
			PWMG->HEIRS = (1 << PWMG_HEIRS_PWM1B_Pos);
			if(initStruct->NCycleIE) PWMG->IE |=  (1 << PWMG_IE_NC1B_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_NC1B_Pos);
			if(initStruct->HEndIE)   PWMG->IE |=  (1 << PWMG_IE_HE1B_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_HE1B_Pos);
			if(initStruct->HCycleIE) PWMG->HCIE |=  (1 << PWMG_HCIE_PWM1B_Pos);
			else                     PWMG->HCIE &= ~(1 << PWMG_HCIE_PWM1B_Pos);
			if(initStruct->HEndIE | initStruct->NCycleIE | initStruct->HCycleIE) NVIC_EnableIRQ(PWM1_GPIOA1_IRQn);
			break;

		case((uint32_t)PWM2):
			PWMG->NCIRS = (1 << PWMG_NCIRS_PWM2B_Pos);
			PWMG->HEIRS = (1 << PWMG_HEIRS_PWM2B_Pos);
			if(initStruct->NCycleIE) PWMG->IE |=  (1 << PWMG_IE_NC2B_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_NC2B_Pos);
			if(initStruct->HEndIE)   PWMG->IE |=  (1 << PWMG_IE_HE2B_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_HE2B_Pos);
			if(initStruct->HCycleIE) PWMG->HCIE |=  (1 << PWMG_HCIE_PWM2B_Pos);
			else                     PWMG->HCIE &= ~(1 << PWMG_HCIE_PWM2B_Pos);
			if(initStruct->HEndIE | initStruct->NCycleIE | initStruct->HCycleIE) NVIC_EnableIRQ(PWM2_UART4_IRQn);
			break;
		
		case((uint32_t)PWM3):
			PWMG->NCIRS = (1 << PWMG_NCIRS_PWM3B_Pos);
			PWMG->HEIRS = (1 << PWMG_HEIRS_PWM3B_Pos);
			if(initStruct->NCycleIE) PWMG->IE |=  (1 << PWMG_IE_NC3B_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_NC3B_Pos);
			if(initStruct->HEndIE)   PWMG->IE |=  (1 << PWMG_IE_HE3B_Pos);
			else                     PWMG->IE &= ~(1 << PWMG_IE_HE3B_Pos);
			if(initStruct->HCycleIE) PWMG->HCIE |=  (1 << PWMG_HCIE_PWM3B_Pos);
			else                     PWMG->HCIE &= ~(1 << PWMG_HCIE_PWM3B_Pos);
			if(initStruct->HEndIE | initStruct->NCycleIE | initStruct->HCycleIE) NVIC_EnableIRQ(PWM3_GPIOB_ACMP_IRQn);
			break;
		}
	}
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_Start()
* 功能说明:	启动PWM，开始PWM输出
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_Start(uint32_t pwmx)
{
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) PWMG->CHEN |= (1 << PWMG_CHEN_PWM0A_Pos);
		else              PWMG->CHEN |= (1 << PWMG_CHEN_PWM0B_Pos);
		break;

	case((uint32_t)PWM1):
		if((pwmx%2) == 0) PWMG->CHEN |= (1 << PWMG_CHEN_PWM1A_Pos);
		else              PWMG->CHEN |= (1 << PWMG_CHEN_PWM1B_Pos);
		break;

	case((uint32_t)PWM2):
		if((pwmx%2) == 0) PWMG->CHEN |= (1 << PWMG_CHEN_PWM2A_Pos);
		else              PWMG->CHEN |= (1 << PWMG_CHEN_PWM2B_Pos);
		break;
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) PWMG->CHEN |= (1 << PWMG_CHEN_PWM3A_Pos);
		else              PWMG->CHEN |= (1 << PWMG_CHEN_PWM3B_Pos);
		break;
	}
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_Stop()
* 功能说明:	关闭PWM，停止PWM输出
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_Stop(uint32_t pwmx)
{
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) PWMG->CHEN &= ~(1 << PWMG_CHEN_PWM0A_Pos);
		else              PWMG->CHEN &= ~(1 << PWMG_CHEN_PWM0B_Pos);
		break;

	case((uint32_t)PWM1):
		if((pwmx%2) == 0) PWMG->CHEN &= ~(1 << PWMG_CHEN_PWM1A_Pos);
		else              PWMG->CHEN &= ~(1 << PWMG_CHEN_PWM1B_Pos);
		break;

	case((uint32_t)PWM2):
		if((pwmx%2) == 0) PWMG->CHEN &= ~(1 << PWMG_CHEN_PWM2A_Pos);
		else              PWMG->CHEN &= ~(1 << PWMG_CHEN_PWM2B_Pos);
		break;
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) PWMG->CHEN &= ~(1 << PWMG_CHEN_PWM3A_Pos);
		else              PWMG->CHEN &= ~(1 << PWMG_CHEN_PWM3B_Pos);
		break;
	} 
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_SetCycle()
* 功能说明:	设置周期
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
*			uint32_t cycle			要设定的周期值,28位
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_SetCycle(uint32_t pwmx, uint32_t cycle)
{
	PWM_TypeDef * PWMx = PWMX[pwmx];
	
	if((pwmx%2) == 0) PWMx->PERA = cycle;
	else              PWMx->PERB = cycle;
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_GetCycle()
* 功能说明:	获取周期
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: uint32_t				获取到的周期值
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t PWM_GetCycle(uint32_t pwmx)
{
	PWM_TypeDef * PWMx = PWMX[pwmx];
	
	if((pwmx%2) == 0) return PWMx->PERA;
	else              return PWMx->PERB;
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_SetHDuty()
* 功能说明:	设置高电平时长
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
*			uint32_t hduty			要设定的高电平时长,28位
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_SetHDuty(uint32_t pwmx, uint32_t hduty)
{
	PWM_TypeDef * PWMx = PWMX[pwmx];
	
	if((pwmx%2) == 0) PWMx->HIGHA = hduty << 1;
	else              PWMx->HIGHB = hduty << 1;
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_GetHDuty()
* 功能说明: 获取高电平时长
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: uint32_t				获取到的高电平时长
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t PWM_GetHDuty(uint32_t pwmx)
{
	PWM_TypeDef * PWMx = PWMX[pwmx];
		
	if((pwmx%2) == 0) return PWMx->HIGHA >> 1;
	else              return PWMx->HIGHB >> 1;
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_SetDeadzone()
* 功能说明:	设置死区时长
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
*			uint16_t deadzone		要设定的死区时长，取值范围0--1023
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_SetDeadzone(uint32_t pwmx, uint16_t deadzone)
{
	PWM_TypeDef * PWMx = PWMX[pwmx];
	
	if((pwmx%2) == 0) PWMx->DZA = deadzone;
	else              PWMx->DZB = deadzone;
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_GetDeadzone()
* 功能说明: 获取死区时长
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: uint16_t					获取到的死区时长
* 注意事项: 无
******************************************************************************************************************************************/
uint16_t  PWM_GetDeadzone(uint32_t pwmx)
{
	PWM_TypeDef * PWMx = PWMX[pwmx];
		
	if((pwmx%2) == 0) return PWMx->DZA;
	else              return PWMx->DZB;
}


/****************************************************************************************************************************************** 
* 函数名称:	PWM_IntNCycleEn()
* 功能说明: 新周期开始中断使能
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_IntNCycleEn(uint32_t pwmx)
{
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) PWMG->IE |= (0x01 << PWMG_IE_NC0A_Pos);
		else			  PWMG->IE |= (0x01 << PWMG_IE_NC0B_Pos);
		break;
	
	case((uint32_t)PWM1):
		if((pwmx%2) == 0) PWMG->IE |= (0x01 << PWMG_IE_NC1A_Pos);
		else			  PWMG->IE |= (0x01 << PWMG_IE_NC1B_Pos);
		break;
	
	case((uint32_t)PWM2):
		if((pwmx%2) == 0) PWMG->IE |= (0x01 << PWMG_IE_NC2A_Pos);
		else			  PWMG->IE |= (0x01 << PWMG_IE_NC2B_Pos);
		break;
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) PWMG->IE |= (0x01 << PWMG_IE_NC3A_Pos);
		else			  PWMG->IE |= (0x01 << PWMG_IE_NC3B_Pos);
		break;
	} 
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_IntNCycleDis()
* 功能说明: 新周期开始中断禁能
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_IntNCycleDis(uint32_t pwmx)
{
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) PWMG->IE &= ~(0x01 << PWMG_IE_NC0A_Pos);
		else			  PWMG->IE &= ~(0x01 << PWMG_IE_NC0B_Pos);
		break;
	
	case((uint32_t)PWM1):
		if((pwmx%2) == 0) PWMG->IE &= ~(0x01 << PWMG_IE_NC1A_Pos);
		else			  PWMG->IE &= ~(0x01 << PWMG_IE_NC1B_Pos);
		break;
	
	case((uint32_t)PWM2):
		if((pwmx%2) == 0) PWMG->IE &= ~(0x01 << PWMG_IE_NC2A_Pos);
		else			  PWMG->IE &= ~(0x01 << PWMG_IE_NC2B_Pos);
		break;
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) PWMG->IE &= ~(0x01 << PWMG_IE_NC3A_Pos);
		else			  PWMG->IE &= ~(0x01 << PWMG_IE_NC3B_Pos);
		break;
	} 
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_IntNCycleClr()
* 功能说明: 新周期开始中断标志清除
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_IntNCycleClr(uint32_t pwmx)
{
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) PWMG->NCIRS = (0x01 << PWMG_NCIRS_PWM0A_Pos);
		else			  PWMG->NCIRS = (0x01 << PWMG_NCIRS_PWM0B_Pos);
		break;
	
	case((uint32_t)PWM1):
		if((pwmx%2) == 0) PWMG->NCIRS = (0x01 << PWMG_NCIRS_PWM1A_Pos);
		else			  PWMG->NCIRS = (0x01 << PWMG_NCIRS_PWM1B_Pos);
		break;
	
	case((uint32_t)PWM2):
		if((pwmx%2) == 0) PWMG->NCIRS = (0x01 << PWMG_NCIRS_PWM2A_Pos);
		else			  PWMG->NCIRS = (0x01 << PWMG_NCIRS_PWM2B_Pos);
		break;
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) PWMG->NCIRS = (0x01 << PWMG_NCIRS_PWM3A_Pos);
		else			  PWMG->NCIRS = (0x01 << PWMG_NCIRS_PWM3B_Pos);
		break;
	} 
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_IntNCycleStat()
* 功能说明: 新周期开始中断是否发生
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: uint32_t				非0 新周期开始中断已发生    0 新周期开始中断未发生
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t PWM_IntNCycleStat(uint32_t pwmx)
{	
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) return (PWMG->NCIF & PWMG_NCIF_PWM0A_Msk);
		else			  return (PWMG->NCIF & PWMG_NCIF_PWM0B_Msk);

	case((uint32_t)PWM1):
		if((pwmx%2) == 0) return (PWMG->NCIF & PWMG_NCIF_PWM1A_Msk);
		else			  return (PWMG->NCIF & PWMG_NCIF_PWM1B_Msk);

	case((uint32_t)PWM2):
		if((pwmx%2) == 0) return (PWMG->NCIF & PWMG_NCIF_PWM2A_Msk);
		else			  return (PWMG->NCIF & PWMG_NCIF_PWM2B_Msk);
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) return (PWMG->NCIF & PWMG_NCIF_PWM3A_Msk);
		else			  return (PWMG->NCIF & PWMG_NCIF_PWM3B_Msk);
	}
	
	return 0;
}


/****************************************************************************************************************************************** 
* 函数名称:	PWM_IntHEndEn()
* 功能说明: 高电平结束中断使能
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_IntHEndEn(uint32_t pwmx)
{
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) PWMG->IE |= (0x01 << PWMG_IE_HE0A_Pos);
		else			  PWMG->IE |= (0x01 << PWMG_IE_HE0B_Pos);
		break;
	
	case((uint32_t)PWM1):
		if((pwmx%2) == 0) PWMG->IE |= (0x01 << PWMG_IE_HE1A_Pos);
		else			  PWMG->IE |= (0x01 << PWMG_IE_HE1B_Pos);
		break;
	
	case((uint32_t)PWM2):
		if((pwmx%2) == 0) PWMG->IE |= (0x01 << PWMG_IE_HE2A_Pos);
		else			  PWMG->IE |= (0x01 << PWMG_IE_HE2B_Pos);
		break;
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) PWMG->IE |= (0x01 << PWMG_IE_HE3A_Pos);
		else			  PWMG->IE |= (0x01 << PWMG_IE_HE3B_Pos);
		break;
	}
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_IntHEndDis()
* 功能说明: 高电平结束中断禁能
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_IntHEndDis(uint32_t pwmx)
{
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) PWMG->IE &= ~(0x01 << PWMG_IE_HE0A_Pos);
		else			  PWMG->IE &= ~(0x01 << PWMG_IE_HE0B_Pos);
		break;
	
	case((uint32_t)PWM1):
		if((pwmx%2) == 0) PWMG->IE &= ~(0x01 << PWMG_IE_HE1A_Pos);
		else			  PWMG->IE &= ~(0x01 << PWMG_IE_HE1B_Pos);
		break;
	
	case((uint32_t)PWM2):
		if((pwmx%2) == 0) PWMG->IE &= ~(0x01 << PWMG_IE_HE2A_Pos);
		else			  PWMG->IE &= ~(0x01 << PWMG_IE_HE2B_Pos);
		break;
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) PWMG->IE &= ~(0x01 << PWMG_IE_HE3A_Pos);
		else			  PWMG->IE &= ~(0x01 << PWMG_IE_HE3B_Pos);
		break;
	} 
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_IntHEndClr()
* 功能说明: 高电平结束中断标志清除
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_IntHEndClr(uint32_t pwmx)
{
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) PWMG->HEIRS = (0x01 << PWMG_HEIRS_PWM0A_Pos);
		else			  PWMG->HEIRS = (0x01 << PWMG_HEIRS_PWM0B_Pos);
		break;
	
	case((uint32_t)PWM1):
		if((pwmx%2) == 0) PWMG->HEIRS = (0x01 << PWMG_HEIRS_PWM1A_Pos);
		else			  PWMG->HEIRS = (0x01 << PWMG_HEIRS_PWM1B_Pos);
		break;
	
	case((uint32_t)PWM2):
		if((pwmx%2) == 0) PWMG->HEIRS = (0x01 << PWMG_HEIRS_PWM2A_Pos);
		else			  PWMG->HEIRS = (0x01 << PWMG_HEIRS_PWM2B_Pos);
		break;
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) PWMG->HEIRS = (0x01 << PWMG_HEIRS_PWM3A_Pos);
		else			  PWMG->HEIRS = (0x01 << PWMG_HEIRS_PWM3B_Pos);
		break;
	} 
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_IntHEndStat()
* 功能说明: 高电平结束中断是否发生
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: uint32_t				非0 高电平结束中断已发生    0 高电平结束中断未发生
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t PWM_IntHEndStat(uint32_t pwmx)
{	
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) return (PWMG->HEIF & PWMG_HEIF_PWM0A_Msk);
		else			  return (PWMG->HEIF & PWMG_HEIF_PWM0B_Msk);

	case((uint32_t)PWM1):
		if((pwmx%2) == 0) return (PWMG->HEIF & PWMG_HEIF_PWM1A_Msk);
		else			  return (PWMG->HEIF & PWMG_HEIF_PWM1B_Msk);

	case((uint32_t)PWM2):
		if((pwmx%2) == 0) return (PWMG->HEIF & PWMG_HEIF_PWM2A_Msk);
		else			  return (PWMG->HEIF & PWMG_HEIF_PWM2B_Msk);
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) return (PWMG->HEIF & PWMG_HEIF_PWM3A_Msk);
		else			  return (PWMG->HEIF & PWMG_HEIF_PWM3B_Msk);
	}
	
	return 0;
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_IntHCycleEn()
* 功能说明: 半周期中断使能
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_IntHCycleEn(uint32_t pwmx)
{
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) PWMG->HCIE |= (0x01 << PWMG_HCIE_PWM0A_Pos);
		else			  PWMG->HCIE |= (0x01 << PWMG_HCIE_PWM0B_Pos);
		break;
	
	case((uint32_t)PWM1):
		if((pwmx%2) == 0) PWMG->HCIE |= (0x01 << PWMG_HCIE_PWM1A_Pos);
		else			  PWMG->HCIE |= (0x01 << PWMG_HCIE_PWM1B_Pos);
		break;
	
	case((uint32_t)PWM2):
		if((pwmx%2) == 0) PWMG->HCIE |= (0x01 << PWMG_HCIE_PWM2A_Pos);
		else			  PWMG->HCIE |= (0x01 << PWMG_HCIE_PWM2B_Pos);
		break;
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) PWMG->HCIE |= (0x01 << PWMG_HCIE_PWM3A_Pos);
		else			  PWMG->HCIE |= (0x01 << PWMG_HCIE_PWM3B_Pos);
		break;
	}
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_IntHCycleDis()
* 功能说明: 半周期中断禁能
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_IntHCycleDis(uint32_t pwmx)
{
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) PWMG->HCIE &= ~(0x01 << PWMG_HCIE_PWM0A_Pos);
		else			  PWMG->HCIE &= ~(0x01 << PWMG_HCIE_PWM0B_Pos);
		break;
	
	case((uint32_t)PWM1):
		if((pwmx%2) == 0) PWMG->HCIE &= ~(0x01 << PWMG_HCIE_PWM1A_Pos);
		else			  PWMG->HCIE &= ~(0x01 << PWMG_HCIE_PWM1B_Pos);
		break;
	
	case((uint32_t)PWM2):
		if((pwmx%2) == 0) PWMG->HCIE &= ~(0x01 << PWMG_HCIE_PWM2A_Pos);
		else			  PWMG->HCIE &= ~(0x01 << PWMG_HCIE_PWM2B_Pos);
		break;
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) PWMG->HCIE &= ~(0x01 << PWMG_HCIE_PWM3A_Pos);
		else			  PWMG->HCIE &= ~(0x01 << PWMG_HCIE_PWM3B_Pos);
		break;
	} 
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_IntHCycleClr()
* 功能说明: 半周期中断标志清除
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void PWM_IntHCycleClr(uint32_t pwmx)
{
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) PWMG->HCIRS = (0x01 << PWMG_HCIRS_PWM0A_Pos);
		else			  PWMG->HCIRS = (0x01 << PWMG_HCIRS_PWM0B_Pos);
		break;
	
	case((uint32_t)PWM1):
		if((pwmx%2) == 0) PWMG->HCIRS = (0x01 << PWMG_HCIRS_PWM1A_Pos);
		else			  PWMG->HCIRS = (0x01 << PWMG_HCIRS_PWM1B_Pos);
		break;
	
	case((uint32_t)PWM2):
		if((pwmx%2) == 0) PWMG->HCIRS = (0x01 << PWMG_HCIRS_PWM2A_Pos);
		else			  PWMG->HCIRS = (0x01 << PWMG_HCIRS_PWM2B_Pos);
		break;
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) PWMG->HCIRS = (0x01 << PWMG_HCIRS_PWM3A_Pos);
		else			  PWMG->HCIRS = (0x01 << PWMG_HCIRS_PWM3B_Pos);
		break;
	} 
}

/****************************************************************************************************************************************** 
* 函数名称:	PWM_IntHCycleStat()
* 功能说明: 半周期中断是否发生
* 输    入: uint32_t pwmx			指定要被设置的PWM，有效值包括PWM0A、PWM0B、PWM1A、PWM1B、PWM2A、PWM2B、PWM3A、PWM3B
* 输    出: uint32_t				非0 半周期中断已发生    0 半周期中断未发生
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t PWM_IntHCycleStat(uint32_t pwmx)
{	
	switch((uint32_t)PWMX[pwmx])
	{
	case((uint32_t)PWM0):
		if((pwmx%2) == 0) return (PWMG->HCIF & PWMG_HCIF_PWM0A_Msk);
		else			  return (PWMG->HCIF & PWMG_HCIF_PWM0B_Msk);

	case((uint32_t)PWM1):
		if((pwmx%2) == 0) return (PWMG->HCIF & PWMG_HCIF_PWM1A_Msk);
		else			  return (PWMG->HCIF & PWMG_HCIF_PWM1B_Msk);

	case((uint32_t)PWM2):
		if((pwmx%2) == 0) return (PWMG->HCIF & PWMG_HCIF_PWM2A_Msk);
		else			  return (PWMG->HCIF & PWMG_HCIF_PWM2B_Msk);
	
	case((uint32_t)PWM3):
		if((pwmx%2) == 0) return (PWMG->HCIF & PWMG_HCIF_PWM3A_Msk);
		else			  return (PWMG->HCIF & PWMG_HCIF_PWM3B_Msk);
	}
	
	return 0;
}

