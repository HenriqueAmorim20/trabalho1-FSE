#ifndef PID_H_
#define PID_H_

void constantes_pid(double Kp_, double Ki_, double Kd_);
void referencias_pid(float reference_);
double controle_pid(double valor_saida);

#endif /* PID_H_ */

