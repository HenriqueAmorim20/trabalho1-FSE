#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bme280.h"
#include "display.h"
#include "gpio.h"
#include "pid.h"
#include "thermometer.h"
#include "uart.h"

#define true 1
#define false 0

int use_terminal = false;
struct bme280_dev bme_connection;
int uart_filesystem, key_gpio = 1;

void finalizarPrograma() {
  system("clear");
  printf("The end\n");
  turn_resistance_off();
  turn_fan_off();
  close_uart(uart_filesystem);
  exit(0);
}

void rotinaUart(int key) {
  system("clear");
  float tempI, tempR, tempE;
  int value_to_send = 0;
  printf("\n---------- PID UART - início da rotina ----------\n");
  constantes_pid(20, 0.1, 100);
  do {
    uart_write(uart_filesystem, GET_INTERNAL_TEMP);
    tempI = uart_read(uart_filesystem, GET_INTERNAL_TEMP).float_value;

    double value_to_send = controle_pid(tempI);

    pwm_control(value_to_send);

    uart_write(uart_filesystem, GET_POTENTIOMETER);
    tempR = uart_read(uart_filesystem, GET_POTENTIOMETER).float_value;

    referencias_pid(tempR);

    tempE = get_current_temperature(&bme_connection);
    printf("tempI: %.2f⁰C - tempR: %.2f⁰C - TempE: %.2f⁰C\n", tempI, tempR, tempE);
    print_display("UART", tempI, tempR, tempE);

    if (!use_terminal) {
      uart_write(uart_filesystem, GET_KEY_VALUE);
      key_gpio = uart_read(uart_filesystem, GET_KEY_VALUE).int_value;
    }

    uart_write_send(uart_filesystem, value_to_send);
  } while (key_gpio == key);
  printf("-----------------------------------------------\n");
}

void terminal_routine(int key, float TR) {
  system("clear");
  float TI, TE;
  double Kp, Ki, Kd;
  int value_to_send = 0;
  printf("\n================== Iniciada rotina PID - Terminal ==================\n");
  
  constantes_pid(20, 0.1, 100);

  do {

    uart_write(uart_filesystem, GET_INTERNAL_TEMP);
    TI = uart_read(uart_filesystem, GET_INTERNAL_TEMP).float_value;

    double value_to_send = controle_pid(TI);

    pwm_control(value_to_send);

    uart_write(uart_filesystem, SEND_REFERENCE_SIGNAL);
    // TR = uart_read(uart_filesystem, GET_POTENTIOMETER).float_value;

    referencias_pid(TR);

    TE = get_current_temperature(&bme_connection);
    printf("TI: %.2f⁰C - TR: %.2f⁰C - TE: %.2f⁰C\n", TI, TR, TE);
    print_display("Terminal", TI, TR, TE);

    if (!use_terminal) {
      uart_write(uart_filesystem, GET_KEY_VALUE);
      key_gpio = uart_read(uart_filesystem, GET_KEY_VALUE).int_value;
    }

    uart_write_send_ref(uart_filesystem, TR);
  } while (key_gpio == key);
  printf("============================================================\n");
}

void curva_routine(int key) {
  system("clear");
  float TI, TR, TE;
  int value_to_send = 0;
  printf("\n================== Iniciada rotina PID - Curva Reflow ==================\n");
  constantes_pid(20, 0.1, 100);
  do {
    uart_write(uart_filesystem, GET_INTERNAL_TEMP);
    TI = uart_read(uart_filesystem, GET_INTERNAL_TEMP).float_value;

    double value_to_send = controle_pid(TI);

    pwm_control(value_to_send);

    uart_write(uart_filesystem, GET_POTENTIOMETER);
    TR = uart_read(uart_filesystem, GET_POTENTIOMETER).float_value;

    referencias_pid(TR);

    TE = get_current_temperature(&bme_connection);
    printf("TI: %.2f⁰C - TR: %.2f⁰C - TE: %.2f⁰C\n", TI, TR, TE);
    print_display("Curva Reflow", TI, TR, TE);

    if (!use_terminal) {
      uart_write(uart_filesystem, GET_KEY_VALUE);
      key_gpio = uart_read(uart_filesystem, GET_KEY_VALUE).int_value;
    }

    uart_write_send(uart_filesystem, value_to_send);
  } while (key_gpio == key);
  printf("============================================================\n");
}

void inicializar() {
  wiringPiSetup();
  turn_resistance_off();
  turn_fan_off();
  connect_display();
  bme_connection = connect_bme();
  uart_filesystem = uart_conectar();
  system("clear");
}

void options() {
  int option, key;
  float tr;
  printf("Acessar programa?\n\t1) Terminal\n\t2) Potenciômetro\n\t3)\n\t");
  scanf("%d", &option);
  switch (option) {
    case 1:
      printf("Temperatura de Referência(TR): \n");
      scanf("%f", &tr);
      terminal_routine(0, tr);
      break;
    case 2:
      use_terminal = true;
      rotinaUart(0);
      break;

    case 3:

      break;
    default:
      system("clear");
      printf("Invalido\n");
      options();
      break;
  }
}

int main() {
  inicializar();
  signal(SIGINT, finalizarPrograma);
  printf("Primeiro trabalho - Henrique Melo");
  options();
  return 0;
}
