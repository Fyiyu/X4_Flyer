#ifndef __UART_H__
#define __UART_H__

#include "stm32f10x.h"
#include <stdio.h>

extern float uart_buffer[24];

void uart_send(unsigned char num);
void uart_putchar(uint8_t DataToSend);
void uart_putshort(uint16_t DataToSend);
void uart_putfloat(float DataToSend);
void doRxData(void);
int fputc(int ch, FILE *f);

#endif
