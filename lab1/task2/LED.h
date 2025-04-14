#ifndef LED_H_
#define LED_H_

#include <stdint.h>

struct LED {
  volatile uint32_t *data_;
  unsigned short index_;
};

void on(struct LED led);
void off(struct LED led);

#endif  // LED_H_