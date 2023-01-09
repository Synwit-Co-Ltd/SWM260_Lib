;******************************************************************************************************************************************
; 文件名称:	startup_SWM260.s
; 功能说明:	SWM260单片机的启动文件
; 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
; 注意事项:
; 版本日期: V1.0.0		2016年1月30日
; 升级记录:
;
;
;******************************************************************************************************************************************
; @attention
;
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
; REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
; FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
; OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
; -ECTION WITH THEIR PRODUCTS.
;
; COPYRIGHT 2012 Synwit Technology
;******************************************************************************************************************************************


; Amount of memory (in bytes) allocated for Stack
Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; Amount of memory (in bytes) allocated for Heap
Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp               ; Top of Stack
                DCD    Reset_Handler              ; Reset Handler
                DCD    NMI_Handler                ; NMI Handler
                DCD    HardFault_Handler          ; Hard Fault Handler
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0
                DCD    SVC_Handler                ; SVCall Handler
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    PendSV_Handler             ; PendSV Handler
                DCD    SysTick_Handler            ; SysTick Handler

                ; External Interrupts
				DCD    UART0_Handler
				DCD    TIMR0_Handler
				DCD    SPI0_Handler
				DCD    UART1_Handler
				DCD    UART2_Handler
				DCD    TIMR1_Handler
				DCD    DMA_Handler
				DCD    PWM0_Handler
				DCD    BTIMR0_Handler
				DCD    TIMR2_Handler
				DCD    TIMR3_Handler
				DCD    WDT_Handler
				DCD    I2C4_Handler
				DCD    UART3_Handler
				DCD    ADC0_Handler
				DCD    BTIMR1_Handler
				DCD    GPIOA9_GPIOC6_Handler
				DCD    GPIOA6_GPIOC7_Handler
				DCD    GPIOA7_GPIOC8_Handler
				DCD    GPIOA8_GPIOC9_Handler
				DCD    GPIOA10_GPIOC10_Handler
				DCD    GPIOA13_GPIOC12_Handler
				DCD    GPIOA12_GPIOC13_Handler
				DCD    GPIOA11_GPIOC14_Handler
				DCD    XTALSTOPDET_GPIOC0_Handler
				DCD    BTIMR2_GPIOB12_Handler
				DCD    PWM1_GPIOA1_Handler
				DCD    PWM2_UART4_Handler
				DCD    BOD_PWMHALT_Handler
				DCD    PWM3_GPIOB_ACMP_Handler
				DCD    SPI1_HALL_GPIOD_Handler
				DCD    BTIMR3_RTC_Handler

__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors



                AREA    |.text|, CODE, READONLY

Reset_Handler    PROC
                 EXPORT  Reset_Handler          [WEAK]
				 IMPORT  __main
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler             [WEAK]
                B       .
                ENDP

HardFault_Handler PROC
                EXPORT  HardFault_Handler       [WEAK]
                B       .
                ENDP
				
SVC_Handler     PROC
                EXPORT  SVC_Handler             [WEAK]
                B       .
                ENDP

PendSV_Handler  PROC
                EXPORT  PendSV_Handler          [WEAK]
                B       .
                ENDP

SysTick_Handler PROC
                EXPORT  SysTick_Handler         [WEAK]
                B       .
                ENDP

UART0_Handler           PROC
                EXPORT  UART0_Handler           [WEAK]
                B       .
                ENDP

TIMR0_Handler           PROC
                EXPORT  TIMR0_Handler           [WEAK]
                B       .
                ENDP

SPI0_Handler            PROC
                EXPORT  SPI0_Handler            [WEAK]
                B       .
                ENDP

UART1_Handler           PROC
                EXPORT  UART1_Handler           [WEAK]
                B       .
                ENDP

UART2_Handler            PROC
                EXPORT   UART2_Handler          [WEAK]
                B       .
                ENDP

TIMR1_Handler           PROC
                EXPORT  TIMR1_Handler           [WEAK]
                B       .
                ENDP

DMA_Handler             PROC
                EXPORT  DMA_Handler             [WEAK]
                B       .
                ENDP

PWM0_Handler            PROC
                EXPORT  PWM0_Handler            [WEAK]
                B       .
                ENDP

BTIMR0_Handler          PROC
                EXPORT  BTIMR0_Handler          [WEAK]
                B       .
                ENDP

TIMR2_Handler           PROC
                EXPORT  TIMR2_Handler           [WEAK]
                B       .
                ENDP
				
TIMR3_Handler           PROC
                EXPORT  TIMR3_Handler           [WEAK]
                B       .
                ENDP

WDT_Handler             PROC
                EXPORT  WDT_Handler             [WEAK]
                B       .
                ENDP

I2C4_Handler            PROC
                EXPORT  I2C4_Handler            [WEAK]
                B       .
                ENDP

UART3_Handler           PROC
                EXPORT  UART3_Handler           [WEAK]
                B       .
                ENDP

ADC0_Handler            PROC
                EXPORT  ADC0_Handler            [WEAK]
                B       .
                ENDP

BTIMR1_Handler          PROC
                EXPORT  BTIMR1_Handler          [WEAK]
                B       .
                ENDP

GPIOA9_GPIOC6_Handler   PROC
                EXPORT  GPIOA9_GPIOC6_Handler   [WEAK]
                B       .
                ENDP

GPIOA6_GPIOC7_Handler   PROC
                EXPORT  GPIOA6_GPIOC7_Handler   [WEAK]
                B       .
                ENDP

GPIOA7_GPIOC8_Handler   PROC
                EXPORT  GPIOA7_GPIOC8_Handler   [WEAK]
                B       .
                ENDP

GPIOA8_GPIOC9_Handler   PROC
                EXPORT  GPIOA8_GPIOC9_Handler   [WEAK]
                B       .
                ENDP

GPIOA10_GPIOC10_Handler PROC
                EXPORT  GPIOA10_GPIOC10_Handler [WEAK]
                B       .
                ENDP

GPIOA13_GPIOC12_Handler PROC
                EXPORT  GPIOA13_GPIOC12_Handler [WEAK]
                B       .
                ENDP

GPIOA12_GPIOC13_Handler PROC
                EXPORT  GPIOA12_GPIOC13_Handler [WEAK]
                B       .
                ENDP

GPIOA11_GPIOC14_Handler   PROC
                EXPORT  GPIOA11_GPIOC14_Handler [WEAK]
                B       .
                ENDP

XTALSTOPDET_GPIOC0_Handler PROC
                EXPORT  XTALSTOPDET_GPIOC0_Handler [WEAK]
                B       .
                ENDP

BTIMR2_GPIOB12_Handler  PROC
                EXPORT  BTIMR2_GPIOB12_Handler  [WEAK]
                B       .
                ENDP

PWM1_GPIOA1_Handler     PROC
                EXPORT  PWM1_GPIOA1_Handler     [WEAK]
                B       .
                ENDP

PWM2_UART4_Handler      PROC
                EXPORT  PWM2_UART4_Handler      [WEAK]
                B       .
                ENDP

BOD_PWMHALT_Handler     PROC
                EXPORT  BOD_PWMHALT_Handler     [WEAK]
                B       .
                ENDP

PWM3_GPIOB_ACMP_Handler PROC
                EXPORT  PWM3_GPIOB_ACMP_Handler [WEAK]
                B       .
                ENDP

SPI1_HALL_GPIOD_Handler PROC
                EXPORT  SPI1_HALL_GPIOD_Handler [WEAK]
                B       .
                ENDP

BTIMR3_RTC_Handler      PROC
                EXPORT  BTIMR3_RTC_Handler      [WEAK]
                B       .
                ENDP

                ALIGN


;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF

                END
