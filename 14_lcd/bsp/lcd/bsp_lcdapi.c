#include "bsp_lcdapi.h"
#include "font.h"

// Bresenham algorithm
void lcd_draw_line(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
    unsigned short t;

    int xerr = 0, yerr = 0, delta_x, delta_y, distance;

    int incx, incy, uRow, uCol;

    delta_x = x2 - x1;

    delta_y = y2 - y1;

    uRow = x1;

    uCol = y1;

    if (delta_x > 0) incx = 1;
    else if (delta_x == 0) incx = 0;
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0) incy = 1;
    else if (delta_y == 0) incy = 0;
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y) distance = delta_x;
    else distance = delta_y;

    for (t = 0; t <= distance; t++)
    {
        lcd_drawpoint(uRow, uCol, tftlcd__dev.forecolor);
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

void lcd_draw_rectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
    lcd_draw_line(x1, y1, x2, y1);
    lcd_draw_line(x1, y1, x1, y2);
    lcd_draw_line(x1, y2, x2, y2);
    lcd_draw_line(x2, y1, x2, y2);
}

void lcd_draw_circle(unsigned short x0, unsigned short y0, unsigned char r)
{
    int mx = x0, my = y0;

    int x = 0, y = r;

    int d = 1 - r;

    while (y > x)
    {
        lcd_drawpoint(x + mx, y + my, tftlcd__dev.forecolor);
        lcd_drawpoint(y + mx, x + my, tftlcd__dev.forecolor);
        lcd_drawpoint(-x + mx, y + my, tftlcd__dev.forecolor);
        lcd_drawpoint(-y + mx, x + my, tftlcd__dev.forecolor);

        lcd_drawpoint(-x + mx, -y + my, tftlcd__dev.forecolor);
        lcd_drawpoint(-y + mx, -x + my, tftlcd__dev.forecolor);
        lcd_drawpoint(x + mx, -y + my, tftlcd__dev.forecolor);
        lcd_drawpoint(y + mx, -x + my, tftlcd__dev.forecolor);

        if (d < 0)
        {
            d = d + 2 * x + 3;
        }
        else
        {
            d = d + 2 * (x - y) +5;
            y--;
        }
        x++;
    }
}

void lcd_show_char(unsigned short x, unsigned short y, unsigned char num, unsigned char size, unsigned char mode)
{
    unsigned char temp, t1, t;

    unsigned short y0 = y;

    unsigned char csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

    num = num - ' ';    // Obtain the show char ASCII value

    for (t = 0; t < csize; t++)
    {
        if (size == 12) temp = asc2_1206[num][t];
        else if (size == 16) temp = asc2_1608[num][t];
        else if (size == 24) temp = asc2_2412[num][t];
        else if (size == 32) temp = asc2_3216[num][t];
        else return;

        for (t1 = 0; t1 < 8; t1++)
        {
            // 0x80 is 128
            if (temp & 0x80) lcd_drawpoint(x, y, tftlcd__dev.forecolor);
            else if (mode == 0) lcd_drawpoint(x, y, tftlcd__dev.backcolor);

            temp <<= 1;

            y++;

            if (y >= tftlcd__dev.height) return;
            if ((y - y0) == size)
            {
                y = y0;
                x++;
                if (x >= tftlcd__dev.width) return;
                break;
            }
        }
    }

}

// Calculating m^n
unsigned int lcd_pow(unsigned char m, unsigned char n)
{
    unsigned int result = 1;
    while (n--) result *= m;
    return result;
}

void lcd_show_num(unsigned short x, unsigned short y, unsigned int num, unsigned char len, unsigned char size)
{
    unsigned char t, temp;
    unsigned char enshow = 0;

    for (t = 0; t < len; t++)
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                lcd_show_char(x + (size / 2) * t, y, ' ', size, 0);
                continue;
            }
            else enshow = 1;
        }
        lcd_show_char(x + (size / 2) * t, y, temp + '0', size, 0);
    }
}

void lcd_show_xnum(unsigned short x, unsigned short y, unsigned int num, unsigned char len, unsigned char size, unsigned char mode)
{
    unsigned char t, temp;
    unsigned char enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                if (mode & 0X80) 
                {
                    lcd_show_char(x + (size / 2) * t, y, '0', size, mode & 0x01);
                }
                else
                {
                    lcd_show_char(x + (size / 2) * t, y, ' ', size, mode & 0x01);
                }
                continue;
            }
            else
            {
                enshow = 1;
            }
        }
        lcd_show_char( x + (size / 2) * t, y, temp + '0', size, mode & 0x01);
    }
}

void lcd_show_string(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char size, char* p)
{
    unsigned char x0 = x;
    width += x;
    height += y;
    while ((*p <= '~') && (*p >= ' '))
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }
        if (y >= height) break;
        lcd_show_char(x, y, *p, size, 0);
        x += size / 2;
        p++;

    }
}