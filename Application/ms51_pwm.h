#ifndef _MS51_PWM_H_
#define _MS51_PWM_H_
#include <stdint.h>
void ms51_pwm_set_period(uint16_t period);
void ms51_pwm_set_duty(float duty);
float ms51_pwm_get_duty(void);
void ms51_pwm_start(void);
void ms51_pwm_stop(void);
#endif