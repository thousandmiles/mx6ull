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
#include "/home/long/src/mx6ull/13_uart/stdio/include/stdio.h"


unsigned int BACKCOLOR[6] = {LCD_BLUE, LCD_GREEN, LCD_RED, LCD_ORANGE, LCD_BLACK, LCD_WHITE};


/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	
	unsigned char key = 0;
	int t = 0;
	int i = 3;
	char buf[160];
	struct rtc_datetime rtcdate;
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
	rtc_init();

	tftlcd_dev.forecolor = LCD_RED;

	lcd_show_string(50, 350, 400, 24, 24, (char*)"ALPHA-IMX6UL RTC TEST");

	tftlcd_dev.forecolor = LCD_BLUE;

	lcd_draw_rectangle(10, 52, 1014, 290);

	lcd_drawline(10, 52, 1014, 290);

	lcd_drawline(10, 290, 1014, 52);

	lcd_draw_Circle(512, 171, 119);

	memset(buf, 0, sizeof(buf));

	while(1)
	{
		if (t == 100)
		{
			t = 0;
			//printf("will be running %d s......\r", i);
			lcd_fill(50, 370, 400, 400, tftlcd_dev.backcolor);
			sprintf(buf, "will be running %ds......", i);
			lcd_show_string(50, 370, 300, 24, 24, buf);
			i--;
			if(i < 0) break;
		}

		key = key_get_value();

		if (key == KEY0_VALUE)
		{
			rtcdate.year = 2023;
			rtcdate.month = 11;
			rtcdate.day = 5;
			rtcdate.hour = 15;
			rtcdate.minute = 50;
			rtcdate.second = 0;

			rtc_setdatetime(&rtcdate);
			lcd_fill(50, 370, 400, 400, tftlcd_dev.backcolor);
			lcd_show_string(50, 370, 300, 24, 24, (char*)"\r\nRTC Init finished!\r\n");
		}

		delay_ms(10);
		t++;
	}

	tftlcd_dev.forecolor = LCD_RED;
	lcd_fill(50, 370, 400, 400, tftlcd_dev.backcolor);
	
	lcd_show_string(50, 370, 300, 24, 24, (char*)"Current Time:");
	tftlcd_dev.forecolor = LCD_BLUE;

	while (1)
	{
		rtc_getdatetime(&rtcdate);
		sprintf(buf, "%d/ %d/ %d %d: %d: %d", 	rtcdate.year,
												rtcdate.month,
												rtcdate.day,
												rtcdate.hour,
												rtcdate.minute,
												rtcdate.second);
		lcd_fill(50, 400, 400, 400, tftlcd_dev.backcolor);
		lcd_show_string(50, 400, 250, 24, 24, buf);

		state = !state;
		led_switch(LED0, state);
		delay_ms(1000);
	}



	return 0;
}
