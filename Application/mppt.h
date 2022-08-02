//this is a hardware independent mppt tracker 
//takes only dev_pwmctrl module as its dependecy
#ifndef _MPPT_TRACKER_H_
#define _MPPT_TRACKER_H_
#include "pi_regulator.h"
//#define CONFIG_USE_PI
#define CONFIG_USE_INCR
#define CONFIG_CV_MODE_VMPP_OF_VOC           80
#define CONFIG_CV_MODE_VPP_TOLARENCE         0.05



#define MPPT_TRACK_PAUSE_ERROR -1

#define OBSERVATION_TIMEOUT         10000   //2000ms
#define IP_CURRENT_REF_TOLARENCE    2    //10 %
#define MPPT_MAX_DUTY_DEFAUT        90//90 %
#define MPPT_MIN_DUTY_DEFAUT           0 //0%
#define MPPT_INCR_CUTY_DEFAULT         0.25 //0.25%
#define DIR_CHECK_MAX_COUNT            2
/** mppt tracking mode*/
enum mppt_track_mode
{
    MPPT_MODE_CV=0,
    MPPT_MODE_ICM
};
/** mppt tarcking state */
enum mppt_track_state
{
    MPPT_TRACK_PERTURB=0,
    MPPT_TRACK_OBSERVATION,
    MPPT_TRACK_FIX
};
/** Tracking Direction on Current Curve **/
enum track_dir
{
    MPPT_DIR_UP=0,
    MPPT_DIR_DOWN=1
};

struct mppt_config
{
unsigned int  step_current;
unsigned int  max_current_limit;
unsigned int  min_current_limit;
unsigned int  least_power_diff;
unsigned int  fix_to_perturb_voltage;
unsigned int  fix_to_perturb_power;
int  max_pwm_duty;
int min_pwm_duty;
#ifdef CONFIG_USE_INCR
float incr_duty_cycle;
#endif
PI_Float_Handle_t *current_pi;
};
int mppt_tracker_init(struct mppt_config* config);
void mppt_tracker_start(unsigned int voc,unsigned long mill);
void mppt_tracker_run(unsigned long current_time_ms);
void mppt_tracker_feed_param(unsigned int  ip_voltage,unsigned int ip_current);
void mppt_tracker_pause(void);
void mppt_tracker_resume(unsigned int current_ref,float duty);
int mppt_tracker_is_paused(void);
void mppt_tracker_internal_param_clear(void);
#endif

