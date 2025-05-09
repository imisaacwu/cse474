/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * May 15, 2025
 * EE/CSE 474: Lab3 Task1a main function
 */

// NOTE: This is the main function for Task 1a. You should start by
// implementing Lab3_Driver.c (STEP 0a, 1-3) and then come back to finish
// the ISR and the main function (STEP 0b, 4-5).

#include <stdint.h>
#include "../Lab3_Inits.h"

#include "../lab3.h"

uint32_t ADC_value;

int main(void) {
  // Select system clock frequency preset
  enum frequency freq = PRESET2; // 60 MHz
  PLL_Init(freq);        // Set system clock frequency to 60 MHz
  LED_Init();            // Initialize the 4 onboard LEDs (GPIO)
  ADCReadPot_Init();     // Initialize ADC0 to read from the potentiometer
  TimerADCTriger_Init(); // Initialize Timer0A to trigger ADC0
  float resistance;
  while(1) {
    // STEP 5: Change the pattern of LEDs based on the resistance.
    // 5.1: Convert ADC_value to resistance in kilo-ohm
    resistance = ADC_value / 409.5;
    // 5.2: Change the pattern of LEDs based on the resistance
    if (resistance < 2.5) {
      GPIODATA(F) &= ~0x11;     // Set LED D3 and LED D4 to 0 (off)
      GPIODATA(N) &= ~0x1;      // Set LED D2 to 0 (off)
      GPIODATA(N) |= 0x2;       // Set LED D1 to 1 (on)
    } else if (resistance < 5.0) {
      GPIODATA(F) &= ~0x11;     // Set LED D4 and LED D3 to 0 (off)
      GPIODATA(N) |= 0x3;       // Set LED D2 and LED D1 to 1 (on)
    } else if (resistance < 7.5) {
      GPIODATA(F) &= ~0x1;      // Set LED D4 to 0 (off) 
      GPIODATA(F) |= 0x10;      // and LED D3 to 1 (on)
      GPIODATA(N) |= 0x3;       // Set LED D2 and LED D1 to 1 (on)
    } else {
      GPIODATA(F) |= 0x11;      // Set LED D3 and LED D4 to 1 (on)
      GPIODATA(N) |= 0x3;       // Set LED D2 and LED D1 to 1 (on)
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
