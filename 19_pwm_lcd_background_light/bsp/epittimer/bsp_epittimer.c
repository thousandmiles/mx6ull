#include "bsp_epittimer.h"
#include "bsp_int.h"
#include "bsp_led.h"

/*
* @description                  : Intialization of epit timer
* @param - frac                 : Frequency division
* @param - value                : Value for countdown
*/
void epit1_init(uint32_t frac, uint32_t value)
{
    if (frac > 0XFFF)
    {
        frac = 0XFFF;
    }

    EPIT1->CR = 0;      /* reset EPIT1 CR register */

    /*
    *   bit[25:24] : clock source selection
    *   bit[15:4] : clock frequency division
    *   bit[3] : clock working mode
    *   bit[2] : clock interrupt enable/disable
    *   bit[1] : clock count initial value source selection
    *   bit[0] : clock enable/disable
    */

    EPIT1->CR = (1<<24);     /* [25:24] = [01]*/
    EPIT1->CR |= (frac<<4);  /* frequency division*/
    EPIT1->CR |= (1<<3);     /* working mode*/
    EPIT1->CR |= (1<<2);     /* clock interrupt enable */
    EPIT1->CR |= (1<<1);     /* clock count initial value */

    EPIT1->LR = value;

    EPIT1->CMPR = 0;

    GIC_EnableIRQ(EPIT1_IRQn); /* enable EPIT1 interrupt*/
    
    system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t)epit1_irqhandler, NULL);

    EPIT1->CR |= 1; /* clock interrupt enable */

}

void epit1_irqhandler(void)
{
    static uint8_t state = 0;

    state = !state;
    
    if (EPIT1->SR & 1)  /* interrupt happens*/
    {
        led_switch(LED0, state);
    }

    EPIT1->SR |= 1;   /* clear using 1 not 0*/
}