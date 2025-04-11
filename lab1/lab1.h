#ifndef LAB1_H_
#define LAB1_H_

#define UNLOCK      0x4C4F434B
#define LOCK        0x0

#define RCGCGPIO    (*((volatile uint32_t *) 0x400FE608))

#define GPIOAFSEL_C (*((volatile uint32_t *) 0x4005A420))
#define GPIOAMSEL_C (*((volatile uint32_t *) 0x4005A528))
#define GPIODATA_C  (*((volatile uint32_t *) 0x4005A3FC))
#define GPIODEN_C   (*((volatile uint32_t *) 0x4005A51C))
#define GPIODIR_C   (*((volatile uint32_t *) 0x4005A400))

#define GPIODATA_F  (*((volatile uint32_t *) 0x4005D3FC))
#define GPIODEN_F   (*((volatile uint32_t *) 0x4005D51C))
#define GPIODIR_F   (*((volatile uint32_t *) 0x4005D400))

#define GPIOCR_J    (*((volatile uint32_t *) 0x40060524))
#define GPIODATA_J  (*((volatile uint32_t *) 0x400603FC))
#define GPIODEN_J   (*((volatile uint32_t *) 0x4006051C))
#define GPIODIR_J   (*((volatile uint32_t *) 0x40060400))
#define GPIOLOCK_J  (*((volatile uint32_t *) 0x40060520))
#define GPIOPUR_J   (*((volatile uint32_t *) 0x40060510))

#define GPIODATA_N  (*((volatile uint32_t *) 0x400643FC))
#define GPIODEN_N   (*((volatile uint32_t *) 0x4006451C))
#define GPIODIR_N   (*((volatile uint32_t *) 0x40064400))

#endif  // LAB1_H_