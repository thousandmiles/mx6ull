#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_exit.h"
#include "bsp_epittimer.h"
#include "bsp_keyfilter.h"
#include "bsp_uart.h"
#include "/home/long/src/mx6ull/13_uart/stdio/include/stdio.h"

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{

	uint8_t state = OFF;

	int_init(); 					/* initialize system interrupt		*/
	imx6u_clkinit();				/* initialize system clock 			*/
	clk_enable();					/* enable all clocks 				*/
	led_init();						/* led 								*/
	beep_init();					/* beep	 							*/
	//key_init();						/* key 								*/
	//epit1_init(0, 66000000*2);		/* clock interrupt					*/
	//filterkey_init();				/* key with timer anti-shake*/
	uart_init();                    /* uart */

	// (1) single char input / output
	/*
	uint8_t a = 0;

	while(1)			
	{	
		put_a_str(" Please input a char: ");
		a = get_a_char();
		put_a_char(a);
		put_a_str("\r\n");

		put_a_str("You have input the: ");
		put_a_char(a);
		put_a_str("\r\n\r\n");

		state =!state;
		led_switch(LED0, state);
	}
	*/

	// (2) printf / scanf
	int a = 0;
	int b = 0;
	while (1)
	{
		printf("input two int numbers, using space separated: ");
		scanf("%d %d", &a, &b);

		printf("\r\n %d + %d = %d \r\n", a, b, a + b);

		state =!state;
		led_switch(LED0, state);

	}

	return 0;
}
