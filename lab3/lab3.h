/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 29, 2025
 * Header file for Lab 3
 */

#ifndef LAB3_H_
#define LAB3_H_

// Register Definitions

// Main Clock Gating Control Registers
#define RCGCGPIO    (*((volatile uint32_t *) 0x400FE608))
#define RCGCTIMER   (*((volatile uint32_t *) 0x400FE604))

// GPIO Register Macros
#define GPIODATA(X)     (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x3FC)))
#define GPIODIR(X)      (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x400)))
#define GPIORIS(X)      (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x414)))
#define GPIODEN(X)      (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x51C)))

// GPIO Port Bases
#define GPIO_A_BASE     0x40058000
#define GPIO_B_BASE     0x40059000
#define GPIO_C_BASE     0x4005A000
#define GPIO_D_BASE     0x4005B000
#define GPIO_E_BASE     0x4005C000
#define GPIO_F_BASE     0x4005D000
#define GPIO_G_BASE     0x4005E000
#define GPIO_H_BASE     0x4005F000
#define GPIO_J_BASE     0x40060000
#define GPIO_K_BASE     0x40061000
#define GPIO_L_BASE     0x40062000
#define GPIO_M_BASE     0x40063000
#define GPIO_N_BASE     0x40064000
#define GPIO_P_BASE     0x40065000
#define GPIO_Q_BASE     0x40066000

#endif  // LAB3_H_