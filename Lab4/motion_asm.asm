;
; Lab 4, ECE 266, Fall 2019
;
; Assembly functions related to motion operations.
;
; Created by Daood Shah
;

; To include names declared in C
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/pin_map.h","driverlib/sysctl.h","driverlib/gpio.h","motion.h"

                .text
                .global checkMotion
               ;.global motionOn
               ;.global motionOff

; GPIO peripheral, port base and pin mask value for the buzzer
; The buzzer is assumed to be connected to J17, and then the buzzer's
; signal pin is connected to PC4.
MOTION_PORT     .field  GPIO_PORTC_BASE
MOTION_PIN      .equ    GPIO_PIN_4

;
; void On(): Turn on the buzzer. It calls GPIOPinWrite() to write 1 to the signal pin.
;
checkMotion        PUSH    {LR}            ; save the return address

                ; Write 1 to the GPIO pin that the buzzer uses:
                ;   GPIOPinWrite(BUZZ_PORT, BUZZ_PIN, BUZZ_PIN)
                LDR     r0, MOTION_PORT
                MOV     r1, #MOTION_PIN
				MOV     r2,#MOTION_PIN
                BL      GPIOPinRead

                POP     {PC}            ; return

;
; void buzzOff(): Turn off the buzzer. It alls GPIOPinWrite() to write 0 to the pin
;
         ; return
