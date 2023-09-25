#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"

int main(void)
{
    clk_enable();
    led_init();
    beep_init();

    while (1)
    {
        led_switch(LED0, ON);
        beep_switch(OFF);
        delayer(1000);

        led_switch(LED0, OFF);
        beep_switch(ON);
        delayer(100);
    }

    return 0;
    
}