/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 14, 2025
 * Header file for Lab 1
 */

#ifndef LAB1_H_
#define LAB1_H_

// Register Definitions

// Main register to enable ports
#define RCGCGPIO    (*((volatile uint32_t *) 0x400FE608))

// Registers for port C: Used in Task 2 for external LEDs
#define GPIOAFSEL_C (*((volatile uint32_t *) 0x4005A420))
#define GPIOAMSEL_C (*((volatile uint32_t *) 0x4005A528))
#define GPIODATA_C  (*((volatile uint32_t *) 0x4005A3FC))
#define GPIODEN_C   (*((volatile uint32_t *) 0x4005A51C))
#define GPIODIR_C   (*((volatile uint32_t *) 0x4005A400))

// Registers for port E: Used in Task 2 for external buttons
#define GPIOAFSEL_E (*((volatile uint32_t *) 0x4005C420))
#define GPIOAMSEL_E (*((volatile uint32_t *) 0x4005C528))
#define GPIODATA_E  (*((volatile uint32_t *) 0x4005C3FC))
#define GPIODEN_E   (*((volatile uint32_t *) 0x4005C51C))
#define GPIODIR_E   (*((volatile uint32_t *) 0x4005C400))

// Registers for port F: Used in Task 1a for on-board LEDs D3 and D4
#define GPIODATA_F  (*((volatile uint32_t *) 0x4005D3FC))
#define GPIODEN_F   (*((volatile uint32_t *) 0x4005D51C))
#define GPIODIR_F   (*((volatile uint32_t *) 0x4005D400))

// Registers for port J: Used in Task 1b for on-board buttons
#define GPIOCR_J    (*((volatile uint32_t *) 0x40060524))
#define GPIODATA_J  (*((volatile uint32_t *) 0x400603FC))
#define GPIODEN_J   (*((volatile uint32_t *) 0x4006051C))
#define GPIODIR_J   (*((volatile uint32_t *) 0x40060400))
#define GPIOLOCK_J  (*((volatile uint32_t *) 0x40060520))
#define GPIOPUR_J   (*((volatile uint32_t *) 0x40060510))

// Registers for port N: Used in Task 1a for on-board LEDs D1 and D2
#define GPIODATA_N  (*((volatile uint32_t *) 0x400643FC))
#define GPIODEN_N   (*((volatile uint32_t *) 0x4006451C))
#define GPIODIR_N   (*((volatile uint32_t *) 0x40064400))


// Additional Macros

// Macros to unlock and lock GPIO_LOCK registers
#define UNLOCK      0x4C4F434B
#define LOCK        0x0

#endif  // LAB1_H_
