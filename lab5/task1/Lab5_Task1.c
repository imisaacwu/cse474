/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * June 5, 2025
 * EE/CSE 474: Lab5 Task1 controls LEDS D1-D4 using an
 * external potentiometer. Increasing the resistance (kΩ),
 * up to 10kΩ, will result in more LEDs being turned on.
 */

#include <stdint.h>
#include "../Lab5_Inits.h"
#include "../lab5.h"

uint32_t ADC_value;

int main(void) {
  // Select system clock frequency preset
  enum frequency freq = PRESET2; // 60 MHz
  PLL_Init(freq);               // Set system clock frequency to 60 MHz
  LEDBreadboard_Init();         // Initialize the 4 onboard LEDs (GPIO)
  ADCReadPot_Init();            // Initialize ADC0 to read from the potentiometer
  TimerADCTriger_Init();        // Initialize Timer0A to trigger ADC0
  PWM_Init();
  float resistance;

  while(1) {
    // 5.1: Convert ADC_value to resistance in kilo-ohm
    resistance = ADC_value / 409.5;

    PWM0CTL = 0x0;
    PWM0CMPB = (int) ((resistance / 10) * 1199);
    PWM0CTL = 0x1;       // Start timer

    // 5.2: Change the pattern of LEDs based on the resistance
    if (resistance < 5) {
      GPIODATA(C) &= ~0x10;     // Set LED C4 to 0 (off)
    } else {
      GPIODATA(C) |= 0x10;      // Set LED C4 to 1 (on)
    }
  }
  return 0;
}

#pragma call_graph_root = "interrupt"
__weak void ADC0SS3_Handler(void) {
  // STEP 4: Implement the ADC ISR.
  // 4.1: Clear the ADC0 interrupt flag
  ADCISC_0 = 0x8;
  // 4.2: Save the ADC value to global variable ADC_value
  ADC_value = (ADCSSFIFO3_0 & 0xFFF);
}
