/*
 * user_api_io.c
 *
 *  Created on: 11.09.2019
 *      Author: VGoertz
 */

#include "user_api_io.h"
#include "io_tables.h"
#include "user_api_pwm.h"
#include "adConv1.h"
#include "flexTimer1.h"
#include "user_code.h"
#include "adc_app.h"

// 0 < No error.
// 1 < Common error.
// 2 < Initialization failed.
// 3 < De-initialization failed.
// 4 < Channel invalid e.g. channel not opened yet.
// 5 < No channel left on device.
// 6 < Pin is not configured as digital output.
// 7 < Pin is not configured as digital input.
// 8 < Error function not implemented.
// 9 < Adc max value is undefined.


extern struct_VIRTUAL_PINS_CFG virtual_pin[VIRTUAL_PIN_MAX - PIN_MAX];

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
void user_do_set(uint16_t pin, uint8_t state)
{
    // Check if the given pin is a physical DO pin or if it is a pwm pin
    if( (g_pin_mux_InitConfigArr[pin].mux == PORT_MUX_AS_GPIO) && (pin < PIN_MAX) )
    {
        // Pin is a physical DO pin
        hal_io_do_set(pin, state);
    }
    else if( (pin >= PIN_MAX) && (pin < VIRTUAL_PIN_MAX) )
    {
        // Pin is a virtual DO pin
		virtual_pin[pin - PIN_MAX].current_val = state;
    }
    else
    {
        // Pin is a PWM pin
        enum_pwm_pin_name pwm_pin = PWM_MAX;
        uint8_t i = 0;

        // Search the resembling PWM pin from the table
        for (i = 0; i < PWM_MAX; i++)
        {

            if (struct_ftm_init_config_tbl[i].io_pin_name == pin)
            {
                // take over the right PWM pin
                pwm_pin = struct_ftm_init_config_tbl[i].pwm_pin_name;
                // Set condition for ending the for loop
                i = PWM_MAX;
            }
            else
            {
                // do nothing
            }
        }

        if (state == 0)
        {
            user_pwm_output_force(pwm_pin, 0);
        }
        else
        {
        	user_pwm_output_force(pwm_pin, 1000);
        }
    }
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint8_t user_do_readback(uint16_t pin)
{
	uint8_t state = 0;

    // Check if the given pin is a physical DO pin or if it is a pwm pin
    if( (g_pin_mux_InitConfigArr[pin].mux == PORT_MUX_AS_GPIO) && (pin < PIN_MAX) )
    {
        // Pin is a physical DO pin
        hal_io_do_readback(pin, &state);
    }
    else if (pin >= PIN_MAX)
    {
        // Pin is a virtual DO pin
    	state = virtual_pin[pin - PIN_MAX].current_val;
    }
    else
    {
        // Pin is a PWM pin
        enum_pwm_pin_name pwm_pin = PWM_MAX;
        uint8_t i = 0;

        // Search the resembling PWM pin from the table
        for (i = 0; i < PWM_MAX; i++)
        {

            if (struct_ftm_init_config_tbl[i].io_pin_name == pin)
            {
                // take over the right PWM pin
                pwm_pin = struct_ftm_init_config_tbl[i].pwm_pin_name;
                // Set condition for ending the for loop
                i = PWM_MAX;
            }
            else
            {
                // do nothing
            }
        }
        hal_pwm_readback_do(pwm_pin, &state);
    }

    return state;
}


/*----------------------------------------------------------------------------*/
/**
* \internal
* Check the pin if it is generally in the valid pin range 0 - PIN_MAX.
* Then, if it is configured for GPIO, read it as such.
* Otherwise, if it is an ADC pin, read that and compare it to DI_EMULATION_THRESHOLD
* \endinternal
*
*/
uint8_t user_di_get(uint16_t pin)
{
    uint8_t state = 0;

    if( pin < PIN_MAX )
    {
        // Valid pin
        if( g_pin_mux_InitConfigArr[pin].mux == PORT_MUX_AS_GPIO )
        {
            // Pin is a physical DI pin or an ADC pin configured for GPIO
            // Pins configured for GPIO can still be sampled by ADC at the same time,
            // see https://community.nxp.com/t5/S32K/Using-GPIO-and-ADC-at-the-same-time-on-S32K14x-officially/m-p/1228132
            hal_io_di_get(pin, &state);
        }
        else
        {
            // pin might be an ADC pin

            // This segment is still required because we cannot be sure all ADC pins are configured for GPIO.
            // Also, this can be used for setting custom voltage thresholds if needed.
            // We don't have support for hysteresis because we didn't require it so far.
            enum_adc_pin_name adc_pin = user_di_find_adc_pin(pin);
            if(adc_pin < ADC_MAX)
            {
#ifdef DI_EMULATION_THRESHOLD
                //pin is an ADC pin and threshold is set by user
                if (adc_results[adc_pin].result_digit >= DI_EMULATION_THRESHOLD)
                {
                    state = 1;
                }
#else
                //pin is an ADC pin and threshold is by default 6V
                if (user_ai_get_mv(adc_pin) >= 6000)
                {
                    state = 1;
                }
#endif
                else
                {
                    state = 0;
                }
            }
            else
            {
                //pin is not a valid ADC pin.
                state = 0;
            }
        }
    }
    else
    {
        // Invalid pin
        state = 0;
    }

    return state;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* Go through all known ADC pins and compare their physical pin. If it matches, return that.
* Otherwise, return ADC_MAX to signal no corresponding pin.
* \endinternal
*
*/
enum_adc_pin_name user_di_find_adc_pin(uint16_t pin)
{
    enum_adc_pin_name adc_pin = ADC_MAX;

    // Search the resembling ADC pin from the table
    for (uint16_t i = 0; i < ADC_MAX; i++)
    {
        if (adc_init_config_tbl[i].io_pin_name == pin)
        {
            // take over the right ADC pin
            adc_pin = i;
            break;
        }
        else
        {
            // do nothing
        }
    }

    return adc_pin;
}
