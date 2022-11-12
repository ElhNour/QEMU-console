#include "main.h"

/**
 * This is the C entry point, upcalled once the hardware has been setup properly
 * in assembly language, see the reset.s file.
 */

int global_1=1;
int global_2=0;
void _start() {// including both interrupts and traps.
  int i = 0;
  int count = 0;
  uart_send_string(UART0, "\nQuit with \"C-a c\" and then type in \"quit\".\n");
  uart_send_string(UART1, "\nHello world!\n");

  while (1) {
    unsigned char c;
    while (0 == uart_receive(UART0, &c)) {
      // friendly reminder that you are polling and therefore spinning...
      // not good for the planet! But until we introduce interrupts,
      // there is nothing you can do about it... except comment out
      // this annoying code ;-)
      count++;
      if (count > 10000000) {
        //uart_send_string(UART0, "\n\rZzzz....\n\r");
        count = 0;
      }
    }
    if (c == '\r')
      uart_send(UART1, '\n');
    uart_send(UART1, c);
  }
}
