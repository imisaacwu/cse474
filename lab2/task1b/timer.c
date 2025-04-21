#include "../lab2.h"
#include "timer.h"

void init(struct Timer timer, unsigned short mode, unsigned uint32_t interval) {
  const volatile uint32_t *ctl = &(((void *) GPTMCTL_0) + (timer.port_ * 0x1000));
  const volatile uint32_t *cfg = &(((void *) GPTMCFG_0) + (timer.port_ * 0x1000));
  const volatile uint32_t *mod = &(((void *) GPTMTAMR_0) + (timer.port_ * 0x1000));
  const volatile uint32_t *ivl = &(((void *) GPTMTAILR_0) + (timer.port_ * 0x1000));

  *ctl &= ~0x101;
  *cfg = 0x00000000;
  *cfg &= ~0x7;
  *mod &= ~0x3;
  *mod |= (mode & 0x3);
  *mod &= ~0x10;
  *ivl = interval;
}



void enable(struct Timer timer) {
  const volatile uint32_t *ctl = &(((void *) GPTMCTL_0) + (timer.port_ * 0x1000));
  *ctl |= 0x1;
}

void disable(struct Timer timer) {
  const volatile uint32_t *ctl = &(((void *) GPTMCTL_0) + (timer.port_ * 0x1000));
  *ctl &= ~0x1;
}

volatile unsigned short isDone(struct Timer timer) {
  const volatile uint32_t *ris = &(((void *) GPTMRIS_0) + (timer.port_ * 0x1000));
  return (*ris & 0x1);
}

void reset(struct Timer timer) {
  const volatile uint32_t *clr = &(((void *) GPTMICR_0) + (timer.port_ * 0x1000));
  *clr |= 0x1;
}
