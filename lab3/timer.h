/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 29, 2025
 * Header file for Timers
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdbool.h>
#include <stdint.h>

#include "lab3.h"

// Represents a Timer, specified by its number
struct Timer {
  unsigned short num_;      // Timer number
  unsigned short mode_;     // Timer mode
  uint32_t interval_;       // Timer interval
  
  volatile uint32_t *ctl;   // Pointer to Timer's GPTMCTL register
  volatile uint32_t *cfg;   // Pointer to Timer's GPTMCFG register
  volatile uint32_t *tamr;  // Pointer to Timer's GPTMTAMR register
  volatile uint32_t *tailr; // Pointer to Timer's GPTMTAILR register
  volatile uint32_t *ris;   // Pointer to Timer's GPTMRIS register
  volatile uint32_t *icr;   // Pointer to Timer's GPTMICR register
};

/**
 * Initializes the passed-in Timer
 * Sets the Timer to be 32-bit and to count down.
 * Will be configured with the Timer's mode.
 * Will initialize the TAILR with the Timer's interval.
 *
 * @param timer the Timer to initialize
 */
void init(struct Timer timer);

/**
 * Enables the passed-in Timer
 *
 * @param timer the Timer to enable
 */
void enable(struct Timer timer);

/**
 * Disables the passed-in Timer
 *
 * @param timer the Timer to disable
 */
void disable(struct Timer timer);

/**
 * Checks whether the passed-in Timer is enabled
 *
 * @param timer the Timer to check
 * @returns true if the timer is enabled, false otherwise
 */
bool isEnabled(struct Timer timer);

/**
 * Checks whether the passed-in Timer has timed out
 *
 * @param timer the Timer to check
 * @returns true if the timer has timed out, false otherwise
 */
bool isDone(struct Timer timer);

/**
 * Resets the interrupt status of the passed-in Timer
 *
 * @param timer the Timer to reset
 */
void reset(struct Timer timer);

#endif  // TIMER_H_
