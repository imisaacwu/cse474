/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 7, 2025
 * TESTING TESTING
 */

#include <stdint.h>

#include "../lab1.h"

// Delays execution for an arbitrary length of time
void delay();

int main(void) {
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

  // Periodic pattern
  while (1) {
    // PN1, PN0, PF4, PF0
    GPIODATA_N |= 0x2;
    delay();
    GPIODATA_N |= 0x1;
    delay();
    GPIODATA_F |= 0x10;
    delay();
    GPIODATA_F |= 0x1;
    delay();
    GPIODATA_N &= ~0x2;
    delay();
    GPIODATA_N &= ~0x1;
    delay();
    GPIODATA_F &= ~0x10;
    delay();
    GPIODATA_F &= ~0x1;
    delay();
  }

  return 0;
}

void delay() {
  for (int i = 0; i < 1000000; i++) {}
}
