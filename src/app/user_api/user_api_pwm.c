/*
 * user_api_pwm.c
 *
 *  Created on: 12.09.2019
 *      Author: VGoertz
 */



#include "user_api_pwm.h"
#include "graph_support.h"
#include "sfl_math.h"
#include "adc_app.h"
#include "user_api_ai.h"
#include "ftm_app.h"


#define ADC_INA_OUT_N               (8u)

pi_regulator_info_t regulator_data = {0, 0, 0};
pi_regulator_data_t mgl_pi_controller_data[PWM_MAX];

#if CONTROLLER_WITH_PRECONTROL
pi_controller_t mgl_pi_controller_vals[PWM_MAX];
#endif

#if CONTROLLER_WITH_PRECONTROL
uint16_t stromvalue1, stromvalue2, stromvalue3, step_regulator, stromvalue4; //debug messages
#endif


// 0 < No error.
// 1 < Common error.
// 2 < Initialization failed.
// 3 < De-initialization failed.
// 4 < Channel invalid e.g. already not opened.
// 5 < No channel left on device.
// 6 < Pwm source is not valid.
// 7 < Pin is master.
// 8 < Error function not implemented.

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_pwm_set_pwm(uint16_t pin, uint16_t freq, uint16_t duty)
{
    uint8_t ret_val;

    // pin check happens in all of these function, but only one
    // needs to return the error code
    ret_val = hal_pwm_set_freq(pin, freq);

    if(!ret_val)
    {
        ret_val = hal_pwm_set_duty(pin, duty);
    }
    else
    {
        // do nothing
    }

    return (uint32_t) ret_val;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_pwm_set_pwm_immediate(uint16_t pin, uint16_t freq, uint16_t duty)
{
    uint8_t ret_val;

    // pin check happens in all of these function, but only one
    // needs to return the error code
    hal_pwm_update_now(pin, 1);
    ret_val = hal_pwm_set_freq(pin, freq);
    hal_pwm_update_now(pin, 0);

    if(!ret_val)
    {
        ret_val = hal_pwm_set_duty(pin, duty);
    }
    else
    {
        // do nothing
    }

    return (uint32_t) ret_val;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_pwm_set_duty(uint16_t pin, uint16_t duty)
{
    return (uint32_t) hal_pwm_set_duty(pin, duty);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_pwm_output_force(uint16_t pin, uint16_t forced_output_value)
{
    return (uint32_t) hal_pwm_force_output(pin, forced_output_value);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*/
uint32_t user_pwm_set_dither(uint16_t pin, uint16_t duty, uint16_t freq, uint16_t dither_duty, uint16_t dither_freq)
{
	uint32_t ret_val = 0;

    if (pin < PWM_MAX)
    {
        // Validate dither duty will not be bigger/smaller then max/min duty possible
        if (dither_duty > duty )
        {
        	// dither duty needs to be less than duty or else it will become negative
            dither_duty = duty;
        }
        else if (dither_duty > 1000 - duty)
        {
        	// dither duty needs to be more than 1000- duty or else it will become more than max duty (1000 promille)
            dither_duty = 1000 - duty;
        }
        else
        {
        	// do nothing
        }

        // dither frequency needs to be bigger then 10 and smaller then freq/2
        if (dither_freq < 10)
        {
        	dither_freq = 10;
        }
        else if (dither_freq > freq / 2)
        {
            dither_freq = freq / 2;
        }
        else
        {
        	// do nothing
        }

        // save dither values into global array for usage inside FTM overflow ISR
        pwm_vals_t[pin].dither_promille = dither_duty;
        pwm_vals_t[pin].dither_counts = freq / dither_freq;
        pwm_vals_t[pin].pwm_promille_backup = duty;  // Backup, this won't change

        // execute only if frequency changed
        if (freq != pwm_vals_t[pin].pwm_frequency_old && freq)
        {
        	ret_val = user_set_pwm_freq(pin, freq);
            pwm_vals_t[pin].pwm_period = (1000 + freq / 2) / freq;   // time period in ms
        }

        if (dither_duty == 0)
        {
            pwm_vals_t[pin].duty_cycle = duty;
        }
        ret_val = user_set_pwm_duty(pin, duty);
    }

    return ret_val;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_pwm_set_global_freq(uint16_t freq)
{
    return (uint32_t) hal_pwm_set_freq_global( freq );
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_pwm_set_freq(uint16_t pin, uint16_t freq)
{
    enum_HAL_PWM_RETURN_VALUE_t error = HAL_PWM_ERROR_GENERAL;

    if(freq == 0)
    {
        error = hal_pwm_set_duty(pin, 0);
    }
    else
    {
        error = hal_pwm_set_freq(pin, freq);
    }
    return (uint32_t) error;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_freq_get_measured_freq(uint16_t pin)
{
    uint32_t ptr_freq;
    hal_freq_get_freq(pin, &ptr_freq);
    return ptr_freq;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_freq_get_measured_duty(uint16_t pin)
{
    uint16_t ptr_freq;
    hal_freq_get_duty(pin, &ptr_freq);
    return (uint32_t) ptr_freq;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* The API function which is between the user and the PI controller.
* \endinternal
*
*
* \test STATUS: *** UNTESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20200908   | Author  | jallgeier
*/
uint32_t user_pwm_current_control(enum_pwm_pin_name pin, uint32_t set_value, uint32_t kp, uint32_t ki)
{
    uint32_t duty_out;

	mgl_pi_controller_data[pin].current_value = adc_results[pin+(ADC_INA_OUT_N-1)].result_filtered;

    //call of the PI controller.
    PI_Controller_Discrete(pin, set_value, mgl_pi_controller_data[pin].current_value, kp, ki, 0u, 1000u, &duty_out, &mgl_pi_controller_data[pin].flag0, &mgl_pi_controller_data[pin].flag1);

    //set the from the controller calculated duty cycle to the corresponding output.
    hal_pwm_set_duty(pin, duty_out);

    return duty_out;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* internal function of the PI controller that is called by the function "user_pwm_current_control"
*
* \endinternal
*
*
* Date       | Type    | Person
* -----------|---------|-----------
* 20200908   | Author  | jallgeier
*/
void PI_Controller_Discrete(enum_pwm_pin_name pin, uint32_t soll, uint32_t ist, uint32_t kp, uint32_t ki, uint32_t minOut, uint32_t maxOut, uint32_t *out,
        uint32_t *flag0, uint32_t *flag1)
{
    //Wenn der Sollwert 0 ist, soll ohne groï¿½e Regelung sofort auf 0 gestellt werden.
    if ( soll == 0 )
    {
        *flag0 = 0;
        *flag1 = 0;
        *out = 0;
    }
    else
    {
        int rtAdd;
        int rtSub;

        rtSub = (int)soll - (int)ist;
        rtAdd = (((int)ki + (int)kp) * rtSub / 1000 + (int)*flag1) - (int)ki * (int)*flag0 / 1000;

        // range limit output
        if ( rtAdd > (int) maxOut )
        {
            rtAdd = (uint32_t) maxOut;
        }
        else if ( rtAdd < (int) minOut )
        {
            rtAdd = (uint32_t) minOut;
        }
        else
        {
            // do nothing
        }

        *flag1 = (int)rtAdd;
        *flag0 = rtSub;

        *out = (uint32_t)rtAdd;
    }
}

#if CONTROLLER_WITH_PRECONTROL
void PI_Controller_Discrete_with_precontrol(enum_pwm_pin_name pin, uint32_t soll, uint32_t ist, uint32_t kp, uint32_t ki, uint32_t minOut, uint32_t maxOut, uint32_t *out, uint32_t *flag0, uint32_t *flag1)
{
    //int delay1_PI = (int)*flag0;
    //int delay2_PI = (int)*flag1;

    uint32_t ausgang;
    //static uint16_t max_current_slope = 150;

    mgl_pi_controller_vals[pin].snapshot_current = user_ai_get_cal(AI_INA_OUT1);

    if ( (mgl_pi_controller_vals[pin].snapshot_current - mgl_pi_controller_vals[pin].old_current_value) < 0 ) //kleine Sicherheit damit es keinen ï¿½berlauf ins negative gibt.
    {
        stromvalue2 = 0;
        mgl_pi_controller_vals[pin].last_current_slope = 0;
    }
    else
    {
        stromvalue2 = mgl_pi_controller_vals[pin].snapshot_current - mgl_pi_controller_vals[pin].old_current_value; //Debug message
        mgl_pi_controller_vals[pin].last_current_slope = mgl_pi_controller_vals[pin].snapshot_current - mgl_pi_controller_vals[pin].old_current_value; //Es wird die Stromsteigung in bezug auf den letzen durchlauf berechnet.
    }

    if ( mgl_pi_controller_vals[pin].last_current_slope > mgl_pi_controller_vals[pin].max_current_slope )
    {
        mgl_pi_controller_vals[pin].max_current_slope = mgl_pi_controller_vals[pin].last_current_slope; // Wird eine neue hï¿½here maximal Steigung erkannt, wird diese fï¿½r kï¿½nfigte berechnungen verwendet.
    }

    stromvalue3 = mgl_pi_controller_vals[pin].max_current_slope; //debug nachricht
    mgl_pi_controller_vals[pin].lower_border = ((mgl_pi_controller_vals[pin].max_current_slope * 100) * (75)) / 10000; //Die grenzen werden aufgrund der maximal zu erwarteten Steigung berechnet.
    //stromvalue4 = lower_border;

    if ( (soll - mgl_pi_controller_vals[pin].old_soll) >= mgl_pi_controller_vals[pin].max_current_slope + 50 ) //ï¿½berprï¿½fung ob ï¿½berhaupt eine Vorsteuerung statt finden soll.
    {
        mgl_pi_controller_vals[pin].pre_control = 0;
        step_regulator = 0;
        mgl_pi_controller_vals[pin].cycles = (soll - mgl_pi_controller_vals[pin].old_soll) / 50; //
        mgl_pi_controller_vals[pin].controller_status = 0;
    }

    stromvalue2 = mgl_pi_controller_vals[pin].cycles;

    //Wenn der Sollwert 0 ist, soll ohne groï¿½e Regelung sofort auf 0 gestellt werden.
    if ( soll == 0 )
    {
        *flag0 = 0;
        *flag1 = 0;
        ausgang = 0;
        *out = ausgang;
        step_regulator = 0; //Nur zu Debug Zwecke
        mgl_pi_controller_vals[pin].pre_control = 0;
        mgl_pi_controller_vals[pin].cycles = 0;
    }
    else
    {

        int delay1_PI = (int) *flag0;
        int delay2_PI = (int) *flag1;

        int rtAdd;
        int rtSub;

        //Differenz zwischen soll und ist(es wird hier nicht 'ist' aus den Funktionsï¿½bergaben genutzt, da es sich hier um eine Mittelwertbildung handeln kann. 'snapshot' ist direkt der Wert vom ADC.
        mgl_pi_controller_vals[pin].difference_current = soll - mgl_pi_controller_vals[pin].snapshot_current;

        //Ist die Sollwert ï¿½nderung groï¿½ genug, um generell eine Vorsteuerung durchzufï¿½hren, so werden hier die maximal durchzufï¿½hrenden Zyklen bestimmt.
        if ( (mgl_pi_controller_vals[pin].difference_current - 51) < 0 )
        {
            mgl_pi_controller_vals[pin].pre_control = mgl_pi_controller_vals[pin].cycles;
        }

        //check ob noch zyklen fï¿½r die Vorsteuerung ï¿½brig sind.
        if ( /*mgl_pi_controller_vals[pin].pre_control < mgl_pi_controller_vals[pin].cycles*/0 )
        { //Vorsteuerung
            step_regulator++; //Debug nachricht

            //Ist die differenz zwischen dem Sollwert und dem aktuellen IST Wert, grï¿½ï¿½er als die bisher ermittelte maximal Stromsteigung, so wird mindestens ein Zyklus mit duty = 1000 durchlaufen.
            if ( mgl_pi_controller_vals[pin].difference_current >= mgl_pi_controller_vals[pin].max_current_slope
                    || (mgl_pi_controller_vals[pin].controller_status & 0x10) )
            {
                ausgang = 1000;
                mgl_pi_controller_vals[pin].controller_status |= 0x01; //Status Bit des Reglers wird gesetzt, welches angibt, das einmal mit duty = 100% durchlaufen wurde.

                //wurde einmal die Duty Stufe 2. ausgefï¿½hrt, der ist Wert jedoch wieder soweit abgefallen, dass Duty Stufe 1 nï¿½tig ist, so wird Duty Stufe 2 gesperrt um ein hin und her zu vermeiden.
                if ( mgl_pi_controller_vals[pin].controller_status & 0x02 )
                {
                    mgl_pi_controller_vals[pin].controller_status |= 0x10;
                }

                //wurde einmal die Duty Stufe 3. ausgefï¿½hrt, der ist Wert jedoch wieder soweit abgefallen, dass Duty Stufe 1 nï¿½tig ist, so wird Duty Stufe 3 gesperrt um ein hin und her zu vermeiden.
                if ( mgl_pi_controller_vals[pin].controller_status & 0x04 )
                {
                    mgl_pi_controller_vals[pin].controller_status |= 0x08;
                }
            } //Ist die differenz des IST zum Sollwert zwischen der maximal ermittelten Stromsteigung und der unteren Grenze wird Duty Stufe 2 verwendet.
            else if ( (mgl_pi_controller_vals[pin].difference_current < mgl_pi_controller_vals[pin].max_current_slope
                    && mgl_pi_controller_vals[pin].difference_current >= mgl_pi_controller_vals[pin].lower_border)
                    || (mgl_pi_controller_vals[pin].controller_status & 0x08) )
            {
                ausgang = 750 - mgl_pi_controller_vals[pin].duty_adaption;
                mgl_pi_controller_vals[pin].controller_status |= 0x02;

                if ( mgl_pi_controller_vals[pin].controller_status & 0x04 )
                {
                    mgl_pi_controller_vals[pin].controller_status |= 0x08;
                }
            }
            else
            {
                ausgang = 500 - mgl_pi_controller_vals[pin].duty_adaption;
                mgl_pi_controller_vals[pin].controller_status |= 0x04;
            }

            //Der merker wie viel Vorsteuerzyklen bereits durchlaufen wurden sind, wird inkrementiert.
            mgl_pi_controller_vals[pin].pre_control++;

            *out = ausgang;
            rtSub = (int) soll - (int) ist;

            delay1_PI = rtSub;

            *flag0 = delay1_PI;
            *flag1 = ausgang - 0; //todo
        }
        else
        { //Regler
            step_regulator = 255;

            if ( (mgl_pi_controller_vals[pin].snapshot_current > soll) && !(mgl_pi_controller_vals[pin].controller_status & 0x80)
                    && ((int) (soll - mgl_pi_controller_vals[pin].old_soll) >= 0) )
            {
                mgl_pi_controller_vals[pin].duty_adaption += 20;
                stromvalue4 = mgl_pi_controller_vals[pin].duty_adaption;
            }

            mgl_pi_controller_vals[pin].controller_status |= 0x80;

            rtSub = (int) soll - (int) ist;
            rtAdd = (((int) ki + (int) kp) * rtSub / 1000 + delay2_PI) - (int) kp * delay1_PI / 1000;

            // range limit output
            if ( rtAdd > (int) maxOut )
            {
                rtAdd = (uint32_t) maxOut;
            }
            else if ( rtAdd < (int) minOut )
            {
                rtAdd = (uint32_t) minOut;
            }
            else
            {
                // do nothing
            }

            // take over output and save integral part
            ausgang = (uint32_t) rtAdd;

            delay2_PI = (int) rtAdd;
            delay1_PI = rtSub;

            // set flags
            *flag0 = delay1_PI;
            *flag1 = delay2_PI;

        }

        *out = ausgang;

    }

    mgl_pi_controller_vals[pin].old_soll = soll; //Damit old_soll im nï¿½chsten Zyklus mit dem neuen Soll Wert verglichen werden kann, wird hier diese Zuweisung gemacht
    mgl_pi_controller_vals[pin].old_current_value = user_ai_get_cal(AI_INA_OUT1);

}
#endif
