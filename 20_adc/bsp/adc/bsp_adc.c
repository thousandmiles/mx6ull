#include "bsp_adc.h"
#include "bsp_delay.h"

int adc1ch1_init(void)
{
    int ret = 0;

    ADC1->CFG = 0;

    ADC1->CFG |= (2<<2) | (3<<0);

    ADC1->GC = 0;

    ADC1->GC |= 1<<0;

    if (adc1_autocalibration() != kStatus_Success)
    {
        ret = -1;
    }

    return ret;
}

// jiao zhun
status_t adc1_autocalibration(void)
{
    status_t ret = kStatus_Success;

    ADC1->GS |= (1<<2);

    ADC1->GC |= (1<<7);

    while ((ADC1->GC & (1<<7)) != 0)
    {
        if ((ADC1->GS & (1<<2)) != 0)
        {
            ret = kStatus_Fail;
            break;
        }
    }

    if ((ADC1->HS & (1<<0)) == 0)
    {
        ret = kStatus_Fail;
    }

    if ((ADC1->GS & (1<<2)) != 0)
    {
        ret = kStatus_Fail;
    }

    return ret;
}

uint32_t getadc_value(void)
{
    ADC1->HC[0] = 0;

    ADC1->HC[0] = (1<<0);

    while ((ADC1->HS & (1<<0)) == 0);

    return ADC1->R[0];

}

unsigned short getadc_average(unsigned char times)
{
    unsigned int temp_val = 0;
    
    unsigned char t;

    for (t = 0; t < times; t++)
    {
        temp_val += getadc_value();
        delay_ms(5);
    }

    return temp_val / times;
}

unsigned short getadc_volt(void)
{
    unsigned int adcvalue = 0;

    unsigned int ret = 0;

    adcvalue = getadc_average(5);

    ret = (float)adcvalue * (3300.0f / 4096.0f);

    return ret;
}