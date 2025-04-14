#include "LED.h"

void on(struct LED led) {
  *led.data_ |= (0x1 << led.index_);
}

void off(struct LED led) {
  *led.data_ &= ~(0x1 << led.index_);
}