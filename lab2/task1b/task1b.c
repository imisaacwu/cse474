/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 29, 2025
 * Task 1b represents a traffic light system where the system will start
 * on the red light when the system is on. Users can press and hold the
 * pedestrian button during a green light to change the light from yellow to red.
 */

#include <stdbool.h>
#include <stdint.h>

#include "../lab2.h"
#include "../timer.h"
#include "LED.h"

// Configures necessary ports
void config_ports();

/**
 * Helper method to handle any inputs relating to the power button
 * (i.e. turning system on/off after being pressed for 2 seconds)
 *
 * @param pwr a pointer to the status of the power button
 * @param grn the green LED output
 * @param ylw the yellow LED output
 * @param red the red LED output
 * @param timer_pwr the 2-second Timer associated with the power button
 * @param timer_5s the 5-second Timer associated with the traffic light cycle
 */
void handle_pwr(unsigned short* pwr, struct LED grn, struct LED ylw, 
                struct LED red, struct Timer timer_pwr, struct Timer timer_5s);

/**
 * Helper method to handle any inputs relating to the pedestrian button
 * (i.e. switching from Go -> Warn if pressed for 2 seconds)
 *
 * @param grn the green LED output
 * @param ylw the yellow LED output
 * @param red the red LED output
 * @param timer_ped the 2-second Timer associated with the pedestrian button
 * @param timer_5s the 5-second Timer associated with the traffic light cycle
 */
void handle_ped(unsigned short* ped, struct LED grn, struct LED ylw, 
                struct LED red, struct Timer timer_ped, struct Timer timer_5s);

/**
 * Helper method to handle time-out events relating to the 5-second timer
 * associated with the traffic light cycle
 *
 * @param grn the green LED output
 * @param ylw the yellow LED output
 * @param red the red LED output
 * @param timer_5s the 5-second Timer associated with the traffic light cycle
 */ 
void handle_5sc(struct LED grn, struct LED ylw, 
                struct LED red, struct Timer timer_5s);

/**
 * Updates the traffic light state based on the given inputs
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
  config_ports();
  
  // Configure timers
  struct Timer timer_pwr = {
    0, TIMER_PERIODIC, 2 * CLK_FRQ,
    &GPTMCTL(0), &GPTMCFG(0), &GPTMTAMR(0), &GPTMTAILR(0), &GPTMRIS(0), &GPTMICR(0)
  };
  
  struct Timer timer_ped = {
    1, TIMER_PERIODIC, 2 * CLK_FRQ,
    &GPTMCTL(1), &GPTMCFG(1), &GPTMTAMR(1), &GPTMTAILR(1), &GPTMRIS(1), &GPTMICR(1)
  };
  
  struct Timer timer_5sc = {
    2, TIMER_PERIODIC, 5 * CLK_FRQ,
    &GPTMCTL(2), &GPTMCFG(2), &GPTMTAMR(2), &GPTMTAILR(2), &GPTMRIS(2), &GPTMICR(2)
  };
  
  init(timer_pwr);
  init(timer_ped);
  init(timer_5sc);
  
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

  while (1) {               
    handle_pwr(&pwr, grn, ylw, red, timer_pwr, timer_5sc);
    handle_ped(&ped, grn, ylw, red, timer_ped, timer_5sc);
    handle_5sc(grn, ylw, red, timer_5sc);
  }
  
  return 0;
}

void config_ports() {
  volatile unsigned short port_delay = 0;
  RCGCGPIO |= 0x14;             // Enable ports C, E
  RCGCTIMER |= 0x7;             // Enable Timers 0-2
  port_delay++;
  port_delay++;
  
  // Configure Port C for LEDs
  GPIOAMSEL_C &= ~0x70;         // Disable analog function of PC4-6
  GPIODIR_C |= 0x70;            // Set PC4-6 to output
  GPIOAFSEL_C &= ~0x70;         // Set PC4-6 regular port function
  GPIODEN_C |= 0x70;            // Enable digital output on PC4-6
  
  // Configure Port E for external buttons
  GPIOAMSEL_E &= ~0x3;          // Disable analog function of PE0-1
  GPIODIR_E &= ~0x3;            // Set PE0-1 to input
  GPIOAFSEL_E &= ~0x3;          // Set PE0-1 regular port function
  GPIODEN_E |= 0x3;             // Enable digital input on PE0-1
}

void handle_pwr(unsigned short* pwr, struct LED grn, struct LED ylw, 
                struct LED red, struct Timer timer_pwr, struct Timer timer_5sc) {
  // Checks if pwr is on or off
  *pwr = (GPIODATA_E & 0x1) == 0x1;
  if (*pwr) {
    enable(timer_pwr); // start timer

    if (isDone(timer_pwr)) { // Power has been held for 2 seconds
      // Reset power timer interrupt status (start counting again)
      reset(timer_pwr);

      if (state == Off) {
        // System was just turned on, start 5-second timer
        enable(timer_5sc);
      }

      tick_traffic(*pwr, 0, grn, ylw, red);

      if (state == Off) {
        // System was just turned off, cancel 5-second timer
        disable(timer_5sc);
        init(timer_5sc);      // Re-configure timer to ensure correct interval
      }
    }
  } else if (isEnabled(timer_pwr)) {
    // Power button released, cancel timer
    disable(timer_pwr);
    init(timer_pwr);          // Re-configure timer to ensure correct interval
  }
}

void handle_ped(unsigned short* ped, struct LED grn, struct LED ylw, 
                struct LED red, struct Timer timer_ped, struct Timer timer_5sc) {
  // Checks if ped is on or off
  *ped = (GPIODATA_E & 0x2) == 0x2;
  if (*ped) {
    enable(timer_ped); // start timer

    if (isDone(timer_ped)) { // Pedestrian has been held for 2 seconds
      // Reset interrupt status (start counting again)
      reset(timer_ped);

      tick_traffic(0, *ped, grn, ylw, red);
      
      if (state == Warn) {
        // Ped moved the state to Warn, reset the timer before going to Stop
        disable(timer_5sc);
        init(timer_5sc);      // Re-configure timer to ensure correct interval
        enable(timer_5sc);
      }
    }
  } else if (isEnabled(timer_ped)) {
    // Pedestrian button released, cancel timer
    disable(timer_ped);
    init(timer_ped);          // Re-configure timer to ensure correct interval
  }
}

void handle_5sc(struct LED grn, struct LED ylw, 
                struct LED red, struct Timer timer_5sc) {
  if (isDone(timer_5sc)) {
    // 5 seconds have elapsed, tick traffic
    tick_traffic(0, 0, grn, ylw, red);
    // Reset 5-second timer
    reset(timer_5sc);
  }             
}

void tick_traffic(unsigned short pwr, unsigned short ped,
                  struct LED grn, struct LED ylw, struct LED red) {
  // Transitions
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
        state = Stop;
      }
      break;
  }
  
  // State actions
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
