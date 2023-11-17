#ifndef _BSP_I2C_H_
#define _BSP_I2C_H_

#include "imx6ull.h"

#define I2C_STATUS_OK               (0)
#define I2C_STATUS_BUSY             (1)
#define I2C_STATUS_IDLE             (2)
#define I2C_STATUS_NAK              (3)
#define I2C_STATUS_ARBITRATIONLOST  (4)
#define I2C_STATUS_TIMEOUT          (5)
#define I2C_STATUS_ADDRNAK          (6)

enum i2c_direction
{
    kI2C_Write = 0,
    kI2C_Read  = 1
};

struct i2c_transfer
{
    unsigned char slaveaddress;
    enum i2c_direction direction;
    unsigned int subaddress;
    unsigned char subaddresssize;
    unsigned char* volatile data;
    volatile unsigned int datasize;
};

void i2c_init(I2C_Type* base);

unsigned char i2c_master_start(I2C_Type* base, unsigned char address, enum i2c_direction direction);

unsigned char i2c_master_repeated_start(I2C_Type* base, unsigned char address, enum i2c_direction direction);

unsigned char i2c_check_and_clear_error(I2C_Type* base, unsigned int status);

unsigned char i2c_master_stop(I2C_Type* base);

void i2c_master_write(I2C_Type* base, const unsigned char* buf, unsigned int size);

void i2c_master_read(I2C_Type* base, unsigned char* buf, unsigned int size);

unsigned char i2c_master_transfer(I2C_Type* base, struct i2c_transfer* xfer);

#endif // !_BSP_I2C_H_