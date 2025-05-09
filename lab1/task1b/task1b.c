/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 7, 2025
 * Task 1b controls the LED D1 and LED D2 whenever
 * the user switch SW1 is pressed (turning on D1) and when
 * the user switch SW2 is presssd (turning on D2)
 */

#include <stdint.h>

#include "../lab1.h"

int main(void) {
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x1100;       // Enable port N and J
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
  
  // Define switch values
  volatile unsigned short sw1;
  volatile unsigned short sw2;

  while (1) {
    // Buttons are active low, so invert
    sw1 = (GPIODATA_J & 0x1) == 0;
    sw2 = (GPIODATA_J & 0x2) == 0;
    
    // Set LED state to input from switch
    GPIODATA_N = (GPIODATA_N & ~0x2) | (sw1 << 1);
    GPIODATA_N = (GPIODATA_N & ~0x1) | (sw2 << 0);
  }
  
  return 0;
}
