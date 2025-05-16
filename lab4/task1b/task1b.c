/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * May 15, 2025
 * EE/CSE 474: Lab4 Task 1b print to LCD
 */

#include <stdint.h>

#include "../Lab4_Inits.h"
#include "../SSD2119_Display.h"
#include "../tm4c1294ncpdt.h"

const float VREFP = 3.3;  // Voltage Reference Positive
const float VREFN = 0.0;  // Voltage Reference Negative
uint32_t ADC_value;
enum frequency freq;

// Configures necessary ports
void config_ports();

int main(void) {
  // Select system clock frequency preset
  config_ports();
  LCD_Init();

  freq = PRESET2;               // 60 MHz
  PLL_Init(freq);               // Set system clock frequency to 60 MHz
  ADCReadTemp_Init();           // Initialize ADC0 to read from the temperature sensor
  TimerADCTriger_Init();        // Initialize Timer0A to trigger ADC0
  
  float temp_C, temp_F;
  
  while(1) {
    // Calculate temperature from the read ADC Value
    temp_C = 147.5 - ((75 * (VREFP - VREFN) * ADC_value) / 4096);
    temp_F = (temp_C * (9.0/5.0)) + 32;
    LCD_Printf("The current temperature is %f C, %f F.\n", temp_C, temp_F);
    LCD_Printf("The current clock frequency is %d MHz.\n", (int) freq);
  }
  
  return 0;
}

void config_ports() {
  volatile unsigned short delay = 0;
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R8;      // Enable port J
  delay++;
  delay++;
  
  GPIO_PORTJ_DIR_R &= ~0x3;                     // Set PJ0 (SW1) and PJ1 (SW2) as inputs
  GPIO_PORTJ_DEN_R |= 0x3;                      // Set PJ0 and PJ1 to digital ports

  GPIO_PORTJ_LOCK_R = 0x4C4F434B;               // Unlock the GPIOCR register
  GPIO_PORTJ_CR_R |= 0x1;                       // Enable writing to GPIOPUR
  GPIO_PORTJ_PUR_R |= 0x3;                      // Enable PJ0 and PJ1's weak pull-up resistor
  GPIO_PORTJ_CR_R &= ~0x1;                      // Disable writing
  GPIO_PORTJ_LOCK_R = 0x0;                      // Lock the GPIOCR register
  
  // Config interrupts for Port J
  GPIO_PORTJ_IM_R &= ~0x3;                      // Disable GPIO interrupts for PJ0 and PJ1
  GPIO_PORTJ_IS_R &= ~0x3;                      // Makes PJ0 and PJ1 edge-sensitive
  GPIO_PORTJ_IBE_R &= ~0x3;                     // Trigger on single edge
  GPIO_PORTJ_IEV_R &= ~0x3;                     // Trigger on the rising edge
  GPIO_PORTJ_ICR_R |= 0x3;                      // Clearing prior interrupt status
  GPIO_PORTJ_IM_R |= 0x3;                       // Re-enable GPIO interrupts for PJ0 and PJ1

  NVIC_EN1_R |= 0x80000;                        // Enable interrupt number 51
  NVIC_PRI12_R |= 0x70000000;                   // Set priority of interrupt 51 to 7
}

/**
 * Called by NVIC whenever Timer0A times out. Stores the result
 * of samples collected by Sequencer 3 into our global ADC_value
 */
#pragma call_graph_root = "interrupt"
__weak void ADC0SS3_Handler(void) {
  // Clear the ADC0 interrupt flag
  ADC0_ISC_R = 0x8;
  // Save the ADC value to global variable ADC_value
  ADC_value = (ADC0_SSFIFO3_R & 0xFFF);
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
  GPIO_PORTJ_ICR_R |= 0x3;
  // Check which button was pressed
  unsigned short sw1 = (GPIO_PORTJ_DATA_R & 0x1) == 0;
  unsigned short sw2 = (GPIO_PORTJ_DATA_R & 0x2) == 0;

  if (sw1) {
    freq = PRESET3;
  } else if (sw2) {
    freq = PRESET1;
  }
  PLL_Init(freq);
}