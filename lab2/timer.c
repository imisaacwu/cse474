#include <stdbool.h>
#include <stdint.h>

#include "lab2.h"
#include "timer.h"

void init(struct Timer timer, unsigned short port, unsigned short mode, uint32_t interval) {
  RCGCTIMER |= (0x1 << port);

  *timer.ctl_ &= ~0x101;        // Disable Timer A/B
  *timer.cfg_ = 0x00000000;     // Reset Timer configs
  *timer.cfg_ &= ~0x7;          // Set Timer to be 32-bit
  *timer.mod_ &= ~0x3;                  
  *timer.mod_ |= (mode & 0x3);  // Set Timer A/B to specified mode
  *timer.mod_ &= ~0x10;         // Set Timer A/B to counts down
  *timer.ivl_ = interval;       // Load interval to achieve specified length
}

void enable(struct Timer timer) {
  *timer.ctl_ |= 0x1;
}

void disable(struct Timer timer) {
  *timer.ctl_ &= ~0x1;
}

bool isEnabled(struct Timer timer) {
  return (*timer.ctl_ & 0x1) == 0x1;
}

bool isDone(struct Timer timer) {
  return (*timer.ris_ & 0x1) == 0x1;
}

void reset(struct Timer timer) {
  *timer.clr_ |= 0x1;
}