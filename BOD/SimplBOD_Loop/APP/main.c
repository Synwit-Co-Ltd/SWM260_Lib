#include "SWM260.h"


int main(void)
{	
	SystemInit();
		
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//����� ��LED
	
	SYS->BODCR = (1 << SYS_BODCR_IE_Pos) |
				 (0 << SYS_BODCR_RSTLVL_Pos) |	//2.0V��λ
				 (0 << SYS_BODCR_RSTLVL_Pos);	//2.7V�����ж�
	NVIC_DisableIRQ(BOD_PWMHALT_IRQn);			//��Ҫ��ѯBODSR.IF���ֲ��봥��BOD�жϣ����뽫BODCR.IE��λ��ͬʱ�ر�BOD��NVIC�ں��ж�
	
	while(1==1)
	{
		while(SYS->BODSR & SYS_BODSR_IF_Msk)
		{
			SYS->BODSR = (1 << SYS_BODSR_IF_Pos);		//����жϱ�־
			
			GPIO_SetBit(GPIOA, PIN5);
		}
		
		GPIO_ClrBit(GPIOA, PIN5);
	}
}

