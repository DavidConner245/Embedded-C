#include  <driverlib/adc.c>
#include <driverlib/adc.h>
#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>

#define ADC_PERIPH SYSCTL_PERIPH_ADC0

void rasInit() {
    SysCtlPeripheralEnable(ADC_PERIPH); // Initiates ADC
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0); // Configures ADC to be priority 0
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH6); // Sets first channel - channel 6
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH7); // Sets second channel - channel 7
    ADCSequenceEnable(ADC0_BASE, 0);
}

/*
void rasvoidRead(uint32_t data[]) {
    ADCProcessorTrigger(ADC0_BASE, 0);
    while(!ADCIntStatus(ADC0_BASE, 0, false)){} // Waiting for ADC to respond
    ADCSequenceDataGet(ADC0_BASE, 0, data); // Gets number from the ADC
    ADCIntClear(ADC0_BASE, 0);
}*/

