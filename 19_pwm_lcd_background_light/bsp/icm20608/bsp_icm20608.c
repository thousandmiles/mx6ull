#include "bsp_icm20608.h"
#include "bsp_delay.h"
#include "bsp_spi.h"

struct icm20608_dev_struc icm20608_dev;

unsigned char icm20608_init(void)
{
    unsigned char regvalue;
    gpio_pin_config_t cs_config;

    // SCLK, MISO, MOSI
    IOMUXC_SetPinMux(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0);
    IOMUXC_SetPinMux(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0);
    IOMUXC_SetPinMux(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0);

    IOMUXC_SetPinConfig(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0x10B1);
    IOMUXC_SetPinConfig(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0x10B1);
    IOMUXC_SetPinConfig(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0x10B1);

    // chip select / slave select
    IOMUXC_SetPinMux(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0x10B0);

    cs_config.direction = kGPIO_DigitalOutput;
    cs_config.outputLogic = 0;
    gpio_init(GPIO1, 20, &cs_config);

    spi_init(ECSPI3);

    // ICM-20608 power management register
    icm20608_write_reg(ICM20_PWR_MGMT_1, 0x80);        // reset
    delay_ms(50);
    icm20608_write_reg(ICM20_PWR_MGMT_1, 0x01);
    delay_ms(50);

    regvalue = icm20608_read_reg(ICM20_WHO_AM_I);

    if(regvalue != ICM20608G_ID && regvalue != ICM20608D_ID)
    {
        return 1;
    }

    icm20608_write_reg(ICM20_SMPLRT_DIV, 0x00);
    icm20608_write_reg(ICM20_GYRO_CONFIG, 0x18);
    icm20608_write_reg(ICM20_ACCEL_CONFIG, 0x18);
    icm20608_write_reg(ICM20_CONFIG, 0x04);
    icm20608_write_reg(ICM20_ACCEL_CONFIG2, 0x04);
    icm20608_write_reg(ICM20_PWR_MGMT_2, 0x00);
    icm20608_write_reg(ICM20_LP_MODE_CFG, 0x00);
    icm20608_write_reg(ICM20_FIFO_EN, 0x00);

    return 0;

}

void icm20608_write_reg(unsigned char reg, unsigned char value)
{
    // bit[7] = 0, write
    // bit[7] = 1, read
    reg &= ~0x80;

    ICM20608_CSN (0);                           // enable SPI transmit
    spich0_readwrite_byte(ECSPI3, reg);
    spich0_readwrite_byte(ECSPI3, value);
    ICM20608_CSN (1);                           // disable SPI transmit

}

unsigned char icm20608_read_reg(unsigned char reg)
{
    unsigned char reg_val;

    reg |= 0x80;

    ICM20608_CSN (0); 
    spich0_readwrite_byte(ECSPI3, reg);
    reg_val = spich0_readwrite_byte(ECSPI3, 0xFF);
    ICM20608_CSN (1); 
    return (reg_val);
}

void icm20608_read_len(unsigned char reg, unsigned char *buf, unsigned char len)
{
    unsigned char i;

    reg |= 0x80;
    ICM20608_CSN (0); 
    spich0_readwrite_byte(ECSPI3, reg);
    for (i = 0; i < len; i++)
    {
        buf[i] = spich0_readwrite_byte(ECSPI3, 0xFF);
    }

    ICM20608_CSN (1); 
}

float icm20608_gyro_scaleget(void)
{
    unsigned char data;
    float gyroscale;

    data = (icm20608_read_reg(ICM20_GYRO_CONFIG) >> 3) & 0x3;

    switch(data)
    {
        case 0:
            gyroscale = 131;
            break;
        case 1:
            gyroscale = 65.5;
            break;
        case 2:
            gyroscale = 32.8;
            break;
        case 3:
            gyroscale = 16.4;
            break;
    }
    return gyroscale;
}

unsigned short icm20608_accel_scaleget(void)
{
    unsigned char data;
    unsigned short accelscale;

    data = (icm20608_read_reg(ICM20_ACCEL_CONFIG) >> 3) & 0x3;

    switch(data)
    {
        case 0:
            accelscale = 16384;
            break;
        case 1:
            accelscale = 8192;
            break;
        case 2:
            accelscale = 4096;
            break;
        case 3:
            accelscale = 2048;
            break;
    }
    return accelscale;    

}

void icm20608_getdata(void)
{
    float gyroscale;
    unsigned short accescale;
    unsigned char data[14];

    icm20608_read_len(ICM20_ACCEL_XOUT_H, data, 14);

    gyroscale = icm20608_gyro_scaleget();
    accescale = icm20608_accel_scaleget();

    icm20608_dev.accel_x_adc = (signed short)((data[0] << 8) | data[1]);
    icm20608_dev.accel_y_adc = (signed short)((data[2] << 8) | data[3]);
    icm20608_dev.accel_z_adc = (signed short)((data[4] << 8) | data[5]);
    icm20608_dev.temp_adc    = (signed short)((data[6] << 8) | data[7]);
    icm20608_dev.gyro_x_adc  = (signed short)((data[8] << 8) | data[9]);
    icm20608_dev.gyro_y_adc  = (signed short)((data[10] << 8) | data[11]);
    icm20608_dev.gyro_z_adc  = (signed short)((data[12] << 8) | data[13]);


    icm20608_dev.gyro_x_act = ((float)(icm20608_dev.gyro_x_adc) / gyroscale) * 100;
    icm20608_dev.gyro_y_act = ((float)(icm20608_dev.gyro_y_adc) / gyroscale) * 100;
    icm20608_dev.gyro_z_act = ((float)(icm20608_dev.gyro_z_adc) / gyroscale) * 100;

    icm20608_dev.accel_x_act = ((float)(icm20608_dev.accel_x_adc) / accescale) * 100;
    icm20608_dev.accel_y_act = ((float)(icm20608_dev.accel_y_adc) / accescale) * 100;
    icm20608_dev.accel_z_act = ((float)(icm20608_dev.accel_z_adc) / accescale) * 100;


    icm20608_dev.temp_act = (((float)(icm20608_dev.temp_adc) - 25) / 326.8 + 25) * 100;

}