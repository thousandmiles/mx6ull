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

void lcdgpio_init(void)
{
    gpio_pin_config_t gpio_config;

    IOMUXC_SetPinMux(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0);

    IOMUXC_SetPinMux(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_GPIO1_IO08, 0);

    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0xB9);

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

    CCM_ANALOG->PLL_VIDEO = (2<<19) | (1<<13) | (loopDiv<<0);

    CCM_ANALOG->MISC2 &= ~(3<<30);
    CCM_ANALOG->MISC2 = 0<<30;

    CCM->CSCDR2 &= ~(7<<15);
    CCM->CSCDR2 |= (2<<15);

    CCM->CSCDR2 &= ~(7<<12);
    CCM->CSCDR2 |= (prediv - 1)<<12;

    CCM->CBCMR &= ~(7<<23);
    CCM->CBCMR |= (div - 1)<<23;

    CCM->CSCDR2 &= ~(7<<9);
    CCM->CSCDR2 |= (0<<9);

}

void lcd_reset(void)
{
    LCDIF->CTRL = 1<<31;
}

void lcd_noreset(void)
{
    LCDIF->CTRL = 0<<31;
}

void lcd_enable(void)
{
    LCDIF->CTRL1 |= 1<<0;
}

inline void lcd_drawpoint(unsigned short x, unsigned short y, unsigned int color)
{
    *(unsigned int *)((unsigned int)tftlcd__dev.framebuffer + tftlcd__dev.pixsize * (tftlcd__dev*y + x)) = color;
}

inline unsigned int lcd_readpoint(unsigned short x, unsigned short y)
{
    return *(unsigned int *)((unsigned int)tftlcd__dev.framebuffer + tftlcd__dev.pixsize * (tftlcd__dev*y + x));
}

void lcd_clear(unsigned int color)
{
    unsigned int num;
    unsigned int i = 0;

    unsigned int * startaddr = (unsigned int *)tftlcd__dev.framebuffer;
    num = (unsigned int)(tftlcd__dev.width * tftlcd__dev.height);

    for (int i = 0; i < num; ++i)
    {
        startaddr[i] = color;
    }
}

void lcd_fill(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned int color)
{
    unsigned int x, y;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > tftlcd__dev.width) x1 = tftlcd__dev.width - 1;
    if (y1 > tftlcd__dev.height) y1 = tftlcd__dev.height - 1;

    for (y = y0; y <= y1; ++y)
    {
        for (x = x0; x <= x1; ++x)
        {
            lcd_drawpoint(x, y, color);
        }
    }
}
