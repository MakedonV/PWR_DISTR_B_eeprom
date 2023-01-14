/*--------------------------------------------------------------------------*/
/** \file           modulhardwarecode.c
*   \brief          Modulehardwarecode for the CC16WP module and its variants.
*   \date           21.12.2022 17:54:20  \author: ApplicsStudio
*   \version        1.00
*   \platform CPU:  S32K  | Compiler: GCC
* --------------------------------------------------------------------------*/
#include "can_app.h"
#include "user_api_io.h"
#include "user_api_ai.h"
#include "user_api_util.h"
#include "modulhardwarecode.h"
#include "hal_io.h"
#include "io_tables.h"
#include "hal_tick.h"
#include "watchdog1.h"
#include "hal_watchdog.h"
#include "sfl_timer.h"

#include "sfl_can_db_tables_data.h"

const struct_can_pin_list can_pin_list[CAN_HANDLE_MAX_NR] =
{
    // enable,  standby
    { CAN_EN1, CAN_STB1 },
    { CAN_EN2, CAN_STB2 },
    { CAN_EN3, CAN_STB3 }
};

struct_ADC_VREF_values_t mgl_adc_vref_values[ADC_MAX + 1] = {
        // pin                          	//  alternate vref         // range switch
		{   .pin = AI_MOTOR_1_VM1,            .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIX_MUX_1,                 .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIX_MUX_2,                 .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },

	    {   .pin = AI_PIGGY_2,                .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_PIGGY_3,                .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_PIGGY_4,                .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_HSD_SENSE_1,            .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_HSD_SENSE_2,            .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_HSD_SENSE_3,            .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_HSD_SENSE_4,            .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },

	    {   .pin = AI_HSD_CURRENT_OUT_13,     .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_HSD_CURRENT_OUT_14,     .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_HSD_CURRENT_OUT_15,     .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_KL30,                   .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },

	    {   .pin = AI_HSD_SENSE_5,            .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX  },
	    {   .pin = AI_HSD_SENSE_6,            .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX  },
	    {   .pin = AI_HSD_SENSE_7,            .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX  },
	    {   .pin = AI_HSD_SENSE_8,            .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX  },
	    {   .pin = AI_HSD_CURRENT_OUT_16,     .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX  },
	    {   .pin = AI_SOLAR_I,                .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX  },

	    {   .pin = AI_TASTER_2,               .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_VREF,                   .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_MOTOR_1_VM,             .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIX_MUX_3,                 .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIX_MUX_4,                 .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_X1000_19_RS485_RS232_AE,.vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
		{   .pin = AI_MOTOR_2_VM1,            .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },

	    {   .pin = AI_PIGGY_1,                .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_TASTER,                 .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_HSD_SENSE_9,            .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_HSD_SENSE_10,           .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_HSD_SENSE_11,           .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_HSD_SENSE_12,           .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_SOLAR_V,                .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_MOTOR_2_VM,             .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },

	    {   .pin = AI_MOTOR_2_IS_NEG,         .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_MOTOR_1_IS_NEG,         .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_MOTOR_2_IS_POSITIVE,    .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AI_MOTOR_1_IS_POSITIVE,    .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   },

	    {   .pin = AIM_IN_1,                  .vrefh2 = 32250 ,         .range_switch = DOM_BEREICH_30V_1   },
	    {   .pin = AIM_IN_2,                  .vrefh2 = 32250 ,         .range_switch = DOM_BEREICH_30V_2   },
	    {   .pin = AIM_IN_3,                  .vrefh2 = 32250 ,         .range_switch = DOM_BEREICH_30V_3   },
	    {   .pin = AIM_IN_4,                  .vrefh2 = 32250 ,         .range_switch = DOM_BEREICH_30V_4   },
	    {   .pin = AIM_IN_5,                  .vrefh2 = 32250 ,         .range_switch = DOM_BEREICH_30V_5   },
	    {   .pin = AIM_IN_6,                  .vrefh2 = 32250 ,         .range_switch = DOM_BEREICH_30V_6   },
	    {   .pin = AIM_IN_7,                  .vrefh2 = 32250 ,         .range_switch = DOM_BEREICH_30V_7   },
	    {   .pin = AIM_POWER_TEMP,            .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },

	    {   .pin = AIM_TEMP_1,                .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_TEMP_3,                .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_TEMP_2,                .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_TEMP_OUT_13_16,        .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_KL_15,                 .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_VDD_NMEA,              .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_PIGGY_VARIANTE_LEFT,   .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_PIGGY_VARIANTE_RIGHT,  .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },

	    {   .pin = AIM_HSD_IO_1 ,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_2 ,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_3 ,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_4 ,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_5 ,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_6 ,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_7 ,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_8 ,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },

	    {   .pin = AIM_HSD_IO_9 ,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_10,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_11,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_12,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_13,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_14,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_15,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },
	    {   .pin = AIM_HSD_IO_16,             .vrefh2 = 0 ,             .range_switch = VIRTUAL_PIN_MAX   },

	    {   .pin = ADC_MAX,                   .vrefh2 = 0,              .range_switch = VIRTUAL_PIN_MAX   }

};

uint32_t modulhardwarecode_adc_get_vrefh2(enum_adc_pin_name const pin, uint32_t const default_range)
{
        uint32_t range = default_range;
        // check if range of given ADC pin is switched and if yes use that value
        // Is the given pin inside the ADC pin count?
        if (pin < ADC_MAX)
        {
                if (mgl_adc_vref_values[pin].range_switch < (enum_pin_name) VIRTUAL_PIN_MAX)
                {
                        // the pin has a range switch
                        if (user_do_readback(mgl_adc_vref_values[pin].range_switch))
                        {
                                // 2nd range is selected
                                range = mgl_adc_vref_values[pin].vrefh2;
                        }
                }
        }

        return range;
}


enum_adc_pin_name modulhardwarecode_adc_get_range_switch(enum_adc_pin_name const pin)
{
        if (pin < ADC_MAX)
        {
                return mgl_adc_vref_values[pin].range_switch;
        }
        else
        {
                return PIN_MAX;
        }
}

void modulhardwarecode_adc_set_measurement_range(enum_adc_pin_name const pin, uint8_t const range)
{
        // Check if pin is valid, has a vrefh2 value and DO pin for range switch are given
        if ( (pin < ADC_MAX) &&
                    (mgl_adc_vref_values[pin].vrefh2 != 0) &&
                    (mgl_adc_vref_values[pin].range_switch < (enum_pin_name) VIRTUAL_PIN_MAX) )
        {
                switch (range)
                {
                    case 0:
                        (void)user_do_set(mgl_adc_vref_values[pin].range_switch, 0);
                        break;

                    case 1:
                        (void)user_do_set(mgl_adc_vref_values[pin].range_switch, 1);
                        break;

                    default:
                        (void)user_do_set(mgl_adc_vref_values[pin].range_switch, 0);
                        break;
                }
        }
        else
        {
                // return invalid pin
        }
}

uint16_t modulhardwarecode_ai_get_temperature(enum_adc_pin_name pin)
{
    const uint8_t calib_points = 6u;
    int16_t adc_temp_x[] = { 442, 721, 1171, 1823, 2604, 3306 };
    int16_t adc_temp_y[] = {  1000,  800,  600,  400,  200, 0 };
    uint16_t retval = 0xffff;
    uint32_t temp = 0;

    if(pin < ADC_MAX)
    {
        temp = user_ai_get(pin);
        retval = user_lookup_1D_16(&adc_temp_x[0],&adc_temp_y[0],calib_points,temp,0);
    }
    else
    {
        retval = 0xffff;
    }
    return retval;
}

void modulhardwarecode_init(void)
{
	// Watchdog
    hal_watchdog_mask(WDT_SIGNAL_MAIN_LOOP_BIT);
    hal_watchdog_start(HAL_WATCHDOG_INTERNAL, 0);

    user_can_init( can_pin_list);

    //shift_reg_init
    hal_io_do_set(DOX_SHIFT_MR_N, 1);      // Master Reset - has to be high while operating
    hal_io_do_set(DOX_SHIFT_ST_CP, 1);   // Values of shift register are copied into the Storage/Latch Register ("Output").
    hal_io_do_set(DOX_SHIFT_OE_N, 0);      // Output Enable - has to be low while operating

    // enable LIN transceiver
    hal_io_do_set(DO_LIN_EN, 1);
}

void modulhardwarecode_sleep(void)
{
    DISABLE_INTERRUPTS();

    // go-to-sleep mode for CAN tranceiver
    hal_io_do_set(CAN_STB1, 0);     // disable CAN1 standby pin
    hal_io_do_set(CAN_STB2, 0);     // disable CAN2 standby pin
    hal_io_do_set(CAN_STB3, 0);     // disable CAN2 standby pin

    // wait until CAN tranceivers are in sleep mode
    uint32_t delay_value = 160000;   // 2 ms / 1/80.000.000 * 160.000
    while(delay_value--);   // Wait 2ms

    // disable CAN tranceiver
    hal_io_do_set(CAN_EN1, 0);      // disable CAN1 enable pin
    hal_io_do_set(CAN_EN2, 0);      // disable CAN2 enable pin
    hal_io_do_set(CAN_EN3, 0);      // disable CAN2 enable pin

    hal_io_do_set(DO_LIN_EN, 0);       // disable LIN transceiver
    hal_io_do_set(DO_POWER_EN, 0);     // disable selbsthaltung

    // wait until 5V DCDC is off
    delay_value = 16000000;  // 200 ms / 1/80.000.000 * 16.000.000
    while(delay_value--);   // Wait 200 ms

    // recovery after timeout, because module didn't go to sleep
    hal_io_do_set(DO_POWER_EN, 1);     // re-enable selbsthaltung
    hal_io_do_set(CAN_EN1, 1);      // enable CAN1 transceiver
    hal_io_do_set(CAN_STB1, 1);     // enable CAN1 standby pin
    hal_io_do_set(CAN_EN2, 1);      // enable CAN2 transceiver
    hal_io_do_set(CAN_STB2, 1);     // enable CAN2 standby pin
    hal_io_do_set(CAN_EN3, 1);      // enable CAN2 transceiver
    hal_io_do_set(CAN_STB3, 1);     // enable CAN2 standby pin
    hal_io_do_set(DO_LIN_EN, 1);       // enable LIN transceiver

    ENABLE_INTERRUPTS();
}

void modulhardwarecode_timer(void)
{
    // empty
}

extern uint8_t multiplex_type_max;
void modulhardwarecode_adc_multiplex(uint8_t multiplex_nr)
{
    switch (multiplex_nr)
    {
        case 0:
            hal_io_do_set(DO_MUX_A_ANA, 0);
            hal_io_do_set(DO_MUX_B_ANA, 0);
            hal_io_do_set(DO_MUX_C_ANA, 0);

            break;

        case 1:
            hal_io_do_set(DO_MUX_A_ANA, 1);
            hal_io_do_set(DO_MUX_B_ANA, 0);
            hal_io_do_set(DO_MUX_C_ANA, 0);

            break;
        case 2:
            hal_io_do_set(DO_MUX_A_ANA, 0);
            hal_io_do_set(DO_MUX_B_ANA, 1);
            hal_io_do_set(DO_MUX_C_ANA, 0);

            break;

        case 3:
            hal_io_do_set(DO_MUX_A_ANA, 1);
            hal_io_do_set(DO_MUX_B_ANA, 1);
            hal_io_do_set(DO_MUX_C_ANA, 0);

            break;
        case 4:
            hal_io_do_set(DO_MUX_A_ANA, 0);
            hal_io_do_set(DO_MUX_B_ANA, 0);
            hal_io_do_set(DO_MUX_C_ANA, 1);

            break;
        case 5:
            hal_io_do_set(DO_MUX_A_ANA, 1);
            hal_io_do_set(DO_MUX_B_ANA, 0);
            hal_io_do_set(DO_MUX_C_ANA, 1);

            break;
        case 6:
            hal_io_do_set(DO_MUX_A_ANA, 0);
            hal_io_do_set(DO_MUX_B_ANA, 1);
            hal_io_do_set(DO_MUX_C_ANA, 1);

            break;
        case 7:
            hal_io_do_set(DO_MUX_A_ANA, 1);
            hal_io_do_set(DO_MUX_B_ANA, 1);
            hal_io_do_set(DO_MUX_C_ANA, 1);

            break;
        default:
            hal_io_do_set(DO_MUX_A_ANA, 0);
            hal_io_do_set(DO_MUX_B_ANA, 0);
            hal_io_do_set(DO_MUX_C_ANA, 0);

            break;
    }

}

/**
 * Public functions module_cyclic
 */
void modulhardwarecode_cyclic(void)
{
    // update shift register

        // Check if a shift register variable was changed
        // Disable store pin
        hal_io_do_set(DOX_SHIFT_ST_CP, 0);


        for(uint8_t pin = 0; pin < (SHIFT_OUTPUT_CHANNELS); pin++)
        {
            // loop through all pins.
            // NOTE: the order of the enums/pins must be identical to what it is on the hardware
            // the number of the enum has an offset of PIN_MAX but in virtual_pin addresses start at 0 to save memory

            // Set shift(clock) pin to low
            hal_io_do_set(DOX_SHIFT_SH_CP, 0);

            // Set the data
            hal_io_do_set(DOX_SHIFT_IN_DS, virtual_pin[pin].current_val);

            // Set shift(clock) pin to high (Shift register will take over the data)
            hal_io_do_set(DOX_SHIFT_SH_CP, 1);
        }


        // Enable storage pin
        hal_io_do_set(DOX_SHIFT_ST_CP, 1);

        // make sure the output is enabled
        hal_io_do_set(DOX_SHIFT_OE_N, 0);      // Output Enable - has to be low while operating
}

