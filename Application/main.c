#include <stdio.h>
#include "MS51_16K.h"
#include "ms51_pwm.h"
#define UART_BAUD_RATE 115200

static void ms51_gpio_config(void)
{
	/** uart pin */
	P06_QUASI_MODE;
	P07_QUASI_MODE;
	/** pwm pin */
	P00_PUSHPULL_MODE;
	P10_PUSHPULL_MODE;
	P00 = 0;
	P10 = 0;
}
static void ms51_adc_config(void)
{
	/* Configuring channel*/
	P03_INPUT_MODE; // SPV_pin
	P04_INPUT_MODE; // Battery Pin
	P11_INPUT_MODE; // Load Pin
	P05_INPUT_MODE; // Temp Pin
	P30_INPUT_MODE; // Charging Pin
	P17_INPUT_MODE; // Discharging Pin

	ENABLE_ADC_CH4; // Temp
	ENABLE_ADC_CH5; // Battery Voltage
	ENABLE_ADC_CH6; // Solar Voltage
	ENABLE_ADC_CH7; // Load Voltage
	ENABLE_ADC_CH0; // Discharging Pin
	ENABLE_ADC_CH1; // Charging Pin
	/* configuring adc periph*/
	// ADCCON1 |= 0X30;   /* clock divider */
	// ADCCON2 |= 0X0E;   /* AQT time */
	// AUXR1 |= SET_BIT4; /* ADC clock low speed */
	set_ADCCON1_ADCEN;
	clr_ADCCON0_ADCF;
}

static void ms51_pwm_config(void)
{
	PWM3_P00_OUTPUT_ENABLE;
	PWM2_P10_OUTPUT_ENABLE;
	/*use system clock for pwm*/
	PWM_CLOCK_FSYS;
	/*Clock is */
	PWM_CLOCK_DIV_2;
	/* pwm is in complementry mode*/
	PWM_COMPLEMENTARY_MODE;
	/* Enable Dead time controll on channel 0 and 1*/
	PWM23_DEADTIME_ENABLE;
	/* Insert the amount of dead time*/
	// Dead time       = 47 <PDTEN.4+PDTCNT[7:0]>/Fsys = 47/Fsys = 47/24000000 = 2 us (max value)
	//PWM_DEAD_TIME_VALUE(47);
	ms51_pwm_dead_time(47);
	/* Set PWM Period*/
	// PWM frequency   = Fpwm/((PWMPH,PWMPL)+1) = (16MHz/8)/(0x7CF+1) = 1KHz (1ms)
	// PWM0 high duty  = PWM0H,PMW0L = 0x03CF = 1/2 PWM period
	PWMPH = 0x02;
	PWMPL = 0x57;
}
static void ms51_uart_init(void)
{
	UART_Open(24000000, UART0_Timer3, UART_BAUD_RATE);
	ENABLE_UART0_PRINTF;
}
void main(void)
{
	// init cloack sys according to app
	MODIFY_HIRC(HIRC_24);
	ms51_gpio_config();
	// init adc
	ms51_adc_config();
	// init pwm
	ms51_pwm_config();
	//uart init 
	ms51_uart_init();
	// test
	ms51_pwm_set_duty(50);
	ms51_pwm_start();
	while (1)
	{
	}
}