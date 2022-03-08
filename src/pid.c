#include "pid.h"

#include <stdio.h>

double valor_saida, controle_sinal;
double reference = 0.0;
double Kp = 0.0;  // Ganho Proporcional
double Ki = 0.0;  // Ganho Integral
double Kd = 0.0;  // Ganho Derivativo
int T = 1.0;      // Período de Amostragem (ms)
unsigned long last_time;
double error_total, previous_error = 0.0;
int signal_MAX = 100.0;
int signal_MIN = -100.0;

void constantes_pid(double Kp_, double Ki_, double Kd_) {
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
}

void referencias_pid(float reference_) {
  reference = (double)reference_;
}

double controle_pid(double valor_saida) {
  double error = reference - valor_saida;

  error_total += error;  // Acumula o error (Termo Integral)

  if (error_total >= signal_MAX) {
    error_total = signal_MAX;
  } else if (error_total <= signal_MIN) {
    error_total = signal_MIN;
  }

  double delta_error =
      error - previous_error;  // Diferença entre os erros (Termo Derivativo)

  controle_sinal = Kp * error + (Ki * T) * error_total +
                      (Kd / T) * delta_error;  // PID calcula sinal de controle

  if (controle_sinal >= signal_MAX) {
    controle_sinal = signal_MAX;
  } else if (controle_sinal <= signal_MIN) {
    controle_sinal = signal_MIN;
  }

  previous_error = error;

  return controle_sinal;
}
