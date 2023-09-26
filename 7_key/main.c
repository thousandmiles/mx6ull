#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"

int main(void)
{

    int key_value = 0;
    uint8_t led_state = OFF;
    uint8_t beep_state = OFF;

    clk_enable();
    led_init();
    beep_init();
    key_init();

    int i = 0;
    while (1)
    {
        key_value = key_get_value();
        if (key_value)
        {
            switch (key_value)
            {
                case KEY0_VALUE: 
                    beep_state = !beep_state;
                    beep_switch(beep_state);
                    break;
            }
        }

        if (i%2 == 0)
        {
            led_state = !led_state;
            led_switch(LED0, led_state);
        }

        delayer(500);
    }

    return 0;
    
}