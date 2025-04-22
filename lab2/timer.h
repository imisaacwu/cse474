#ifndef TIMER_H_
#define TIMER_H_

#include <stdbool.h>
#include <stdint.h>

#include "lab2.h"

struct Timer {
  volatile uint32_t *ctl_;
  volatile uint32_t *cfg_;
  volatile uint32_t *mod_;
  volatile uint32_t *ivl_;
  volatile uint32_t *ris_;
  volatile uint32_t *clr_;
};

void init(struct Timer timer, unsigned short port, unsigned short mode, uint32_t interval);
void enable(struct Timer timer);
void disable(struct Timer timer);
bool isEnabled(struct Timer timer);
bool isDone(struct Timer timer);
void reset(struct Timer timer);

#endif  // TIMER_H_
