/*
 * user_api_ai.c
 *
 *  Created on: 11.09.2019
 *      Author: VGoertz
 */

#include "user_api_ai.h"
#include "adc_app.h"


/*----------------------------------------------------------------------------*/
/**
 * \internal
 * This function will switch the adc_vref value (used for calculation of
 * standardized values) according to the selected measurement range
 * \endinternal
 *
 */
void user_ai_set_measurement_range(enum_adc_pin_name pin, uint8_t range)
{
#if ADC_SECOND_RANGE
    modulhardwarecode_adc_set_measurement_range(pin, range);
//  #warning "user_ai_set_measurement_range() is deprecated. Please use modulhardwarecode_adc_set_measurement_range(pin, range) instead."
#endif
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t user_ai_get(enum_adc_pin_name pin)
{
    uint16_t ret_val = 0u;
    hal_io_ai_get_digits(pin, &ret_val);
    return ret_val;
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t user_ai_get_mv(enum_adc_pin_name pin)
{
	uint16_t ret_val = 0u;

    if ( (pin > ADC_MAX) || (adc_config_tbl[pin].resolution == 0u) )
    {
        // pin is invalid or we need to avoid avoid divide-by-zero
    	// we'll just return 0 since we don't have another way of signaling it
        ret_val = 0;
    }
	else
	{
		//calculate adc value in mV
		ret_val = ((user_ai_get(pin) * adc_get_measurement_range(pin)) / adc_config_tbl[pin].resolution);
	}

	return ret_val;
}


/*----------------------------------------------------------------------------*/
/**
*  Returns the calibrated value in mV of the eligible pin (see enum_adc_pin_name)
*
* \internal
* \endinternal
*
*/
uint16_t user_ai_get_cal(enum_adc_pin_name pin)
{
	uint16_t retval = 0u;

	retval = adc_results[pin].result_cal;

    return retval;
}

/*----------------------------------------------------------------------------*/
/**
*  Returns the filtered calibrated value in mV of the eligible pin (see enum_adc_pin_name)
*
* \internal
* \endinternal
*
*/
uint16_t user_ai_get_filtered(enum_adc_pin_name pin)
{
    uint16_t retval = 0u;

    retval = adc_results[pin].result_filtered;

    return retval;
}
