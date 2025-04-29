/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 28, 2025
 * Task 2c represents a traffic light system where the system will start
 * on the red light when the system is on. Users can press and hold the
 * pedestrian button during a green light to change the light from yellow to red.
 */

#include <stdbool.h>
#include <stdint.h>

#include "../lab2.h"
#include "../LED.h"
#include "../timer.h"

// Configures necessary ports
void config_ports();

/** Updates the traffic light state based on the given inputs
 *
 * @param pwr the state of the power button input
 * @param ped the state of the pedestrian button input
 */
void tick_traffic(unsigned short pwr, unsigned short ped);

// The set of possible states for the traffic light system
enum States { Off, Stop, Warn, Go } state;

// Connect configured Port C pins to LEDs
struct LED grn = {&GPIODATA_C, 4};
struct LED ylw = {&GPIODATA_C, 5};
struct LED red = {&GPIODATA_C, 6};

// Global timers
struct Timer timer_pwr = {
  0, TIMER_PERIODIC, 2 * CLK_FRQ,
  &GPTMCTL(0), &GPTMCFG(0), &GPTMTAMR(0), &GPTMTAILR(0), &GPTMRIS(0), &GPTMICR(0)
};
struct Timer timer_ped = {
  1, TIMER_PERIODIC, 2 * CLK_FRQ,
  &GPTMCTL(1), &GPTMCFG(1), &GPTMTAMR(1), &GPTMTAILR(1), &GPTMRIS(1), &GPTMICR(1)
};
struct Timer timer_5s = {
  2, TIMER_PERIODIC, 5 * CLK_FRQ,
  &GPTMCTL(2), &GPTMCFG(2), &GPTMTAMR(2), &GPTMTAILR(2), &GPTMRIS(2), &GPTMICR(2)
};

int main(void) {
  config_ports();
  
  // Configure timers
  init(timer_pwr);
  GPTMIMR(0) |= 0x1;    // Enable interrupt on time-out
  NVIC_EN0 |= 0x80000;  // Enable interrupt number 19
  
  init(timer_ped);
  GPTMIMR(1) |= 0x1;    // Enable interrupt on time-out
  NVIC_EN0 |= 0x200000; // Enable interrupt number 21

  init(timer_5s);
  GPTMIMR(2) |= 0x1;    // Enable interrupt on time-out
  NVIC_EN0 |= 0x800000; // Enable interrupt number 23
  
  off(grn);
  off(ylw);
  off(red);
 
  state = Off;

  while (1) {}
  
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
  
  // Configure interrupts for Port E
  GPIOIM_E &= ~0x3;             // Disable GPIO interrupts for PE0 and PE1
  GPIOIS_E &= ~0x3;             // Makes PE0 and PE1 edge-sensitive
  GPIOIBE_E |= 0x3;             // Trigger on both edges
  GPIOICR_E |= 0x3;             // Clear prior interrupt statuses
  GPIOIM_E |= 0x3;              // Re-enable GPIO interrupts for PE0 and PE1
  
  NVIC_EN0 |= 0x10;             // Enable interrupt number 4
  NVIC_PRI1 |= 0x70;            // Set priority of interrupt 4 to 7
}

void tick_traffic(unsigned short pwr, unsigned short ped) {
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

#pragma call_graph_root = "interrupt"
__weak void PortE_Handler ( void ) {
  // Reset interrupt status
  GPIOICR_E |= 0x3;
  // Check button statuses
  unsigned short pwr = (GPIODATA_E & 0x1) == 0x1;
  unsigned short ped = (GPIODATA_E & 0x2) == 0x2;
  
  if (pwr) {
    enable(timer_pwr); // start timer
  } else if (isEnabled(timer_pwr)) {
    // Power button released, cancel timer
    disable(timer_pwr);
    init(timer_pwr);          // Re-configure timer to ensure correct interval
  }
  
  if (ped) {
    enable(timer_ped); // start timer
  } else if (isEnabled(timer_ped)) {
    // Pedestrian button released, cancel timer
    disable(timer_ped);
    init(timer_ped);          // Re-configure timer to ensure correct interval
  }
}

#pragma call_graph_root = "interrupt"
__weak void Timer0A_Handler ( void ) {
  // Power was just held for 2 seconds
  // Reset power timer interrupt status (start counting again)
  reset(timer_pwr);
  
  if (state == Off) {
    // System was just turned on, start 5-second timer
    enable(timer_5s);
  }

  tick_traffic(1, 0);

  if (state == Off) {
    // System was just turned off, cancel 5-second timer
    disable(timer_5s);
    init(timer_5s);      // Re-configure timer to ensure correct interval
  }
}

#pragma call_graph_root = "interrupt"
__weak void Timer1A_Handler ( void ) {
  // Pedestrian button was just held for 2 seconds
  // Reset interrupt status (start counting again)
  reset(timer_ped);

  tick_traffic(0, 1);
  
  if (state == Warn) {
    // Ped moved the state to Warn, reset the timer before going to Stop
    disable(timer_5s);
    init(timer_5s);      // Re-configure timer to ensure correct interval
    enable(timer_5s);
  }
}

#pragma call_graph_root = "interrupt"
__weak void Timer2A_Handler ( void ) {
  // Reset 5-second timer
  reset(timer_5s);
  // 5 seconds have elapsed, tick traffic
  tick_traffic(0, 0);
}