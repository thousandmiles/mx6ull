#ifndef _BSP_LCDAPI_H_

#include "imx6ull.h"
#include "bsp_lcd.h"

void lcd_draw_line(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);

void lcd_draw_rectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);

void lcd_draw_circle(unsigned short x0, unsigned short y0, unsigned char r);

void lcd_show_char(unsigned short x, unsigned short y, unsigned char num, unsigned char size, unsigned char mode);

unsigned int lcd_pow(unsigned char m, unsigned char n);

void lcd_show_num(unsigned short x, unsigned short y, unsigned int num, unsigned char len, unsigned char size);

void lcd_show_xnum(unsigned short x, unsigned short y, unsigned int num, unsigned char len, unsigned char size, unsigned char mode);

void lcd_show_string(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char size, char* p);


#endif // !_BSP_LCDAPI_H_