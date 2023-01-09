#ifndef __SWM260_PWM_H__
#define	__SWM260_PWM_H__


typedef struct {
	uint8_t  PWMnXN;		//是否在PWMnXN引脚输出互补波形，如对于PWM0A，可以在PWM0AN引脚输出互补波形
							//注意：PWMnXN = 1时，输出频率降低到一半
	uint8_t  clkdiv;		//PWM_CLKDIV_1、PWM_CLKDIV_2、... ...

	uint32_t cycle;			//周期,28位
	uint32_t hduty;			//占空比，高电平时长，28位
	uint16_t deadzone;		//死区时长，取值0--1023
	uint8_t  initLevel;		//初始输出电平，0 低电平    1 高电平
	
	uint8_t  HEndIE;		//高电平结束中断使能
	uint8_t  NCycleIE;		//新周期开始中断使能
	uint8_t  HCycleIE;		//半周期    中断使能
} PWM_InitStructure;


#define PWM0A	0
#define PWM0B	1
#define PWM1A	2
#define PWM1B	3
#define PWM2A	4
#define PWM2B	5
#define PWM3A	6
#define PWM3B	7


#define PWM_CLKDIV_1	0
#define PWM_CLKDIV_2	1
#define PWM_CLKDIV_4	2
#define PWM_CLKDIV_8	3
#define PWM_CLKDIV_16	4
#define PWM_CLKDIV_32	5
#define PWM_CLKDIV_64	6
#define PWM_CLKDIV_128	7



void PWM_Init(uint32_t pwmx, PWM_InitStructure * initStruct);	//PWM初始化
void PWM_Start(uint32_t pwmx);									//启动PWM，开始PWM输出
void PWM_Stop(uint32_t pwmx);									//关闭PWM，停止PWM输出

void PWM_SetCycle(uint32_t pwmx, uint32_t cycle);				//设置周期
uint32_t PWM_GetCycle(uint32_t pwmx);	   						//获取周期
void PWM_SetHDuty(uint32_t pwmx, uint32_t hduty);				//设置高电平时长
uint32_t PWM_GetHDuty(uint32_t pwmx);							//获取高电平时长
void PWM_SetDeadzone(uint32_t pwmx, uint16_t deadzone);			//设置死区时长
uint16_t  PWM_GetDeadzone(uint32_t pwmx);		 				//获取死区时长

void PWM_IntNCycleEn(uint32_t pwmx);   							//新周期开始中断使能
void PWM_IntNCycleDis(uint32_t pwmx);  							//新周期开始中断禁能
void PWM_IntNCycleClr(uint32_t pwmx);  							//新周期开始中断标志清除
uint32_t PWM_IntNCycleStat(uint32_t pwmx); 						//新周期开始中断是否发生

void PWM_IntHEndEn(uint32_t pwmx); 								//高电平结束中断使能
void PWM_IntHEndDis(uint32_t pwmx);								//高电平结束中断禁能
void PWM_IntHEndClr(uint32_t pwmx);	 							//高电平结束中断标志清除
uint32_t PWM_IntHEndStat(uint32_t pwmx);						//高电平结束中断是否发生

void PWM_IntHCycleEn(uint32_t pwmx);							//半周期中断使能
void PWM_IntHCycleDis(uint32_t pwmx);							//半周期中断禁能
void PWM_IntHCycleClr(uint32_t pwmx);							//半周期中断标志清除
uint32_t PWM_IntHCycleStat(uint32_t pwmx);						//半周期中断是否发生

#endif //__SWM260_PWM_H__
