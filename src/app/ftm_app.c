#include "ftm_app.h"
#include "pins_port_hw_access.h"
#include "hal_pwm.h"


/* Global array with values needed to handle the PWM dither feature */
struct_pwm_vals_def pwm_vals_t[PWM_MAX + 1];


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*/
uint32_t change_pin_to_ftm(enum_pin_name pin)
{
	uint32_t i;
	uint32_t ret_val = 0;
	for( i = 0; i < PIN_CFG_ALT_MAX; i++)
	{
		if(pin_cfg_alts[i].pin_name == pin)
		{
		    // structure in ram with pin config gets changed and pin mux gets reinitialized
			PINS_SetMuxModeSel(g_pin_mux_InitConfigArr[pin].base, g_pin_mux_InitConfigArr[pin].pinPortIdx, pin_cfg_alts[i].ftm_alt_cfg);
			// TODO: KBA is this needed? g_pin_mux_InitConfigArr[pin].mux = pin_cfg_alts[i].ftm_alt_cfg;
			pin_cfg_alts[i].active_alt = 1; // ftm alt is active
			ret_val = 1;
		}
	}
	return ret_val;
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*/
uint32_t change_pin_to_adc(enum_pin_name pin)
{
	uint32_t i;
	uint32_t ret_val = 0;
	for( i = 0; i < PIN_CFG_ALT_MAX; i++)
	{
		if(pin_cfg_alts[i].pin_name == pin)
		{
            // structure in ram with pin config gets changed and pin mux gets reinitialized
			PINS_SetMuxModeSel(g_pin_mux_InitConfigArr[pin].base, g_pin_mux_InitConfigArr[pin].pinPortIdx, pin_cfg_alts[i].adc_alt_cfg);
			// TODO: KBA is this needed? g_pin_mux_InitConfigArr[pin].mux = pin_cfg_alts[i].adc_alt_cfg;
            pin_cfg_alts[i].active_alt = 0; // adc alt is active
			ret_val = 1;
		}
	}
	return ret_val;
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*/
void FTM_IRQHandler(uint8_t module)
{
	uint8_t i;

	for (i = 0; i < PWM_MAX; i++)
	{
		if (struct_ftm_config_tbl[i].pwm_instance == module)	// Update only the dither of the pins of the FTM module which triggered the interrupt
		{
			if (pwm_vals_t[i].dither_promille)
			{
				// If dither is active
				pwm_vals_t[i].dither_counter++;

				// Invert the dither after half the PWM period
				if (pwm_vals_t[i].dither_counter >= (pwm_vals_t[i].dither_counts + 1) / 2)
				{
					pwm_vals_t[i].dither_counter = 0;

					pwm_vals_t[i].dither_stat = ~pwm_vals_t[i].dither_stat + 1; // invert  1 will become -1 and -1 will become 1

					// Set new duty cycle
					hal_pwm_set_duty(i, (uint16_t)((pwm_vals_t[i].pwm_promille_backup + pwm_vals_t[i].dither_stat * pwm_vals_t[i].dither_promille)));
				}
				else
				{
					// do nothing
				}
			}
			else
			{
				// do nothing
			}
		}
		else
		{
			// do nothing
		}
	}
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*/
void FTM0_IRQHandler(void)
{
	FTM_IRQHandler(MODULE_0);
	FTM_DRV_ClearStatusFlags(MODULE_0, FTM_TIME_OVER_FLOW_FLAG);
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*/
void FTM1_IRQHandler(void)
{
	FTM_IRQHandler(MODULE_1);
	FTM_DRV_ClearStatusFlags(MODULE_1, FTM_TIME_OVER_FLOW_FLAG);
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*/
void FTM2_IRQHandler(void)
{
	FTM_IRQHandler(MODULE_2);
	FTM_DRV_ClearStatusFlags(MODULE_2, FTM_TIME_OVER_FLOW_FLAG);
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*/
void FTM3_IRQHandler(void)
{
	FTM_IRQHandler(MODULE_3);
	FTM_DRV_ClearStatusFlags(MODULE_3, FTM_TIME_OVER_FLOW_FLAG);
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*/
void FTM4_IRQHandler(void)
{
	FTM_IRQHandler(MODULE_4);
	FTM_DRV_ClearStatusFlags(MODULE_4, FTM_TIME_OVER_FLOW_FLAG);
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*/
void FTM5_IRQHandler(void)
{
	FTM_IRQHandler(MODULE_5);
	FTM_DRV_ClearStatusFlags(MODULE_5, FTM_TIME_OVER_FLOW_FLAG);
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*/
void FTM6_IRQHandler(void)
{
	FTM_IRQHandler(MODULE_6);
	FTM_DRV_ClearStatusFlags(MODULE_6, FTM_TIME_OVER_FLOW_FLAG);
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*/
void FTM7_IRQHandler(void)
{
	FTM_IRQHandler(MODULE_7);
	FTM_DRV_ClearStatusFlags(MODULE_7, FTM_TIME_OVER_FLOW_FLAG);
}

