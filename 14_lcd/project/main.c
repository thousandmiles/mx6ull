#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_uart.h"
#include "bsp_lcd.h"
#include "bsp_lcdapi.h"
#include "/home/long/src/mx6ull/13_uart/stdio/include/stdio.h"


unsigned int BACKCOLOR[6] = {LCD_BLUE, LCD_GREEN, LCD_RED, LCD_ORANGE, LCD_BLACK, LCD_WHITE};


/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{

	unsigned char index = 0;
	unsigned char state = OFF;

	int_init(); 					/* initialize system interrupt		*/
	imx6u_clkinit();				/* initialize system clock 			*/
    delay_init();
	clk_enable();
	led_init();
	beep_init();
	uart_init();
	lcd_init();

	tftlcd_dev.forecolor = LCD_RED;

	lcd_show_string(10, 10, 400, 32, 32, (char*)"ALPHA-IMX6UL ELCD TEST");

	lcd_draw_rectangle(10, 52, 1014, 290);

	lcd_drawline(10, 52, 1014, 290);

	lcd_drawline(10, 290, 1014, 52);

	lcd_draw_Circle(512, 171, 119);

	while(1)
	{
		index++;
		if(index == 6) index = 0;

		lcd_fill(0, 300, 1023, 599, BACKCOLOR[index]);

		lcd_show_string(800, 10, 240, 32, 32, (char*)"INDEX = ");

		lcd_shownum(896, 10, index, 2, 32);

		state = !state;
		led_switch(LED0, state);
		delay_ms(1000);
	}
	

	return 0;
}
