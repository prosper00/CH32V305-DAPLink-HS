#ifndef _USB2UART_H
#define _USB2UART_H

#include <stdio.h>
#include "ch32v30x.h"
#include "dap_main.h"
#include "usb2uart.h"

#define UART_DMA_BUF_LEN 512

void uartx_preinit(void);
void usb2uart_rx_handle(void);

#endif