#include "MS51_16K.h"
#include "adc_2.h"
#include "pwm_2.h"
#include "mppt_track_1.h"


#define Voltage_Factor  0.00544
#define Battery_Factor  0.00540
#define Current_Factor_CHG  0.12 // factor charging current
#define Current_Factor_DCHG  1.55 //factor discharging current
#define Load_Voltage_Factor  0.00887636      // VOLTAGE FECTOR PER ADC VALUE = 0.00887636
#define MeasurementsToAverage 20
#define battery_offset 0.20
#define Solar_offset  0.00
//////////////Variable declaration///////////////
int timeout_count =0,i=0;
float AverageADC_CHG=0.00,AverageADC_DCHG=0.00;
/////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
float SPV_VOLT = 0.00,BAT_VOLT=0.00,Load_Voltage=0.00,CHG_CRT=0.00,DCHG_CRT = 0.00;
unsigned int Temperature = 0;
int chgg =0,dischgg =0;
float BAT_PWR = 0.00,SPV_CRT = 0.00,SPV_PWR =0.00,SSL_PWR=0.00;//SSL = Solar Street Light
float  Battery_State_Of_Charge = 0.00,Battery_Depth_Of_Discharge = 0.00;

/////////////////////////////////////////////////////////////////////////////
void ADC_init()
{
 //////////ADC channel//////////
  P03_INPUT_MODE; 	//SPV_pin
	P04_INPUT_MODE; 	//Battery Pin
	P11_INPUT_MODE; 	//Load Pin
	P05_INPUT_MODE; 	//Temp Pin
	P30_INPUT_MODE; 	//Charging Pin
	P17_INPUT_MODE; 	//Discharging Pin

	ENABLE_ADC_CH4; //Temp
	ENABLE_ADC_CH5; //Battery Voltage
	ENABLE_ADC_CH6; //Solar Voltage
	ENABLE_ADC_CH7; //Load Voltage
	ENABLE_ADC_CH0; //Discharging Pin
	ENABLE_ADC_CH1; //Charging Pin
///////////////////////////////	

 

}



///////////////////main ADC single channel selection read//////// 
unsigned int ADC_read(unsigned char Channel)
{	
  register unsigned int value = 0x0000;
  (ADCCON0)&=~(0xf);//Flush the chanel select
	 ADCCON0|=(Channel);
//	 ADCCON1|=0X30;            /* clock divider */
//   ADCCON2|=0X0E;            /* AQT time */
//   AUXR1|=SET_BIT4;          /* ADC clock low speed */
	 clr_ADCCON0_ADCF;
   set_ADCCON0_ADCS;
	 while(ADCF == 0);
//		 while(ADCF == 0)
//	{
//		if(timeout_count++>2)
//		{
//			//P13=0;
//			return;
//		}
//	//	nop;
//	//	nop;
//	} 
	 
  value = ADCRH;
  value <<= 4;
  value |= ADCRL;
//  DISABLE_ADC;
  return value;
}

/////////////////////Average Current Value of Charging and Discharging Current////////////////////
void Average_Charging_Value (void)
{
	chgg  = ADC_read(1);
	 for( i = 0; i < MeasurementsToAverage; ++i)
  {
    AverageADC_CHG += ADC_read(1);
		AverageADC_DCHG += ADC_read(0);
		
  }
  AverageADC_CHG /= MeasurementsToAverage;
	AverageADC_DCHG /= MeasurementsToAverage;
	
	CHG_CRT = (Current_Factor_CHG * AverageADC_CHG);
  DCHG_CRT = ((Current_Factor_DCHG * AverageADC_DCHG)/1000);             		
}

float Average_Current_Value (unsigned char Channel)
{
	 for( i = 0; i < MeasurementsToAverage; ++i)
  {
    AverageADC_CHG += ADC_read(Channel);
	
  }
  AverageADC_CHG /= MeasurementsToAverage;

   return AverageADC_CHG; 
  
}
////////////////////////ADC different channel//////////////////////////////////////////////////
void multiple_channel_ADC_read()
{
	Temperature =Average_Current_Value(4);//Temperature
	AverageADC_CHG =0;
  BAT_VOLT = (Average_Current_Value(5)*Battery_Factor)-0.20;//Battery Voltage
  AverageADC_CHG =0;
	SPV_VOLT = Average_Current_Value(6)*Voltage_Factor;//Solar Panel
	AverageADC_CHG =0;
	Load_Voltage =Average_Current_Value(7)*Load_Voltage_Factor;//Load Voltage
	//Average_Charging_Value ();
  //Temperature =Average_Current_Value(4);
	
	CHG_CRT = (Current_Factor_CHG * Average_Current_Value (1));
  AverageADC_CHG =0; 
	DCHG_CRT = ((Current_Factor_DCHG * Average_Current_Value (0))/1000); 
  AverageADC_CHG =0;
}
////////////////////////////////////Calulation using ADC/////////////
void Calulation_using_ADC()
{
 
	multiple_channel_ADC_read();

  BAT_PWR = BAT_VOLT * CHG_CRT ;// Battery Power
	SPV_CRT = ((BAT_PWR / SPV_VOLT) * 1.1);//Solar Current
	SPV_PWR = SPV_VOLT * SPV_CRT ;//Solar Power
	SSL_PWR = Load_Voltage * DCHG_CRT ;//Street light power
  Battery_State_Of_Charge = ((( BAT_VOLT - 12.2 ) / 2.3 ) * 100 ); //(((aval bat volt-bat low volt)/full bat v-low battry volt)*100)
	Battery_Depth_Of_Discharge = (( BAT_VOLT / 14.5 ) * 100 );

}