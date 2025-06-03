/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * June 5, 2025
 * Implementations for Timer functions
 */

#include <stdbool.h>
#include <stdint.h>

#include "lab5.h"
#include "timer.h"

void init(struct Timer timer) {  
  *timer.ctl &= ~0x101;                 // Disable Timer
  *timer.cfg = 0x00000000;              // Reset Timer configs
  *timer.cfg &= ~0x7;                   // Set Timer to be 32-bit
  *timer.tamr &= ~0x3;
  *timer.tamr |= (timer.mode_ & 0x3);   // Set Timer to specified mode
  *timer.tamr &= ~0x10;                 // Set Timer to count down
  *timer.tailr = timer.interval_;       // Load interval 
}

void enable(struct Timer timer) {
  *timer.ctl |= 0x1;
}

void disable(struct Timer timer) {
  *timer.ctl &= ~0x1;
}

bool isEnabled(struct Timer timer) {
  return (*timer.ctl & 0x1) == 0x1;
}

bool isDone(struct Timer timer) {
  return (*timer.ris & 0x1) == 0x1;
}

void reset(struct Timer timer) {
  *timer.icr = 0x1;
}