#include "adc.h"

/**
 * Initialize ADC for Lab 8, Part 1.
 * Written by Lucas England.
 *
 * in adc.h:
 * ```
 * #define ADC_MODULE_0 1
 * #define ADC_SS_0 1
 * ```
 */
void adc_init() {
	SYSCTL_RCGCADC_R |= ADC_MODULE_0;
	SYSCTL_RCGCGPIO_R |= 0x02;

	while ((SYSCTL_PRGPIO_R & 0x02)) {
		/* do nothing */
	};

	GPIO_PORTB_DIR_R &= ~0x10;
	GPIO_PORTB_AFSEL_R |= 0x10;
	GPIO_PORTB_DEN_R &= ~0x10;
	GPIO_PORTB_AMSEL_R |= 0x10;

	while (!(SYSCTL_PRADC_R & ADC_MODULE_0)) {
		/* do nothing */
	}

	ADC0_PC_R &= ~0xF;
	ADC0_PC_R |= 0x1;

	ADC0_ACTSS_R &= ~ADC_SS_0;
	ADC0_EMUX_R &= ~0x000F;
	ADC0_SSMUX0_R &= ~0x000F;
	ADC0_SSMUX0_R += 10;
	ADC0_SSCTL0_R = 0x0006;
	ADC0_IM_R &= ~ADC_SS_0;
	ADC0_ACTSS_R |= ADC_SS_0;
}
