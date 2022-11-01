/*
 * Versatile Platform Board (Versatile PB)
 */

#ifndef BOARD_VPB_H_
#define BOARD_VPB_H_

/*
 * Four PL011 UART Controllers
 */
#define UART0 0x101f1000
#define UART1 0x101f2000
#define UART2 0x101f3000
#define UART3 0x10009000

/*
 * PL011 UART
 */
#include "uart.h"

#endif /* BOARD_VPB_H_ */
