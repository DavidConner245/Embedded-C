//David Conner, Daood Shah, Fall 2019

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "seg7digit.h"
#include "ranger.h"

static uint8_t colon = 0b10000000;
int distance_mode = 1; // This decides which unit of measurement the 7 segment display outputs in. 0 = millimeters; 1 = feet/inches


static uint8_t seg7Coding[11] = {
        0b00111111,         // digit 0
        0b00000110,         // digit 1
        0b01011011,         // digit 2
        0b01001111,         // digit 3
        0b01100110,         // digit 4
        0b01101101,         // digit 5
        0b01111101,         // digit 6
        0b00000111,         // digit 7
        0b01111111,         // digit 8
        0b01101111,         // digit 9
        0b00000000,         // off digit
        };


// Stop watch increment function, written in assembly
extern void stopWatchIncrement(seg7Display_t* seg7Display);

// The running state of the stop-watch system
enum {
    Start, Run, Pause
}  sysState = Start;

// The initial state of the 7-segment display: "00:00" with colon on
seg7Display_t seg7Display = {
    0, 0, 0, 0, 1
};


// SW1 is the START/PAUSE/RESUME button.
void
checkPushButton(uint32_t time)
{
    int code = pbRead();
    uint32_t delay;

    switch (code) {
    case 1:                         // SW1 is the Start/Pause/Resume button
        if (sysState == Run){
            sysState = Pause;
        }
        else
            (sysState = Run);

        delay = 250;                // software de-bouncing
        break;

    case 2:                         // SW2 switches between millimeter and feet/inches mode
        if (distance_mode == 0){
            uprintf("Mode switched to feet/inches");
            distance_mode = 1;
        }
        else{
            uprintf("Mode switched to millimeters");
            distance_mode = 0;
        }

        delay = 250;                // software debouncing
        break;

    default:
        delay = 10;
    }

    schdCallback(checkPushButton, time + delay);
}


void rangeData(uint32_t time){
    uint32_t distance = rangerDetect2(); // Distance in 10mm (1cm)

    uprintf("%d\n\r", distance);

    int R2; // Rightmost digit of 7-segment display
    int R1;
    int L2;
    int L1; // Leftmost digit of 7-segment display

    uint8_t code[4];

    if (sysState == Start){ //Initial state of display will be 00:00
        R2 = 0;
        R1 = 0;
        L2 = 0;
        L1 = 0;

        //The following code actually outputs the R2, R1, L2, L1, Colon values to the display
        code[0] = seg7Coding[R2] + colon; // Rightmost digit
        code[1] = seg7Coding[R1] + colon;

        code[2] = seg7Coding[L2] + colon;
        code[3] = seg7Coding[L1] + colon; //This is the leftmost number

        seg7Update(code); // Updates the 7-segment display
    }
    else if (sysState == Pause){ // If system state is paused, the program will skip all the following if statements and go to the end

    }
    else if (distance_mode == 0){ // Millimeters
        colon = 0; // Turns off colon



        distance = distance / 250;

        if (distance > 9999){ // Maximizes display to 9999
            R2 = 9;
            R1 = 9;
            L2 = 9;
            L1 = 9;
        }
        else{ // Otherwise will display normally

            R2 = distance % 10;
            R1 = distance / 10 % 10;
            L2 = distance / 100 % 10;
            L1 = distance / 1000 % 10;
        }

        // The following code actually outputs the R2, R1, L2, L1, Colon values to the display
        if(L1 == 0){
            L1 = 10;
        }
        if(L2 == 0){
            L2 = 10;
        }
        if(R1 == 0 & L2 == 0){
            R1 = 10;
        }
        code[0] = seg7Coding[R2] + colon; // Rightmost digit
        code[1] = seg7Coding[R1] + colon;
        code[2] = seg7Coding[L2] + colon;
        code[3] = seg7Coding[L1] + colon; // Leftmost digit

        seg7Update(code); //Updates the 7-segment display

    }
    else{ // Feet/inches
        colon = 0b10000000; // Activates colon
        distance = distance / 2500;

        int distance_feet = ((float) distance / 2.54) / 12; // 1 foot
        int distance_inches = ((float) distance / 2.54) - distance_feet * 12; // 1 inch

        R2 = distance_inches % 10;
        R1 = distance_inches / 10 % 10;
        L2 = distance_feet % 10;
        L1 = distance_feet / 10 % 10;

        uprintf("%d/n/r", R1);

        if (L1 == 0)
                   L1 = 10;

               if(L2 == 0)
                   L2 = 10;

               if(R1 == 0 && L2 == 0 && L1 == 0)
                   R1 = 10;

        // Outputs the R2, R1, L2, L1, and colon values to the 7-segment display
        code[0] = seg7Coding[R2] + colon; // Rightmost digit
        code[1] = seg7Coding[R1] + colon;
        code[2] = seg7Coding[L2] + colon;
        code[3] = seg7Coding[L1] + colon; // Leftmost digit

        seg7Update(code); // Updates the 7 digit display
    }

    schdCallback(rangeData, time + 200);
}

int main(void)
{
    lpInit();
    seg7Init();
    rangerInit();

    seg7DigitUpdate(&seg7Display);
    schdCallback(rangeData, 1000);
    schdCallback(checkPushButton, 1000);

    while (true) {
        schdExecute();
    }
}
