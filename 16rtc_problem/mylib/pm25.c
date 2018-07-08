#include "pm25.h"
#include "adc.h"
#include "delay.h"
#include "bitband.h"

#include "stm32f4xx_conf.h"
#include <math.h>

void pm_init(void)
{
	// GPIOC13
	GPIO_InitTypeDef initValue;
	
	adc_init();
	
	initValue.GPIO_Pin = GPIO_Pin_13;
	initValue.GPIO_Mode = GPIO_Mode_OUT;
	initValue.GPIO_OType = GPIO_OType_PP;
	initValue.GPIO_Speed = GPIO_Speed_50MHz;
	initValue.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &initValue);
}

/*
3000+: �ǳ���
1050-3000: ��
300-1050: һ��
150-300: ��
75-150: �ܺ�
0-75: �ǳ���
*/
int pm_get(void)
{
	int value = 0;
	double f;
	
	PCOut(13) = 0;
	delay_us(280);
	value = adc_get();
	delay_us(40);
	PCOut(13) = 1;
	delay_us(9680);
	f = value;
    if (f > 36.455)
		f = ((f / 4096) - 0.0356) * 120000 * 0.035;
	return floor(f);
}

