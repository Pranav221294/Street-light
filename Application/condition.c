#include "MS51_16K.h"
#include "adc_2.h"
#include "pwm_2.h"
#include "mppt_track_1.h"
#include "condition.h"

enum { CHARG_INIT = 0,BAT_CONDITION = 1}
CHARG_STATE = CHARG_INIT ;
extern float SPV_PWR,BAT_VOLT,CHG_CRT,SPV_VOLT;
bit pwm_on_off=0;
bit over_voc_f = 0,over_current_f=0;
//enum { START = 0, CHECK = 1, FIX = 2, RETRY_PLUS = 3, RETRY_MINUS = 4 }
//extern enum MPPT_Track_State ;
extern int PWM;
extern xdata long unsigned int mppt_delay;
extern xdata  unsigned long  int ms;
////////////////////////////////////////////////////////////////////////////////
void solar_charging_on_condition()
{
	//printf("C11");
  switch(CHARG_STATE)
		{
		  case CHARG_INIT:
		  {
				if(SPV_VOLT>17&&(pwm_on_off==1))
				{
					printf("A\n");
					charging_side_initilzation();
					pwm_on_off = 0;
			    CHARG_STATE = BAT_CONDITION;
				}			
			}
				break;
		 
		case BAT_CONDITION:
		{	
						if(SPV_VOLT>17)//5T
						{//  printf("B1\n");
						 if(CHG_CRT<=8.00)
						 {					// printf("B2\n");
							 if(BAT_VOLT >= 12.20 && BAT_VOLT < 13.75)
							 {
                     //set_PWM2(72);
                 mppt_track();
						
							 }
						 if(BAT_VOLT >= 13.75 && BAT_VOLT < 14.50)
							 {
								 Battery_upto_95();
								// set_PWM2(65);
								// printf("B3\n");
						 
							 }
						 if(BAT_VOLT >=14.50)
							 {
								 charging_pwm_off();
							//	 printf("C\n");
							 }
							 
						CHARG_STATE = BAT_CONDITION;
						}
				 } 
				
		}	
	  	break;
	
	}


  if(SPV_VOLT<15.50&&(!pwm_on_off))// Charging of condition
	//if(SPV_VOLT<13.5) 
	{
	  pwm_on_off =1;
	  P00 = 0;
	  P10 = 0;
	  charging_pwm_off();printf("D\n");CHARG_STATE = CHARG_INIT ;
		DISABLE_TIMER0_INTERRUPT ;clr_TCON_TR0;
		mppt_delay = 0;
		ms = 0;
		 init();
		//	MPPT_Track_State = 0;
	}


}

/////////////////////////////////////////////////////////////////////////////////////
void solar_fault()
{

//  if(SPV_VOLT>22.00)
//	{
//	 over_voc_f =1;printf("E");
//	}
	
// if(CHG_CRT>8.00)
//	{
//	 over_current_f =1;printf("F\n");
//	}

//	if(over_voc_f||over_current_f)
//	{
//	 charging_pwm_off();
//	 over_voc_f =0;printf("G\n");
//	 over_current_f =0;
//		P00 = 0;
//	  P10 = 0;CHARG_STATE = CHARG_INIT ;
//	}
	

	
}
//////////////////////////////////////////////////////////////////////////////