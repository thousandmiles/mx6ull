#include "bsp_gpio.h"

/*

gpio_init()         - Initializes GPIO

gpio_pinread()      - Read value from GPIO

gpio_pinwrite()     - Write value to GPIO

*/

void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t* config)
{
    if (config->direction == kGPIO_DigitalInput)        // digital input
    {
        base->GDIR &= ~(1<<pin);
    }
    else                                                // digital output
    {
        base->GDIR |= (1<<pin);
        gpio_pinwrite(base, pin, config->outputLogic);  // default output
    }
}

// read DR register value
int gpio_pinread(GPIO_Type *base, int pin)
{
    return (((base->DR)>>pin) & 0x1);
}

void gpio_pinwrite(GPIO_Type *base, int pin, int value)
{
    if (value == 0U)
    {
        base->DR &= ~(1<<pin);  // output 0
    }
    else
    {
        base->DR |= (1<<pin);   // output 1
    }
}
