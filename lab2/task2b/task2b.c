/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 29, 2025
 * Task 2b is a simple two-button system that enables and disables
 * the periodic flashing of an on-board LED.
 */

#include <stdint.h>

#include "../lab2.h"
#include "../timer.h"

// Configures necessary ports
void config_ports();

// Global timer
struct Timer timer = {
  0, TIMER_PERIODIC, 1 * CLK_FRQ,
  &GPTMCTL(0), &GPTMCFG(0), &GPTMTAMR(0), &GPTMTAILR(0), &GPTMRIS(0), &GPTMICR(0)
};

int main(void) {
  config_ports();

  init(timer);
  GPTMIMR(0) |= 0x1;        // Enable interrupt on time-out
  NVIC_EN0 |= 0x80000;      // Enable interrupt number 19
  enable(timer);

  while (1) {}
  
  return 0;
}

void config_ports() {
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x1100;       // Enable port N and J
  RCGCTIMER |= 0x1;         // Enable Timer 0
  delay++;
  delay++;

  GPIODIR_N |= 0x3;         // Set PN0 (LED D2) and PN1 (LED D1) as outputs
  GPIODEN_N |= 0x3;         // Set PN0 and PN1 to digital ports
  GPIODATA_N &= ~0x3;       // Set PN0 and PN1 to 0 (off)
  
  GPIODIR_J &= ~0x3;        // Set PJ0 (SW1) and PJ1 (SW2) as inputs
  GPIODEN_J |= 0x3;         // Set PJ0 and PJ1 to digital ports

  GPIOLOCK_J = UNLOCK;      // Unlock the GPIOCR register
  GPIOCR_J |= 0x1;          // Enable writing to GPIOPUR
  GPIOPUR_J |= 0x3;         // Enable PJ0 and PJ1's weak pull-up resistor
  GPIOCR_J &= ~0x1;         // Disable writing
  GPIOLOCK_J = LOCK;        // Lock the GPIOCR register
  
  // Config interrupts for Port J
  GPIOIM_J &= ~0x3;         // Disable GPIO interrupts for PJ0 and PJ1
  GPIOIS_J &= ~0x3;         // Makes PJ0 and PJ1 edge-sensitive
  GPIOIBE_J &= ~0x3;        // Trigger on single edge
  GPIOIEV_J |= 0x3;         // Trigger on the rising edge
  GPIOICR_J |= 0x3;         // Clearing prior interrupt status
  GPIOIM_J |= 0x3;          // Re-enable GPIO interrupts for PJ0 and PJ1

  NVIC_EN1 |= 0x80000;      // Enable interrupt number 51
  NVIC_PRI12 |= 0x70000000; // Set priority of interrupt 51 to 7
}

/**
 * Method that handles the time-out events of Timer0A
 * Called by NVIC when Timer0A times out.
 * Will clear the interrupt status (i.e. resets timer)
 * Blinks LED D1.
 */
#pragma call_graph_root = "interrupt"
__weak void Timer0A_Handler ( void ) {
  reset(timer);
  GPIODATA_N ^= 0x2;
}

/**
 * Method that handles rising edges on Port J
 * Called by NVIC when any GPIO on Port J has a rising edge
 * Will clear the interrupt status (i.e. allow for more interrupts)
 * If button SW1 was pressed, will stop the flashing of LED1 and turn
 * on LED 2. If button SW2 was pressed, will re-enable the flashing of
 * LED1 if previously disabled and turn off LED 2.
 */
#pragma call_graph_root = "interrupt"
__weak void PortJ_Handler ( void ) {
  // Reset interrupt status
  GPIOCR_J |= 0x3;
  // Check which button was pressed
  unsigned short sw1 = (GPIODATA_J & 0x1) == 0;
  unsigned short sw2 = (GPIODATA_J & 0x2) == 0;

  if (sw1) {
    // Disable Timer 0A
    disable(timer);
    // Turn on LED2
    GPIODATA_N |= 0x1;
  } else if (sw2) {
    // Enable Timer 0A
    enable(timer);
    // Turn off LED2
    GPIODATA_N &= ~0x1;
  }
}
