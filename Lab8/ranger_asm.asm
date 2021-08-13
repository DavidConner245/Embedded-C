
 .cdecls "stdint.h", "stdbool.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "launchpad.h", "ranger.h","driverlib/timer.h", "driverlib/uart.h"
					.text

RANGER_TIMER3		.field 	SYSCTL_PERIPH_TIMER3;
RANGER_TIMER3BASE	.field 	TIMER3_BASE;

rangerDetect2	PUSH {r1, r2,r3, LR}

				BL	rangerDetect
;rising and falling edges
Loop			LDR	r0, RANGER_TIMER3BASE       ; loads r0 register
				MOV	r1, #0
				BL	TimerIntStatus
				CMP r0, #0
				BEQ Loop

				LDR	r0, RANGER_TIMER3BASE
				MOV	r1,	#TIMER_B
				BL	TimerValueGet               ; branch
				STR	r0, [sp]                    ; Stores [sp] to r0

				LDR	r0, RANGER_TIMER3BASE       ; Loads register r0
				MOV	r1, #TIMER_CAPB_EVENT
				BL	TimerIntClear
;falling
Loop2			LDR	r0, RANGER_TIMER3BASE
				MOV	r1, #0                      ; CPU loads 30 to 1
				BL	TimerIntStatus
				CMP r0, #0
				BEQ Loop2

				LDR	r0, RANGER_TIMER3BASE       ; Loads register r0
				MOV r1, #TIMER_B
				BL	TimerValueGet
				STR	r0, [sp, #4]                ; Stores [sp, #4] to r0

				LDR	r0, RANGER_TIMER3BASE       ; loads register r0
				MOV	r1, #TIMER_CAPB_EVENT
				BL	TimerIntClear               ; Branch to TimerIntClear

; Rising and falling
				LDR r1, [sp]                   ; Loads register r1 with [sp]
				LDR r0, [sp, #4]               ; Loads register r0 with [sp, #4]
				SUB r0, r0, r1                 ; Subtract r0 with r1 and store it into r0
				STR r0, [sp, #8]               ; Store [sp, #8] to r0
				LDR r0, [sp, #8]               ; Loads r0 with [sp, #8]

				POP {r1, r2, r3, PC}           ; used to implement stack
				.endasmfunc
