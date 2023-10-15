#include "bsp_key.h"
#include "bsp_gpio.h"
#include "bsp_delay.h"

void key_init(void)
{
    gpio_pin_config_t key_config;

    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);

    /*
    Configure the GPIO1_IO18: IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03
    --bit [16]:0            HYS: disabled
    --bit [15:14]:11        PUS: 22 k 
    --bit [13]:1            PUE: pull
    --bit [12]:1            PKE: pull/keeper enable
    --bit [11]:0            ODE: open circuit disabled
    --bit [7:6]:10          SPEED: moderate
    --bit [5:3]:000         DSE: R0/R6
    --bit [0]:0             SRE: low rate

    bit[16:0]: 0    1   1   1   1   0   0   0   0   1   0   0   0   0   0   0   0
    bit        16   15  14  13  12  11  10  9   8   7   6   5   4   3   2   1   0
               0    -------F------  ------0------   ------8------   ------0------
    --> F080
    */

    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    key_config.direction = kGPIO_DigitalInput;
    gpio_init(GPIO1, 18, &key_config);
}


/*
return 0 represents that key was not pressed.
*/
int key_get_value(void)
{
    int ret = 0;
    static uint8_t release = 1;                                 // up/down flag

    if ((release == 1)&&(gpio_pinread(GPIO1, 18) == 0))         // key is pressed down
    {   
        delayer(10);                                            // anti-shake
        release = 0;                                            // flag reverse
        if (gpio_pinread(GPIO1, 18) == 0)
        {
            ret = KEY0_VALUE;
        }
    }
    else if (gpio_pinread(GPIO1, 18) == 1)                      // key is not pressed
    {
        ret = 0;
        release = 1;
    }
    return ret;
}


