/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * May 25, 2025
 * EE/CSE 474: Lab4 Task1a initializes the LCD and 
 * fills the screen with magenta.
 */

#include "../SSD2119_Display.h"

int main() {
  LCD_Init();                   // Initialize the LCD driver                 
  LCD_ColorFill(Color4[5]);     // Fill the screen with magenta
}