#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include <driverlib/uart.h>
#include "launchpad.h"

#define TIMER3  TIMER3_BASE

void rangerInit(){

    //Enables a peripheral for GPIOB
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    //Enables a peripheral for TIMER#
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    //Configures the timer(s)
    TimerConfigure(TIMER3, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME_UP));


    //configure timerB
    TimerControlEvent(TIMER3, TIMER_B, TIMER_EVENT_BOTH_EDGES);
    //Enables the timer(s)
    TimerEnable(TIMER3, TIMER_B);

}

void rangerDetect(){


    //Configures pin(s) for use as GPIO outputs
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);
    //Writes a value to the specified pin(s).
    //High voltage for 2 microseconds
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);
    //low output
    waitUs(2);


    ////Writes a value to the specified pin(s).
    //High Voltage for 5 microseconds
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
    waitUs(5);

    //Writes a value to the specified pin(s).
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);
    //Configures pin(s) for use by the Timer peripheral
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_3);
    //Configures the alternate function of a GPIO pin
    GPIOPinConfigure(GPIO_PB3_T3CCP1);
    //Clears timer interrupt sources
    TimerIntClear(TIMER3, TIMER_CAPB_EVENT);

}
