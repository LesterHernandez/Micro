#ifndef UART_H_
#define UART_H_

#include "stm32f3xx.h"                  // Device header

void USART_config(uint32_t baudrate);
void USART_Send(char c);
void USART_putString(char * string);

#endif /* UART_H_ */