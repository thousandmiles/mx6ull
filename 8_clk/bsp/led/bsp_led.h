#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "imx6ull.h"

#define LED0 0
#define ON   1
#define OFF  0

void led_init(void);
void led_switch(int led, int status);

#endif // !BSP_LED_H#define 