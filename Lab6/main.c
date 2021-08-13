#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <driverlib/sysctl.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/i2c.h>
#include "launchpad.h"
#include "seg7.h"
#include "seg7digit.h"
#include "ras.h"

int a, b, c, d;

static uint8_t colon = 0;

static uint8_t seg7Coding[10] = {
        0b00111111,         // digit 0
        0b00000110,         // digit 1
        0b01011011,         // digit 2
        0b01001111,         // digit 3
        0b01100110,         // digit 4
        0b01101101,         // digit 5
        0b01111101,         // digit 6
        0b00000111,         // digit 7
        0b01111111,         // digit 8
        0b01100111,         // digit 9
        // MORE CODINGS
        };


void clockUpdate(uint32_t time) {                     // pointer to a 4-byte array
    colon = 0b10000000;

    uint8_t code[4];            // The 7-segment code for the four clock digits
    int test[2];
    rasvoidRead(test);
    uprintf("%d, %d\n\r", test[0], test[1]);
    a = (test[0] / 41) % 10;
    b = (test[0] / 410) % 10;
    c = (test[1] / 41) % 10;
    d = (test[1] / 410) % 10;

    code[0] = seg7Coding[a] + colon;
    code[1] = seg7Coding[b] + colon;
    code[2] = seg7Coding[c] + colon;
    code[3] = seg7Coding[d] + colon;
    seg7Update(code);


    schdCallback(clockUpdate, time + 500);
}

int main(void) {
    lpInit();
    seg7Init();
    rasInit();

    schdCallback(clockUpdate, 1000);

    while (true) {
        schdExecute();
    }
}
