#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include "pwmbuzzer.h"

#define WTIMER0          WTIMER0_BASE
#define PORTC           GPIO_PORTC_BASE

/*
 * Initialize the timer PWM functions connected to the three sub-LEDs.
 *    Red:      PF1, T0CCP1, Timer 0, sub-timer B
 *    Blue:     PF2, T1CCP0, Timer 1, sub-timer A
 *    Green:    PF3, T1CCP1, Timer 1, sub-time B
 */
void buzzerPwmInit()
{
    // Enable Timer 0 and Timer 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);


    // Connect pins to those timers
    GPIOPinTypeTimer(PORTC, GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);

    // Select PWM for Timer 0 sub-Timer B, and Timer 1 sub-Timers A & B
    TimerConfigure(WTIMER0, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));

    // Invert the PWM waveform, so that the Match register value is the pulse width.
    // Otherwise, the pulse width will be (Load value) - (Match value).
    // The inversion is done by enabling output inversion on the PWM pins.
    TimerControlLevel(WTIMER0, TIMER_B, true /* output inversion */);

    // Enable the Timer 0's TimerB and Timer 1's TimerA and TimerB
    TimerEnable(WTIMER0, TIMER_B);
}
