/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * June 5, 2025
 * EE/CSE 474: Lab5 drivers header file
 */

#ifndef _Lab5_Inits_H_
#define _Lab5_Inits_H_

// Preset frequency for 120 MHZ, 60 MHZ, and 12 MHZ clock
// Pass in the preset to Timer1_Init and PLL_Init to configure
// the system clock to specified frequency
enum frequency {PRESET1 = 120, PRESET2 = 60, PRESET3 = 12};

// Initializes the PLL module and generates a system clock frequency
// that equal to the frequency preset.
// Returns 1 if configured successfully, -1 if you select a non-exist preset.
int PLL_Init(enum frequency freq);

// Initializes GPIO output pins for breadboard LEDs.
void LEDBreadboard_Init(void);

// Initializes the 4 onboard LEDs.
void LEDOnboard_Init(void);

// Initializes ADC Module 0 Sample Sequencer 3. The module is triggered by
// Timer module 0 subtimer A at 1 HZ. The interupt of ADC0 SS3 will be
// generated whenever the A/D conversion is done.
void ADCReadPot_Init(void);

// Initializes ADC Module 0 Sample Sequencer 3. The module is triggered by
// Timer module 0 subtimer A at 1 HZ. The interupt of ADC0 SS3 will be
// generated whenever the A/D conversion is done.
void ADCReadTemp_Init(void);

// Initializes Timer 0A to trigger ADC0 at 1 HZ.
void TimerADCTriger_Init(void);

// Initializes PWM for use with LEDs.
// Configures M0PWM1 at 25 kHz at an initial 75% duty cycle
void PWM_Init(void);

#endif  // _Lab5_Inits_H_
