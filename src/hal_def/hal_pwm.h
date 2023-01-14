#ifndef HAL_PWM_H
#define HAL_PWM_H
/*----------------------------------------------------------------------------*/
/**
* \file         hal_pwm.h
* \brief        Definitions for hardware independent PWM functionality.
*/
/*----------------------------------------------------------------------------*/
/** 
* \addtogroup   doc_hal_pwm HAL PWM
* @{
* \brief        This documentation part describes the interface function definitions for a hardware-independent and abstracted PWM interface.
* \details      Main goal is to create a hardware-platform independent layer, that can be accessed from different HW-platforms.
*               Pulse Width Modulation (PWM) describes a digital output that is turned of and on in a given frequency. 
*               The time between turn on and turn off given as divisor (t_on / (ton + t_off)) is called dutycycle and its value is in percent or permill.        
*/
/*----------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "hal_data_types.h"

// ---------------------------------------------------------------------------------------------------
// defines
// ---------------------------------------------------------------------------------------------------
#define HAL_PWM_DUTY_MIN                           0u   ///< Minimum dutycycle for a PWM channel.
#define HAL_PWM_DUTY_MAX                        1000u   ///< Maximum dutycycle of a PWM channel.

// ---------------------------------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------
/** This enum lists all possible error codes that a PWM function can return.*/
typedef enum
{
    HAL_PWM_OK                      = 0u,   ///< No error.
    HAL_PWM_ERROR_GENERAL               ,   ///< Common error.
    HAL_PWM_ERROR_INIT_FAILED           ,   ///< Initialization failed.
    HAL_PWM_ERROR_DEINIT_FAILED         ,   ///< De-initialization failed.
    HAL_PWM_ERROR_CHANNEL_INVALID       ,   ///< Channel invalid e.g. already not opened.
    HAL_PWM_ERROR_NO_CHANNEL_LEFT       ,   ///< No channel left on device.
    HAL_PWM_ERROR_PWM_SOURCE_INVALID    ,   ///< Pwm source is not valid.
    HAL_PWM_ERROR_PWM_MASTER            ,   ///< Pin is master.
    HAL_PWM_ERROR_NOT_IMPLEMENTED           ///< Error function not implemented.

} enum_HAL_PWM_RETURN_VALUE_t;

/** This enum lists the PWM ranges that can be set*/
typedef enum
{
	HAL_PWM_RANGE_1			= 0x00U,	/*!< edge counting time/ frequency range 1 */
	HAL_PWM_RANGE_2			= 0x01U,	/*!< edge counting time/ frequency range 2 */
	HAL_PWM_RANGE_3			= 0x02U,	/*!< edge counting time/ frequency range 3 */
}enum_HAL_PWM_RANGE_t;

typedef enum
{
    HAL_NO_PIN_CONTROL          = 0x00U,       /*!< No trigger */
    HAL_RISING_EDGE             = 0x01U,       /*!< Rising edge trigger */
    HAL_FALLING_EDGE            = 0x02U,       /*!< Falling edge trigger */
    HAL_BOTH_EDGES              = 0x03U        /*!< Rising and falling edge trigger */
} enum_HAL_EDGE_ALIGNMENT_MODE_t;


// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
*   \brief    Get pointer to ftm config structure
*   \details
*
*   \pre        The function #hal_pwm_init, #hal_freq_init must be called once before.
*
*   \param      pwm_pin_name   [in]  uint32_t    pwm pin name
*
*   \return     index of config structure to make ftm configuratble out of io-def
*/
uint32_t get_ftm_setting_ptr(uint32_t pwm_pin_name);

/*----------------------------------------------------------------------------*/
/**
*   \brief    Intialize PWM functionality.
*   \details
*
*   \return   Returns zero for success or a positive error code from #enum_HAL_PWM_RETURN_VALUE_t as feedback.
*/
enum_HAL_PWM_RETURN_VALUE_t hal_pwm_init(uint32_t pwm_count);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Release PWM functionality.
*   \details
*
*   \pre        The function #hal_pwm_init must be called once before.
*
*   \return     Returns zero for success or a positive error code from #enum_HAL_PWM_RETURN_VALUE_t as feedback.
*/
enum_HAL_PWM_RETURN_VALUE_t hal_pwm_deinit(void);

/*----------------------------------------------------------------------------*/
/**
*   \brief    Intialize frequency capturing functionality.
*   \details
*
*   \return   Returns zero for success or a positive error code from #enum_HAL_PWM_RETURN_VALUE_t as feedback.
*/
enum_HAL_PWM_RETURN_VALUE_t hal_freq_init(uint32_t freq_count);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Release freq capturing functionality.
*   \details
*
*   \pre        The function #hal_freq_init must be called once before.
*
*   \return     Returns zero for success or a positive error code from #enum_HAL_PWM_RETURN_VALUE_t as feedback.
*/
enum_HAL_PWM_RETURN_VALUE_t hal_freq_deinit(void);


/*----------------------------------------------------------------------------*/
/**
*   \brief      Read the set dutycycle.
*   \details    Read the dutycycle that is set for actual PWM frequency. 
*
*   \pre        The function #hal_pwm_init must be called once before.
*
*   \param      pin         [in] uint16_t Port pin identifier. 
*   \param      ptr_duty    [in] uint16_t* Pointer to returned val of pwm duty cycle 0 - 1000 (0 - 100%).
*   \return     Returns zero for success or a positive error code from #enum_HAL_PWM_RETURN_VALUE_t as feedback.
*/
enum_HAL_PWM_RETURN_VALUE_t hal_freq_get_duty(uint16_t pin, uint16_t* const ptr_duty);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Set pwm duty cycle in % * 100
*   \details
*
*   \pre        The function #hal_pwm_init must be called once before.
*
*   \param      pin         [in] uint16_t Port pin identifier.
*   \param      duty        [in] uint16_t PWM duty cycle 0 - 1000 (0 - 100%).
*   \return     Returns zero for success or a positive error code from #enum_HAL_PWM_RETURN_VALUE_t as feedback.
*/
enum_HAL_PWM_RETURN_VALUE_t hal_pwm_set_duty(uint16_t pin, uint16_t duty);

/*----------------------------------------------------------------------------*/
/**
* \brief    Forces the duty cycle of the PWM signal on the given value via software output control
* \details  This function will force the PWM output to 100% or 0% almost instantly (On every rising edge of the FTM clock).
*           DISCLAIMER: This function should mainly be used to operate the PWM pins as digital output.
*           Using it as a regular PWM output may lead to undefined behavior.
*
* \param    pin                    [in] uint16_t               From io_tables.h in struct_ftm_config_tbl (PWM_HSDx_OUTx)
* \param    forced_output_value    [in] uint16_t               force Value of the duty cycle to be set, currently only values of 1000 and 0 are supported
*
* \return   uint32_t   Return code
*/
enum_HAL_PWM_RETURN_VALUE_t hal_pwm_force_output(uint16_t pin, uint16_t forced_output_value);

/*----------------------------------------------------------------------------*/
/**
* \brief    This functions sets the FTM synchronization timing
* \details  Updates the FTM synchronization mechanism, this should only be used when the pwm modules have timing problems.
*           If enabled the FTM module will not wait for reload points and updates the registry values instantly.
*
* \param    pin         [in] uint16_t   From io_tables.h in struct_ftm_config_tbl (PWM_HSDx_OUTx)
* \param    enable      [in] uint8_t    bool value, 0 = FTM_WAIT_LOADING_POINTS, 1 = FTM_UPDATE_NOW
*
* \return   uint32_t   Return code
*/
enum_HAL_PWM_RETURN_VALUE_t hal_pwm_update_now(uint16_t pin, uint8_t enable);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Return the state of the ouput control register of the given pin
*   \details
*
*   \pre        The function #hal_pwm_init must be called once before.
*
*   \param      pin          [in] uint16_t Port pin identifier.
*   \param      ptr_state    [in] uint8_t* Pointer to returned state of the pi
*   \return     Returns zero for success or a positive error code from #enum_HAL_PWM_RETURN_VALUE_t as feedback.
*/
enum_HAL_PWM_RETURN_VALUE_t hal_pwm_readback_do(uint16_t pin, uint8_t* ptr_state);

/*----------------------------------------------------------------------------*/
/**
*   \brief      definition of hal_pwm_freq_readback to get pwm frequency in Hz
*   \details
*
*   \pre        The function #hal_pwm_init must be called once before.
*
*   \param      pin         [in] uint16_t Port pin identifier. 
*   \param      ptr_freq    [in] uint32_t* Pointer to returned val of pwm frequency in Hz.
*   \return     Returns zero for success or a positive error code from #enum_HAL_PWM_RETURN_VALUE_t as feedback.
*/
enum_HAL_PWM_RETURN_VALUE_t hal_pwm_readback_freq(uint16_t pin, uint32_t* const ptr_freq);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Set pwm frequency of all as pwm configured modules in Hz
*   \details
*
*   \pre        The function #hal_pwm_init must be called once before.
*
*   \param      freq        [in] uint32_t PWM frequency in Hz.
*   \return     Returns zero for success or a positive error code from #enum_HAL_PWM_RETURN_VALUE_t as feedback.
*/
enum_HAL_PWM_RETURN_VALUE_t hal_pwm_set_freq_global(uint32_t freq);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Set pwm frequency in Hz
*   \details
*
*   \pre        The function #hal_pwm_init must be called once before.
*
*   \param      pin         [in] uint16_t Port pin identifier. 
*   \param      freq        [in] uint32_t PWM frequency in Hz.
*   \return     Returns zero for success or a positive error code from #enum_HAL_PWM_RETURN_VALUE_t as feedback.
*/
enum_HAL_PWM_RETURN_VALUE_t hal_pwm_set_freq(uint16_t pin, uint32_t freq);

/*----------------------------------------------------------------------------*/
/**
*   \brief      definition of hal_freq_freq_get to capture input frequency in Hz
*   \details
*
*   \pre        The function #hal_freq_init must be called once before.
*
*   \param      pin         [in] uint16_t Port pin identifier.
*   \param      ptr_freq    [in] uint32_t* Pointer to returned val of input frequency in Hz.
*   \return     Returns zero for success or a positive error code from #enum_HAL_PWM_RETURN_VALUE_t as feedback.
*/
enum_HAL_PWM_RETURN_VALUE_t hal_freq_get_freq(uint16_t pin, uint32_t* const ptr_freq);

/*----------------------------------------------------------------------------*/
/**
*   \brief      definition of hal_pwm_process_cyclic function to process pwm cyclic
*   \details
*
*   \pre        The function #hal_pwm_init must be called once before.
*
*   \return     Returns zero for success or a positive error code from #enum_HAL_PWM_RETURN_VALUE_t as feedback.
*/
enum_HAL_PWM_RETURN_VALUE_t hal_pwm_process_cyclic(void);

/**
*   \ingroup
*   \brief    Function to configure the resolution of frequency measurement for a ftm module
*   \details
* 			The range option configures the prescaler of the timer to get a better resolution in time periode measurement.
* 			It also changes the measurement time in edge count mode.
*
*   \pre        The function #hal_freq_init must be called once before.
*
*   \param    instance                  [in] uint8_t
*   \param    range                     [in] enum_HAL_PWM_RANGE_t
*   \return   enum_HAL_PWM_RETURN_VALUE_t
*/
enum_HAL_PWM_RETURN_VALUE_t hal_freq_config_freq_measurment( uint8_t instance, enum_HAL_PWM_RANGE_t range );

/*----------------------------------------------------------------------------*/
/**
*   \ingroup
*   \brief  Function to readback the detected edges of the corresponding pin
*   \details
* 			ptr_edge contains the counted edges in the configured time
* 			The measure time can be configured with the method: hal_pwm_config_freq_measurement
*
* 			t_measure contains the configured measuring time
*
*   \pre        The function #hal_freq_init must be called once before.
*
*   \param    pin                       [in] uint16_t
*   \param    ptr_edge                  [in] uint32_t*
*   \param    t_measure                 [in] uint16_t*
*   \return   enum_HAL_PWM_RETURN_VALUE_t
*/
enum_HAL_PWM_RETURN_VALUE_t hal_freq_edge_counting(uint16_t pin, uint32_t* ptr_edge, uint16_t* const t_measure);

/*----------------------------------------------------------------------------*/
/**
*   \ingroup
*   \brief  Function to select the trigger edge of the corresponding ftm input
*   \details
*
*   \pre        The function #hal_freq_init must be called once before.
*
*   \param    pin                       [in] uint16_t
*   \param    edge_select               [in] enum_HAL_EDGE_ALIGNMENT_MODE_t
*   \return   enum_HAL_PWM_RETURN_VALUE_t
*/
enum_HAL_PWM_RETURN_VALUE_t hal_freq_init_ftm_edge(uint16_t pin, enum_HAL_EDGE_ALIGNMENT_MODE_t edge_select );

/*----------------------------------------------------------------------------*/
/**
*   \ingroup
*   \brief  Function to get the value of edge counter
*   \details
*
*   \pre        The function #hal_freq_init must be called once before.
*
*   \param    pin                       [in] uint16_t
*   \param    value           		    [in] uint32_t*
*   \return   enum_HAL_PWM_RETURN_VALUE_t
*/
enum_HAL_PWM_RETURN_VALUE_t hal_freq_get_edge_counter_value(uint16_t pin, uint32_t* value);

/*----------------------------------------------------------------------------*/
/**
*   \ingroup
*   \brief  Function to set the value of edge counter
*   \details
*
*   \pre        The function #hal_freq_init must be called once before.
*
*   \param    pin                       [in] uint16_t
*   \param    value           		    [in] uint32_t
*   \return   enum_HAL_PWM_RETURN_VALUE_t
*/
enum_HAL_PWM_RETURN_VALUE_t hal_freq_set_edge_counter_value(uint16_t pin, uint32_t value);

 /** @}*/


#endif
