#ifndef LAB1_H_
#define LAB1_H_

#define UNLOCK      0x4C4F434B
#define LOCK        0x0

#define RCGCGPIO    (*((volatile uint32_t *) 0x400FE608))

#define GPIODIR_F   (*((volatile uint32_t *) 0x4005D400))
#define GPIODEN_F   (*((volatile uint32_t *) 0x4005D51C))
#define GPIODATA_F  (*((volatile uint32_t *) 0x4005D3FC))

#define GPIODIR_J   (*((volatile uint32_t *) 0x40060400))
#define GPIODEN_J   (*((volatile uint32_t *) 0x4006051C))
#define GPIODATA_J  (*((volatile uint32_t *) 0x400603FC))
#define GPIOLOCK_J  (*((volatile uint32_t *) 0x40060520))
#define GPIOCR_J    (*((volatile uint32_t *) 0x40060524))
#define GPIOPUR_J   (*((volatile uint32_t *) 0x40060510))

#define GPIODIR_N   (*((volatile uint32_t *) 0x40064400))
#define GPIODEN_N   (*((volatile uint32_t *) 0x4006451C))
#define GPIODATA_N  (*((volatile uint32_t *) 0x400643FC))

#endif  // LAB1_H_