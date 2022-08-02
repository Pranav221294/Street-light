#ifndef _ANALOG_SENSE_H_
#define _ANALOG_SENSE_H_
#define ANALOG_SENSE_BATTERY_VOLTAGE_CHANNEL 5
#define ANALOG_SENSE_PV_VOLTAGE_CHANNEL 6
#define ANALOG_SENSE_BATT_CURRENT_CHANNEL 0
#define ANALOG_SENSE_PV_CURRRNT_CHANNEL 1
#define ANALOG_SENSE_BATTERY_VOLTAGE_BUFF_SIZE 10
#define ANALOG_SENSE_PV_VOLTAGE_BUFF_SIZE 10
#define ANALOG_SENSE_BATT_CURRENT_BUFF_SIZE 10
#define ANALOG_SENSE_PV_CURRRNT_BUFF_SIZE 10
int analog_sense_get_battery_voltage(void);
int analog_sense_get_pv_voltage(void);
int analog_sense_get_pv_current(void);
int analog_sense_get_battery_current(void);
void analog_sense_event_loop(void);
#endif