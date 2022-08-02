#include "mppt.h"
#include "MS51_16K.h"
/** Some weak functions*/
void ll_mppt_set_duty(float duty) __attribute__((weak));
float ll_mppt_get_duty(void) __attribute__((weak));
/**Logical representation of any point on power curve*/
struct mpp_s
{
	uint16_t v;
	uint16_t i;
	uint32_t p;
};
/** real time parameter structure for mppt*/
struct mppt_param
{
	unsigned long now;
	unsigned long last_exec_time;
	unsigned long last_perturb_ts;
	enum mppt_track_mode mode;
	enum mppt_track_state state;
	enum track_dir dir;
	bool is_paused;
	uint16_t current_ref;
	uint8_t track_dir_check_count;
	float present_duty;
	struct mpp_s mpp;
	unsigned int voltage;
	unsigned int current;
	unsigned int power;
	unsigned int voc;
	unsigned int cv_vmpp;
};
/** Internal vaiables*/
struct mppt_param _params;
struct mppt_config *_config;
//#include "WAsys_logging.h"
extern uint8_t log_print(char *str);
/**
 *
 *
 */
static void logf(char *c, ...)
{
}
static void current_limit(uint16_t *value, int upper_limit, int lower_limit)
{
	// it will bound the value inside the upper and lower limit;
	if (*value > upper_limit)
	{
		*value = upper_limit;
	}
	if (*value <= lower_limit)
	{
		*value = lower_limit;
	}
}

/**
 * @brief is_in_tolarance_range(uint16_t src_value,uint16_t target_value,uint16_t tolarances)
 *
 */
static bool is_refference_current_in_range(uint16_t src_value, uint16_t target_value, uint16_t tolarances)
{
	uint16_t range_min = target_value - (target_value * (float)tolarances / 100);
	uint16_t range_max = target_value + (target_value * (float)tolarances / 100);
	if (src_value > range_min && src_value < range_max)
	{
		//		tolarance_count++;
		//		if(tolarance_count>50)
		//		{
		//			tolarance_count=0;
		//			return true;
		//		}
		return true;
	}

	return false;
}

/**
 * this function will store presents values as a reference of local maximum Power
 *
 */
static void set_local_max_point(void)
{
	_params.mpp.i = _params.current;
	_params.mpp.v = _params.voltage;
	_params.mpp.p = _params.mpp.i * _params.mpp.v;
}

int mppt_tracker_init(struct mppt_config *config)
{
	if (config == NULL)
	{
		return -1;
	}
	else
	{
		_config = config;
		_params.is_paused = true;
	}
	return 0;
}
void mppt_tracker_run(unsigned long current_time_ms)
{
	unsigned long now_ms = current_time_ms;
	int err_i = 0; // error with respect to refference current
	int del_v;
	int del_i;
	int del_p;
	unsigned int v;
	unsigned int i;
	unsigned int p;

	// check for valid config
	if (_config == NULL)
	{
		return;
	}
	// check if paused
	if (_params.is_paused)
	{
		return;
	}
	// Parameter
	i = _params.current;
	v = _params.voltage;
	p = i * v; // mppt_handle->ip_power;//Can be directly multiplied p=v*i

	uint16_t current_ref_error;
	uint16_t pwm_duty_ref;
	// Get Imput current diff
	err_i = (_params.current > _params.current_ref) ? (params.current - _params.current_ref) : (-1) * (_params.current_ref - params.current);
	del_v = (_params.voltage > _params.mpp.v) ? (_params.voltage - _params.mpp.v) : (-1) * (_params.mpp.v - _params.voltage);
	del_i = (_params.current > _params.mpp.i) ? (_params.current - _params.mpp.i) : (-1) * (_params.mpp.i - _params.current);
	del_p = (p > _params.mpp.p) ? (p - _params.mpp.p) : (-1) * (_params.mpp.p - p);

	// Execution time controll
	// if (now_ms > (mppt_handle->last_exec_time))
	// {
	// 	if (now_ms - (mppt_handle->last_exec_time) < (mppt_handle->mppt_ctrl_period))
	// 	{
	// 		return 0;
	// 	}
	// 	else
	// 	{
	// 		mppt_handle->last_exec_time = now_ms;
	// 		// Noting Much
	// 		// will continue to execute run code
	// 	}
	// }
	// else
	// {
	// 	return 0;
	// }
	// mppt state machine
	if (_params.mode == MPPT_MODE_CV)
	{
		if (is_refference_current_in_range(_params.current, _params.current_ref, IP_CURRENT_REF_TOLARENCE))
		{
			if (v > (_params.cv_vmpp + (_params.cv_vmpp * CONFIG_CV_MODE_VPP_TOLARENCE)))
			{
				_params.current_ref += _config->step_current;
				set_local_max_point();
			}
			else if (v < (_params.cv_vmpp - (_params.cv_vmpp * CONFIG_CV_MODE_VPP_TOLARENCE)))
			{
				_params.current_ref -= _config->step_current;
				set_local_max_point();
			}
			else
			{
				// Jump to icm mode
				set_local_max_point();
				_params.mode = MPPT_MODE_ICM;
				_params.state = MPPT_TRACK_FIX;
			}

			// check for slope dv/di
			(_params.voltage < _params.mpp.v)
			{
				if (del_v / del_i > 100)
				{
					// change the mode to icm
					set_local_max_point();
					_params.mode = MPPT_MODE_ICM;
					_params.state = MPPT_TRACK_FIX;
				}
			}
		}
	}
	else
	{
		// mppt icm state machine
		switch (_params.state)
		{
		case MPPT_TRACK_PERTURB:
		{
			// Setting Local Maximum Point
			set_local_max_point();
			logf("pp i:%d,v:%d\n", i, v);
			_params.current_ref = _params.current;
			// Setting Last Perturb ts
			_params.last_perturb_ts = now_ms;
			if (_params.dir == MPPT_DIR_UP)
			{
				_params.current_ref += _config->step_current;
				// logf("up trk i:%d\n", mppt_handle->current_ref);
			}
			else
			{
				_params.current_ref -= _config->step_current;
				// logf("down trk i:%d\n", mppt_handle->current_ref);
			}
			_params.state = MPPT_TRACK_OBSERVATION;
			break;
		}
		case MPPT_TRACK_OBSERVATION:
		{
			// Observation Logic
			if (now_ms > (_params.last_perturb_ts) && (now_ms - (_params.last_perturb_ts)) > OBSERVATION_TIMEOUT)
			{
				// timeout condition
			}
			else
			{
				if (is_refference_current_in_range(_params.current, _params.current_ref, IP_CURRENT_REF_TOLARENCE))
				{
					if (del_p / del_v == 0)
					{
						// reached maximum power
						set_local_max_point();
						_params.current_ref = _params.current;
						_params.state = MPPT_TRACK_FIX;
					}
					else if (del_p / del_v > 0)
					{
						// decrease duty cycle;
						_params.dir == MPPT_DIR_DOWN;
						_params.state = MPPT_TRACK_PERTURB;
					}
					else if (del_p / del_v < 0)
					{
						_params.dir == MPPT_DIR_UP;
						_params.state = MPPT_TRACK_PERTURB;
					}
				}
			}
			break;
		}
		case MPPT_TRACK_FIX:
		{

			// Checking Voltage deviation
			if (del_v > 0 && del_v > _config->fix_to_perturb_voltage)
			{
				_params.dir == MPPT_DIR_UP;
				_params.state = MPPT_TRACK_PERTURB;
			}
			else if (del_v < 0 && del_v < (-1) * _config->fix_to_perturb_voltage)
			{
				_params.dir == MPPT_DIR_DOWN;
				_params.state = MPPT_TRACK_PERTURB;
			}
			else
			{
				// NOthing
			}
			break;
		}
		}
	}
	// limit current
	current_limit(&_params.current_ref, _config->max_current_limit, _config->min_current_limit);
#ifdef CONFIG_USE_INCR
	// if (i > (mppt_handle->current_ref) + 30) // static histerisis
	// {
	// 	mppt_handle->present_duty -= mppt_handle->incr_duty_cycle;
	// }
	// else if (i < (mppt_handle->current_ref))
	// {
	// 	mppt_handle->present_duty += mppt_handle->incr_duty_cycle;
	// }
#endif
	_params.present_duty += PI_Float_Calc(_config->current_pi, (err_i));
	// Fixing Limit of Duty Cycle
	if (_params.present_duty >= _config->max_pwm_duty)
	{
		_params.present_duty = _config->max_pwm_duty;
	}
	if (_params.present_duty < _config->min_pwm_duty)
	{
		_params.present_duty = _config->min_pwm_duty;
	}

	// Setting Duty Cycle to drv_pwmctrl
	if (ll_mppt_get_duty() = _params.present_duty)
	{
		ll_mppt_set_duty(_params.present_duty);
	}
}
void mppt_tracker_start(unsigned int voc, unsigned long mill)
{
	_params.voc = voc;
	_params.mode = MPPT_MODE_CV;
	_params.cv_vmpp = voc * (float)CONFIG_CV_MODE_VMPP_OF_VOC / 100.0;
	_params.now = mill;
	_params.is_paused = false;
}
void mppt_tracker_feed_param(unsigned int ip_voltage, unsigned int ip_current)
{
	_params.current = ip_current;
	_params.voltage = ip_voltage;
	return 0;
}
void mppt_tracker_pause(void)
{
	_parems.is_paused = true;
}
void mppt_tracker_resume(unsigned int current_ref, float duty)
{
	_params.state = MPPT_TRACK_PERTURB;
	_params.dir = MPPT_DIR_UP;
	_params.current_ref = current_ref;
	_params.present_duty = duty;
	_params.is_paused = false;
}
int mppt_tracker_is_paused(void)
{
	return _params.is_paused;
}
// enum track_state mppt_tracker_get_state(mppt_tracker_t* phandle)
// {
//     return phandle->state;
// }
/**
 * @brief Clears all the realtime param of mppt
 *
 */
void mppt_tracker_internal_param_clear(void)
{
	_params.mpp.i = 0;
	_params.mpp.v = 0;
	_params.mpp.p = 0;
	_params.current_ref = _config->min_current_limit;
	_params.state = MPPT_TRACK_PERTURB;
	_params.dir = MPPT_DIR_UP;
	_params.is_paused = true;
}
