
#include "app_battery_charger.h"
#include "MS51_16K.h"
#define ADC_BATTERY_VOLTAGE_CHANNEL 5
#define ADC_PV_VOLTAGE_CHANNEL 6
#define ADC_PV_CURRENT_CHANNEL 1
#define ADC_BATTERY_CURRENT_CHANNEL

/** charge controller state machine */
enum battery_charging_state_e _cc_state;
/** Variable */
uint16_t battery_voltage;
uint16_t pv_voltage;
uint16_t battery_current;
uint16_t pv_current;

void app_battery_charger_init(void)
{
}
void app_battery_charger_eventloop(void)
{
    //Check execution interval
    
    //Get all the parameter needed 

    switch (_cc_state)
    {
    case CC_IDLE:
        break;
    case CC_IDLE_START:
        break;
    case CC_CALIBRATION:
        break;
    case CC_PARAM_CHECK:
        break;
    case CC_CONSTANT_CURRENT:
        break;
    case CC_CCONSTANT_VOLTAGE:
        break;
    case CC_IDLE_STOP:
        break;
    case CC_ERROR:
        break;
    }
}