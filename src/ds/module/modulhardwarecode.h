#ifndef SRC_MODULHARDWARECODE_H_
#define SRC_MODULHARDWARECODE_H_

/*--------------------------------------------------------------------------*/
/** \file           modulhardwarecode.h
*   \brief          Module specific functions
*   \date           21.12.2022 17:54:20  \author: ApplicsStudio
*   \version        1.00
*   \platform CPU:  S32K  | Compiler: GCC
* --------------------------------------------------------------------------*/
#include <stdint.h>

#include "io_tables.h"

#define AD_FILTER     	   1u //< The maximum amount of values the floating average should be calculated. Range 1..10
#define HW_CALIBRATION_SUPPORT 1u  //< enable code that handles calibration, like user_ai_get_cal(). only available on some hw.
#define ADC_SECOND_RANGE       1u
#define MULTIPLEX_STABILITY_CYCLES 20
#define SHIFT_OUTPUT_CHANNELS 21

typedef struct {
        enum_adc_pin_name   pin;
        uint32_t            vrefh2;
        enum_pin_name       range_switch;
}struct_ADC_VREF_values_t;

extern struct_ADC_VREF_values_t mgl_adc_vref_values[ADC_MAX + 1];

uint32_t modulhardwarecode_adc_get_vrefh2(enum_adc_pin_name const pin, uint32_t const default_range);
void modulhardwarecode_adc_set_measurement_range(enum_adc_pin_name const pin, uint8_t const range);
uint16_t modulhardwarecode_ai_get_temperature(enum_adc_pin_name pin);

void modulhardwarecode_init(void);
void modulhardwarecode_sleep(void);
void modulhardwarecode_timer(void);
void modulhardwarecode_adc_multiplex(uint8_t multiplex_nr);
void modulhardwarecode_cyclic(void);

#endif // SRC_MODULHARDWARECODE_H_