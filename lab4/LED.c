/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * May 25, 2025
 * Implementations for LED functions
 */

#include "LED.h"

void on(struct LED led) {
  LCD_DrawFilledCircle(led.x, led.y, led.r, *led.colorOn);
}

void off(struct LED led) {
  LCD_DrawFilledCircle(led.x, led.y, led.r, *led.colorOff);
}
