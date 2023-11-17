#include "bsp_spi.h"
#include "bsp_gpio.h"

void spi_init(ECSPI_Type* base)
{
    base->CONREG = 0;
    base->CONREG |= (1<<0) | (1<<3) | (1<<4) | (7<<20);

    base->CONFIGREG = 0;

    base->PERIODREG = 0x2000;

    /*
    *   PER_DIVIDER = 0x9    bits:15~12
    *   POST_DIVIDER = 0     bits:11~8
    */
    base->CONREG &= ~((0xF<<12) | (0xF<<8));    // clear
    base->CONREG |= (0x9<<12);                  // SPI CLK = 6Mhz
}

unsigned char spich0_readwrite_byte(ECSPI_Type* base, unsigned char txdata)
{
    unsigned int spirxdata = 0;
    unsigned int spitxdata = txdata;

    // select channel 0
    base->CONREG &= ~(3<<18);       // clear
    base->CONREG |= (0<<18);        // channel 0

    while ((base->STATREG & (1<<0)) == 0);      // wait TXFIFO is empty. If this bit is 0, then FIFO is not empty

    base->TXDATA = spitxdata;

    while ((base->STATREG & (1<<3)) == 0);      // wait RXFIFO has data. If this bit is 0, then FIFO has nothing.

    spirxdata = base->RXDATA;

    return spirxdata;
}