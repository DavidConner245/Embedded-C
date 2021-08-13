/*
 * ECE 266, Fall 2019, Project
 *
 * main.c
 * Created David Conner and Daood Shah
 */


#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"
#include "pwmbuzzer.h"


#define BUZZER_MAX_PERIOD (uint32_t) (50000000/263.31)
#define BUZZER_MIN_PERIOD (uint32_t) (50000000/523.20)


// ON/Off state type
typedef enum {On, Off} OnOff_t;

// System running state
//static OnOff_t sysState = On;

// Alarm state
static OnOff_t alarmState = On;

// Notes for the song
double const song[] =
//  E4,     E4,     E4,     C4,     E4,             G4,     G3,
   {329.63, 329.63, 329.63, 261.63, 329.63, 000.00, 196.00,
//  C4,     G3,     E3,     A3,     B3,     Bb3,    A3,     G3,
    261.63, 196.00, 164.81, 220.00, 000.00, 233.08, 220.00, 196.00,
//  E4,     G4,     A4,     F4,     G4,     E4,     C4,     D4,     B3,
    329.63, 392.00, 000.00, 349.23, 392.00, 329.63, 261.63, 293.66, 000.00,
//  C4,     G3,     E3,     A3,     B3,     Bb3,    A3,     G3,     E4,
    261.63, 196.00, 164.81, 220.00, 246.94, 000.00, 220.00, 196.00, 329.63,
//  G4,     A4,     F4,     G4,     E4,     C4,     D4,     B3,
    392.00, 440.00, 000.00, 392.00, 329.63, 261.63, 293.66, 246.94,
//  G4,     F#3,    F4,     D4,     E4,     G3,     A3,     C4,     A3,
    000.00, 185.00, 349.23, 293.66, 329.63, 196.00, 000.00, 261.63, 220.00,
//  C4,     D4,     G4,     F#4,    F4,     D4,     E4,     C5,     C5,     C5
    261.63, 293.66, 392.00, 000.00, 349.23, 293.66, 329.63, 523.25, 523.25, 000.00,000.00,000.00
//  G4,     F#4,    F4,     D4,     E4,     G3,     A3,     C4,     A3,     C4,
   // 392.00, 369.99, 349.23, 293.66, 000.00, 000.00, 000.00, 000.00, 000.00
//  D4,     D#4,    D4,     C4
   };

// Buzzer state
// static OnOff_t buzzingState = Off;

typedef struct {
    OnOff_t buzzingState;
    int pwmPeriod;              // PWM period for LED
    int maxPulseWidth;          // maximum pulse width
} buzzer_t;

buzzer_t buzzer = {On, BUZZER_MAX_PERIOD , 50000000/329.63}; // Frequency
 int noteIndex = 0;

// void playMusic()
// Callback function for playing the buzzer
void callbackBuzzerPlay(uint32_t time) {                    // the scheduled time
    uint32_t delay = 10;
    if (alarmState == On) {

        // Turn the buzzer on and off alternatively
        // Adjust the time values to control the sound intensity
        switch (buzzer.buzzingState) {
        case On:
            //buzzerOff();
            buzzerPwmSet(buzzer.pwmPeriod,0);
            buzzer.buzzingState = Off;
            delay = 200;                           // off for 200 ms
            break;

        case Off:
            //buzzerOn();
            if (song[noteIndex]!=0) {
                buzzer.pwmPeriod = 50000000/song[noteIndex];
                buzzer.maxPulseWidth = (uint64_t) buzzer.pwmPeriod*0.25;
            }
            else {
                buzzer.pwmPeriod = 50000000/song[1];
                buzzer.maxPulseWidth = 0 ;
            }

            buzzerPwmSet(buzzer.pwmPeriod,buzzer.maxPulseWidth);
            buzzer.buzzingState = On;
            noteIndex++;
            delay = 120;                            // on for 12 ms
            break;
        }
    }

    // schedule the next callback
    schdCallback(callbackBuzzerPlay, time + delay);
}

// Motion Sensor
void callbackPIRmotionSensor(uint32_t time) {
    int delay = 5000;
        if (checkMotion()) {
            alarmState = On;
            ledTurnOnOff(true /* red */, false /* blue */, false /* green */);
            uprintf("%s\n\r", "Motion Detected");
        } else {
            alarmState = On;
            ledTurnOnOff(false /* red */, false /* blue */, true /* green */);
            uprintf("%s\n\r", "Motion Not Detected");
        }
        schdCallback(callbackPIRmotionSensor, time + delay);
}

/*
 * The main function
 */
int main(void) {
    lpInit();
    PIRInit();
    buzzerPwmInit();

    // Print out a start message
    uprintf("%s\n\r", "Project");

    // Schedule the first callback events
    schdCallback(callbackBuzzerPlay, 1000);
    //schdCallback(callbackCheckPushButton, 1002);
    schdCallback(callbackPIRmotionSensor, 1002);


    // Turn on the green sub-LED
    ledTurnOnOff(false /* red */, false /* blue */, true /* green */);

    // Run the callback scheduler
    while (true) {
        schdExecute();
    }
}
