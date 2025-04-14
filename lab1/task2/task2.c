#include <stdbool.h>
#include <stdint.h>

#include "../lab1.h"
#include "LED.h"

void delay();
void tick_traffic(volatile unsigned short pwr, volatile unsigned short ped,
                  struct LED grn, struct LED ylw, struct LED red);

enum States { Off, Stop, Warn, Go } state;

int main(void) {
  volatile unsigned short port_delay = 0;
  RCGCGPIO |= 0x14;        // Enable port C
  port_delay++;
  port_delay++;
  
  GPIOAMSEL_C &= ~0x70;   // Disable analog function of PC4-6
  GPIODIR_C |= 0x70;      // Set PC4-6 to output
  GPIOAFSEL_C &= ~0x70;   // Set PC4-6 regular port function
  GPIODEN_C |= 0x70;      // Enable digital output on PC4-6
  
  GPIOAMSEL_E &= ~0x3;    // Disable analog function of PE0-1
  GPIODIR_E &= ~0x3;      // Set PE0-1 to input
  GPIOAFSEL_E &= ~0x3;    // Set PE0-1 regular port function
  GPIODEN_E |= 0x3;       // Enable digital input on PE0-1
  
  struct LED grn = {&GPIODATA_C, 4};
  struct LED ylw = {&GPIODATA_C, 5};
  struct LED red = {&GPIODATA_C, 6};
  
  off(grn);
  off(ylw);
  off(red);
 
  state = Off;
  
  volatile unsigned short pwr;
  volatile unsigned short ped;

  while (1) {
    pwr = (GPIODATA_E & 0x1) == 0x1;
    ped = (GPIODATA_E & 0x2) == 0x2;
     
    tick_traffic(pwr, ped, grn, ylw, red);
    delay();
  }
  
  return 0;
}

void delay() {
  for (int i = 0; i < 1000000; i++) {}
}

void tick_traffic(volatile unsigned short pwr, volatile unsigned short ped,
                  struct LED grn, struct LED ylw, struct LED red) {
  switch (state) {
    case Stop:
      off(red);
      if (pwr) {
        state = Off;
      } else if (ped) {
        state = Stop;
      } else {
        state = Go;
      }
      break;

    case Warn:
      off(ylw);
      if (pwr) {
        state = Off;
      } else {
        state = Stop;
      }
      break;
    
    case Go:
      off(grn);
      if (pwr) {
        state = Off;
      } else if (ped) {
        state = Warn;
      } else {
        state = Stop;
      }
      break;
    
    case Off:
      if (pwr) {
        state = Go;
      }
      break;
  }
  
  switch (state) {
    case Stop:
      on(red);
      break;
    
    case Warn:
      on(ylw);
      break;
      
    case Go:
      on(grn);
      break;
    
    case Off:
      break;
  }
}