/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * May 25, 2025
 * EE/CSE 474: Lab4 Task2a implements a traffic light
 * controller system using virtual LEDs and buttons on a LCD screen
 */

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "../SSD2119_Display.h"
#include "../SSD2119_Touch.h"
#include "../Lab4_Inits.h"
#include "../tm4c1294ncpdt.h"
#include "../LED.h"
#include "../timer.h"
#include "../traffic_fsm.h"

// Global virtual LEDs
struct LED red = {50, 50, 50, &Color4[12], &Color4[4]};
struct LED ylw = {160, 50, 50, &Color4[14], &Color4[16]};
struct LED grn = {270, 50, 50, &Color4[10], &Color4[2]};

// Traffic Light FSM State
State state;

enum frequency freq;

// Global timers
struct Timer timer_pwr = {
  0, TIMER_PERIODIC, 2 * CLK_FRQ,
  &TIMER0_CTL_R, &TIMER0_CFG_R, &TIMER0_TAMR_R,
  &TIMER0_TAILR_R, &TIMER0_RIS_R, &TIMER0_ICR_R
};
struct Timer timer_ped = {
  0, TIMER_PERIODIC, 2 * CLK_FRQ,
  &TIMER1_CTL_R, &TIMER1_CFG_R, &TIMER1_TAMR_R,
  &TIMER1_TAILR_R, &TIMER1_RIS_R, &TIMER1_ICR_R
};
struct Timer timer_5s = {
  0, TIMER_PERIODIC, 5 * CLK_FRQ,
  &TIMER2_CTL_R, &TIMER2_CFG_R, &TIMER2_TAMR_R,
  &TIMER2_TAILR_R, &TIMER2_RIS_R, &TIMER2_ICR_R
};

// Constants for the button locations in terms of
// the coordinates returned by Touch_Read functions
const int center_x_pwr = 1300;
const int center_x_ped = 1780;
const int center_y = 780;

// Calculates the Euclidean distance between two coordinates
float dist(int x1, int x2, int y1, int y2);

int main() {
  freq = PRESET2;               // 60 MHz
  PLL_Init(freq);               // Set system clock frequency to 60 MHz

  // Initialie LCD screen and LCD touch functionality
  LCD_Init();
  Touch_Init();

  // Draw the power button
  LCD_DrawFilledCircle(105, 200, 30, Color4[5]);
  LCD_SetCursor(95, 195);
  LCD_PrintString("pwr");
  // Draw the pedestrian button
  LCD_DrawFilledCircle(215, 200, 30, Color4[5]);
  LCD_SetCursor(205, 195);
  LCD_PrintString("ped");
  
  // Configure timers & their interrupts
  SYSCTL_RCGCTIMER_R |= 0x7;             // Enable Timers 0-2
  init(timer_pwr);
  TIMER0_IMR_R |= 0x1;    // Enable interrupt on time-out
  NVIC_EN0_R |= 0x80000;  // Enable interrupt number 19
  
  init(timer_ped);
  TIMER1_IMR_R |= 0x1;    // Enable interrupt on time-out
  NVIC_EN0_R |= 0x200000; // Enable interrupt number 21

  init(timer_5s);
  TIMER2_IMR_R |= 0x1;    // Enable interrupt on time-out
  NVIC_EN0_R |= 0x800000; // Enable interrupt number 23

  // Initialize LEDs
  off(red);
  off(ylw);
  off(grn);

  state = Off;
  unsigned long x, y;

  while (1) {
    // Poll touch location
    x = Touch_ReadX();
    y = Touch_ReadY();
    // Do button logic
    if (dist(center_x_pwr, x, center_y, y) <= 130) {
      // start 2-sec power timer
      enable(timer_pwr);
    } else if (isEnabled(timer_pwr)) {
      disable(timer_pwr);
      init(timer_pwr);
    }
    
    if (dist(center_x_ped, x, center_y, y) <= 130) {
      // start 2-sec power timer
      enable(timer_ped);
    } else if (isEnabled(timer_ped)) {
      disable(timer_ped);
      init(timer_ped);
    }
  }
  
  return 0;
}

float dist(int x1, int x2, int y1, int y2) {
  float x_dist = (x2 - x1) * (x2 - x1);
  float y_dist = (y2 - y1) * (y2 - y1);
  return sqrt(x_dist + y_dist);
}

/**
 * Handles the events triggered when the power button's timer
 * times out. If the system was off, it will now be turned on to
 * the red state for 5 seconds. Otherwise, it will be turned off.
 */
#pragma call_graph_root = "interrupt"
__weak void Timer0A_Handler ( void ) {
  // Power was just held for 2 seconds
  if (state == Off) {
    // System was just turned on, start 5-second timer
    enable(timer_5s);
  }

  tick_traffic(&state, 1, 0, red, ylw, grn);

  if (state == Off) {
    // System was just turned off, cancel 5-second timer
    disable(timer_5s);
    // Re-configure timer to ensure correct interval
    init(timer_5s);             
  }
}

/**
 * Handles the events triggered when the pedestrian button's timer
 * times out. In the green state, this will transition the system
 * to the warn state. In the warn state, it will reset the 5-second
 * timer such that the system will be held in the warn state for 5
 * seconds.
 */
#pragma call_graph_root = "interrupt"
__weak void Timer1A_Handler ( void ) {
  // Pedestrian button was just held for 2 seconds
  if (state == Go) {
    tick_traffic(&state, 0, 1, red, ylw, grn);
  }
  
  if (state == Warn) {
    // Ped moved the state to Warn, reset the timer before going to Stop
    disable(timer_5s);
    // Re-configure timer to ensure correct interval
    init(timer_5s);      
    enable(timer_5s);
  }
}

/**
 * Handles the events triggered upon timeout of the 5-second timer.
 * Resets the timer and tick traffic to the next state.
 */
#pragma call_graph_root = "interrupt"
__weak void Timer2A_Handler ( void ) {
  // Reset 5-second timer
  reset(timer_5s);

  // Reset pedestrian timer so edge cases between Stop and Go are consistent
  if (isEnabled(timer_ped)) {
    disable(timer_ped);
    init(timer_ped);
    enable(timer_ped);
  }
  // 5 seconds have elapsed, tick traffic
  tick_traffic(&state, 0, 0, red, ylw, grn);
}
