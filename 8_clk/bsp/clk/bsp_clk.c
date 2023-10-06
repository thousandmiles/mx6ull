#include "bsp_clk.h"

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

/*
1. Initialize the ARM clock (PLL1) with the specified frequency 792MHz.
2. Initialize PLL2 and PLL3 with default frequency.
*/
void imx6u_clkinit(void)
{
	uint32_t reg = 0;
	/*
	1. PLL1 clock
	1.1 Usually, PLL1 clock is driven by pll1_sw_clk. This clock has two sources: pll1_main_clk and step_clk.
		pll1_sw_clk is the register to switch between pll1_main_clk and step_clk.
		pll1_sw_clk is defined in CCM->CCSR[2]
		pll1_sw_clk == 0    -> pll1_main_clk
		pll1_sw_clk == 1 	-> step_clk
	*/
	if(((CCM->CCSR >> 2) &0x1 ) == 0)	// pll1_sw_clk == 0
	{
		CCM->CCSR &= ~(1<<8);			// set step_clk to 24MHz
		CCM->CCSR |= (1<<2);			// pll1_sw_clk == 1
	}

	/*
	1.2 Set pll1_main_clk to 792MHz.
	*/
	CCM_ANALOG->PLL_ARM = 66 & 0x7F;	// DIV_SELECT
	CCM_ANALOG->PLL_ARM |= (1<<13);		// enable PLL1
	CCM->CCSR &= ~(1<<2);				// switch back to pll1_main_clk
	CCM->CACRR = 0;						// frequency division = 1

	/*
	2. Initialize PLL2 and its' PFDs. PLL2 has a default value of 528.
	*/
	reg = CCM_ANALOG->PFD_528;
	reg &= ~(0x3F3F3F3F); 
	reg |= 32<<24;
	reg |= 24<<16;
	reg |= 16<<8;
	reg |= 27<<0;
	CCM_ANALOG->PFD_528 = reg;

	/*
	3. Initialize PLL3 and its' PFDs. PLL3 has a default value of 480.
	*/
	reg = 0;
	reg = CCM_ANALOG->PFD_480;
	reg &= ~(0x3F3F3F3F); 
	reg |= 19<<24;
	reg |= 17<<16;
	reg |= 16<<8;
	reg |= 12<<0;
	CCM_ANALOG->PFD_480 = reg;

	/*
	4. Set AHB clock
	*/
	CCM->CBCMR &= ~(3<<18);
	CCM->CBCMR |= 1<<18;
	CCM->CBCDR &= ~(1<<25);
	while (CCM->CDHIPR & (1<<5));

	/*
	Demo:
	*/
	# if 0
	CCM->CBCDR &= ~(7<<10);
	CCM->CBCDR |= 2<<10;
	while (CCM->CDHIPR & (1<<1));
	# endif
	
	/*
	5. Set IPG clock
	*/
	CCM->CBCDR &= ~(3<<8);
	CCM->CBCDR |= 1<<8;

	/*
	6. Set PERCLK clock
	*/
	CCM->CSCMR1 &= ~(1<<6);
	CCM->CSCMR1 &= ~(7<<0);
	
}