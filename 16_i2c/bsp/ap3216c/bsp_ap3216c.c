#include "bsp_ap3216c.h"
#include "bsp_i2c.h"
#include "bsp_delay.h"
#include "cc.h"
#include "stdio.h"

unsigned char ap3216c_init(void)
{
    unsigned char data = 0;

    IOMUXC_SetPinMux(IOMUXC_UART4_TX_DATA_I2C1_SCL, 1);

    IOMUXC_SetPinMux(IOMUXC_UART4_RX_DATA_I2C1_SDA, 1);

    IOMUXC_SetPinConfig(IOMUXC_UART4_TX_DATA_I2C1_SCL, 0x70B0);

    IOMUXC_SetPinConfig(IOMUXC_UART4_RX_DATA_I2C1_SDA, 0x70B0);

    i2c_init(I2C1);

    ap3216c_writeonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0x04);

    delay_ms(50);

    ap3216c_writeonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0x03);

    data = ap3216c_readonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG);

    if (data == 0x03)
    {
        return 0;
    }
    else
    {
        return 1;
    }

    // return (data == 0x03) ? 0 : 1;
}

unsigned char ap3216c_writeonebyte(unsigned char addr, unsigned char reg, unsigned char data)
{
    unsigned char status = 0;

    unsigned char writedata = data;

    struct i2c_transfer masterXfer;

    masterXfer.slaveaddress = addr;             // device address
    masterXfer.direction = kI2C_Write;          // write data
    masterXfer.subaddress = reg;                // register to be written
    masterXfer.subaddresssize = 1;              
    masterXfer.data = &writedata;
    masterXfer.datasize = 1;

    if (i2c_master_transfer(I2C1, &masterXfer))
    {
        status = 1;
    }

    return status;
}

unsigned char ap3216c_readonebyte(unsigned char addr, unsigned char reg)
{
    unsigned char val = 0;

    struct i2c_transfer masterXfer;

    masterXfer.slaveaddress = addr;

    masterXfer.direction = kI2C_Read;

    masterXfer.subaddress = reg;

    masterXfer.subaddresssize = 1;

    masterXfer.data = &val;

    masterXfer.datasize = 1;

    i2c_master_transfer(I2C1, &masterXfer);

    return val;
}

void ap3216c_readdata(unsigned short * ir, unsigned short * ps, unsigned short * als)
{
    unsigned char buf[6];

    unsigned char i;

    for (i = 0; i < 6; i++)
    {
        buf[i] = ap3216c_readonebyte(AP3216C_ADDR, AP3216C_IRDATALOW + i);
    }

    if (buf[0] & 0x80)
    {
        *ir = 0;
    }
    else
    {
        *ir = ((unsigned short)buf[1] << 2) | (buf[0] & 0x03);
    }

    *als = ((unsigned short)buf[3] << 8) | buf[2];

    if (buf[4] & 0x40)
    {
        *ps = 0;
    }
    else
    {
        *ps = ((unsigned short)(buf[5] & 0x3F) << 4) | (buf[4] & 0x0F);
    }
}