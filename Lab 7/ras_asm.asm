				.cdecls "stdbool.h","stdint.h","inc/hw_memmap.h","inc/hw_adc.h","driverlib/adc.h","ras.h"

                .text
                .global rasRead

ADC	.field	ADC0_BASE


rasvoidRead		PUSH	{r0, LR}

			LDR		r0, ADC
			MOV		r1, #0
			BL		ADCProcessorTrigger

loop
			LDR		r0, ADC
			MOV		r1, #0
			MOV		r2, #0
			BL		ADCIntStatus
			CMP		r0, #0
			BEQ		loop

			LDR		r0, ADC
			MOV		r1, #0
			POP		{r2}
			BL		ADCSequenceDataGet

			LDR		r0, ADC
			MOV		r1, #0
			BL		ADCIntClear

			POP		{PC}

