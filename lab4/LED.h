/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * May 25, 2025
 * Header file for LEDs
 */

#ifndef LED_H_
#define LED_H_

#include <stdint.h>

// Represents a virtual LED that can be drawn to a LCD screen
struct LED {
  unsigned short x;
  unsigned short y;
  unsigned short r;
  const unsigned short *colorOn;
  const unsigned short *colorOff;
};

// Turns the specified LED on
void on(struct LED led);

// Turns the specified LED off
void off(struct LED led);

#endif  // LED_H_
