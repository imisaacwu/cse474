 /**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * May 15, 2025
 * Header file containing register definitions for Lab 3
 */

#ifndef LAB5_H_
#define LAB5_H_

// Register Definitions

// Main Clock Gating Control Registers
#define RCGCTIMER   (*((volatile uint32_t *) 0x400FE604))
#define RCGCGPIO    (*((volatile uint32_t *) 0x400FE608))
#define RCGCUART    (*((volatile uint32_t *) 0x400FE618))
#define RCGCADC     (*((volatile uint32_t *) 0x400FE638))
#define RCGCPWM     (*((volatile uint32_t *) 0x400FE640))

// GPIO Register Macros
#define GPIODATA(X)     (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x3FC)))
#define GPIODIR(X)      (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x400)))
#define GPIOIS(X)       (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x404)))
#define GPIOIBE(X)      (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x408)))
#define GPIOIEV(X)      (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x40C)))
#define GPIOIM(X)       (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x410)))
#define GPIORIS(X)      (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x414)))
#define GPIOICR(X)      (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x41C)))
#define GPIOAFSEL(X)    (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x420)))
#define GPIOPUR(X)      (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x510)))
#define GPIODEN(X)      (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x51C)))
#define GPIOLOCK(X)     (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x520)))
#define GPIOCR(X)       (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x524)))
#define GPIOAMSEL(X)    (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x528)))
#define GPIOPCTL(X)     (*((volatile uint32_t *) (GPIO_ ## X ## _BASE + 0x52C)))

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

// Timer Macros
#define GPTMCFG(X)      (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x000)))
#define GPTMTAMR(X)     (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x004)))
#define GPTMCTL(X)      (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x00C)))
#define GPTMIMR(X)      (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x018)))
#define GPTMRIS(X)      (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x01C)))
#define GPTMICR(X)      (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x024)))
#define GPTMTAILR(X)    (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x028)))
#define GPTMADCEV(X)    (*((volatile uint32_t *) (TIMER_ ## X ## _BASE + 0x070)))

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

// Registers for ADC
#define ADCACTSS_0      (*((volatile uint32_t *) 0x40038000))
#define ADCIM_0         (*((volatile uint32_t *) 0x40038008))
#define ADCISC_0        (*((volatile uint32_t *) 0x4003800C))
#define ADCEMUX_0       (*((volatile uint32_t *) 0x40038014))
#define ADCSSMUX3_0     (*((volatile uint32_t *) 0x400380A0))
#define ADCSSCTL3_0     (*((volatile uint32_t *) 0x400380A4))
#define ADCSSFIFO3_0    (*((volatile uint32_t *) 0x400380A8))
#define ADCSSEMUX3_0    (*((volatile uint32_t *) 0x400380B8))
#define ADCSSTSH3_0     (*((volatile uint32_t *) 0x400380BC))
#define ADCCC_0         (*((volatile uint32_t *) 0x40038FC8))

// Registers for UART
#define UARTDR(X)       (*((volatile uint32_t *) (UART_ ## X ## _BASE + 0x000)))
#define UARTFR(X)       (*((volatile uint32_t *) (UART_ ## X ## _BASE + 0x018)))
#define UARTIBRD(X)     (*((volatile uint32_t *) (UART_ ## X ## _BASE + 0x024)))
#define UARTFBRD(X)     (*((volatile uint32_t *) (UART_ ## X ## _BASE + 0x028)))
#define UARTLCRH(X)     (*((volatile uint32_t *) (UART_ ## X ## _BASE + 0x02C)))
#define UARTCTL(X)      (*((volatile uint32_t *) (UART_ ## X ## _BASE + 0x030)))
#define UARTCC(X)       (*((volatile uint32_t *) (UART_ ## X ## _BASE + 0xFC8)))

// UART Bases
#define UART_0_BASE    0x4000C000
#define UART_3_BASE    0x4000F000

// Registers for PWM
#define PWMENABLE        (*((volatile uint32_t *) 0x40028008))
#define PWM0CTL         (*((volatile uint32_t *) 0x40028040))
#define PWM0LOAD        (*((volatile uint32_t *) 0x40028050))
#define PWM0GENB        (*((volatile uint32_t *) 0x40028064))
#define PWM0CMPB        (*((volatile uint32_t *) 0x4002805C))

#define PWMCC           (*((volatile uint32_t *) 0x40028FC8))

// PWM Bases
#define PWM_0_ BASE    0x40028064

// Registers for the NVIC
#define NVIC_EN0    (*((volatile uint32_t *) 0xE000E100))
#define NVIC_EN1    (*((volatile uint32_t *) 0xE000E104))
#define NVIC_PRI12  (*((volatile uint32_t *) 0xE000E430))

// Additional Macros

// Macros to unlock and lock GPIO_LOCK registers
#define UNLOCK      0x4C4F434B
#define LOCK        0x0
#define BIT(N)  (0x1 << N)

#endif  // LAB5_H_