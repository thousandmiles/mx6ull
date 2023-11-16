#ifndef _BSP_EXIT_H_

#define _BSP_EXIT_H_

#include "imx6ull.h"

void exit_init(void);

void gpio1_io18_irqhandler(void);   // interrupt handler function


#endif // !_BSP_EXIT_H_