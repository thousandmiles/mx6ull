#include "bsp_beep.h"
#include 

void beep_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x10b0);

    GPIO5->GDIR |= (1<<1);  // enable output
    GPIO5->DR   |= (1<<1);  // output 1
}

void beep_switch(int status)
{
    if (status == ON) 
    {

        GPIO5->DR &= ~(1<<1);

    } 
    else 
    {

        GPIO5->DR |= (1<<1);
    }
}