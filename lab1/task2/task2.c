#include <stdbool.h>
#include <stdint.h>

#include "../lab1.h"
#include "LED.c"

void delay();

int main(void) {
  volatile unsigned short port_delay = 0;
  RCGCGPIO |= 0x4;        // Enable port C
  port_delay++;
  port_delay++;
  
  GPIOAMSEL_C &= ~0x70;   // Disable analog function of PC4-6
  GPIODIR_C |= 0x70;      // Set PC4-6 to output
  GPIOAFSEL_C &= ~0x70;   // Set PC4-6 regular port function
  GPIODEN_C |= 0x70;      // Enable digital output on PC4-6
  
  struct LED grn = {&GPIODATA_C, 4};
  struct LED ylw = {&GPIODATA_C, 5};
  struct LED red = {&GPIODATA_C, 6};
  
  off(grn);
  off(ylw);
  off(red);
 
  while (1) {
    on(red);
    delay();
    
    off(red);
    on(grn);
    delay();
    
    off(grn);
    on(ylw);
    delay();
    
    off(ylw);
  }
  
  return 0;
}

void delay() {
  for (int i = 0; i < 1000000; i++) {}
}