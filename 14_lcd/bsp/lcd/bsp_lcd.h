#ifndef _BSP_LCD_H_
#define _BSP_LCD_H_

#include "imx6ull.h"

#define LCD_BLACK   0X00000000

#define LCD_WHITE   0X00FFFFFF

#define LCD_BLUE    0X000000FF

#define LCD_GREEN   0X0000FF00

#define LCD_RED     0X00FF0000

#define LCD_ORANGE  0X00FFA500

#define LCD_TRANSPARENT 0X00000000

#define LCD_FRAMEBUF_ADDR (0X89000000)

struct tftlcd_typedef{
    unsigned short height;
    unsigned short width;
    unsigned char pixsize;
    unsigned short vspw;
    unsigned short vbpd;
    unsigned short vfpd;
    unsigned short hspw;
    unsigned short hbpd;
    unsigned short hfpd;
    unsigned int framebuffer;
    unsigned int forecolor;
    unsigned int backcolor;
};

extern struct tftlcd_typedef tftlcd__dev;

void lcd_init(void);

void lcdgpio_init(void);

void lcdclk_init(unsigned char loopDiv, unsigned char prediv, unsigned char div);

void lcd_reset(void);

void lcd_noreset(void);

void lcd_enable(void);

//void video_pllinit(unsigned char loopdivi, unsigned char posydivi);

inline void lcd_drawpoint(unsigned short x, unsigned short y, unsigned int color);

inline unsigned int lcd_readpoint(unsigned short x, unsigned short y);

void lcd_clear(unsigned int color);

void lcd_fill(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned int color);

#endif // !_BSP_LCD_H_


