/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * May 25, 2025
 * EE/CSE 474: Lab4 Task1c configures the LCD screen to display
 * two virtual buttons switching the frequency of the system clock
 * to either 12 MHz or 120 MHz. Users can also see the temperature
 * of the board in degrees F and C.  
 */

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "../SSD2119_Display.h"
#include "../SSD2119_Touch.h"
#include "../Lab4_Inits.h"
#include "../tm4c1294ncpdt.h"

const float VREFP = 3.3;        // Voltage Reference Positive
const float VREFN = 0.0;        // Voltage Reference Negative
uint32_t ADC_value;
enum frequency freq;
const int center_x_12 = 1400;   // X-coordinate of the center of the 12 MHz button
const int center_x_120 = 1860;  // X-coordinate of the center of the 120 MHz button
const int center_y = 780;       // Y-coordinate of the center of both buttons

/**
 * Calculate the Euclidean distance between point A (x1, y1) and point B (x2, y2)
 */
float dist(int x1, int x2, int y1, int y2);

int main() {
  LCD_Init();
  Touch_Init();
  
  // Draw the virtual, 12 MHz button
  LCD_DrawFilledCircle(110, 200, 30, Color4[5]);
  LCD_SetCursor(90, 195);
  LCD_PrintString("12 MHz");
  
  // Draw the virtual, 120 MHz button
  LCD_DrawFilledCircle(220, 200, 30, Color4[5]);
  LCD_SetCursor(200, 195);
  LCD_PrintString("120 MHz");
  
  freq = PRESET2;               // 60 MHz
  PLL_Init(freq);               // Set system clock frequency to 60 MHz
  ADCReadTemp_Init();           // Initialize ADC0 to read from the temperature sensor
  TimerADCTriger_Init();        // Initialize Timer0A to trigger ADC0
    
  unsigned long x, y;           // (x, y) point being touched
  
  while(1) {
    x = Touch_ReadX();
    y = Touch_ReadY();
    
    float temp_C, temp_F;

    // Calculate temperature from the read ADC Value
    temp_C = 147.5 - ((75 * (VREFP - VREFN) * ADC_value) / 4096);
    temp_F = (temp_C * (9.0/5.0)) + 32;
    LCD_SetCursor(20, 10);
    LCD_Printf("The current temperature is %f C, %f F.  \n", temp_C, temp_F);
    LCD_SetCursor(20, 120);
    LCD_Printf("The current clock frequency is %d MHz. \n", (int) freq);
    printf("Touch at (%d, %d)\n", x, y);
    // If the (x, y) being touched is within the 12 MHz button's range
    if (dist(center_x_12, x, center_y, y) <= 130) {
      freq  = PRESET3;          // Set system clock to 12 MHz
      PLL_Init(freq);
    }
    // If the (x, y) being touched is within the 120 MHz button's range
    if (dist(center_x_120, x, center_y, y) <= 130) {
      freq  = PRESET1;          // Set system clock to 120 MHz
      PLL_Init(freq);
    }
  }
}

float dist(int x1, int x2, int y1, int y2) {
  float x_dist = (x2 - x1) * (x2 - x1);
  float y_dist = (y2 - y1) * (y2 - y1);
  return sqrt(x_dist + y_dist);
}

/**
 * Called by NVIC whenever Timer0A times out. Stores the result
 * of samples collected by Sequencer 3 into our global ADC_value
 */
#pragma call_graph_root = "interrupt"
__weak void ADC0SS3_Handler(void) {
  // Clear the ADC0 interrupt flag
  ADC0_ISC_R = 0x8;
  // Save the ADC value to global variable ADC_value
  ADC_value = (ADC0_SSFIFO3_R & 0xFFF);
}