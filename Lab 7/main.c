/*
 * main.c: Starter code for Lab 7, Fall 2019
 *
 * Created by Zhao Zhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "launchpad.h"
#include "ras.h"
#include "pwmled.h"
#include "pwmbuzzer.h"

// LED-related constant
#define LED_PWM_PERIOD          5000
#define LED_MAX_PULSE_WIDTH     100
#define BUZZER_PWM_PERIOD       50000000
#define BUZZER_MAX_PULSE_WIDTH  (50000000) * (0.25) // 25% of the pulse width


typedef enum{ On, Off } OffON_t;


// Buzzer state
static OffON_t buzzerState = Off;

//values returned by knob1 and knob2
uint32_t returnedValues[2] = { 0, 0 };


typedef struct
{
  int pwmPeriod;        // PWM period for LED
  int maxPulseWidth;        // maximum pulse width
} LedState_t;

static LedState_t led = { LED_PWM_PERIOD /* PWM Period */ ,
  LED_MAX_PULSE_WIDTH /*Multiply factor */
};

//period and max pulse width for buzzer
static LedState_t buzzer = { BUZZER_PWM_PERIOD, BUZZER_MAX_PULSE_WIDTH };

// A sine function that uses degree as input. It converts degree to radian and then
// calls the sin() function of the C library.
static inline double
sine (uint16_t degree)
{
  double radian = 2 * M_PI * ((double) (degree % 360) / 360);
  return sin (radian);
}

// LED playing callback function
void
callbackLedPlay (uint32_t time)
{
  static uint16_t angle = 0;    // the degree of angle, used for calculating sine value
  int delay = 5;        // the callback delay in ms

  // Calculate PWM parameters for red, blue, and green sub-LEDs using sine function.
  // Use phase shift of 60, 30, and 0 degrees for red, blue, and green

  // Reads in angle from RAS
  rasvoidRead (returnedValues);


  //changes the value and therefore changes the brightness of the LED.
  //Max brightness when (1.0-((double)returnedValues[0]/4096)) = 1 or 100%
  int pulseWidthRed =
    sine (angle + 72) * led.maxPulseWidth * (1.0 -
                         ((double) returnedValues[0] /
                          4096));
  int pulseWidthBlue =
    sine (angle + 36) * led.maxPulseWidth * (1.0 -
                         ((double) returnedValues[0] /
                          4096));
  int pulseWidthGreen =
    sine (angle) * led.maxPulseWidth * (1.0 -
                    ((double) returnedValues[0] / 4096));


  // Set the PWM parameters for the three sub-LEDs
  ledPwmSet (led.pwmPeriod, pulseWidthRed, pulseWidthBlue, pulseWidthGreen);

  // Advance the angle by one degree, so a play period is 360 * 5 = 1800 ms
  angle = (angle + 1) % 360;

  // Schedule the next callback
  schdCallback (callbackLedPlay, time + delay);
}



// Callback function for buzzer

void
callbackbuzzerPlay (uint32_t time)
{
  uint32_t delay = 10;
  //rasvoidRead (returnedValues);
  //Ratio = (1.0-[(double)returnedValues[0]/4096]


  int periodChange =
    (double) buzzer.pwmPeriod / (261.63 +
                 (1.0 -
                  ((double) returnedValues[1] / 4096)) *
                 261.63);


  int buzzerWidthChange =
    0.25 * periodChange * (1.0 - ((double) returnedValues[0] / 4096));


  switch (buzzerState)
    {
    case On:
      buzzerPwmSet (buzzer.pwmPeriod, 0);
      buzzerState = Off;
      delay = 2800;     // off for 2800 ms
      break;

    case Off:
      buzzerPwmSet (periodChange, buzzerWidthChange);
      buzzerState = On;
      delay = 200;      // on for 200 ms
      break;
    }


  // schedules the next callback
  schdCallback (callbackbuzzerPlay, time + delay);
}



void main (void)
{
  lpInit();
  ledPwmInit();
  rasInit();
  buzzerPwmInit();

  // Schedule the first callback events for LED flashing and push button checking.
  // Those trigger callback chains. The time unit is millisecond.
  schdCallback (callbackLedPlay, 1002);
  schdCallback (callbackbuzzerPlay, 1000);

  // Loop forever
  while (true)
    {
      schdExecute ();
    }
}
