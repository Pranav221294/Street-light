#include "MS51_16K.h"
#include "adc_2.h"
#include "pwm_2.h"
#include "mppt_track_1.h"

#define TH0_INIT        0xA2 
#define TL0_INIT        0x42

xdata  unsigned long  int ms = 0;

void Timer0_ISR (void) interrupt 1  //interrupt address is 0x000B
{
    _push_(SFRS);
  
    TH0 = TH0_INIT;
    TL0 = TL0_INIT;    
    TF0 = 0;
    P14 = ~P14;                     // GPIO1 toggle when interrupt
  ms++;
	if(ms>0xffffffff)
	 {
	   ms =0; 
	 }
    _pop_(SFRS);
}

unsigned long  int systick()
{

	 return ms;


}


void pwm_init()
{
  ///////Charging Pin////////////
   P00_PUSHPULL_MODE;
   P10_PUSHPULL_MODE;
	 P00 = 0;
	 P10 = 0;
  ///////Charging Pin////////////
 

}


void set_PWM_period(unsigned int value)
{
  PWMPL = (value & 0x00FF);
  PWMPH = ((value & 0xFF00) >> 8);
}


void set_PWM0(unsigned int value)
{
  PWM0L = (value & 0x00FF);
  PWM0H = ((value & 0xFF00) >> 8);
  set_PWMCON0_LOAD;
}

void set_PWM2(unsigned int value)
{
  PWM2L = (value & 0x00FF);
  PWM2H = ((value & 0xFF00) >> 8);
 set_PWMCON0_LOAD;
}


void set_PWM3(unsigned int value)
{
  PWM3L = (value & 0x00FF);
  PWM3H = ((value & 0xFF00) >> 8);
 set_PWMCON0_LOAD;
}


/*PWM deadtime define */
void PWM_DEAD_TIME_VALUE(unsigned int  DeadTimeData)
{
  unsigned char deadtmphigh,deadtmplow;
  deadtmplow = DeadTimeData;
  deadtmphigh = DeadTimeData>>8;
  BIT_TMP = EA;
  EA = 0;
  if (deadtmphigh==0x01)
  {
    TA = 0xAA;
    TA = 0x55;
    PDTEN|=0x10;
  }
  TA = 0xAA;
  TA = 0x55;
  PDTCNT = deadtmplow;
  EA = BIT_TMP;
}

void charging_side_initilzation()
{
         PWM3_P00_OUTPUT_ENABLE;  
	       PWM2_P10_OUTPUT_ENABLE;
   
	      PWM_COMPLEMENTARY_MODE;
				PWM_CLOCK_DIV_8;
	
//        PWMPH = 0x00;
//        PWMPL = 0x64;
	  
	   set_PWM_period(100);// around 20khz set 
	   set_PWM2(75);
		  
//        PWM2H = 0x00;
//        PWM2L = 0x5A;
			
      	PWM23_DEADTIME_ENABLE ;
        PWM_DEAD_TIME_VALUE(64);// 64 = 4us,32 = 2us,160 = 10 us
		  
        set_PWMCON0_LOAD;
        set_PWMCON0_PWMRUN;
////////////////////////////////  
    ENABLE_TIMER0_MODE1;
    TIMER0_FSYS;

    TH0 = TH0_INIT;
    TL0 = TL0_INIT;
    
    ENABLE_TIMER0_INTERRUPT;                       //enable Timer0 interrupt
    ENABLE_GLOBAL_INTERRUPT;                       //enable interrupts
  
    set_TCON_TR0;     
///////////////////////////////////////////

}

void charging_pwm_off()
{
  PWM3_P00_OUTPUT_DISABLE; 
  PWM2_P10_OUTPUT_DISABLE;
}
