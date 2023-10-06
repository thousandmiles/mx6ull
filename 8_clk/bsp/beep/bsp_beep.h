#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

#include "imx6ull.h"
#define ON   1
#define OFF  0

void beep_init(void);
void beep_switch(int status);

#endif // !__BSP_BEEP_H