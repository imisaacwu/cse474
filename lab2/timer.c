#include <stdbool.h>
#include <stdint.h>

#include "lab2.h"
#include "timer.h"

void init(struct Timer timer, unsigned short mode, uint32_t interval) {
  RCGCGPIO |= (0x1 << timer.port_);

  *timer.ctl_ &= ~0x101;
  *timer.cfg_ = 0x00000000;
  *timer.cfg_ &= ~0x7;
  *timer.mod_ &= ~0x3;
  *timer.mod_ |= (mode & 0x3);
  *timer.mod_ &= ~0x10;
  *timer.ivl_ = interval;
}



void enable(struct Timer timer) {
  *timer.ctl_ |= 0x1;
}

void disable(struct Timer timer) {
  *timer.ctl_ &= ~0x1;
}

bool isDone(struct Timer timer) {
  return (*timer.ris_ & 0x1) == 0x1;
}

void reset(struct Timer timer) {
  *timer.clr_ |= 0x1;
}
