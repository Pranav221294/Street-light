void set_PWM_period(unsigned int value);
void set_PWM0(unsigned int value);
void set_PWM2(unsigned int value);
void set_PWM3(unsigned int value);
void PWM_DEAD_TIME_VALUE(unsigned int  DeadTimeData);
void charging_side_initilzation();
void charging_pwm_off();
void pwm_init();
void Timer0_ISR (void);
unsigned long  int systick();