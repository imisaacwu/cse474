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
void tick_traffic(struct Timer clk_timer, bool pwr, bool ped,
                  struct LED grn, struct LED ylw, struct LED red);

// The set of possible states for the traffic light system
enum States { Off, Stop, Warn, Go } state;

int main(void) {
  volatile unsigned short port_delay = 0;
  RCGCGPIO |= 0x1014;       // Enable ports C, E
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

  struct Timer clk_timer = {&GPTMCTL_0, &GPTMCFG_0, &GPTMTAMR_0, &GPTMTAILR_0, &GPTMRIS_0, &GPTMICR_0};
  init(clk_timer, 0, 0x2, 80000000);
  
  struct Timer pwr_timer = {&GPTMCTL_1, &GPTMCFG_1, &GPTMTAMR_1, &GPTMTAILR_1, &GPTMRIS_1, &GPTMICR_1};
  init(pwr_timer, 1, 0x2, 32000000);
  
  struct Timer ped_timer = {&GPTMCTL_2, &GPTMCFG_2, &GPTMTAMR_2, &GPTMTAILR_2, &GPTMRIS_2, &GPTMICR_2};
  // Likes to crash at this line for some reason?
  init(ped_timer, 2, 0x2, 32000000);
  
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
    
    tick_traffic(clk_timer, isDone(pwr_timer), isDone(ped_timer), grn, ylw, red);
                  
    if (pwr) {
      if (!isEnabled(pwr_timer)) {
        enable(pwr_timer);
      } else if (isDone(pwr_timer)) {
        // This results in weird behavior on power-up
        //reset(pwr_timer);
      }
    } else if (isEnabled(pwr_timer)) {
      reset(pwr_timer);
      disable(pwr_timer);
    }
    
    if (ped) {
      if (!isEnabled(ped_timer)) {
        enable(ped_timer);
      } else if (isDone(ped_timer)) {
        //reset(ped_timer);
      }
    } else if (isEnabled(ped_timer)) {
      reset(ped_timer);
      disable(ped_timer);
    }
    
    GPIODATA_N = (GPIODATA_N & 0x1) | pwr;
    GPIODATA_N = (GPIODATA_N & 0x2) | (ped << 1);
  }
  
  return 0;
}

// Timer is not quite consistent
void tick_traffic(struct Timer clk_timer, bool pwr, bool ped,
                  struct LED grn, struct LED ylw, struct LED red) {
  switch (state) {
    case Stop:  // Transitions
      off(red);
      if (pwr) {
        state = Off;
      } else if (ped) {
        state = Stop;
      } else {
        // Start clk_timer to wait 5 seconds before transitioning
        if (!isEnabled(clk_timer)) {
          enable(clk_timer);
        } else if (isDone(clk_timer)) {
          reset(clk_timer);
          state = Go;
        }
      }
      break;

    case Warn:
      off(ylw);
      if (pwr) {
        state = Off;
      } else {
        // Start clk_timer to wait 5 seconds before transitioning
        if (!isEnabled(clk_timer)) {
          enable(clk_timer);
        } else if (isDone(clk_timer)) {
          reset(clk_timer);
          state = Stop;
        }
      }
      break;
    
    case Go:
      off(grn);
      if (pwr) {
        state = Off;
      } else if (ped) {
        state = Warn;
        // Re-init to reset timer state
        init(clk_timer, 0, 0x2, 32000000);
      } else {
        // Start clk_timer to wait 5 seconds before transitioning
        if (!isEnabled(clk_timer)) {
          enable(clk_timer);
        } else if (isDone(clk_timer)) {
          reset(clk_timer);
          state = Stop;
        }
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