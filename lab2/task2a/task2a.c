/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 30, 2025
 * Task 2a controls the LEDs on the TIVA board such that
 * LEDs D1-D4 turn on and off in a sequential manner. LED
 * handling is done via an interrupt handler instead of
 * the main program.
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

// Global variable to keep track of which part of the cycle we are in
uint32_t i = 0;

int main(void) {
  config_ports();

  init(timer);
  GPTMIMR(0) |= 0x1;        // Enable interrupt on time-out
  NVIC_EN0 |= 0x80000;      // Enable interrupt number 19
  enable(timer);
  
  while(1) {}
  
  return 0;
}

void config_ports() {
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x1020;       // Enable ports N and F
  RCGCTIMER |= 0x1;         // Enable Timer 0
  delay++;
  delay++;

  GPIODIR_F |= 0x11;        // Set PF0 (LED D4) and PF4 (LED D3) as outputs
  GPIODEN_F |= 0x11;        // Set PF0 and PF4 to digital ports

  GPIODIR_N |= 0x3;         // Set PN0 (LED D2) and PN1 (LED D1) as outputs
  GPIODEN_N |= 0x3;         // Set PN0 and PN1 to digital ports

  GPIODATA_F &= ~0x11;      // Set PF0 and PF4 to 0 (off)
  GPIODATA_N &= ~0x3;       // Set PN0 and PN1 to 0 (off)
}

/**
 * Handles the event trigger when Timer 0A times out. Resets
 * the timer then switches on D1-D4 sequentially such that
 * all LEDs turn on then off in a periodic manner.
 */
#pragma call_graph_root = "interrupt"
__weak void Timer0A_Handler ( void ) { 
  // Reset Timer 0A
  GPTMICR(0) |= 0x1;
  switch (i++ % 4) {
    case 0:
      GPIODATA_N ^= 0x2;      // Flip LED D1
      break;
    case 1:
      GPIODATA_N ^= 0x1;
      break;
    case 2:
      GPIODATA_F ^= 0x10;
      break;
    case 3:
      GPIODATA_F ^= 0x1;
      break;
  }       
}
