#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#include "../lab2.h"

struct Timer {
  unsigned short port_;
  unsigned short index_;
}

void init(struct Timer timer, bool ab, unsigned short mode,
          bool count_down, unsigned uint32_t interval);
void enable(struct Timer timer);
void disable(struct Timer timer);
int isDone(struct Timer timer);
void reset(struct Timer timer);

#endif  // TIMER_H_
