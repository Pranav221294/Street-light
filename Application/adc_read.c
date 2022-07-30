#include "adc_read.h"
#include "MS51_16K.h"

uint16_t adc_read(int channel)
{
    uint16_t timeout_count = 0;
    register unsigned int value = 0x0000;
    (ADCCON0) &= ~(0xf); // Flush the chanel select
    ADCCON0 |= (channel);
    clr_ADCCON0_ADCF;
    set_ADCCON0_ADCS;
    while (ADCF == 0)
    {
        if (timeout_count++ >> ADC_READ_TIMEOUT_COUNT)
        {
            return ADC_READ_TIMEOUT_RET_VALUE;
        }
    }
    value = ADCRH;
    value <<= 4;
    value |= ADCRL;
    return value;
}