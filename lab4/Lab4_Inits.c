/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * May 25, 2025
 * EE/CSE 474: Lab4 drivers starter code
 */

#include "PLL_Header.h"
#include "Lab4_Inits.h"

// STEP 0a: Include your header file here
#include "timer.h"
#include "tm4c1294ncpdt.h"

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

void ADCReadTemp_Init(void) {
  volatile unsigned short delay = 0;
  SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R0;                // Enable ADC0 clock
  delay++;
  delay++;
  
  PLLFREQ0 |= 0x00800000;                               // Power up the PLL
  while (PLLSTAT != 0x1);                               // Wait for PLL to lock
  ADC0_CC_R |= 0x1;                                     // Configure ADCCC to use the clock source defined by ALTCLKCFG
  
  ADC0_ACTSS_R &= ~0x8;                                 // Disable sample sequencer 3
  ADC0_EMUX_R |= 0x5000;                                // Select timer as the trigger for SS3
  ADC0_SSTSH3_R = (ADC0_SSTSH3_R & ~0xF) | 0x4;         // Set sample and hold width to 16 ADC clocks
  ADC0_SSCTL3_R |= 0xE;                                 // Configure ADCSSCTL3 register
  ADC0_IM_R |= 0x8;                                     // Set the SS3 interrupt mask
  NVIC_EN0_R |= (0x1 << 17);                            // Set the corresponding bit for ADC0 SS3 in NVIC
  ADC0_ACTSS_R |= 0x8;                                  // Enable ADC0 SS3
}

void TimerADCTriger_Init(void) {
  volatile unsigned short delay = 0;
  SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;     // Enable Timer 0
  delay++;
  delay++;
  
  struct Timer timer = {
    0, 0x2, 1 * 16000000,
    &TIMER0_CTL_R, &TIMER0_CFG_R, &TIMER0_TAMR_R,
    &TIMER0_TAILR_R, &TIMER0_RIS_R, &TIMER0_ICR_R
  };
  init(timer);
  TIMER0_CTL_R |= 0x20;   // Enable Timer A as an ADC trigger
  TIMER0_ADCEV_R |= 0x1;  // Enable Timer A time-out to trigger ADC
  enable(timer);        // Start timer
}