/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */
#ifndef PING_H_
#define PING_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

/**
 * Initialize ping sensor. Uses PB3 and Timer 3B
 */
void ping_init (void);

void ping_trigger (void){
    g_state = LOW;

    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x0100;
    TIMER3_IMR_R &= ~0x0400;

    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x08;

    // Use PB3 as normal GPIO output to send trigger pulse
    GPIO_PORTB_DIR_R |= 0x08;           // PB3 output
    GPIO_PORTB_DATA_R &= ~0x08;         // Start low
    timer_waitMicros(2);
    GPIO_PORTB_DATA_R |= 0x08;          // High pulse
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~0x08;         // End pulse
    GPIO_PORTB_DIR_R &= ~0x08;          // Back to input

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x0400;

    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x08;
    TIMER3_IMR_R |= 0x0400;
    TIMER3_CTL_R |= 0x0100;
}

void ping_trigger (void);

/**
 * @brief Timer3B ping ISR
 */
void TIMER3B_Handler(void);

/**
 * @brief Calculate the distance in cm
 *
 * @return Distance in cm
 */
float ping_getDistance (void);

#endif /* PING_H_ */
