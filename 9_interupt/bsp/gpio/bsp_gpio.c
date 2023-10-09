#include "bsp_gpio.h"

/*

gpio_init()         - Initializes GPIO

gpio_pinread()      - Read value from GPIO

gpio_pinwrite()     - Write value to GPIO

*/

void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t* config)
{
    gpio_disableint(base, pin);                         // disable interrupt

    if (config->direction == kGPIO_DigitalInput)        // digital input
    {
        base->GDIR &= ~(1<<pin);
    }
    else                                                // digital output
    {
        base->GDIR |= (1<<pin);
        gpio_pinwrite(base, pin, config->outputLogic);  // default output
    }

    gpio_intconfig(base, pin, config->interruptMode);
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

void gpio_intconfig(GPIO_Type* base, uint16_t pin, gpio_interupt_mode_t pinInterruptMode)
{
    volatile uint32_t* icr;
    uint32_t icrShift;

    icrShift = pin;

    base->EDGE_SEL &= ~(1<<pin);

    if (pin < 16)   // ICR1 register
    {
        icr = &(base->ICR1);
    }
    else
    {
        icr = &(base->ICR2);
        icrShift -= 16;
    }

    switch (pinInterruptMode)
    {
        case(kGPIO_IntLowLevel):
        {
            *icr &= ~(3<<(2*icrShift));
            break;
        }
        case(kGPIO_IntHighLevel):
        {
            *icr = (*icr & (~(3<<(2*icrShift)))) | (1<<(2*icrShift));
            break;
        }
        case(kGPIO_IntRisingEdge):
        {
            *icr = (*icr & (~(3<<(2*icrShift)))) | (2<<(2*icrShift));
            break;
        }
        case(kGPIO_IntFallingEdge):
        {
            *icr |= (3<<(2*icrShift));
            break;
        }
        case(kGPIO_IntRisingOrFallingEdge):
        {
            base->EDGE_SEL |= (1<<pin);
        }
        default:
            break;
    }
}

void gpio_enableint(GPIO_Type* base, uint16_t pin)
{
    base->IMR |= (1<<pin);
}

void gpio_disableint(GPIO_Type* base, uint16_t pin)
{
    base->IMR &= ~(1<<pin);
}

void gpio_clearintflags(GPIO_Type* base, uint16_t pin)
{
    base->ISR |= (1<<pin);
}