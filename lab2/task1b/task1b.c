/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 14, 2025
 * Task 1b represents a traffic light system where the system will start
 * on the red light when the system is on. Users can press and hold the
 * pedestrian button during a green light to change the light from yellow to red.
 */

#include <stdbool.h>
#include <stdint.h>

#include "../lab2.h"
#include "../timer.h"
#include "LED.h"

/** Updates the traffic light state based on the given inputs
 *
 * @param pwr the state of the power button input
 * @param ped the state of the pedestrian button input
 * @param grn the green LED output
 * @param ylw the yellow LED output
 * @param red the red LED output
 */
void tick_traffic(unsigned short pwr, unsigned short ped,
                  struct LED grn, struct LED ylw, struct LED red);

// The set of possible states for the traffic light system
enum States { Off, Stop, Warn, Go } state;

int main(void) {
  volatile unsigned short port_delay = 0;
  RCGCGPIO |= 0x1014;       // Enable ports C, E
  RCGCTIMER |= 0x7;
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
  
  GPIODIR_N |= 0x3;         // Set PN0 (LED D2) and PN1 (LED D1) as outputs
  GPIODEN_N |= 0x3;         // Set PN0 and PN1 to digital ports
  GPIODATA_N &= ~0x3;       // Set PN0 and PN1 to 0 (off)
  
  GPTMCTL_0 &= ~0x101;      // Disable Timer A/B
  GPTMCFG_0 = 0x00000000;   // Reset Timer 0 configs
  GPTMCFG_0 &= ~0x7;        // Set Timer 0 to be 32-bit
  GPTMTAMR_0 |= 0x3;
  GPTMTAMR_0 &= ~0x1;       // Set Timer A/B to Period Timer mode
  GPTMTAMR_0 &= ~0x10;      // Set Timer A/B to counts down
  GPTMTAILR_0 = 32000000;   // Load 32,000,000 to achieve 0.5Hz
  
  GPTMCTL_1 &= ~0x101;      // Disable Timer A/B
  GPTMCFG_1 = 0x00000000;   // Reset Timer 1 configs
  GPTMCFG_1 &= ~0x7;        // Set Timer 1 to be 32-bit
  GPTMTAMR_1 |= 0x3;
  GPTMTAMR_1 &= ~0x1;       // Set Timer A/B to Period Timer mode
  GPTMTAMR_1 &= ~0x10;      // Set Timer A/B to counts down
  GPTMTAILR_1 = 32000000;   // Load 32,000,000 to achieve 0.5Hz
  
  GPTMCTL_2 &= ~0x101;      // Disable Timer A/B
  GPTMCFG_2 = 0x00000000;   // Reset Timer 2 configs
  GPTMCFG_2 &= ~0x7;        // Set Timer 2 to be 32-bit
  GPTMTAMR_2 |= 0x3;
  GPTMTAMR_2 &= ~0x1;       // Set Timer A/B to Period Timer mode
  GPTMTAMR_2 &= ~0x10;      // Set Timer A/B to counts down
  GPTMTAILR_2 = 80000000;   // Load 80,000,000 to achieve 0.2Hz
  
  // Connect configured Port C pins to LEDs
  struct LED grn = {&GPIODATA_C, 4};
  struct LED ylw = {&GPIODATA_C, 5};
  struct LED red = {&GPIODATA_C, 6};
  
  off(grn);
  off(ylw);
  off(red);
 
  state = Off;
  
  unsigned short pwr;
  unsigned short ped;
  
  unsigned short TATORIS_pwr;
  unsigned short TATORIS_ped;
  unsigned short TATORIS_5sec;

  while (1) {
    // Checks if pwr is on or off
    pwr = (GPIODATA_E & 0x1) == 0x1;
    // Checks if ped is on or off
    ped = (GPIODATA_E & 0x2) == 0x2;
    
    // pre: pwr_timer and ped_timer is off (isDone == true)
                  
    if (pwr) {
      GPTMCTL_0 |= 0x1; // start timer
      TATORIS_pwr = (GPTMRIS_0 & 0x1);
      if (TATORIS_pwr == 0x1 && pwr) { // Power has been held for 2 seconds
        // Reset power timer interrupt status (start counting again)
        GPTMICR_0 |= 0x1;

        if (state == Off) {
          // System was just turned on, start 5-second timer
          GPTMCTL_2 |= 0x1;
        }

        tick_traffic(pwr, 0, grn, ylw, red);

        if (state == Off) {
          // System was just turned off, cancel 5-second timer
          GPTMCTL_2 &= ~0x1;
          GPTMCFG_2 = 0x00000000;
          GPTMCFG_2 &= ~0x7;        // Set Timer 2 to be 32-bit
          GPTMTAMR_2 |= 0x3;
          GPTMTAMR_2 &= ~0x1;       // Set Timer A/B to Period Timer mode
          GPTMTAMR_2 &= ~0x10;      // Set Timer A/B to counts down
          GPTMTAILR_2 = 80000000;   // Load 80,000,000
        }
      }
    } else {
      // Power button released, cancel timer
      GPTMCTL_0 &= ~0x1;
      GPTMCFG_0 = 0x00000000;
      GPTMCFG_0 &= ~0x7;        // Set Timer 0 to be 32-bit
      GPTMTAMR_0 |= 0x3;
      GPTMTAMR_0 &= ~0x1;       // Set Timer A/B to Period Timer mode
      GPTMTAMR_0 &= ~0x10;      // Set Timer A/B to counts down
      GPTMTAILR_0 = 32000000;   // Load 32,000,000
    }
    
    if (ped) {
      GPTMCTL_1 |= 0x1;
      TATORIS_ped = (GPTMRIS_1 & 0x1);
      if (TATORIS_ped == 0x1) {
        // Reset interrupt status (start counting again)
        GPTMICR_1 |= 0x1;
        tick_traffic(0, ped, grn, ylw, red);
        
        if (state == Warn) {
          // Ped moved the state to Warn, reset the timer before going to Stop
          GPTMCTL_2 &= ~0x1;
          GPTMCFG_2 = 0x00000000;
          GPTMCFG_2 &= ~0x7;        // Set Timer 2 to be 32-bit
          GPTMTAMR_2 |= 0x3;
          GPTMTAMR_2 &= ~0x1;       // Set Timer A/B to Period Timer mode
          GPTMTAMR_2 &= ~0x10;      // Set Timer A/B to counts down
          GPTMTAILR_2 = 80000000;   // Load 80,000,000
          GPTMCTL_2 |= 0x1;         // Re-enable 5-second timer
        }
      }
    } else {
      // Pedestrian button released, cancel timer
      GPTMCTL_1 &= ~0x1;        // Disable Timer A/B
      GPTMCFG_1 = 0x00000000;   // Reset Timer 1 configs
      GPTMCFG_1 &= ~0x7;        // Set Timer 1 to be 32-bit
      GPTMTAMR_1 |= 0x3;
      GPTMTAMR_1 &= ~0x1;       // Set Timer A/B to Period Timer mode
      GPTMTAMR_1 &= ~0x10;      // Set Timer A/B to counts down
      GPTMTAILR_1 = 32000000;   // Load 32,000,000 to achieve 0.5Hz
    }
    
    TATORIS_5sec = (GPTMRIS_2 & 0x1);
    if (TATORIS_5sec == 0x1) {
      // 5 seconds have elapsed, tick traffic
      tick_traffic(0, 0, grn, ylw, red);
      // Reset 5-second timer
      GPTMICR_2 |= 0x1;
    }
    
    // Debug code?
    // GPIODATA_N = (GPIODATA_N & ~0x1) | pwr;
    // GPIODATA_N = (GPIODATA_N & ~0x2) | (ped << 1);
  }
  
  return 0;
}

// Timer is not quite consistent
void tick_traffic(unsigned short pwr, unsigned short ped,
                  struct LED grn, struct LED ylw, struct LED red) {
  switch (state) {
    case Stop:  // Transitions
      off(red);
      if (pwr) {
        state = Off;
      } else if (ped) {  // Stay on red if ped was pressed at time of tick
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
        state = Stop;
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