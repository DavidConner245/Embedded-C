				.cdecls "driverlib/adc.c", "driverlib/adc.h", "stdint.h", "stdbool.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/sysctl.h", "driverlib/gpio.h"

                .text
                .global rasvoidRead

ADC	.field	ADC0_BASE


rasvoidRead PUSH	{r0, LR} // puts the r0 register onto the stack
			LDR		r0, ADC // pushes r0 into the stack. Stores return value into r0
			MOV		r1, #0
			BL		ADCProcessorTrigger

loop        LDR		r0, ADC
			MOV		r1, #0
			MOV		r2, #0
			BL		ADCIntStatus
			CMP		r0, #0
			BEQ		loop

			LDR		r0, ADC // loads memory address of base register into r0
			MOV		r1, #0
			POP		{r2}
			BL		ADCSequenceDataGet

			LDR		r0, ADC // loads base register address into r0
			MOV		r1, #0 // loads number of the sequencer into r1
			BL		ADCIntClear

			POP		{PC}

