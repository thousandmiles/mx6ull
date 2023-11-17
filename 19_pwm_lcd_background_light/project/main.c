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
#include "bsp_ft5xx6.h"
#include "bsp_gt9147.h"
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
	gt9147_init();

	if (gt_init_fail == 1)
	{
		ft5426_init();
	}

	tftlcd_dev.forecolor = LCD_RED;

	lcd_show_string(50, 10, 400, 24, 24, (char*)"ALPHA-IMX6UL TOUCH SCREEN TEST");

	lcd_show_string(50, 40, 200, 16, 16, (char*)"TOUCH SCREEN TEST");

	lcd_show_string(50, 60, 200, 16, 16, (char*)"ATOM@ALIENTEK");

	lcd_show_string(50, 80, 200, 16, 16, (char*)"2023/11/16");


	lcd_show_string(50, 130, 400, 16, 16, (char*)"TP Num: ");

	lcd_show_string(50, 150, 200, 16, 16, (char*)"Point0 X: ");

	lcd_show_string(50, 170, 200, 16, 16, (char*)"Point0 Y: ");

	lcd_show_string(50, 190, 200, 16, 16, (char*)"Point1 X: ");

	lcd_show_string(50, 210, 200, 16, 16, (char*)"Point1 Y: ");

	lcd_show_string(50, 230, 200, 16, 16, (char*)"Point2 X: ");

	lcd_show_string(50, 250, 200, 16, 16, (char*)"Point2 Y: ");

	lcd_show_string(50, 270, 200, 16, 16, (char*)"Point3 X: ");

	lcd_show_string(50, 290, 200, 16, 16, (char*)"Point3 Y: ");

	lcd_show_string(50, 310, 200, 16, 16, (char*)"Point4 X: ");

	lcd_show_string(50, 330, 200, 16, 16, (char*)"Point4 Y: ");

	tftlcd_dev.forecolor = LCD_BLUE;

	while(1)
	{

		if (gt_init_fail == 1)
		{
			lcd_shownum(50 + 72, 130, ft5426_dev.point_num, 1, 16);

			lcd_shownum(50 + 72, 150, ft5426_dev.x[0], 5, 16);

			lcd_shownum(50 + 72, 170, ft5426_dev.y[0], 5, 16);

			lcd_shownum(50 + 72, 190, ft5426_dev.x[1], 5, 16);

			lcd_shownum(50 + 72, 210, ft5426_dev.y[1], 5, 16);

			lcd_shownum(50 + 72, 230, ft5426_dev.x[2], 5, 16);

			lcd_shownum(50 + 72, 250, ft5426_dev.y[2], 5, 16);

			lcd_shownum(50 + 72, 270, ft5426_dev.x[3], 5, 16);

			lcd_shownum(50 + 72, 290, ft5426_dev.y[3], 5, 16);

			lcd_shownum(50 + 72, 310, ft5426_dev.x[4], 5, 16);

			lcd_shownum(50 + 72, 330, ft5426_dev.y[4], 5, 16);
		}
		else
		{
			lcd_shownum(50 + 72, 130, gt9147_dev.point_num, 1, 16);

			lcd_shownum(50 + 72, 150, gt9147_dev.x[0], 5, 16);

			lcd_shownum(50 + 72, 170, gt9147_dev.y[0], 5, 16);

			lcd_shownum(50 + 72, 190, gt9147_dev.x[1], 5, 16);

			lcd_shownum(50 + 72, 210, gt9147_dev.y[1], 5, 16);

			lcd_shownum(50 + 72, 230, gt9147_dev.x[2], 5, 16);

			lcd_shownum(50 + 72, 250, gt9147_dev.y[2], 5, 16);

			lcd_shownum(50 + 72, 270, gt9147_dev.x[3], 5, 16);

			lcd_shownum(50 + 72, 290, gt9147_dev.y[3], 5, 16);

			lcd_shownum(50 + 72, 310, gt9147_dev.x[4], 5, 16);

			lcd_shownum(50 + 72, 330, gt9147_dev.y[4], 5, 16);
		}


		delay_ms(10);

		++i;

		if (i == 50)
		{
			i = 0;

			state = !state;

			led_switch(LED0, state);
		}
	
	}

	return 0;
}
