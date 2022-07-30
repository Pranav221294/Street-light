#ifndef _ADC_READ_H_
#define _ADC_READ_H_
#include <stdint.h>
#define ADC_READ_TIMEOUT_COUNT    200
#define ADC_READ_TIMEOUT_RET_VALUE  0xFFFF
uint16_t adc_read(int channel);
#endif