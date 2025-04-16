#ifndef LED_H_
#define LED_H_

#include <stdint.h>

// Represents an LED connected to a specific GPIO pin.
struct LED {
  volatile uint32_t *data_;  // Pointer to a GPIO data register
  unsigned short index_;     // Specific pin (the bit pos in the given port)
};

// Turns the LED on
void on(struct LED led);
// Turns the LED off
void off(struct LED led);

#endif  // LED_H_