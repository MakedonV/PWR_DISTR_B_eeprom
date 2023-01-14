#ifndef SRC_USER_API_PWM_H_
#define SRC_USER_API_PWM_H_
/*----------------------------------------------------------------------------*/
/**
* \file         user_api_pwm.h
* \brief        Declaration of PWM functionality
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup     user_api_pwm PWM
* \{
* \brief        This handles the configuration of the PWM signal.
* \details      The IO pins must be configured for PWM functionality.
*/
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include "hal_pwm.h"
#include "io_tables.h"

typedef struct
{
    uint32_t current_value;
    uint32_t duty_out_before;
    uint32_t duty_out_after;
} pi_regulator_info_t;

typedef struct
{
    uint32_t current_value;
    uint32_t flag0;
    uint32_t flag1;
} pi_regulator_data_t;

#if CONTROLLER_WITH_PRECONTROL
typedef struct {
  uint8_t pre_control;
  uint8_t cycles;
  uint8_t controller_status;
  uint16_t old_soll;
  uint16_t  snapshot_current;
  int16_t difference_current;
  uint16_t max_current_slope;
  uint16_t last_current_slope;
  uint16_t old_current_value;
  uint32_t lower_border;
  uint16_t duty_adaption;
}pi_controller_t;
#endif

/*----------------------------------------------------------------------------*/
/**
* \brief    Set the PWM output: frequency and duty cycle synchronously
* \details  It is preferred to use the separate functions user_pwm_set_duty() and user_pwm_set_freq()
*           This will update the frequency and duty cycle once a FTM cycle is finished.
*           An FTM cycle lasts as long as a period=1/frequency.
*           So e.g. for 500 Hz it can take 2 ms for the new values to be set.
*
*           The PWM frequency is shared between channels on a single FlexTimer Module (FTM)
*           On CC16WP the following assignment is used:
*
*           FTM0:  PWM_HSD_OUT1, PWM_HSD_OUT3, PWM_HSD_OUT7
*           FTM1:  PWM_HSD_OUT6
*           FTM2:  PWM_HSD_OUT0, PWM_HSD_OUT2, PWM_HSD_OUT4, PWM_HSD_OUT6
*
* \param    pin     [in] uint16_t       From io_tables.h in struct_ftm_config_tbl (PWM_HSDx_OUTx)
* \param    freq    [in] uint16_t       Value of the frequency to set
* \param    duty    [in] uint16_t       Value of the duty cycle to set
*
* \return   uint32_t   Return code
*/
uint32_t user_pwm_set_pwm(uint16_t pin, uint16_t freq, uint16_t duty);
#define user_set_pwm( pin, freq, duty ) user_pwm_set_pwm( pin, freq, duty )


/*----------------------------------------------------------------------------*/
/**
* \brief    Set the PWM output: frequency and duty cycle immediately
* \details  This will immediately update the frequency and duty cycle.
*           BE AWARE: the immediate update can lead to frequency artifacts.
*           It should only be used if you know what you are doing.
*
*           The PWM frequency can only be set once per FlexTimer Module (FTM).
*           FTM units have multiple channels. On CC16WP the following assignment is used:
*
*           FTM0  PWM_HSD_OUT1, PWM_HSD_OUT3, PWM_HSD_OUT7
*           FTM1  PWM_HSD_OUT6
*           FTM2  PWM_HSD_OUT0, PWM_HSD_OUT2, PWM_HSD_OUT4, PWM_HSD_OUT6
*
* \param    pin     [in] uint16_t       From io_tables.h in struct_ftm_config_tbl (PWM_HSDx_OUTx)
* \param    freq    [in] uint16_t       frequency to be set in Hz
* \param    duty    [in] uint16_t       duty cycle to be set in 0.1%
*
* \return   uint32_t   Return code
*/
uint32_t user_pwm_set_pwm_immediate(uint16_t pin, uint16_t freq, uint16_t duty);


/*----------------------------------------------------------------------------*/
/**
* \brief    Set the duty cycle of PWM signal
*
* \param    pin     [in] uint16_t       From io_tables.h in struct_ftm_config_tbl (PWM_HSDx_OUTx)
* \param    duty    [in] uint16_t       Value of the duty cycle to set
*
* \return   uint32_t   Return code
*/
uint32_t user_pwm_set_duty(uint16_t pin, uint16_t duty);
#define user_set_pwm_duty( pin, duty ) user_pwm_set_duty( pin, duty )


/*----------------------------------------------------------------------------*/
/**
* \brief    Forces a PWM channel to behave similar to a digital output (via software output control).
* \details  This function will force the PWM output to 100% or 0% almost instantly (On every rising edge of the FTM clock).
*           DISCLAIMER: This function should mainly be used to operate the PWM pins as digital output.
*           Using it as a regular PWM output may lead to undefined behavior.
*
* \param    pin                    [in] uint16_t               From io_tables.h in struct_ftm_config_tbl (PWM_HSDx_OUTx)
* \param    forced_output_value    [in] uint16_t               force Value of the duty cycle to be set, currently only values of 1000 and 0 are supported
*
* \return   uint32_t   Return code
*/
uint32_t user_pwm_output_force(uint16_t pin, uint16_t forced_output_value);


/*----------------------------------------------------------------------------*/
/**
* \brief    Function to set the duty cycle of a PWM pin with a dither
* \details  This function will set the PWM pin to the given duty cycle "duty" with the frequency
* 			"freq". A dither will be added onto the signal oscillating with (freq / dither_freq)
* 			around the duty cycle.
* 			For the first half of the period ( (freq / dither_freq) / 2 ) it will be duty - dither_duty,
* 			and for the second half it will be duty + dither duty
*
* 			For example:
*
* 			user_set_pwm_dither(PWM_HSD2_OUT4, 500, 1000, 400, 20);
*
* 			duty_cycle = 500 promille
* 			frequency = 1000 Hz
* 			dither duty_cycle = 400 promille
* 			dither frequency = 20 Hz
*
* 			The dither will be oscillating around the PWM signal for (freq / dither_freq) = 1000/20 = 50
* 			periods. This means that for 25 periods the duty cycle of the PWM signal will be duty + dither_duty
* 			= 900 promille and for the second 25 periods it will be inverted to duty - dither_duty = 100 promille
*
* \pre
*
* \param    pin         [in] uint16_t	PWM pin name from struct_FTM_CONFIG inside io_tables.c
* \param    duty        [in] uint16_t	Duty cycle of the PWM signal
* \param    freq        [in] uint16_t	Frequency of the PWM signal
* \param    dither_duty [in] uint16_t	Duty Cycle of the dither, will be periodically added/subtracted from the duty cycle
* \param    dither_freq [in] uint16_t	Frequency of the dither from which the periods of the dither will be calculated
* \return   uint32_t    Return code
*/
uint32_t user_pwm_set_dither(uint16_t pin, uint16_t duty, uint16_t freq, uint16_t dither_duty, uint16_t dither_freq);


/*----------------------------------------------------------------------------*/
/**
* \brief    Set the frequency for all PWM outputs
*
* \param    freq    [in] uint16_t       Value of the frequency to set
*
* \return   uint32_t   Return code
*/
uint32_t  user_pwm_set_global_freq(uint16_t freq);
#define user_set_pwm_freq_global( freq ) user_pwm_set_global_freq( freq )

/*----------------------------------------------------------------------------*/
/**
* \brief    Set the frequency of PWM signal.
*
* \param    pin     [in] uint16_t       From io_tables.h in struct_ftm_config_tbl (PWM_HSDx_OUTx)
* \param    freq    [in] uint16_t       Value of the frequency to set
*
* \return   uint32_t   Return code
*/
uint32_t user_pwm_set_freq(uint16_t pin, uint16_t freq);
#define user_set_pwm_freq( pin, freq ) user_pwm_set_freq( pin, freq )

/*----------------------------------------------------------------------------*/
/**
* \brief    Read the set frequency at the specific frequency input pin
*
* \param    pin       [in] uint16_t     From io_tables.h in g_pin_mux_InitConfigArr (MC_AI_A_INx)
*
* \return   uint32_t     Pointer to the variable, to save the read value
*/
uint32_t user_freq_get_measured_freq(uint16_t pin);
#define user_get_measured_freq( pin ) user_freq_get_measured_freq( pin )

/*----------------------------------------------------------------------------*/
/**
* \brief    Read the set duty at the specific frequency input pin
*
* \param    pin       [in] uint16_t     From io_tables.h in g_pin_mux_InitConfigArr (MC_AI_A_INx)
*
* \return   uint32_t     Pointer to the variable, to save the read value
*/
uint32_t user_freq_get_measured_duty(uint16_t pin);
#define user_get_measured_duty( pin ) user_freq_get_measured_duty( pin )



// user_set_pwm_freq must be set beforehand
/*----------------------------------------------------------------------------*/
/**
* \brief
* \details
* The API function which is between the user and the PI controller. This calls a simple discrete PI controller with the gain factors
* KP and KI and sets the output value automatic to the desired level. The actual value of the output current is hand over to this function
* internal.
*
* \pre
*
* \param    pin        [in] enum_pwm_pin_name
* \param    set_value  [in] uint32_t
* \param    kp         [in] uint32_t
* \param    ki         [in] uint32_t
* \return   uint32_t
*/
uint32_t user_pwm_current_control(enum_pwm_pin_name pin, uint32_t set_value, uint32_t kp, uint32_t ki);

/*----------------------------------------------------------------------------*/
/**
* \brief
* \details
* internal function of the PI controller that is called by the function "user_pwm_current_control"
*
* \pre
*
* \param    pin    [in] enum_pwm_pin_name
* \param    soll   [in] uint32_t
* \param    ist    [in] uint32_t
* \param    kp     [in] uint32_t
* \param    ki     [in] uint32_t
* \param    minOut [in] uint32_t
* \param    maxOut [in] uint32_t
* \param    *out   [in] uint32_t
* \param    *flag0 [in] uint32_t
* \param    *flag1 [in] uint32_t
*
*/
void PI_Controller_Discrete(enum_pwm_pin_name pin, uint32_t soll, uint32_t ist, uint32_t kp, uint32_t ki, uint32_t minOut, uint32_t maxOut, uint32_t *out, uint32_t *flag0, uint32_t *flag1);

//function under construciton
void PI_Controller_Discrete_with_precontrol(enum_pwm_pin_name pin, uint32_t soll, uint32_t ist, uint32_t kp, uint32_t ki, uint32_t minOut, uint32_t maxOut, uint32_t *out, uint32_t *flag0, uint32_t *flag1);



#endif /* SRC_USER_API_PWM_H_ */
/** \} */
