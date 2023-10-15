#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_exit.h"
#include "bsp_epittimer.h"
#include "bsp_keyfilter.h"

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	unsigned char state = OFF;

	int_init(); 					/* initialize system interrupt		*/
	imx6u_clkinit();				/* initialize system clock 			*/
	clk_enable();					/* enable all clocks 				*/
	led_init();						/* led 								*/
	beep_init();					/* beep	 							*/
	//key_init();						/* key 								*/
	//epit1_init(0, 66000000*2);		/* clock interrupt					*/
	filterkey_init();				/* key with timer anti-shake*/

	while(1)			
	{	
		state = !state;
		led_switch(LED0, state);
		delayer(500);
	}

	return 0;
}
