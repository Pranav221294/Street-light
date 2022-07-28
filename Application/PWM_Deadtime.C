/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2020 nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************/
/* Website: http://www.nuvoton.com                                                                         */
/*  E-Mail : MicroC-8bit@nuvoton.com                                                                       */
/*  Date   : June/21/2020                                                                                   */
/***********************************************************************************************************/

/************************************************************************************************************/
/*  File Function: MS51 DEMO project                                                                        */
/************************************************************************************************************/

#include "MS51_16K.h"
#include "adc_2.h"
#include "pwm_2.h"
#include "mppt_track_1.h"
#include "condition.h"
///////Variable declaration///////////



unsigned int  SPV_VOLT11 = 0,pwm1=0;
xdata int adc_read_delay=0,print_delay = 0;

bit mppt_f;

xdata char BAT[10],SPV[10],LODV[10],Temp[10],Chg_c[10],disChg_c[10];
char is_pwm_on;
////////////////////////////////////
extern float SPV_VOLT,BAT_VOLT,Load_Voltage,CHG_CRT,DCHG_CRT;
extern unsigned int Temperature;
extern chgg,dischgg;
//extern MPPT_Track_State;
///////////////////////////////////////////////////
void printing_param();

//////////////////////////////////////////////////











void main (void) 
{
	
P14_PUSHPULL_MODE ;
	pwm_init();
  ADC_init();
////////Rx Tx/////////////////////	
	P06_QUASI_MODE;
	P07_QUASI_MODE;
	
    MODIFY_HIRC(HIRC_24);
    UART_Open(24000000,UART0_Timer3,115200);
    
	  ENABLE_UART0_PRINTF;
/////////////////////////////////////	  
 
 ///////////////////////////////////////  
  while(1)
	{
  
		//P14 =~P14;
		// adc_read_delay++;
	//if(adc_read_delay>90)
	//  {
		   Calulation_using_ADC();
		  
		solar_charging_on_condition();
//	   adc_read_delay =0;
//		solar_fault();
	//	}
		
	  	
	 
	 
    print_delay++;
   if(print_delay>1000)
	 {
		 print_delay =0;
	   printing_param();
	 
	 }

 }

}


void printing_param()
{
  
     sprintf(BAT,"B_V:%0.2f ",BAT_VOLT);
		 printf(BAT);
		 sprintf(SPV,"S_V:%0.2f ",SPV_VOLT);
		 printf(SPV);
		 sprintf(LODV,"L_V:%0.2f ",Load_Voltage);
		 printf(LODV);
		 sprintf(Temp,"Temp:%d ",Temperature);
		 printf(Temp);
		 sprintf(Chg_c,"CHG:%0.2f ",CHG_CRT); 
		 printf(Chg_c);
		 sprintf(disChg_c,"DCHG:%0.2f\n",DCHG_CRT);
		 printf(disChg_c);


}