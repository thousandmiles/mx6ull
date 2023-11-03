#include "bsp_lcd.h"
#include "bsp_gpio.h"
#include "bsp_delay.h"
#include "stdio.h"


struct tftlcd_typedef tftlcd_dev;

void lcd_init(void)
{

	lcdgpio_init(); 
	lcd_reset();	
	delay_ms(10);	
	lcd_noreset();	

	tftlcd_dev.height = 600;
	tftlcd_dev.width = 1024;
	tftlcd_dev.vspw = 3;
	tftlcd_dev.vbpd = 20;
	tftlcd_dev.vfpd = 12;
	tftlcd_dev.hspw = 20;
	tftlcd_dev.hbpd = 140;
	tftlcd_dev.hfpd = 160;
	lcdclk_init(32, 3, 5); 

	tftlcd_dev.pixsize = 4; 
	tftlcd_dev.framebuffer = LCD_FRAMEBUF_ADDR;
	tftlcd_dev.backcolor = LCD_WHITE; 
	tftlcd_dev.forecolor = LCD_BLACK; 


	LCDIF->CTRL |= (1 << 19) | (1 << 17) | (0 << 14) | (0 << 12) |
				   (3 << 10) | (3 << 8) | (1 << 5) | (0 << 1);

	LCDIF->CTRL1 = 0X7 << 16;


	LCDIF->TRANSFER_COUNT = (tftlcd_dev.height << 16) | (tftlcd_dev.width << 0);


	LCDIF->VDCTRL0 = 0; 

	LCDIF->VDCTRL0 = (0 << 29) | (1 << 28) | (0 << 27) |
						(0 << 26) | (0 << 25) | (1 << 24) |
						(1 << 21) | (1 << 20) | (tftlcd_dev.vspw << 0);


	LCDIF->VDCTRL1 = tftlcd_dev.height + tftlcd_dev.vspw + tftlcd_dev.vfpd + tftlcd_dev.vbpd; // VSYNC閸涖劍婀�


	LCDIF->VDCTRL2 = (tftlcd_dev.hspw << 18) | (tftlcd_dev.width + tftlcd_dev.hspw + tftlcd_dev.hfpd + tftlcd_dev.hbpd);

	LCDIF->VDCTRL3 = ((tftlcd_dev.hbpd + tftlcd_dev.hspw) << 16) | (tftlcd_dev.vbpd + tftlcd_dev.vspw);

	LCDIF->VDCTRL4 = (1 << 18) | (tftlcd_dev.width);


	LCDIF->CUR_BUF = (unsigned int)tftlcd_dev.framebuffer;
	LCDIF->NEXT_BUF = (unsigned int)tftlcd_dev.framebuffer;

	lcd_enable(); /* 娴ｈ儻鍏楲CD 	*/
	delay_ms(10);
	lcd_clear(LCD_WHITE); /* 濞撳懎鐫� 		*/
}

unsigned short lcd_read_panelid(void)
{
	unsigned char idx = 0;

	IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0);
	IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0X10B0);

	gpio_pin_config_t idio_config;
	idio_config.direction = kGPIO_DigitalOutput;
	idio_config.outputLogic = 1;
	gpio_init(GPIO3, 3, &idio_config);

	IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_GPIO3_IO12, 0); /* B7(M2) */
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_GPIO3_IO20, 0); /* G7(M1) */
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_GPIO3_IO28, 0); /* R7(M0) */

	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_GPIO3_IO12, 0xF080);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_GPIO3_IO20, 0xF080);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_GPIO3_IO28, 0xF080);

	idio_config.direction = kGPIO_DigitalInput;
	gpio_init(GPIO3, 12, &idio_config);
	gpio_init(GPIO3, 20, &idio_config);
	gpio_init(GPIO3, 28, &idio_config);

	idx = (unsigned char)gpio_pinread(GPIO3, 28);		
	idx |= (unsigned char)gpio_pinread(GPIO3, 20) << 1; 
	idx |= (unsigned char)gpio_pinread(GPIO3, 12) << 2; 

	if (idx == 0)
		return ATK4342; 
	else if (idx == 1)
		return ATK7084; 
	else if (idx == 2)
		return ATK7016; 
	else if (idx == 4)
		return ATK4384;
	else if (idx == 5)
		return ATK1018; 
	else if (idx == 7)
		return ATKVGA; 
	else
		return 0;
}

void lcdgpio_init(void)
{
	gpio_pin_config_t gpio_config;


	IOMUXC_SetPinMux(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA04_LCDIF_DATA04, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA05_LCDIF_DATA05, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA06_LCDIF_DATA06, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_LCDIF_DATA07, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA08_LCDIF_DATA08, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA09_LCDIF_DATA09, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA10_LCDIF_DATA10, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA11_LCDIF_DATA11, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA12_LCDIF_DATA12, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA13_LCDIF_DATA13, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA14_LCDIF_DATA14, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_LCDIF_DATA15, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA16_LCDIF_DATA16, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA17_LCDIF_DATA17, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA18_LCDIF_DATA18, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA19_LCDIF_DATA19, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA20_LCDIF_DATA20, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA21_LCDIF_DATA21, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA22_LCDIF_DATA22, 0);
	// IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_LCDIF_DATA23, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_CLK_LCDIF_CLK, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0);
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_GPIO1_IO08, 0); 


	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA04_LCDIF_DATA04, 0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA05_LCDIF_DATA05, 0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA06_LCDIF_DATA06, 0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_LCDIF_DATA07, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA08_LCDIF_DATA08, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA09_LCDIF_DATA09, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA10_LCDIF_DATA10, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA11_LCDIF_DATA11, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA12_LCDIF_DATA12, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA13_LCDIF_DATA13, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA14_LCDIF_DATA14, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_LCDIF_DATA15, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA16_LCDIF_DATA16, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA17_LCDIF_DATA17, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA18_LCDIF_DATA18, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA19_LCDIF_DATA19, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA20_LCDIF_DATA20, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA21_LCDIF_DATA21, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA22_LCDIF_DATA22, 0xB9);
	// IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_LCDIF_DATA23, 0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_CLK_LCDIF_CLK, 0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0xB9);
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_GPIO1_IO08, 0xB9); 


	gpio_config.direction = kGPIO_DigitalOutput; 
	gpio_config.outputLogic = 1;				 
	gpio_init(GPIO1, 8, &gpio_config);			
	gpio_pinwrite(GPIO1, 8, 1);				
}


void lcdclk_init(unsigned char loopDiv, unsigned char prediv, unsigned char div)
{

	CCM_ANALOG->PLL_VIDEO_NUM = 0; 
	CCM_ANALOG->PLL_VIDEO_DENOM = 0;


	CCM_ANALOG->PLL_VIDEO = (2 << 19) | (1 << 13) | (loopDiv << 0);


	CCM_ANALOG->MISC2 &= ~(3 << 30);
	CCM_ANALOG->MISC2 = 0 << 30;

	CCM->CSCDR2 &= ~(7 << 15);
	CCM->CSCDR2 |= (2 << 15); 

	CCM->CSCDR2 &= ~(7 << 12);
	CCM->CSCDR2 |= (prediv - 1) << 12; 


	CCM->CBCMR &= ~(7 << 23);
	CCM->CBCMR |= (div - 1) << 23;


	CCM->CSCDR2 &= ~(7 << 9); 
	CCM->CSCDR2 |= (0 << 9); 
}


void lcd_reset(void)
{
	LCDIF->CTRL = 1 << 31;
}


void lcd_noreset(void)
{
	LCDIF->CTRL = 0 << 31; /* 閸欐牗绉峰鍝勫煑婢跺秳缍� */
}


void lcd_enable(void)
{
	LCDIF->CTRL |= 1 << 0; /* 娴ｈ儻鍏楨LCDIF */
}

void lcd_drawpoint(unsigned short x, unsigned short y, unsigned int color)
{
	*(unsigned int *)((unsigned int)tftlcd_dev.framebuffer +
					  tftlcd_dev.pixsize * (tftlcd_dev.width * y + x)) = color;
}


unsigned int lcd_readpoint(unsigned short x, unsigned short y)
{
	return *(unsigned int *)((unsigned int)tftlcd_dev.framebuffer +
							 tftlcd_dev.pixsize * (tftlcd_dev.width * y + x));
}


void lcd_clear(unsigned int color)
{
	unsigned int num;
	unsigned int i = 0;

	unsigned int *startaddr = (unsigned int *)tftlcd_dev.framebuffer; // 閹稿洤鎮滅敮褏绱︾€涙﹢顩婚崷鏉挎絻
	num = (unsigned int)tftlcd_dev.width * tftlcd_dev.height;		  // 缂傛挸鍟块崠鐑樷偓濠氭毐鎼达拷
	for (i = 0; i < num; i++)
	{
		startaddr[i] = color;
	}
}


void lcd_fill(unsigned short x0, unsigned short y0,
			  unsigned short x1, unsigned short y1, unsigned int color)
{
	unsigned short x, y;

	if (x0 < 0)
		x0 = 0;
	if (y0 < 0)
		y0 = 0;
	if (x1 >= tftlcd_dev.width)
		x1 = tftlcd_dev.width - 1;
	if (y1 >= tftlcd_dev.height)
		y1 = tftlcd_dev.height - 1;

	for (y = y0; y <= y1; y++)
	{
		for (x = x0; x <= x1; x++)
			lcd_drawpoint(x, y, color);
	}
}
