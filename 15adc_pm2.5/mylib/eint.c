#include "eint.h"
#include "stm32f4xx_conf.h"

void eint_init(void)
{
	GPIO_InitTypeDef initValue = {0};
	NVIC_InitTypeDef nvicInitValue = {0};
	EXTI_InitTypeDef extiInitValue = {0};
	
	// ��GPIOxʱ��, SYSCFGʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	// ����GPIOx����
	initValue.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7; 
	initValue.GPIO_Mode = GPIO_Mode_IN;
	initValue.GPIO_PuPd = GPIO_PuPd_NOPULL;
 	GPIO_Init(GPIOF, &initValue);
	
	initValue.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOE, &initValue);
	
	// �����ⲿ�ж��ߵ�����
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource9);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource8);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource7);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource6);
	
	// ��main�����з������ȼ���Դ
	// �����жϿ�����NVIC���жϺ�
	nvicInitValue.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvicInitValue.NVIC_IRQChannelCmd = ENABLE;
	nvicInitValue.NVIC_IRQChannelPreemptionPriority = 2;
	nvicInitValue.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvicInitValue);
	
	// �����ⲿ�ж���
	extiInitValue.EXTI_Line = EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
	extiInitValue.EXTI_LineCmd = ENABLE;
	extiInitValue.EXTI_Mode = EXTI_Mode_Interrupt;
	extiInitValue.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&extiInitValue);
}

void set_handler(irq_handler h1, irq_handler h2, irq_handler h3, irq_handler h4)
{
	handler1 = h1;
	handler2 = h2;
	handler3 = h3;
	handler4 = h4;
}

void EXTI9_5_IRQHandler(void)
{
	// �����ⲿ�ж���
	if (EXTI_GetFlagStatus(EXTI_Line9) == SET) {
		handler1();
		// ��Ӧ�жϺ����
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	
	if (EXTI_GetFlagStatus(EXTI_Line8) == SET) {
		handler2();
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
		
	if (EXTI_GetFlagStatus(EXTI_Line7) == SET) {
		handler3();
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
		
	if (EXTI_GetFlagStatus(EXTI_Line6) == SET) {
		handler4();
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}




