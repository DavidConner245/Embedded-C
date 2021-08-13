; pwmled_asm_direct.asm: Assembly PWM-LED functions
; Project, ECE 266, Fall 2019
;
; This version uses direct I/O register access. It is starter code.
;
; Created by Zhao Zhang
;

; Include C header files
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","inc/hw_timer.h","inc/hw_gpio.h","driverlib/pin_map.h","driverlib/timer.h","pwmbuzzer.h"

                .text

;*****************************************************************************
; Pointers to GPTM's (General-Purpose Timer Module) I/O registers for Project
;
; The buzzer is connected to J17 of Grove base, which is PC5 and routed to
; WT0CCP1 (wide channel 0, sub-timer B).
;*****************************************************************************

; WTIMER0's B Interval Load (GPTMTBILR) register, offset 0x02C
; For PWM, It stores the PWM period
TIMER_TBILP_ptr    	.field  WTIMER0_BASE + TIMER_O_TBILR

; WTIMER0's B Match (GPTMTBMATCHR) register, offset 0x034
; For PWM, it stores the PWM pulse width for the inverted mode
TIMER_TBMATCHR_ptr 	.field  WTIMER0_BASE + TIMER_O_TBMATCHR


buzzerPwmSet              LDR r2, TIMER_TBILP_ptr
                          STR r0, [r2]

                          LDR r2, TIMER_TBMATCHR_ptr
                          STR r1, [r2]

                          BX  lr

