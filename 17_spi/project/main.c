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
#include "bsp_icm20608.h"
#include "bsp_spi.h"
#include "/home/long/src/mx6ull/13_uart/stdio/include/stdio.h"
 

unsigned int BACKCOLOR[6] = {LCD_BLUE, LCD_GREEN, LCD_RED, LCD_ORANGE, LCD_BLACK, LCD_WHITE};


void integer_display(unsigned short x, unsigned short y, unsigned char size, signed int num)
{
	char buf[200];

	lcd_fill(x, y, x + 50, y + size, tftlcd_dev.backcolor);

	memset(buf, 0, sizeof(buf));

	if (num < 0)
	{
		sprintf(buf, "-%d", -num);
	}
	else
	{
		sprintf(buf, "%d", num);
	}

	lcd_show_string(x, y, 50, size, size, buf);
}


void decimals_display(unsigned short x, unsigned short y, unsigned char size, signed int num)
{
	signed int integ;

	signed int fract;

	signed int uncomptemp = num;

	char buf[200];

	if (num < 0)
	{
		uncomptemp = -uncomptemp;
	}

	integ = uncomptemp /100;

	fract = uncomptemp % 100;

	memset(buf, 0, sizeof(buf));

	if (num < 0)
	{
		sprintf(buf, "-%d.%d", integ, fract);
	}
	else
	{
		sprintf(buf, "%d.%d", integ, fract);
	}

	lcd_fill(x, y, x + 60, y + size, tftlcd_dev.backcolor);

	lcd_show_string(x, y, 60, size, size, buf);
}


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

	unsigned char state = OFF;

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

	tftlcd_dev.forecolor = LCD_RED;

	lcd_show_string(50, 10, 400, 24, 24, (char*)"ALPHA-IMX6UL SPI TEST");

	lcd_show_string(50, 40, 200, 16, 16, (char*)"ICM20608 TEST");

	lcd_show_string(50, 60, 200, 16, 16, (char*)"ATOM@ALIENTEK");

	lcd_show_string(50, 80, 200, 16, 16, (char*)"2023/11/13");


	while (icm20608_init())
	{
		lcd_show_string(50, 100, 200, 16, 16, (char*)"ICM20608 Check Failed!");

		delay_ms(500);

		lcd_show_string(50, 100, 200, 16, 16, (char*)"Please Check         ");

		delay_ms(500);
	}


	lcd_show_string(50, 100, 200, 16, 16, (char*)"ICM20608 Ready!");
	


	lcd_show_string(50, 130, 200, 16, 16, (char*)"accel x: ");

	lcd_show_string(50, 150, 200, 16, 16, (char*)"accel y: ");

	lcd_show_string(50, 170, 200, 16, 16, (char*)"accel z: ");

	lcd_show_string(50, 190, 200, 16, 16, (char*)"gyro  x: ");

	lcd_show_string(50, 210, 200, 16, 16, (char*)"gyro  x: ");

	lcd_show_string(50, 230, 200, 16, 16, (char*)"gyro  z: ");

	lcd_show_string(50, 250, 200, 16, 16, (char*)"temp   : ");



	lcd_show_string(50 + 181, 130, 200, 16, 16, (char*)"g");

	lcd_show_string(50 + 181, 150, 200, 16, 16, (char*)"g");

	lcd_show_string(50 + 181, 170, 200, 16, 16, (char*)"g");

	lcd_show_string(50 + 181, 190, 200, 16, 16, (char*)"o/s");

	lcd_show_string(50 + 181, 210, 200, 16, 16, (char*)"o/s");

	lcd_show_string(50 + 181, 230, 200, 16, 16, (char*)"o/s");

	lcd_show_string(50 + 181, 250, 200, 16, 16, (char*)"C");


	tftlcd_dev.forecolor = LCD_BLUE;

	while(1)
	{

		icm20608_getdata();

		integer_display(50 + 70, 130, 16, icm20608_dev.accel_x_adc);

		integer_display(50 + 70, 150, 16, icm20608_dev.accel_y_adc);

		integer_display(50 + 70, 170, 16, icm20608_dev.accel_z_adc);

		integer_display(50 + 70, 190, 16, icm20608_dev.gyro_x_adc);

		integer_display(50 + 70, 210, 16, icm20608_dev.gyro_y_adc);

		integer_display(50 + 70, 230, 16, icm20608_dev.gyro_z_adc);

		integer_display(50 + 70, 250, 16, icm20608_dev.temp_adc);



		integer_display(50 + 70 + 50, 130, 16, icm20608_dev.accel_x_act);

		integer_display(50 + 70 + 50, 150, 16, icm20608_dev.accel_y_act);

		integer_display(50 + 70 + 50, 170, 16, icm20608_dev.accel_z_act);

		integer_display(50 + 70 + 50, 190, 16, icm20608_dev.gyro_x_act);

		integer_display(50 + 70 + 50, 210, 16, icm20608_dev.gyro_y_act);

		integer_display(50 + 70 + 50, 230, 16, icm20608_dev.gyro_z_act);

		integer_display(50 + 70 + 50, 250, 16, icm20608_dev.temp_act);



		delay_ms(120);
		state = !state;
		led_switch(LED0, state);
	
	}

	return 0;
}
