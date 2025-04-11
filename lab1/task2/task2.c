#include <stdbool.h>
#include <stdint.h>

#include "../lab1.h"

int main(void) {
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x104;                                                            
  delay++;
  delay++;
  
  GPIOAMSEL_C &= ~0x10;   // Disable analog function of PC4
  GPIODIR_C |= 0x10;      // Set PC4 to output
  GPIOAFSEL_C &= ~0x10;   // Set PC4 regular port function
  GPIODEN_C |= 0x10;      // Enable digital output on PC4
  
  GPIODIR_J &= ~0x3;      // Set PJ0 (SW1) and PJ1 (SW2) as inputs
  GPIODEN_J |= 0x3;      // Set PJ0 and PJ1 to digital ports

  GPIOLOCK_J = UNLOCK;  // Unlock the GPIOCR register
  GPIOCR_J |= 0x1;       // Enable writing to GPIOPUR
  GPIOPUR_J |= 0x3;      // Enable PJ0 and PJ1's weak pull-up resistor
  GPIOCR_J &= ~0x1;       // Disable writing
  GPIOLOCK_J = LOCK;    // Lock the GPIOCR register
  
  // Define switch values
  volatile unsigned short sw1;
  
  while (1) {
    sw1 = (GPIODATA_J & 0x1) == 0;
    
    GPIODATA_C = (GPIODATA_C & ~0x10) | (sw1 << 4);
  }
  
  return 0;
}