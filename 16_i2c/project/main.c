#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_uart.h"
#include "bsp_lcd.h"
#include "bsp_lcdapi.h"
#include "bsp_rtc.h"
#include "bsp_ap3216c.h"
#include "/home/long/src/mx6ull/13_uart/stdio/include/stdio.h"


unsigned int BACKCOLOR[6] = {LCD_BLUE, LCD_GREEN, LCD_RED, LCD_ORANGE, LCD_BLACK, LCD_WHITE};


/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	
	unsigned short ir, als, ps;

	unsigned char state = OFF;

	// initialze

	int_init(); 					/* initialize system interrupt		*/
	imx6u_clkinit();				/* initialize system clock 			*/
    delay_init();
	clk_enable();
	led_init();
	beep_init();
	uart_init();
	lcd_init();

	tftlcd_dev.forecolor = LCD_RED;

	lcd_show_string(30, 50, 200, 16, 16, (char*)"ALPHA-IMX6UL IIC TEST");

	lcd_show_string(30, 70, 200, 16, 16, (char*)"AP3216C TEST");

	lcd_show_string(30, 90, 200, 16, 16, (char*)"ATOM@ALIENTEK");

	lcd_show_string(30, 110, 200, 16, 16, (char*)"2023/11/09");


	while (ap3216c_init())
	{
		lcd_show_string(30, 130, 200, 16, 16, (char*)"AP3216C Check Failed!");

		delay_ms(500);

		lcd_show_string(30, 130, 200, 16, 16, (char*)"Please Check         ");

		delay_ms(500);
	}

	lcd_show_string(30, 130, 200, 16, 16, (char*)"AP3216C Ready!");

	lcd_show_string(30, 160, 200, 16, 16, (char*)"IR: ");

	lcd_show_string(30, 180, 200, 16, 16, (char*)"PS: ");

	lcd_show_string(30, 200, 200, 16, 16, (char*)"ALS: ");
	
	tftlcd_dev.forecolor = LCD_BLUE;

	while (1)
	{
		ap3216c_readdata(&ir, &ps, &als);

		lcd_shownum(30 + 32, 160, ir, 5, 16);

		lcd_shownum(30 + 32, 180, ps, 5, 16);

		lcd_shownum(30 + 32, 200, als, 5, 16);
		
		delay_ms(120);

		state = !state;

		led_switch(LED0, state);
	}

	return 0;
}
