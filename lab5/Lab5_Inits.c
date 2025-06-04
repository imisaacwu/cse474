/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * June 5, 2025
 * EE/CSE 474: Lab5 drivers starter code
 */

#include "PLL_Header.h"
#include "Lab5_Inits.h"

// STEP 0a: Include your header file here
// YOUR CUSTOM HEADER FILE HERE
#include "lab5.h"
#include "timer.h"

int PLL_Init(enum frequency freq) {
    // Do NOT modify this function.
    MOSCCTL &= ~(0x4);                      // Power up MOSC
    MOSCCTL &= ~(0x8);                      // Enable MOSC
    while ((RIS & 0x100) == 0) {};          // Wait for MOSC to be ready
    RSCLKCFG |= (0x3 << 20);                // Select MOSC as system clock source
    RSCLKCFG |= (0x3 << 24);                // Select MOSC as PLL clock source

    PLLFREQ0 |= 0x60;                       // Set MINT field
    PLLFREQ1 |= 0x4;                        // Set N field

    MEMTIM0 &= ~((0xF << 22) | (0xF << 6));     // Reset FBCHT and EBCHT
    MEMTIM0 &= ~((0xF << 16) | (0xF << 0));     // Reset EWS and FWS
    MEMTIM0 &= ~((0x1 << 21) | (0x1 << 5));     // Reset FBCE and EBCE

    RSCLKCFG &= ~(0x1 << 28);                   // Temporarilly bypass PLL

    switch (freq) {
        case 120:
            MEMTIM0 |= (0x6 << 22) | (0x6 << 6);    // Set FBCHT and EBCHT
            MEMTIM0 |= (0x5 << 16) | (0x5 << 0);    // Set EWS and FWS
            RSCLKCFG |= 0x3;                        // Set PSYSDIV to use 120 MHZ clock
            RSCLKCFG &= ~0x3FC;                     // Update PSYSDIV field
            break;
        case 60:
            MEMTIM0 |= (0x3 << 22) | (0x3 << 6);    // Set FBCHT and EBCHT
            MEMTIM0 |= (0x2 << 16) | (0x2 << 0);    // Set EWS and FWS
            RSCLKCFG |= 0x7;                        // Set PSYSDIV to use 60 MHZ clock
            RSCLKCFG &= ~0x3F8;                     // Update PSYSDIV field
            break;
        case 12:
            MEMTIM0 |= (0x1 << 21) | (0x1 << 5);    // Set FBCE and EBCE
            RSCLKCFG |= 0x27;                       // Set PSYSDIV to use 12 MHZ clock
            RSCLKCFG &= ~0x3D8;                     // Update PSYSDIV field
            break;
        default:
            return -1;
    }

    RSCLKCFG |= (0x1 << 30);                // Enable new PLL settings
    PLLFREQ0 |= (0x1 << 23);                // Power up PLL
    while ((PLLSTAT & 0x1) == 0) {};        // Wait for PLL to lock and stabilize

    RSCLKCFG |= (0x1u << 31) | (0x1 << 28);  // Use PLL and update Memory Timing Register
    return 1;
}

void LEDOnboard_Init(void) {
  // STEP 1: Initialize the 4 on board LEDs by initializing the corresponding
  // GPIO pins.

  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x1020;       // Enable ports N and F
  delay++;
  delay++;

  GPIODIR(F) |= 0x11;        // Set PF0 (LED D4) and PF4 (LED D3) as outputs
  GPIODEN(F) |= 0x11;        // Set PF0 and PF4 to digital ports

  GPIODIR(N) |= 0x3;         // Set PN0 (LED D2) and PN1 (LED D1) as outputs
  GPIODEN(N) |= 0x3;         // Set PN0 and PN1 to digital ports

  GPIODATA(F) &= ~0x11;      // Set PF0 and PF4 to 0 (off)
  GPIODATA(N) &= ~0x3;       // Set PN0 and PN1 to 0 (off)
}

void ADCReadPot_Init(void) {
  // STEP 2: Initialize ADC0 SS3.
  // 2.1: Enable the ADC0 clock
  volatile unsigned short delay = 0;
  RCGCADC |= 0x1;
  // 2.2: Delay for RCGCADC (Refer to page 1073)
  delay++;
  delay++;

  // 2.3: Power up the PLL (if not already)
  PLLFREQ0 |= 0x00800000; // we did this for you
  // 2.4: Wait for the PLL to lock
  while (PLLSTAT != 0x1); // we did this for you
  // 2.5: Configure ADCCC to use the clock source defined by ALTCLKCFG
  ADCCC_0 |= 0x1;
  // 2.6: Enable clock to the appropriate GPIO Modules (Hint: Table 15-1)
  RCGCGPIO |= 0x10;
  // 2.7: Delay for RCGCGPIO
  delay++;
  delay++;
  // 2.8: Set the GPIOAFSEL bits for the ADC input pins
  GPIOAFSEL(E) |= 0x8;
  // 2.9: Clear the GPIODEN bits for the ADC input pins
  GPIODEN(E) &= ~0x8;
  // 2.10: Disable the analog isolation circuit for ADC input pins (GPIOAMSEL)
  GPIOAMSEL(E) |= 0x8;
  // 2.11: Disable sample sequencer 3 (SS3)
  ADCACTSS_0 &= ~0x8;
  // 2.12: Select timer as the trigger for SS3
  ADCEMUX_0 |= 0x5000;
  // 2.13: Select the analog input channel for SS3 (Hint: Table 15-1)
  ADCSSEMUX3_0 &= ~0x1;
  ADCSSMUX3_0 &= ~0xF;
  // 2.14: Configure ADCSSCTL3 register
  ADCSSCTL3_0 |= 0x6;
  // 2.15: Set the SS3 interrupt mask
  ADCIM_0 |= 0x8;
  // 2.16: Set the corresponding bit for ADC0 SS3 in NVIC
  NVIC_EN0 |= (0x1 << 17);
  // 2.17: Enable ADC0 SS3
  ADCACTSS_0 |= 0x8;
}

void TimerADCTriger_Init(void) {
  // Initialize Timer0A to trigger ADC0 at 1 HZ.
  volatile unsigned short delay = 0;
  RCGCTIMER |= 0x1;     // Enable Timer 0
  delay++;
  delay++;
  
  struct Timer timer = {
    0, TIMER_PERIODIC, 0.1 * CLK_FRQ,
    &GPTMCTL(0), &GPTMCFG(0), &GPTMTAMR(0), &GPTMTAILR(0), &GPTMRIS(0), &GPTMICR(0)
  };
  init(timer);
  GPTMCTL(0) |= 0x20;   // Enable Timer A as an ADC trigger
  GPTMADCEV(0) |= 0x1;  // Enable Timer A time-out to trigger ADC
  enable(timer);        // Start timer
}

void PWM_Init(void) {
  volatile unsigned short delay = 0;
  RCGCPWM |= 0x1;       // Enable PWM Module 0
  RCGCGPIO |= 0x20;     // Enable Port F
  delay++;
  delay++;
  
  GPIOAFSEL(F) |= 0x2;  // Set PF1 to alternate function
  GPIOPCTL(F) &= ~0xF0; // Clear M0PMW1 port control
  GPIOPCTL(F) |= 0x60;  // Set it to M0PMW1
  GPIODIR(F) |= 0x2;    // Set PF1 as output
  GPIODEN(F) |= 0x2;    // Enable digital function on PF1
  
  PWMCC |= 0x000;       // Set PWM to system clock
  PWM0CTL = 0x0;
  // Drive pwmB low when it hits comparator
  // Else, drive high when we hit the load (stay high while we approach comparator)
  PWM0GENB = 0x80C;   
  PWM0LOAD = 2399;
  PWM0CMPB = 599;      // Set duty cycle to 75% (comparator value)
  PWM0CTL = 0x1;       // Start timer
  PWMENABLE |= 0x2;    // Enable PWM outputs for M0PMW1
}