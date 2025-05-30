/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 30, 2025
 * Task 1a controls the LEDs on the TIVA board such that
 * LEDs D1-D4 turn on and off in a sequential manner at 
 * frequency of 1 Hz
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

  init(timer);              // Configure Timer 0
  enable(timer);            // Start timer
  
  // Periodic pattern
  while(1) {
    while (!isDone(timer));
    reset(timer);
    GPIODATA_N ^= 0x2;      // Flip LED D1
    
    while (!isDone(timer));
    reset(timer);
    GPIODATA_N ^= 0x1;      // Flip LED D2
    
    while (!isDone(timer));
    reset(timer);
    GPIODATA_F ^= 0x10;     // Flip LED D3
    
    while (!isDone(timer));
    reset(timer);
    GPIODATA_F ^= 0x1;      // Flip LED D4
  }
  return 0;
}

void config_ports() {
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x1020;       // Enable ports N and F
  delay++;
  delay++;

  GPIODIR_F |= 0x11;        // Set PF0 (LED D4) and PF4 (LED D3) as outputs
  GPIODEN_F |= 0x11;        // Set PF0 and PF4 to digital ports

  GPIODIR_N |= 0x3;         // Set PN0 (LED D2) and PN1 (LED D1) as outputs
  GPIODEN_N |= 0x3;         // Set PN0 and PN1 to digital ports

  GPIODATA_F &= ~0x11;      // Set PF0 and PF4 to 0 (off)
  GPIODATA_N &= ~0x3;       // Set PN0 and PN1 to 0 (off)
  
}