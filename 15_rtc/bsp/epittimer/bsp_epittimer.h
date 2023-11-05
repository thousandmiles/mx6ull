#ifndef _BSP_EPITTIMER_H_

#define _BSP_EPITTIMER_H_

#include "imx6ull.h"

void epit1_init(uint32_t frac, uint32_t value);

void epit1_irqhandler(void);


#endif // !_BSP_EPITTIMER_H_