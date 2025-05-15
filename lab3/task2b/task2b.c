/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * May 15, 2025
 * EE/CSE 474: Lab3 Task2b main function
 */

#include <stdint.h>
#include <stdio.h>  // printf()

#include "../lab3.h"

// Configures necessary ports
void config_ports();

// Function that will read the given character from UART
// and returns it back to the sender.
void return_to_sender(char);

int main(void) {
  // Select system clock frequency preset
  config_ports();
  
  while(1) {
    // Check if the read register is not empty
    if ((UARTFR(3) & (1 << 6))) {
      // Read the character
      char c = UARTDR(3);
      return_to_sender(c);
    }
  }
  
  return 0;
}

void config_ports() {
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x1;              // Enable port A
  RCGCUART |= 0x8;              // Enable UART 3
  delay++;
  delay++;
  
  // Configure UART 0 Rx/Tx on Port A
  GPIOLOCK(A) = UNLOCK;         // Unlock the GPIOCR register
  GPIOCR(A) |= 0x30;            // Enable writing to GPIOAFSEL
  GPIOAFSEL(A) |= 0x30;         // Set Port PA4 and PA5 to do alternate functions
  GPIOCR(A) &= ~0x30;           // Disable writing
  GPIOLOCK(A) = LOCK;           // Lock the GPIOCR register
  GPIOPCTL(A) |= 0x110000;      // Set PA4/PA5 to UART 3 Rx/Tx
  GPIODEN(A) |= 0x30;           // Enable digital function for PA4 & PA5
  
  // Configure UART
  UARTCTL(3) &= ~0x1;
  //while (UARTFR_0 & 0x8);       // Wait for transmit/receive to complete
  //UARTLCRH_0 &= ~0x10;          // Flush transmit FIFO
  
  // Set UART to use the Alternate Clock Source from (ALTCLKCFG)
  UARTCC(3) |= 0x5;

  // We want the baud rate to be 9600 with ClkDiv=16 and UARTSysClk=16MHz
  // So we need BRDI=104 and BRDF=0.16... -> integer(0.16... * 64 + 0.5) = 11
  UARTIBRD(3) = 104;
  UARTFBRD(3) = 11;
  // Write to UARTLCRH_0 to update baud rate configs, keep default settings
  //UARTLCRH_0 = 0;
  // Set UART Word Length to 8 bits
  UARTLCRH(3) |= 0x60;
  // Enable UART FIFOs
  // UARTLCRH_0 |= 0x10;
  
  // Enable UART
  UARTCTL(3) |= 0x1;
}

void return_to_sender(char c) {
  // Wait to make sure the transmit register is empty
  while ((UARTFR(3) & (1 << 5)));
  // Send the character received
  UARTDR(3) = c;
}