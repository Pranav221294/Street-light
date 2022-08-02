#ifndef _BATTERY_CHARGER_H_
#define _BATTERY_CHARGER_H_
#define CC_EXEC_INTERVAL   50//every 50 millis
enum battery_charging_state_e
{
    CC_IDLE=0,
    CC_IDLE_START,
    CC_CALIBRATION,
    CC_PARAM_CHECK,
    CC_CONSTANT_CURRENT,
    CC_CCONSTANT_VOLTAGE,
    CC_IDLE_STOP,
    CC_ERROR
};
void app_battery_charger_init(void);
void app_battery_charger_eventloop(void);
#endif