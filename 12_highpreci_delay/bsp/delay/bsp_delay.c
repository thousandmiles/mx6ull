#include "bsp_delay.h"

void delay_short(volatile unsigned int n)
{
	while(n--){}
}

void delayer(volatile unsigned int n)
{
	while(n--)
	{
		delay_short(0x7ff);
	}
}

void delay_init(void)
{
	GPT1->CR = 0;

	GPT1->CR = 1 << 15;				// CR register reset

	while ((GPT1->CR>>15) & 1);		// Wait for CR register reset

	GPT1->CR = 1 << 6;	

	GPT1->PR = 65;

	GPT1->OCR[0] = MAX_CNT;

	GPT1->CR |= 1;
}

void delay_us(uint32_t usdelay)
{
	uint32_t oldcnt, newcnt;

	uint32_t tcntvalue = 0;

	oldcnt = GPT1->CNT;

	while (1)
	{
		newcnt = GPT1->CNT;

		if (newcnt != oldcnt)
		{
			if (oldcnt < newcnt)
			{
				tcntvalue += newcnt - oldcnt;
			}
			else
			{
				tcntvalue += MAX_CNT - oldcnt + newcnt;
			}
			
			oldcnt = newcnt;

			if (tcntvalue >= usdelay)
			{
				break;
			}
		}
	}
}

void delay_ms(uint32_t msdelay)
{
	int i = 0;
	for (; i < msdelay; ++i)
	{
		delay_us(1000);
	}
}