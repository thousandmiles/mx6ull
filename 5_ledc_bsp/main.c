#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"

int main(void)
{
    clk_enable();
    led_init();

    while (1)
    {
        led_switch(LED0, ON);

        delayer(1000);

        led_switch(LED0, OFF);

        delayer(100);
    }

    return 0;
    
}