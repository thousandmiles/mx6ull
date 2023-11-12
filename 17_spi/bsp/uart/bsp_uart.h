#ifndef _BSP_UART_H_

#define _BSP_UART_H_

#include "imx6ull.h"

void uart_init(void);

void uart_io_init(void);

void uart_disable(UART_Type* base);

void uart_enable(UART_Type* base);

void uart_softreset(UART_Type* base);

void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz);

void putc(uint8_t c);

void puts(char* str);

uint8_t getc(void);

void raise(int sig_nr);


#endif // !_BSP_UART_H_
