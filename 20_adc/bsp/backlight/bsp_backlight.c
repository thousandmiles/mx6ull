#include "bsp_backlight.h"
#include "bsp_int.h"

struct backlight_dev_struct backlight_dev;

void backlight_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_PWM1_OUT, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_PWM1_OUT, 0xB090);

    PWM1->PWMCR = 0;
    PWM1->PWMCR |= (1<<26) | (1<<16) | (65<<4);

    pwm1_setperiod_value(1000);

    backlight_dev.pwm_duty = 50;

    for (int i = 0; i < 4; i++)
    {
        pwm1_setduty(backlight_dev.pwm_duty);
    }

    PWM1->PWMIR |= 1<<0;

    system_register_irqhandler(PWM1_IRQn, (system_irq_handler_t)pwm1_irqhandler, NULL);

    GIC_EnableIRQ(PWM1_IRQn);

    PWM1->PWMSR = 0;

    pwm1_enable();
}

void pwm1_enable(void)
{
    PWM1->PWMCR |= 1<<0;
}

void pwm1_setsample_value(unsigned int value)
{
    PWM1->PWMSAR = (value & 0xFFFF);
}

void pwm1_setperiod_value(unsigned int value)
{
    unsigned int regvalue = 0;

    if (value < 2)
    {
        regvalue = 2;
    }
    else
    {
        regvalue = value - 2;
    }

    PWM1->PWMPR = (regvalue & 0xFFFF);
}

void pwm1_setduty(unsigned char duty)
{
    unsigned short preiod;

    unsigned short sample;

    backlight_dev.pwm_duty = duty;

    preiod = PWM1->PWMPR + 2;

    sample = preiod * backlight_dev.pwm_duty / 100;

    pwm1_setsample_value(sample);
}

void pwm1_irqhandler(void)
{
    if (PWM1->PWMSR & (1<<3))
    {
        pwm1_setduty(backlight_dev.pwm_duty);
        PWM1->PWMSR |= (1<<3);
    }
}