#include "MS51_16K.h"
#include "adc_2.h"
#include "pwm_2.h"
#include "mppt_track_1.h"

#define FULL_BATT 1450

xdata float CSP=0.00,VOLT_DIFF=0.00,CHG_CTRL_FACTOR=0.00;
extern float SPV_PWR,BAT_VOLT;
xdata int PWM = 50,PWM_DIFF=0;
xdata int MPPT_RETRY=0;
xdata int AVL_BATT = 0;
xdata long unsigned int mppt_delay = 0; 
xdata 	char PWM_1[16]={0},PWR_1[16]={0},CSP_1[16]={0};
extern float SPV_PWR,BAT_VOLT,CHG_CRT,SPV_VOLT;
//enum { START = 0, CHECK = 1, FIX = 2, RETRY_PLUS = 3, RETRY_MINUS = 4 }
//MPPT_Track_State = START ;
//enum  { SLOW_START = 0,START = 1, CHECK = 2, FIX = 3, RETRY_PLUS = 4, RETRY_MINUS = 5}
//MPPT_Track_State = SLOW_START ;
//extern enum MPPT_Track_State mppt_state1;

//enum MPPT_Track_State;
MPPT_Track_State mppt_state = START;

//extern unsigned long  int systick();
//enum { SLOW_START = 0,START = 1, CHECK = 2, FIX = 3, RETRY_PLUS = 4, RETRY_MINUS = 5}
//MPPT_Track_State = SLOW_START ;
//////////////Mppt Track//////////
void mppt_track()//Battery b/w 12.20 to 13.75 with minimum current
{

if(systick()- mppt_delay >= 100)
{ 
switch (  mppt_state )	
      {	
				
				case START:
        {
					printf("START\n");
					CSP = 0;
					PWM = 72;
					PWM++;
					set_PWMCON0_PWMRUN;
					set_PWM2(PWM);			   
					mppt_state = CHECK;
				}
				break;
        case CHECK:
				{
				
						//printf("Check1");
					if ( SPV_PWR > CSP )
					{
//						printf("power up\n");
//						sprintf(PWR_1,"SPV_PWR:%0.2f  ",SPV_PWR);
//			      printf(PWR_1);
			
//		    	  sprintf(CSP_1,"CSP_PWR:%0.2f ",CSP);
//		    	  printf(CSP_1);				
						CSP = 0;
						CSP = SPV_PWR;
						PWM++;
					  set_PWMCON0_PWMRUN;
						set_PWM2(PWM);
			      sprintf(PWM_1,"PWM:%d \n ",PWM);
		        printf(PWM_1);
						
						
						mppt_state = CHECK;
					}
				  if ( SPV_PWR < CSP )
					{
						
					//	printf("Check2\n");
//						printf("Power down\n  ");
//						sprintf(PWR_1,"SPV_PWR:%0.2f  ",SPV_PWR);
//			      printf(PWR_1);
			
//		    	  sprintf(CSP_1,"CSP_PWR:%0.2f ",CSP);
//		    	  printf(CSP_1);
						
						
						CSP = SPV_PWR;
						PWM--;
						set_PWM2(PWM);
			      set_PWMCON0_PWMRUN;
//						sprintf(PWM_1,"PWM:%d \n ",PWM);
//		        printf(PWM_1);
						mppt_state = RETRY_PLUS;						
					}
				}
				break;
				case FIX:
				{
           set_PWM2(PWM);
			      set_PWMCON0_PWMRUN;
					//printf("Fix\n");
					CSP = SPV_PWR;
//	          sprintf(PWM_1,"PWM:%d \n ",PWM);
//		        printf(PWM_1);
          MPPT_RETRY++;
				
					if ( MPPT_RETRY > 500 )
					{
					//	printf("Fix\n");
						
						  CSP = 0;
							CSP = SPV_PWR;
						  PWM--;
						 set_PWM2(PWM);
			       set_PWMCON0_PWMRUN;
						
						  mppt_state = RETRY_MINUS;
						  MPPT_RETRY = 0;
					}
				}
				break;
				case RETRY_PLUS:
				{
					//	printf("RETRY_PLUS\n");
					  if ( SPV_PWR > CSP )
					  {
//						 printf("RETRY_PLUS SPV_PWR > CSP ");
//						sprintf(PWR_1,"SPV_PWR:%0.2f  ",SPV_PWR);
//			      printf(PWR_1);
			
//		    	  sprintf(CSP_1,"CSP_PWR:%0.2f ",CSP);
//		    	  printf(CSP_1);
					
							
						  CSP = SPV_PWR;
						  PWM--;
						//	printf("RETRY_PLUS1\n");
							
							  set_PWM2(PWM);
			        set_PWMCON0_PWMRUN;
							
//							sprintf(PWM_1,"PWM:%d \n ",PWM);
//		          printf(PWM_1);
						  mppt_state = RETRY_PLUS;
					  }
				   if ( SPV_PWR < CSP )
					  {
//						printf("RETRY_PLUS SPV_PWR < CSP ");
//						sprintf(PWR_1,"SPV_PWR:%0.2f  ",SPV_PWR);
//			      printf(PWR_1);
//			
//		    	  sprintf(CSP_1,"CSP_PWR:%0.2f ",CSP);
//		    	  printf(CSP_1);
					 
						  
						  CSP = SPV_PWR;
						  PWM++;
//							sprintf(PWM_1,"PWM:%d \n ",PWM);
//		          printf(PWM_1);
							
						set_PWM2(PWM);
			        set_PWMCON0_PWMRUN;
						  
							mppt_state = FIX;
             							
					  }
				}
				break;
				case RETRY_MINUS:
				{
					  if ( SPV_PWR > CSP )
					  {
						  CSP = SPV_PWR;
						  PWM--;
							printf("RETRY_Minus SPV_PWR > CSP  ");
							sprintf(PWR_1,"SPV_PWR:%0.2f  ",SPV_PWR);
			       printf(PWR_1);
//			
//		    	  sprintf(CSP_1,"CSP_PWR:%0.2f ",CSP);
//		    	  printf(CSP_1);
							
							
  						set_PWM2(PWM);
		         set_PWMCON0_PWMRUN;
//							sprintf(PWM_1,"PWM:%d \n ",PWM);
//		          printf(PWM_1);
						  
							mppt_state = RETRY_MINUS;
					  }
				   if ( SPV_PWR < CSP )
					  {
							
							
						CSP = SPV_PWR;
						  PWM++;		
//							printf("RETRY_Minus SPV_PWR < CSP ");
//							sprintf(PWR_1,"SPV_PWR:%0.2f  ",SPV_PWR);
//			       printf(PWR_1);
			
//		    	  sprintf(CSP_1,"CSP_PWR:%0.2f ",CSP);
//		    	  printf(CSP_1);
//							sprintf(PWM_1,"PWM:%d \n ",PWM);
//		          printf(PWM_1);
               set_PWM2(PWM);
			        set_PWMCON0_PWMRUN;							
						  mppt_state = RETRY_PLUS;					
					  }
					
				}
				break;
			}


  mppt_delay = systick();

}		
//			sprintf(PWR_1,"SPV_PWR:%0.2f ",SPV_PWR);
//			printf(PWR_1);
//			
//			sprintf(CSP_1,"CSP_PWR:%0.2f \n",CSP);
//			printf(CSP_1);
//       set_PWM2(PWM);
//			
//			set_PWMCON0_LOAD;
//      set_PWMCON0_PWMRUN;
			//sprintf(PWM_1,"PWM:%d \n ",PWM);
		 // printf(PWM_1);
//			PWM_DIFF = PWM - 60;
//      VOLT_DIFF = (FULL_BATT-(BAT_VOLT*100))/10 ;
//      CHG_CTRL_FACTOR = PWM_DIFF / VOLT_DIFF ;		
//   			
		}

/////////Battery upto 95/////////////
void Battery_upto_95()//Battery b/w 13.75 to 14.50 with minimum current 
{

	  AVL_BATT = BAT_VOLT * 100;
	  PWM =((FULL_BATT - AVL_BATT)/10)*CHG_CTRL_FACTOR;
		PWM = 60 + PWM ;
		PWM++;
		PWM++;

	
	
    set_PWM2(PWM);
		set_PWMCON0_LOAD;
    set_PWMCON0_PWMRUN;

}

///////////////////////
void init()
{
enum { SLOW_START = 0,START = 1, CHECK = 2, FIX = 3, RETRY_PLUS = 4, RETRY_MINUS = 5}
MPPT_Track_State = SLOW_START ;
}
	
	