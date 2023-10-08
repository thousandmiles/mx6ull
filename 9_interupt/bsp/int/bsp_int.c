#include "bsp_int.h"

/* interupt nested counter */
static unsigned int irqNesting;

/* interupt service function table */
static sys_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS];

/* interupt initialization function */
void int_init(void)
{
    GIC_Init();
    system_irqtable_init();
    __set_VBAR((uint32_t)0X878000000); /* interupt vector table offset*/
}

/* initialize the interupt service table */
void system_irqtable_init(void)
{
    unsigned int i = 0;
    irqNesting = 0;

    for (int i = 0; i < NUMBER_OF_INT_VECTORS; i++)
    {
        system_register_irqhandler((IRQn_Type)i, default_irqhandler, NULL);
    }
}

/* 
* @description          : initialize the interupt function of a specific interrupt ID
* @param - irq          : interrupt ID
* @param - handler      : interrupt function
* @param - userParam    : function parameter
*/
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam)
{
    irqTable[irq].irqHandler = handler;
    irqTable[irq].userParam = userParam;
}

/*
* @description      : interupt function of a specific interrupt ID
* @param - giccIar  : interrupt ID
*/
void system_irqhandler(unsigned int giccIar)
{
    uint32_t intNum = giccIar & 0x3FFUL;  // extract the lower 10 bits

    /* Check the ID is whether valid */
    if (intNum >= NUMBER_OF_INT_VECTORS)
    {
        return;
    }

    irqNesting++;

    irqTable[intNum].irqHandler(intNum, irqTable[intNum].userParam);

    irqNesting--;

}

/*
* @description : default interupt function
*/
void default_irqhandler(unsigned int giccIar, void *userParam)
{
    while(1){};
}