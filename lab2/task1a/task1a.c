/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 18, 2025
 * Task 1a controls the LEDs on the TIVA board such that
 * LEDs D1-D4 turn on and off in a sequential manner every 1 Hz.
 */

#include <stdint.h>

#include "../lab2.h"
#include "../timer.h"

 int main(void) {
  volatile unsigned short tick = 0;
  RCGCGPIO |= 0x1020;       // Enable ports N and F
  tick++;
  tick++;

  GPIODIR_F |= 0x11;        // Set PF0 (LED D4) and PF4 (LED D3) as outputs
  GPIODEN_F |= 0x11;        // Set PF0 and PF4 to digital ports

  GPIODIR_N |= 0x3;         // Set PN0 (LED D2) and PN1 (LED D1) as outputs
  GPIODEN_N |= 0x3;         // Set PN0 and PN1 to digital ports

  GPIODATA_F &= ~0x11;      // Set PF0 and PF4 to 0 (off)
  GPIODATA_N &= ~0x3;       // Set PN0 and PN1 to 0 (off)
  
  struct Timer timer = {
    &GPTMCTL_0, &GPTMCFG_0, &GPTMTAMR_0, &GPTMTAILR_0, &GPTMRIS_0, &GPTMICR_0
  };
  init(timer, 0, 0x2, 16000000);
  enable(timer);
  
  volatile unsigned short TATORIS;
  
  // Periodic pattern
  while(1) {

    while (!isDone(timer));
    reset(timer);
    GPIODATA_N ^= 0x2;      // Flip LED D1
    
    while (!isDone(timer));
    reset(timer);
    GPIODATA_N ^= 0x1;
    
    while (!isDone(timer));
    reset(timer);
    GPIODATA_F ^= 0x10;
    
    while (!isDone(timer));
    reset(timer);
    GPIODATA_F ^= 0x1;
  }
  return 0;
}