#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include "imx6ull.h"

void delayer(volatile unsigned int n);

void delay_init(void);

void delay_us(uint32_t usdelay);

void delay_ms(uint32_t msdelay);

#define MAX_CNT 0XFFFFFFFF

#endif // !__BSP_DELAY_H
