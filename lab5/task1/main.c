/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * June 5, 2025
 * EE/CSE 474: Lab5 Task1 controls an external LED using an
 * external potentiometer. Users can increase the resistance, which
 * brightens the external LED and they can decrease the 
 * resistance to dim the LED.
 */

#include <stdint.h>
#include "../Lab5_Inits.h"
#include "../lab5.h"

uint32_t ADC_value;

int main(void) {
  // Select system clock frequency preset
  enum frequency freq = PRESET2; // 60 MHz
  PLL_Init(freq);               // Set system clock frequency to 60 MHz
  ADCReadPot_Init();            // Initialize ADC0 to read from the potentiometer
  TimerADCTriger_Init();        // Initialize Timer0A to trigger ADC0
  PWM_Init();                   // Initialize M0PWM1        
  float resistance;
  int cmp;

  while(1) {
    // Convert ADC_value to resistance in kilo-ohm
    resistance = ADC_value / 409.5;
    // Convert resistance to comparator value
    cmp = (int) ((resistance / 10) * 2300) + 50;

    PWM0CTL = 0x0;
    PWM0CMPB = cmp;
    PWM0CTL = 0x1;       // Start timer
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
