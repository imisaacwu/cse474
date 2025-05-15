/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * May 15, 2025
 * EE/CSE 474: Lab3 Task2a reads temperature input from
 * the on-board internal temperature sensor and prints it
 * to the PuTTY terminal.
 */

#include <stdint.h>
#include <stdio.h>  // printf()
#include <string.h> // strlen

#include "../Lab3_Inits.h"
#include "../lab3.h"
#include "../timer.h"

const float VREFP = 3.3;  // Voltage Reference Positive
const float VREFN = 0.0;  // Voltage Reference Negative
uint32_t ADC_value;
char message[100];
int i;

// Configures necessary ports
void config_ports();

int main(void) {
  // Select system clock frequency preset
  config_ports();

  enum frequency freq = PRESET2;  // 60 MHz
  PLL_Init(freq);                 // Set system clock frequency to 60 MHz
  ADCReadTemp_Init();             // Initialize ADC0 to read from the temperature sensor
  TimerADCTriger_Init();          // Initialize Timer0A to trigger ADC0
  
  float temperature;
  
  while(1) {
    temperature = 147.5 - ((75 * (VREFP - VREFN) * ADC_value) / 4096);
    sprintf(message, "Temp: %.2f C\n", temperature);
    printf(message);
    
    for (i = 0; i < strlen(message); i++) {
      while ((UARTFR(0) & (1 << 5)));
      UARTDR(0) = message[i];
    }
  }
  
  return 0;
}

void config_ports() {
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x101;            // Enable port J and A
  RCGCUART |= 0x1;              // Enable UART 0
  delay++;
  delay++;
  
  GPIODIR(J) &= ~0x3;           // Set PJ0 (SW1) and PJ1 (SW2) as inputs
  GPIODEN(J) |= 0x3;            // Set PJ0 and PJ1 to digital ports

  GPIOLOCK(J) = UNLOCK;         // Unlock the GPIOCR register
  GPIOCR(J) |= 0x3;             // Enable writing to GPIOPUR
  GPIOPUR(J) |= 0x3;            // Enable PJ0 and PJ1's weak pull-up resistor
  GPIOCR(J) &= ~0x3;            // Disable writing
  GPIOLOCK(J) = LOCK;           // Lock the GPIOCR register
  
  // Config interrupts for Port J
  GPIOIM(J) &= ~0x3;            // Disable GPIO interrupts for PJ0 and PJ1
  GPIOIS(J) &= ~0x3;            // Makes PJ0 and PJ1 edge-sensitive
  GPIOIBE(J) &= ~0x3;           // Trigger on single edge
  GPIOIEV(J) &= ~0x3;           // Trigger on the rising edge
  GPIOICR(J) |= 0x3;            // Clearing prior interrupt status
  GPIOIM(J) |= 0x3;             // Re-enable GPIO interrupts for PJ0 and PJ1

  NVIC_EN1 |= 0x80000;          // Enable interrupt number 51
  NVIC_PRI12 |= 0x70000000;     // Set priority of interrupt 51 to 7
  
  // Configure UART 0 Rx/Tx on Port A
  GPIOLOCK(A) = UNLOCK;         // Unlock the GPIOCR register
  GPIOCR(A) |= 0x3;             // Enable writing to GPIOAFSEL
  GPIOAFSEL(A) |= 0x3;          // Set Port PA0 and PA1 to do alternate functions
  GPIOCR(A) &= ~0x3;            // Disable writing
  GPIOLOCK(A) = LOCK;           // Lock the GPIOCR register
  GPIOPCTL(A) |= 0x11;          // Set PA0/PA1 to UART 0 Rx/Tx
  GPIODEN(A) |= 0x3;            // Enable digital function for PA0 & PA1
  
  // Configure UART
  UARTCTL(0) &= ~0x1;
  //while (UARTFR_0 & 0x8);       // Wait for transmit/receive to complete
  //UARTLCRH_0 &= ~0x10;          // Flush transmit FIFO
  
  // Set UART to use the Alternate Clock Source from (ALTCLKCFG)
  UARTCC(0) |= 0x5;

  // We want the baud rate to be 9600 with ClkDiv=16 and UARTSysClk=16MHz
  // So we need BRDI=104 and BRDF=0.16... -> integer(0.16... * 64 + 0.5) = 11
  UARTIBRD(0) = 104;
  UARTFBRD(0) = 11;
  // Write to UARTLCRH_0 to update baud rate configs, keep default settings
  //UARTLCRH_0 = 0;
  // Set UART Word Length to 8 bits
  UARTLCRH(0) |= 0x60;
  // Enable UART FIFOs
  // UARTLCRH_0 |= 0x10;
  
  // Enable UART
  UARTCTL(0) |= 0x1;
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