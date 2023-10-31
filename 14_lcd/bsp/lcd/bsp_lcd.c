#include "bsp_lcd.h"
#include "bsp_gpio.h"
#include "bsp_delay.h"
#include "/home/long/src/mx6ull/13_uart/stdio/include/stdio.h"

struct tftlcd_typedef tftlcd__dev;

void lcd_init(void)
{
    lcdgpio_init();

    lcdclk_init(32, 3, 5);

    lcd_reset();

    delay_ms(10);

    lcd_noreset();

    tftlcd__dev.height = 600;
    tftlcd__dev.width = 1024;
    tftlcd__dev.pixsize = 4;
    tftlcd__dev.vspw = 3;
    tftlcd__dev.vbpd = 20;
    tftlcd__dev.vfpd = 12;
    tftlcd__dev.hspw = 20;
    tftlcd__dev.hbpd = 140;
    tftlcd__dev.hfpd = 160;
    tftlcd__dev.framebuffer = LCD_FRAMEBUF_ADDR;
    tftlcd__dev.backcolor = LCD_WHITE;
    tftlcd__dev.forecolor = LCD_BLACK;

    LCDIF->CTRL |= (1<<19) | (1<<17) | (0<<14) | (0<<12) | (3<<10) | (3<<8) | (1<<5) | (0<<1);

    LCDIF->CTRL1 = 7<<16;

    LCDIF->TRANSFER_COUNT = (tftlcd__dev.height<<16) | (tftlcd__dev.width<<0);

    LCDIF->VDCTRL0 = 0;

    LCDIF->VDCTRL0 = (0<<29) | (1<<28) | (0<<27) | (0<<26) | (0<<25) | (1<<24) | (1<<21) | (1<<20) |(tftlcd__dev.vspw<<0);

    LCDIF->VDCTRL1 = tftlcd__dev.height + tftlcd__dev.vspw + tftlcd__dev.vfpd + tftlcd__dev.vbpd;

    LCDIF->VDCTRL2 = (tftlcd__dev.hspw<<18) | (tftlcd__dev.hspw + tftlcd__dev.width + tftlcd__dev.hfpd + tftlcd__dev.hbpd);

    LCDIF->VDCTRL3 = ((tftlcd__dev.hbpd + tftlcd__dev.hspw) << 16) | (tftlcd__dev.vbpd + tftlcd__dev.vspw);

    LCDIF->VDCTRL4 = (1<<18) | tftlcd__dev.width;

    LCDIF->CUR_BUF = (unsigned int)tftlcd__dev.framebuffer;

    LCDIF->NEXT_BUF = (unsigned int)tftlcd__dev.framebuffer;

    lcd_enable();

    delay_ms(10);

    lcd_clear(LCD_WHITE);

}