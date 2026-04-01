#ifndef ADC_H_
#define ADC_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#define ADC_MODULE_0 1
#define ADC_MODULE_1 2

#define ADC_SS_0 1
#define ADC_SS_1 2
#define ADC_SS_2 4
#define ADC_SS_3 8

void adc_init(void);
uint16_t adc_read(void);
uint16_t adc_read_average(void);

#endif
