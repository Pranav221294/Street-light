#ifndef _MS51_PWM_H_
#define _MS51_PWM_H_
void ms51_pwm_set_period(int period);
void ms51_pwm_set_duty(float duty);
float ms51_pwm_get_duty(void);
void ms51_pwm_dead_time(unsigned int deadtime);
void ms51_pwm_start(void);
void ms51_pwm_stop(void);
#endif