/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 14, 2025
 * Task 1b represents a traffic light system where the system will start
 * on the red light when the system is on. Users can press and hold the
 * pedestrian button during a green light to change the light from yellow to red.
 */

#include <stdint.h>

#include "../lab2.h"
#include "LED.h"

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
  
  // Enable Timer 0, 1, and 2
  RCGCTIMER |= 0x7;
  
  // Configure Timer 0 to be our 5-second timer
  GPTMCTL_0 &= ~0x101;      // Disable Timer A/B
  GPTMCFG_0 = 0x00000000;   // Reset Timer 0 configs
  GPTMCFG_0 &= ~0x7;        // Set Timer 0 to be 32-bit
  GPTMTAMR_0 |= 0x3;
  GPTMTAMR_0 &= ~0x1;       // Set Timer A/B to Period Timer mode
  GPTMTAMR_0 &= ~0x10;      // Set Timer A/B to counts down
  GPTMTAILR_0 = 0x4C4B400;  // Load 80,000,000 to achieve 5 seconds
  
  // Configure Timer 1 to be our 2-second power button timer
  GPTMCTL_1 &= ~0x101;      // Disable Timer A/B
  GPTMCFG_1 = 0x00000000;   // Reset Timer 1 configs
  GPTMCFG_1 &= ~0x7;        // Set Timer 1 to be 32-bit
  GPTMTAMR_1 |= 0x3;
  GPTMTAMR_1 &= ~0x2;       // Set Timer A/B to One-shot Timer mode
  GPTMTAMR_1 &= ~0x10;      // Set Timer A/B to counts down
  GPTMTAILR_1 = 0x1E84800;  // Load 32,000,000 to achieve 2 seconds
  
  // Configure Timer 2 to be our 2-second pedestrian button timer
  GPTMCTL_2 &= ~0x101;      // Disable Timer A/B
  GPTMCFG_2 = 0x00000000;   // Reset Timer 2 configs
  GPTMCFG_2 &= ~0x7;        // Set Timer 2 to be 32-bit
  GPTMTAMR_2 |= 0x3;
  GPTMTAMR_2 &= ~0x2;       // Set Timer A/B to One-shot Timer mode
  GPTMTAMR_2 &= ~0x10;      // Set Timer A/B to counts down
  GPTMTAILR_2 = 0x1E84800;  // Load 32,000,000 to achieve 2 seconds

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
  
  volatile unsigned short pwr_pressed;
  voltaile unsigned short ped_pressed;
  
  volatile unsigned short TATORIS_0;
  volatile unsigned short TATORIS_1;
  volatile unsigned short TATORIS_2;

  while (1) {
    // Checks if pwr is on or off
    pwr_pressed = (GPIODATA_E & 0x1) == 0x1;
    // Checks if ped is on or off
    ped_pressed = (GPIODATA_E & 0x2) == 0x2;
    
    // Timer states
    TATORIS_0 = (GPTMRIS_0 & 0x1);
    TATORIS_1 = (GPTMRIS_1 & 0x1);
    TATORIS_2 = (GPTMRIS_2 & 0x1);

    if (pwr_pressed && (GPTMCTL_1 & 0x1) == 0x0) {
      // Not yet enabled, start the timer
      GPTMCTL_1 |= 0x1;
    } else {
      // Released, stop timer
      GPTMICR_1 |= 0x1;
      GPTMCTL_1 &= ~0x1;
    }
    
    if (ped_pressed && (GPTMCTL_2 & 0x1) == 0x0) {
      // Not yet enabled, start the timer
      GPTMCTL_2 |= 0x1;
    } else {
      // Released, stop timer
      GPTMICR_2 |= 0x1;
      GPTMCTL_2 &= ~0x1;
    }
    
    pwr = TATORIS_1;
    ped = TATORIS_2;
     
    tick_traffic(pwr, ped, grn, ylw, red);
  }
  
  return 0;
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