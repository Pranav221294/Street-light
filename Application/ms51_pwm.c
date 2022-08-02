#include "ms51_pwm.h"
#include "MS51_16K.h"
static unsigned int cached_dead_time;

static void PWM_DEAD_TIME_VALUE(unsigned int  DeadTimeData)
{
    unsigned char deadtmphigh, deadtmplow;
	deadtmplow = DeadTimeData;
	deadtmphigh = DeadTimeData >> 8;
	BIT_TMP = EA;
	EA = 0;
	if (deadtmphigh == 0x01)
	{
		TA = 0xAA;
		TA = 0x55;
		PDTEN |= 0x10;
	}
	TA = 0xAA;
	TA = 0x55;
	PDTCNT = deadtmplow;
	EA = BIT_TMP;
}
void ms51_pwm_set_period(int period)
{
    PWMPH = period >> 8;
    PWMPL = period & 0x00FF;
}
void ms51_pwm_set_duty(float duty)
{
    uint16_t period = 0;
    uint16_t pwm_value = 0;
    if (duty < 0 && duty > 100)
    {
        return;
    }
    period = (uint16_t)(PWMPH << 8) | (PWMPL);
    if (period <= 0)
    {
        return;
    }
    pwm_value = period * (duty / 100.0);
    PWM2L = (pwm_value & 0x00FF);
    PWM2H = ((pwm_value & 0xFF00) >> 8);
    set_PWMCON0_LOAD;
}
float ms51_pwm_get_duty(void)
{
    uint16_t period = 0;
    uint16_t pwm_value = 0;
    period = (uint16_t)(PWMPH << 8) | (PWMPL);
    pwm_value = (uint16_t)(PWM2H << 8) | (PWM2L);
    if (period <= 0)
    {
        return 0;
    }
    return ((float)(pwm_value * 100) / period);
}
void ms51_pwm_start(void)
{
	PWM3_P00_OUTPUT_ENABLE;
    PWM2_P10_OUTPUT_ENABLE;
    PWM23_DEADTIME_ENABLE;
    PWM_DEAD_TIME_VALUE(cached_dead_time);
    set_PWMCON0_LOAD;
    set_PWMCON0_PWMRUN;
}
void ms51_pwm_stop(void)
{
    PWM3_P00_OUTPUT_DISABLE;
    PWM2_P10_OUTPUT_DISABLE;
    clr_PWMCON0_LOAD;
    clr_PWMCON0_PWMRUN;
}
void ms51_pwm_dead_time(unsigned int DeadTimeData)
{
    cached_dead_time=DeadTimeData;
    PWM_DEAD_TIME_VALUE(DeadTimeData);
}
