/*
author: forrest long and openedv

project: led lighting

version: 1.0

description: c file for led lighting

arm: I.MX6ULL
*/

#include "main.h"

/*
Enable all clocks
*/
void clk_enable(void)
{
    CCM_CCGR0 = 0xFFFFFFFF;
    CCM_CCGR1 = 0xFFFFFFFF;
    CCM_CCGR2 = 0xFFFFFFFF;
    CCM_CCGR3 = 0xFFFFFFFF;
    CCM_CCGR4 = 0xFFFFFFFF;
    CCM_CCGR5 = 0xFFFFFFFF;
    CCM_CCGR6 = 0xFFFFFFFF;
}

/*
Initialize the GPIO1
*/
void led_init(void)
{
    SW_MUX_GPIO01_IO03 = 0x5;

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
   SW_PAD_GPIO01_IO03 = 0x10B0;

   GPIO1_DR = 0;

   GPIO1_GDIR = 0x8;
}


void led_open(void) 
{
    GPIO1_DR &= ~(1<<3);        // set GPIO1_DR bit[3] to 0
}


void led_close(void)
{
    GPIO1_DR |= (1<<3);         // set GPIO1_DR bit[3] to 1
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
        led_open();
        delay(500);

        led_close();
        delay(500);
    }
    return 0;
}