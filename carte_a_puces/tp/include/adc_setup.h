#ifndef ADC_SETUP_H
#define ADC_SETUP_H

#include <stdint.h>

void Adc_8bit_autotrigger_setup(uint8_t prescaler, char interrupt);

#endif /*ADC_SETUP_H*/