#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

#include "imx6ull.h"

enum keyvalue
{
    KEY_NONE = 0,
    KEY0_VALUE,
};

void key_init(void);

int key_get_value(void);


#endif // !_BSP_KEY_H_

