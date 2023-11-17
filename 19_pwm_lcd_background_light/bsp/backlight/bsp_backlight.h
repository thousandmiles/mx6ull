#ifndef _BSP_BACKLIGHT_H_
#define _BSP_BACKLIGHT_H_
#include "imx6ull.h"

struct backlight_dev_struct
{
    unsigned char pwm_duty;
};

void backlight_init(void);

void pwm1_enable(void);

void pwm1_setsample_value(unsigned int value);

void pwm1_setperiod_value(unsigned int value);

void pwm1_setduty(unsigned char duty);

void pwm1_irqhandler(void);


#endif