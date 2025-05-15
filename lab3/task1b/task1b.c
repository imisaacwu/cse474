/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * May 15, 2025
 * EE/CSE 474: Lab3 Task1b main function
 */

#include <stdint.h>
#include <stdio.h>  // printf()

#include "../Lab3_Inits.h"
#include "../lab3.h"

const float VREFP = 3.3;  // Voltage Reference Positive
const float VREFN = 0.0;  // Voltage Reference Negative
uint32_t ADC_value;

// Configures necessary ports
void config_ports();

int main(void) {
  // Select system clock frequency preset
  config_ports();

  enum frequency freq = PRESET2;  // 60 MHz
  PLL_Init(freq);                 // Set system clock frequency to 60 MHz
  LED_Init();                     // Initialize the 4 onboard LEDs (GPIO)
  ADCReadTemp_Init();             // Initialize ADC0 to read from the temperature sensor
  TimerADCTriger_Init();          // Initialize Timer0A to trigger ADC0
  
  float temperature;
  
  while(1) {
    // Calculate temperature from the read ADC Value
    temperature = 147.5 - ((75 * (VREFP - VREFN) * ADC_value) / 4096);
    printf("Temperature: %f C\n", temperature);
  }
  
  return 0;
}

void config_ports() {
  volatile unsigned short delay = 0;
  RCGCGPIO |= BIT(8);       // Enable port N and J
  delay++;
  delay++;
  
  GPIODIR(J) &= ~0x3;        // Set PJ0 (SW1) and PJ1 (SW2) as inputs
  GPIODEN(J) |= 0x3;         // Set PJ0 and PJ1 to digital ports

  GPIOLOCK(J) = UNLOCK;      // Unlock the GPIOCR register
  GPIOCR(J) |= 0x1;          // Enable writing to GPIOPUR
  GPIOPUR(J) |= 0x3;         // Enable PJ0 and PJ1's weak pull-up resistor
  GPIOCR(J) &= ~0x1;         // Disable writing
  GPIOLOCK(J) = LOCK;        // Lock the GPIOCR register
  
  // Config interrupts for Port J
  GPIOIM(J) &= ~0x3;         // Disable GPIO interrupts for PJ0 and PJ1
  GPIOIS(J) &= ~0x3;         // Makes PJ0 and PJ1 edge-sensitive
  GPIOIBE(J) &= ~0x3;        // Trigger on single edge
  GPIOIEV(J) &= ~0x3;        // Trigger on the rising edge
  GPIOICR(J) |= 0x3;         // Clearing prior interrupt status
  GPIOIM(J) |= 0x3;          // Re-enable GPIO interrupts for PJ0 and PJ1

  NVIC_EN1 |= 0x80000;      // Enable interrupt number 51
  NVIC_PRI12 |= 0x70000000; // Set priority of interrupt 51 to 7
}

/**
 * Called by NVIC whenever Timer0A times out. Stores the result
 * of samples collected by Sequencer 3 into our global ADC_value
 */
#pragma call_graph_root = "interrupt"
__weak void ADC0SS3_Handler(void) {
  // Clear the ADC0 interrupt flag
  ADCISC_0 = 0x8;
  // Save the ADC value to global variable ADC_value
  ADC_value = (ADCSSFIFO3_0 & 0xFFF);
}

/**
 * Method that handles rising edges on Port J
 * Called by NVIC when any GPIO on Port J has a rising edge
 * Will clear the interrupt status (i.e. allow for more interrupts)
 *
 * If button SW1 is pressed, will switch the clock frequency to 12 MHz
 * If button SW2 is pressed, will switch the clock frequency to 120 MHz
 */
#pragma call_graph_root = "interrupt"
__weak void PortJ_Handler ( void ) {
  // Reset interrupt status
  GPIOICR(J) |= 0x3;
  // Check which button was pressed
  unsigned short sw1 = (GPIODATA(J) & 0x1) == 0;
  unsigned short sw2 = (GPIODATA(J) & 0x2) == 0;

  if (sw1) {
    PLL_Init(PRESET3);
  } else if (sw2) {
    PLL_Init(PRESET1);
  }
}