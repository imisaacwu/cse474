/**
 * EE/CSE 474: Lab3 Task1b main function
 */

#include <stdint.h>
#include <stdio.h>

#include "../Lab3_Inits.h"
#include "../lab3.h"

const float VREFP = 3.3;
const float VREFN = 0.0;

uint32_t ADC_value;

int main(void) {
  // Select system clock frequency preset
  enum frequency freq = PRESET2; // 60 MHz
  PLL_Init(freq);        // Set system clock frequency to 60 MHz
  LED_Init();            // Initialize the 4 onboard LEDs (GPIO)
  ADCReadTemp_Init();     // Initialize ADC0 to read from the temperature sensor
  TimerADCTriger_Init(); // Initialize Timer0A to trigger ADC0
  
  float temperature, temp;
  
  while(1) {
    temp = 147.5 - ((75 * (VREFP - VREFN) * ADC_value) / 4096);
    if (temp != temperature) {
      temperature = temp;
      printf("Temperature: %f C\n", temperature);
    }
  }
  
  return 0;
}

#pragma call_graph_root = "interrupt"
__weak void ADC0SS3_Handler(void) {
  // Clear the ADC0 interrupt flag
  ADCISC_0 = 0x8;
  // Save the ADC value to global variable ADC_value
  ADC_value = (ADCSSFIFO3_0 & 0xFFF);
}