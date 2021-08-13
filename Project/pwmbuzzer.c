/*
 * pwmled.c: Starter code for ECE 266 Project, Fall 2019
 *
 * This file contains the initialization function for Timer/PWM hardware used
 * with the on-board sub-LEDs.
 *
 * Created by Zhao Zhang
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include <pwmbuzzer.h>
#include "pwmbuzzer.h"

void buzzerPwmInit() {
    //Enable peripheral of GPIOC and WTIMER0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);

    TimerConfigure(WTIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));
    TimerControlLevel(WTIMER0_BASE, TIMER_B,true /* output inversion */);

    TimerEnable(WTIMER0_BASE, TIMER_B);
    TimerLoadSet(WTIMER0_BASE, TIMER_B,1005);
    TimerMatchSet(WTIMER0_BASE, TIMER_B,0);
}
