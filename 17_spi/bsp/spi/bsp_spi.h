#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_

#include "imx6ull.h"

void spi_init(ECSPI_Type* base);

unsigned char spich0_readwrite_byte(ECSPI_Type* base, unsigned char txdata);

#endif // !_BSP_SPI_H_