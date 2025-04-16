/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 14, 2025
 * Task 2 represents a traffic light system where the system will start
 * on the red light when the system is on. Users can press the pedestrian
 * during a green light to change the light from yellow to red.
 */

#include <stdint.h>
#include "../lab1.h"
#include "LED.h"

// Adds a delay of ~0.3 seconds
void delay();

/** Updates the traffic light state based on the given inputs
 *
 * @param pwr the state of the power button input
 * @param ped the state of the pedestrian button input
 * @param grn the green LED output
 * @param ylw the yellow LED output
 * @param red the red LED output
 */
void tick_traffic(volatile unsigned short pwr, volatile unsigned short ped,
                  struct LED grn, struct LED ylw, struct LED red);

// The set of possible states for the traffic light system
enum States { Off, Stop, Warn, Go } state;

int main(void) {
  volatile unsigned short port_delay = 0;
  RCGCGPIO |= 0x14;       // Enable ports C, E
  port_delay++;
  port_delay++;
  
  // Configure Port C for LEDs
  GPIOAMSEL_C &= ~0x70;   // Disable analog function of PC4-6
  GPIODIR_C |= 0x70;      // Set PC4-6 to output
  GPIOAFSEL_C &= ~0x70;   // Set PC4-6 regular port function
  GPIODEN_C |= 0x70;      // Enable digital output on PC4-6
  
  // Configure Port E for external buttons
  GPIOAMSEL_E &= ~0x3;    // Disable analog function of PE0-1
  GPIODIR_E &= ~0x3;      // Set PE0-1 to input
  GPIOAFSEL_E &= ~0x3;    // Set PE0-1 regular port function
  GPIODEN_E |= 0x3;       // Enable digital input on PE0-1
  
  // Connect configured Port C pins to LEDs
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
    // Checks if pwr is on or off
    pwr = (GPIODATA_E & 0x1) == 0x1;
    // Checks if ped is on or off
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
    case Stop:  // Transitions
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
  
  switch (state) {  // State actions
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
