#include "rtc.h"

int rtc_init(struct time_st *t)
{
	uint32_t backupRegister;
	RTC_InitTypeDef RTC_InitValue;
	
	//1.��pwrʱ�ӣ���ΪҪ����pwr������VBAT(���ݵ�·�����а���RTC��RTC�ı��ݼĴ���)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	//2.�ر�RTC��д������Ĭ����д������(ֻ�ܶ�����д)
	PWR_BackupAccessCmd(ENABLE);
	//3.�趨ʱ�䣨�ȶ�ȡ���ݼĴ�����ֵ���ж��Ƿ��ǵ�һ������rtc_init����)
	//ע�⣺backupRegister�кܶ���������������ÿ�ζ�ȡ4���֣�RTC_BKP_DR1�Ĵ����ı��
	backupRegister = RTC_ReadBackupRegister(RTC_BKP_DR1);
	if (backupRegister != 0x0101) {
	//4.����ǵ�һ������rtc_init������
	//		a.��LSEʱ�ӣ�32768HZ��
			RCC_LSEConfig(RCC_LSE_ON);
	//		b.�ȴ�LSE�ȶ�'
			while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	//		c.ΪRTCѡ��ʱ��Դ
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	//		d.����RTC��ʱ��
			RCC_RTCCLKCmd(ENABLE);
	//		e.��ʼ��RTC(RTC_Init)
			RTC_InitValue.RTC_AsynchPrediv = 127;
			RTC_InitValue.RTC_HourFormat = RTC_HourFormat_24;
			RTC_InitValue.RTC_SynchPrediv = 255;
			RTC_Init(&RTC_InitValue);
	//		f.����ʱ�������
			rtc_set_time(t);
	//		g.д���ݼĴ�������Ϊ�Ѿ����ù�ʱ��ı�־��
			RTC_WriteBackupRegister(RTC_BKP_DR1, 0x0101);
	}
	//5.��RTC��д����
	PWR_BackupAccessCmd(DISABLE);
	return  backupRegister;
}

void rtc_get_time(struct time_st *t)
{
	//���ʱ��
	RTC_TimeTypeDef RTC_TimeValue;
	RTC_DateTypeDef RTC_DateValue;
	
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeValue);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateValue);
	
	t->Y = RTC_DateValue.RTC_Year + 1900;
	t->M = RTC_DateValue.RTC_Month + 1;
	t->D = RTC_DateValue.RTC_Date;
	t->w = RTC_DateValue.RTC_WeekDay + 1;
	
	t->h = RTC_TimeValue.RTC_Hours;
	t->m = RTC_TimeValue.RTC_Minutes;
	t->s = RTC_TimeValue.RTC_Seconds;
}

//���ô˺���֮ǰҪ�ر�VBAT��д����
void rtc_set_time(struct time_st *t)
{
	RTC_TimeTypeDef RTC_TimeValue;
	RTC_DateTypeDef RTC_DateValue;
	
	//����ʱ��
	//RTC_TimeValue.RTC_H12 = t->am;
	RTC_TimeValue.RTC_Hours = t->h;
	RTC_TimeValue.RTC_Minutes = t->m;
	RTC_TimeValue.RTC_Seconds = t->s;
	
	RTC_DateValue.RTC_Date = t->D;
	RTC_DateValue.RTC_Month = t->M - 1;
	RTC_DateValue.RTC_WeekDay = t->w - 1;
	RTC_DateValue.RTC_Year = t->Y - 1900;
	
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeValue);
	RTC_SetDate(RTC_Format_BIN, &RTC_DateValue);
}



