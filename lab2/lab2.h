/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 14, 2025
 * Header file for Lab 2
 */

#ifndef LAB2_H_
#define LAB2_H_

// Register Definitions

// Main register to enable ports
#define RCGCGPIO    (*((volatile uint32_t *) 0x400FE608))
#define RCGCTIMER   (*((volatile uint32_t *) 0x400FE604))

// Registers for Timer 0
#define GPTMCTL_0   (*((volatile uint32_t *) 0x4003000C))
#define GPTMCFG_0   (*((volatile uint32_t *) 0x40030000))
#define GPTMTAMR_0  (*((volatile uint32_t *) 0x40030004))
#define GPTMTAILR_0 (*((volatile uint32_t *) 0x40030028))
#define GPTMRIS_0   (*((volatile uint32_t *) 0x4003001C))
#define GPTMICR_0   (*((volatile uint32_t *) 0x40030024))

// Registers for Timer 1
#define GPTMCTL_1   (*((volatile uint32_t *) 0x4003100C))
#define GPTMCFG_1   (*((volatile uint32_t *) 0x40031000))
#define GPTMTAMR_1  (*((volatile uint32_t *) 0x40031004))
#define GPTMTAILR_1 (*((volatile uint32_t *) 0x40031028))
#define GPTMRIS_1   (*((volatile uint32_t *) 0x4003101C))
#define GPTMICR_1   (*((volatile uint32_t *) 0x40031024))

// Registers for Timer 2
#define GPTMCTL_2   (*((volatile uint32_t *) 0x4003200C))
#define GPTMCFG_2   (*((volatile uint32_t *) 0x40032000))
#define GPTMTAMR_2  (*((volatile uint32_t *) 0x40032004))
#define GPTMTAILR_2 (*((volatile uint32_t *) 0x40032028))
#define GPTMRIS_2   (*((volatile uint32_t *) 0x4003201C))
#define GPTMICR_2   (*((volatile uint32_t *) 0x40032024))

// Registers for port C: Used in Task 1b for external LEDs
#define GPIOAFSEL_C (*((volatile uint32_t *) 0x4005A420))
#define GPIOAMSEL_C (*((volatile uint32_t *) 0x4005A528))
#define GPIODATA_C  (*((volatile uint32_t *) 0x4005A3FC))
#define GPIODEN_C   (*((volatile uint32_t *) 0x4005A51C))
#define GPIODIR_C   (*((volatile uint32_t *) 0x4005A400))

// Registers for port E: Used in Task 1b for external buttons
#define GPIOAFSEL_E (*((volatile uint32_t *) 0x4005C420))
#define GPIOAMSEL_E (*((volatile uint32_t *) 0x4005C528))
#define GPIODATA_E  (*((volatile uint32_t *) 0x4005C3FC))
#define GPIODEN_E   (*((volatile uint32_t *) 0x4005C51C))
#define GPIODIR_E   (*((volatile uint32_t *) 0x4005C400))

// Registers for port F: Used in Task 1a for on-board LEDs D3 and D4
#define GPIODATA_F  (*((volatile uint32_t *) 0x4005D3FC))
#define GPIODEN_F   (*((volatile uint32_t *) 0x4005D51C))
#define GPIODIR_F   (*((volatile uint32_t *) 0x4005D400))

// Registers for port N: Used in Task 1a for on-board LEDs D1 and D2
#define GPIODATA_N  (*((volatile uint32_t *) 0x400643FC))
#define GPIODEN_N   (*((volatile uint32_t *) 0x4006451C))
#define GPIODIR_N   (*((volatile uint32_t *) 0x40064400))


// Additional Macros

// Macros to unlock and lock GPIO_LOCK registers
#define UNLOCK      0x4C4F434B
#define LOCK        0x0

#endif  // LAB2_H_
