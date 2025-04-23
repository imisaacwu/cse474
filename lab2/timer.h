#ifndef TIMER_H_
#define TIMER_H_

#include <stdbool.h>
#include <stdint.h>

#include "lab2.h"

struct Timer {
  unsigned short num_;
  unsigned short mode_;
  uint32_t interval_;
  
  volatile uint32_t *ctl;
  volatile uint32_t *cfg;
  volatile uint32_t *tamr;
  volatile uint32_t *tailr;
  volatile uint32_t *ris;
  volatile uint32_t *icr;
};

void init(struct Timer timer);
void enable(struct Timer timer);
void disable(struct Timer timer);
bool isEnabled(struct Timer timer);
bool isDone(struct Timer timer);
void reset(struct Timer timer);

#endif  // TIMER_H_
