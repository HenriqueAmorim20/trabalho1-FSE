#ifndef UART_H
#define UART_H

#define GET_INTERNAL_TEMP 0xC1
#define GET_POTENTIOMETER 0xC2
#define GET_KEY_VALUE 0xC3
#define SEND_SIGNAL 0xD1
#define SEND_REFERENCE_SIGNAL 0xD2
#define SEND_SYSTEM_STATE 0xD3
#define SEND_CONTROL_MODE 0xD4

typedef struct Number_type {
  int int_value;
  float float_value;
} Number_type;

int uart_conectar();
void uart_write(int filestream, unsigned char code);
void uart_write_send(int filestream, int value);
void uart_write_send_ref(int filestream, float reference_signal);
Number_type uart_read(int filestream, unsigned char code);
void close_uart(int filestream);

#endif /* UART_H */
