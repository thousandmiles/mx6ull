#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

#include "imx6ull.h"

int adc1ch1_init(void);

// jiao zhun
status_t adc1_autocalibration(void);

uint32_t getadc_value(void);

unsigned short getadc_average(unsigned char times);

unsigned short getadc_volt(void);

#endif // !_BSP_ADC_H_