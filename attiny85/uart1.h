#pragma once
void init_uart1(unsigned long f_cpu, uint8_t tx_pin);
void send_uart1(char c);
void print_uart1(const char* str);
void puts_uart1(const char* str);
