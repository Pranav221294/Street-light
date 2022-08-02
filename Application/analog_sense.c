#include "analog_sense.h"
#include "MS51_16K.h"
#include "adc_read.h"
enum analog_sense_e
{
    ANALOG_SENSE_BATTERY_VOLTAGE = 0,
    ANALOG_SENSE_PV_VOLTAGE,
    ANALOG_SENSE_BATT_CURRENT,
    ANALOG_SENSE_PV_CURRENT,
    ANALOG_SENSE_SENTIMEL
};
/* information related to the analog sensing*/
struct analog_sense_meta
{
    int channel;
    uint16_t avg_value;
    uint16_t *raw_buff;
    int raw_buff_size;
    int index;
};

/* static raw buffer for analog sense*/
static uint16_t _bv_buff[ANALOG_SENSE_BATTERY_VOLTAGE_BUFF_SIZE];
static uint16_t _pv_buff[ANALOG_SENSE_PV_VOLTAGE_BUFF_SIZE];
static uint16_t _bc_buff[ANALOG_SENSE_BATT_CURRENT_BUFF_SIZE];
static uint16_t _pvc_buff[ANALOG_SENSE_PV_CURRRNT_BUFF_SIZE];
/*analog sensing meta array*/
struct analog_sense_meta _as_meta[ANALOG_SENSE_SENTIMEL] =
    {
        [ANALOG_SENSE_BATTERY_VOLTAGE] = {
            ANALOG_SENSE_BATTERY_VOLTAGE_CHANNEL, 0, _bv_buff, ANALOG_SENSE_BATTERY_VOLTAGE_BUFF_SIZE, 0},
        [ANALOG_SENSE_PV_VOLTAGE] = {ANALOG_SENSE_PV_VOLTAGE_CHANNEL, 0, _pv_buff, ANALOG_SENSE_PV_VOLTAGE_BUFF_SIZE, 0},
        [ANALOG_SENSE_BATT_CURRENT] = {ANALOG_SENSE_BATT_CURRENT_CHANNEL, 0, _bc_buff, ANALOG_SENSE_BATT_CURRENT_BUFF_SIZE, 0},
        [ANALOG_SENSE_PV_CURRENT] = {ANALOG_SENSE_PV_CURRRNT_CHANNEL, 0, _pvc_buff, ANALOG_SENSE_PV_CURRRNT_BUFF_SIZE, 0}};

/*Sensing index*/
static int _analog_sense_index;
int analog_sense_get_battery_voltage(void)
{
    return _as_meta[ANALOG_SENSE_BATTERY_VOLTAGE].avg_value;
}
int analog_sense_get_pv_voltage(void)
{
    return _as_meta[ANALOG_SENSE_PV_VOLTAGE].avg_value;
}
int analog_sense_get_pv_current(void)
{
    return _as_meta[ANALOG_SENSE_BATT_CURRENT].avg_value;
}
int analog_sense_get_battery_current(void)
{
    return _as_meta[ANALOG_SENSE_PV_CURRENT].avg_value;
}
void analog_sense_event_loop(void)
{
    int i = 0;
    int raw_buff_index = 0;
    int adc_channel = 1;
    uint32_t temp_sum = 0;
    // Get raw buff index
    raw_buff_index = _as_meta[_analog_sense_index].index;
    // Get adc channel
    adc_channel = _as_meta[_analog_sense_index].channel;
    // Get the raw value from adc
    _as_meta[_analog_sense_index].raw_buff[raw_buff_index] = adc_read(adc_channel);
    // Calculate average
    for (i = 0; i < _as_meta[_analog_sense_index].raw_buff_size; i++)
    {
        temp_sum += _as_meta[_analog_sense_index].raw_buff[i];
    }
    _as_meta[_analog_sense_index].avg_value = temp_sum / _as_meta[_analog_sense_index].raw_buff_size;
    // increment rw buff index
    raw_buff_index += 1;
    if (raw_buff_index > _as_meta[_analog_sense_index].raw_buff_size - 1)
    {
        _as_meta[_analog_sense_index].index = 0;
    }
    else
    {
        _as_meta[_analog_sense_index].index = raw_buff_index;
    }

    // increment index of analog sense
    _analog_sense_index += 1;
    if (_analog_sense_index > ANALOG_SENSE_SENTIMEL - 1)
    {
        _analog_sense_index = 0;
    }
}