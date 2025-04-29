/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 14, 2025
 * Header file for LEDs
 */

#ifndef LED_H_
#define LED_H_

#include <stdint.h>

// Represents an LED connected to a specific GPIO pin.
struct LED {
  volatile uint32_t *data_;  // Pointer to a GPIO data register
  unsigned short index_;     // Specific pin (the bit position in the given port)
};

// Turns the specified LED on
void on(struct LED led);

// Turns the specified LED off
void off(struct LED led);

#endif  // LED_H_
