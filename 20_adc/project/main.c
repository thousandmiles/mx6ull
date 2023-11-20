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
#include "bsp_adc.h"
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
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{

	unsigned char i = 0;
	unsigned int adcvalue = 0;
	unsigned char state = OFF;
	signed int integ;
	signed int fract;

	// initialze
	imx6ul_hardfpu_enable();
	int_init(); 					/* initialize system interrupt		*/
	imx6u_clkinit();				/* initialize system clock 			*/
    delay_init();
	clk_enable();
	led_init();
	beep_init();
	uart_init();
	lcd_init();

	adc1ch1_init();

	tftlcd_dev.forecolor = LCD_RED;

	lcd_show_string(50, 10, 400, 24, 24, (char*)"ALPHA-IMX6UL PWM ADC TEST");

	lcd_show_string(50, 40, 200, 16, 16, (char*)"ATOM@ALIENTEK");

	lcd_show_string(50, 60, 200, 16, 16, (char*)"2023/11/20");

	lcd_show_string(50, 90, 400, 16, 16, (char*)"ADC ori Value: 0000");

	lcd_show_string(50, 110, 400, 16, 16, (char*)"ADC Val Value: 0.00 V");

	tftlcd_dev.forecolor = LCD_BLUE;

	while (1)
	{
		adcvalue = getadc_average(5);

		lcd_showxnum(162, 90, adcvalue, 4, 16, 0);

		adcvalue = getadc_volt();

		integ = adcvalue / 1000;

		fract = adcvalue % 1000;

		lcd_showxnum(162, 110, integ, 1, 16, 0);

		lcd_showxnum(178, 110, fract, 3, 16, 0x80);

		delay_ms(50);

		i++;

		if (i == 10)
		{
			i = 0;

			state = !state;

			led_switch(LED0, state);
		}

	}

	return 0;
}
