/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 29, 2025
 * Header file for Lab 2
 */

#ifndef LAB2_H_
#define LAB2_H_

// Register Definitions

// Main register to enable ports
#define RCGCGPIO    (*((volatile uint32_t *) 0x400FE608))
#define RCGCTIMER   (*((volatile uint32_t *) 0x400FE604))

// Registers for port C: Used in Task 1b, 2c for external LEDs
#define GPIOAFSEL_C (*((volatile uint32_t *) 0x4005A420))
#define GPIOAMSEL_C (*((volatile uint32_t *) 0x4005A528))
#define GPIODATA_C  (*((volatile uint32_t *) 0x4005A3FC))
#define GPIODEN_C   (*((volatile uint32_t *) 0x4005A51C))
#define GPIODIR_C   (*((volatile uint32_t *) 0x4005A400))

// Registers for port E: Used in Task 1b, 2c for external buttons
#define GPIOAFSEL_E (*((volatile uint32_t *) 0x4005C420))
#define GPIOAMSEL_E (*((volatile uint32_t *) 0x4005C528))
#define GPIODATA_E  (*((volatile uint32_t *) 0x4005C3FC))
#define GPIODEN_E   (*((volatile uint32_t *) 0x4005C51C))
#define GPIODIR_E   (*((volatile uint32_t *) 0x4005C400))
#define GPIOIBE_E   (*((volatile uint32_t *) 0x4005C408))
#define GPIOICR_E   (*((volatile uint32_t *) 0x4005C41C))
#define GPIOIEV_E   (*((volatile uint32_t *) 0x4005C40C))
#define GPIOIM_E    (*((volatile uint32_t *) 0x4005C410))
#define GPIOIS_E    (*((volatile uint32_t *) 0x4005C404))

// Registers for port F: Used in Task 1a, 2a for on-board LEDs D3 and D4
#define GPIODATA_F  (*((volatile uint32_t *) 0x4005D3FC))
#define GPIODEN_F   (*((volatile uint32_t *) 0x4005D51C))
#define GPIODIR_F   (*((volatile uint32_t *) 0x4005D400))

// Registers for port J: Used in Task 2b for on-board buttons
#define GPIOCR_J    (*((volatile uint32_t *) 0x40060524))
#define GPIODATA_J  (*((volatile uint32_t *) 0x400603FC))
#define GPIODEN_J   (*((volatile uint32_t *) 0x4006051C))
#define GPIODIR_J   (*((volatile uint32_t *) 0x40060400))
#define GPIOIBE_J   (*((volatile uint32_t *) 0x40060408))
#define GPIOICR_J   (*((volatile uint32_t *) 0x4006041C))
#define GPIOIEV_J   (*((volatile uint32_t *) 0x4006040C))
#define GPIOIM_J    (*((volatile uint32_t *) 0x40060410))
#define GPIOIS_J    (*((volatile uint32_t *) 0x40060404))
#define GPIOLOCK_J  (*((volatile uint32_t *) 0x40060520))
#define GPIOPUR_J   (*((volatile uint32_t *) 0x40060510))
#define GPIORIS_J   (*((volatile uint32_t *) 0x40060414))

// Registers for port N: Used in Task 1a, 2a, 2b for on-board LEDs D1 and D2
#define GPIODATA_N  (*((volatile uint32_t *) 0x400643FC))
#define GPIODEN_N   (*((volatile uint32_t *) 0x4006451C))
#define GPIODIR_N   (*((volatile uint32_t *) 0x40064400))

// Registers for NVIC
#define NVIC_EN0    (*((volatile uint32_t *) 0xE000E100))
#define NVIC_EN1    (*((volatile uint32_t *) 0xE000E104))

#define NVIC_PRI1   (*((volatile uint32_t *) 0xE000E404))
#define NVIC_PRI12  (*((volatile uint32_t *) 0xE000E430))

// Additional Macros

// Macros to unlock and lock GPIO_LOCK registers
#define UNLOCK      0x4C4F434B
#define LOCK        0x0

// Timer Macros
#define GPTMCTL(X)      (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x00C)))
#define GPTMCFG(X)      (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x000)))
#define GPTMTAMR(X)     (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x004)))
#define GPTMTAILR(X)    (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x028)))
#define GPTMRIS(X)      (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x01C)))
#define GPTMICR(X)      (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x024)))
#define GPTMIMR(X)      (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x018)))

// Timer Bases
#define TIMER_0_BASE    0x40030000
#define TIMER_1_BASE    0x40031000
#define TIMER_2_BASE    0x40032000
#define TIMER_3_BASE    0x40033000
#define TIMER_4_BASE    0x40034000
#define TIMER_5_BASE    0x40035000
#define TIMER_6_BASE    0x400E0000
#define TIMER_7_BASE    0x400E1000
   
// More Timer Macros
#define CLK_FRQ         16000000
#define TIMER_ONE_SHOT  0x1
#define TIMER_PERIODIC  0x2
#define TIMER_CAPTURE   0x3

#endif  // LAB2_H_
