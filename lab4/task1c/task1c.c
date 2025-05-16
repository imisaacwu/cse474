#include <stdio.h>

#include "../SSD2119_Display.h"
#include "../SSD2119_Touch.h"

int main() {
  LCD_Init();
  Touch_Init();
  
  LCD_DrawFilledCircle(50, 800, 30, Color4[5]);
  LCD_SetCursor(20, 800);
  LCD_PrintString("12 MHz");
  
  unsigned long x, y;
  
  while(1) {
    x = Touch_ReadX();
    y = Touch_ReadY();
    
    printf("Touch at (%d, %d)\n", x, y);
  }
}