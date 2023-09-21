/*
author: forrest long and openedv

project: led lighting

version: 1.0

description: c file for led lighting using struct definition of registers

arm: I.MX6ULL
*/
#include "imx6ul.h"


void clk_enable(void)
{
	CCM->CCGR0 = 0XFFFFFFFF;
	CCM->CCGR1 = 0XFFFFFFFF;

	CCM->CCGR2 = 0XFFFFFFFF;
	CCM->CCGR3 = 0XFFFFFFFF;
	CCM->CCGR4 = 0XFFFFFFFF;
	CCM->CCGR5 = 0XFFFFFFFF;
	CCM->CCGR6 = 0XFFFFFFFF;
}

void led_init(void)
{
	IOMUX_SW_MUX->GPIO1_IO03 = 0X5;	
    /*
    Configure the GPIO1_IO03: IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03
    --bit [16]:0            HYS: disabled
    --bit [15:14]:00        PUS: pull down enable
    --bit [13]:0            PUE: keeper status
    --bit [12]:1            PKE: pull/keeper enable
    --bit [11]:0            ODE: open circuit disabled
    --bit [7:6]:10          SPEED: moderate
    --bit [5:3]:110         DSE: R0/R6
    --bit [0]:0             SRE: low rate

    bit[16:0]: 0    0   0   0   1   0   0   0   0   1   0   1   1   0   0   0   0
    bit        16   15  14  13  12  11  10  9   8   7   6   5   4   3   2   1   0
               0    -------1------  ------0------   ------B------   ------0------
    --> 10B0
    */
    IOMUX_SW_PAD->GPIO1_IO03 = 0X10B0;

	GPIO1->GDIR = 0X0000008;

	GPIO1->DR &= ~(1 << 3);
}

void led_on(void)
{
	GPIO1->DR &= ~(1<<3); 
}

void led_off(void)
{
	GPIO1->DR |= (1<<3); 
}

void delay_short(volatile unsigned int n)
{
	while(n--){}
}

void delay(volatile unsigned int n)
{
	while(n--)
	{
		delay_short(0x7ff);
	}
}

int main(void)
{
	clk_enable();	
	led_init();		

	while(1)	
	{	
		led_off();	
		delay(200);		

		led_on();		
		delay(500);		
	}

	return 0;
}
