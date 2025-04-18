/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 14, 2025
 * Implementations for LED functions
 */

#include "LED.h"

void on(struct LED led) {
  *led.data_ |= (0x1 << led.index_);
}

void off(struct LED led) {
  *led.data_ &= ~(0x1 << led.index_);
}
