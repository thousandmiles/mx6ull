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
#include "bsp_backlight.h"
#include "/home/long/src/mx6ull/13_uart/stdio/include/stdio.h"


void imx6ul_hardfpu_enable(void)
{
	unsigned int cpacr;

	unsigned int fpexc;

	cpacr = __get_CPACR();

	cpacr = (cpacr & ~(CPACR_ASEDIS_Msk | CPACR_D32DIS_Msk)) | (3UL << CPACR_cp10_Pos) | (3UL << CPACR_cp11_Pos);

	__set_CPACR(cpacr);

	fpexc = __get_FPEXC();

	fpexc |= 0x40000000UL;

	__set_FPEXC(fpexc);
}


/*
 * @description	: main function
 * @param 		: None
 * @return 		: None
 */
int main(void)
{

	unsigned char keyvalue = 0;
	unsigned char i = 0;
	unsigned char state = OFF;
	unsigned char duty = 0;

	// initialze
	imx6ul_hardfpu_enable();
	int_init(); 				
	imx6u_clkinit();			
    delay_init();
	clk_enable();
	led_init();
	beep_init();
	uart_init();
	lcd_init();

	backlight_init();

	tftlcd_dev.forecolor = LCD_RED;

	lcd_show_string(50, 10, 400, 24, 24, (char*)"ALPHA-IMX6UL PWM BACKLIGHT TEST");

	lcd_show_string(50, 40, 200, 16, 16, (char*)"ATOM@ALIENTEK");

	lcd_show_string(50, 70, 200, 16, 16, (char*)"2023/11/17");

	lcd_show_string(50, 100, 400, 16, 16, (char*)"PWM Duty:   %");


	if (tftlcd_dev.id == ATKVGA)
	{
		duty = 100;
	}
	else
	{
		duty = 10;
	}

	lcd_shownum(50 + 72, 100, duty, 3, 16);

	pwm1_setduty(duty);

	while (1)
	{
		keyvalue = key_get_value();

		if (keyvalue == KEY0_VALUE)
		{
			duty += 10;
			if (duty > 100)
			{
				duty = 10;
			}
			lcd_shownum(50 + 72, 100, duty, 3, 16);
			pwm1_setduty(duty);
		}

		delay_ms(10);
		i++;
		if (i == 50)
		{
			i = 0;
			state = !state;
			led_switch(LED0, state);
		}

	}

	return 0;
}
