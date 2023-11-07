#include "bsp_i2c.h"
#include "bsp_delay.h"
#include "stdio.h"

void i2c_init(I2C_Type* base)
{
    base->I2CR &= ~(1<<7);

    base->IFDR = 0x15;

    base->I2CR |= (1<<7);
}

unsigned char i2c_master_repeated_start(I2C_Type* base, unsigned char address, enum i2c_direction direction)
{
    // slave mode and busy ...
    if ((base->I2SR & (1<<5)) && ((base->I2CR & (1<<5)) == 0))
    {
        return 1;
    }

    base->I2CR |= (1<<4) | (1<<2);

    base->I2DR = ((unsigned int)address << 1) | ((direction == kI2C_Read) ? 1 : 0);

    return 0;
}

unsigned char i2c_master_start(I2C_Type* base, unsigned char address, enum i2c_direction direction)
{
    if (base->I2SR & (1<<5))
    {
        return 1;
    }

    base->I2CR |= (1<<5) | (1<<4);

    base->I2DR = ((unsigned int)address << 1) | ((direction == kI2C_Read) ? 1 : 0);

    return 0;
}

unsigned char i2c_check_and_clear_error(I2C_Type* base, unsigned int status)
{
    if (status & (1<<4))
    {
        base->I2SR &= ~(1<<4);
        base->I2CR &= ~(1<<7);
        base->I2CR |= (1<<7);
        return I2C_STATUS_ARBITRATIONLOST;
    }
    else if(status & (1<<0))
    {
        return I2C_STATUS_NAK;
    }
    return I2C_STATUS_OK;
}

unsigned char i2c_master_stop(I2C_Type* base)
{
    unsigned short timeout = 0XFFFF;

    base->I2CR &= ~((1<<5) | (1<<4) | (1<<3));

    while (base->I2SR & (1<<5))
    {
        timeout--;
        if (timeout == 0)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }
    return I2C_STATUS_OK;
}