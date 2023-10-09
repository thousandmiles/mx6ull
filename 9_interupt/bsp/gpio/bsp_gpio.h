#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_

#include "imx6ull.h"

typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput = 0U,        
    kGPIO_DigitalOutput = 1U,
}gpio_pin_direction_t;


/* interupt enum */
typedef enum _gpio_interupt_mode
{
    kGPIO_NoIntmode = 0U,       /* no interupt */
    kGPIO_IntLowLevel = 1U, 
    kGPIO_IntHighLevel = 2U,
    kGPIO_IntRisingEdge = 3U,
    kGPIO_IntFallingEdge = 4U,
    kGPIO_IntRisingOrFallingEdge = 5U,
}gpio_interupt_mode_t;


typedef struct _gpio_pin_config
{
    gpio_pin_direction_t direction;
    uint8_t outputLogic;
    gpio_interupt_mode_t interruptMode;
}gpio_pin_config_t;

void gpio_init(GPIO_Type* base, int pin, gpio_pin_config_t* config);

int gpio_pinread(GPIO_Type* base, int pin);

void gpio_pinwrite(GPIO_Type* base, int pin, int value);

void gpio_intconfig(GPIO_Type* base, uint16_t pin, gpio_interupt_mode_t pinInterruptMode);

void gpio_enableint(GPIO_Type* base, uint16_t pin);

void gpio_disableint(GPIO_Type* base, uint16_t pin);

void gpio_clearintflags(GPIO_Type* base, uint16_t pin);

#endif // !