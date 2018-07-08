#include "led.h"
#include "relay.h"
#include "button.h"
#include "buzzer.h"

static int count = 0;

void TimingDelay_Decrement(void)
{
	if (count == 0) return;
	count--;
}

void my_delay_ms(int ms)
{
	count = ms;
	while (count);
}

int main(void)
{			
	// �趨����ֵ
	SysTick_Config(SystemCoreClock / 8 / 1000);
	// ѡ��ʱ��Դ
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	// ��������˳���ܱ�
			
	led_init();
	relay_init();
	buzzer_init();
	
	while (1) {
		led_on(0);
		relay_on(0);
		buzzer_on();
		my_delay_ms(500);
		led_off(0);
		relay_off(0);
		buzzer_off();
		my_delay_ms(500);
	}
}


