#ifndef __ADC_APP_H_
#define __ADC_APP_H_
/*----------------------------------------------------------------------------*/
/**
* \file         adc_app.h
* \brief        Definition of ADC Application
* \date         20180108
* \author       MMueller
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup
* \brief
* \details
*/
/*----------------------------------------------------------------------------*/

#include "adConv1.h"
#include "pdb1.h"
#include "hal_io.h"
#include "pins_driver.h"
#include "io_tables.h" //can be deleted again, after the mean calc has been reworked
#include "modulhardwarecode.h"


// ===================================================================================================
// Defines
// ===================================================================================================

// ===================================================================================================
// Typedef
// ===================================================================================================
typedef struct
 {
    uint16_t a_data[AD_FILTER];
    uint8_t index_next_value;
    uint16_t amount_of_values;
 } floating_avg_data_t;       ///< Typedef that contains the data, which is used to calculate the floating average values.

typedef struct
{
    uint32_t current_value;
    uint16_t amount_values;
} floating_avg_values_data_t;

typedef struct
{
        uint16_t result_raw;
        uint16_t result_digit;      // the raw values from the adc. Cached to allow better synchronization with the other ADC values.
        uint16_t result_unit;     // values converted to unit e.g. mV//todo Kommentare anpassen
        uint16_t result_cal;      // calibrated values
        uint16_t result_filtered; // filtered calibrated values (if available). can be in mV or any other unit depending on the hardware.
}struct_adc_results_t;


extern struct_adc_results_t adc_results[ADC_MAX];


// ===================================================================================================
// Public function prototypes
// ===================================================================================================
void ADC_init(void);
void ADC1_IRQHandler(void);
void ADC2_IRQHandler(void);

void adc_processing(uint8_t *multiplex_group, bool hw_calibration_support);
uint16_t adc_get_module_type();

uint16_t get_mean_analog_value(enum_adc_pin_name analog_pin, uint8_t AmountValues);

uint16_t get_mean_analog_value_exact(enum_adc_pin_name analog_pin);

uint32_t adc_get_measurement_range(enum_adc_pin_name const pin);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
* This function is used to copy the ADC results from the ADC interrupt to a structure element which can be used for further processing
* without disabling the ADC interrupts again.
*
* \pre
*
* \param    adc_instance      [in] uint8_t
* \param    adc_done_flag     [in] uint8_t*
* \param    adc_counter       [in] uint32_t*
* \param    adc_interrupt     [in] IRQn_Type
* \return   void
*/
void adc_copy_results_from_interrupt(uint8_t adc_instance, uint8_t* adc_done_flag, uint32_t* adc_counter, IRQn_Type adc_interrupt );


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
* adds a value to the floating average array. The desired amount, over how many values the mean value is to be formed,
* can be passed with the parameter FloatAvgData.AmountOfValues included in the FloatingAvgData_t type.
*
* \pre
*
* \param    *FloatAvgData [in] FloatingAvgData_t
* \param    NewValue      [in] uint16_t
* \return   void
*/
void add_to_float_avg(floating_avg_data_t *float_avg_data, uint16_t new_value);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Function that returns the calculated output value of the floating average.
* \details
*
* \param    *FloatAvgData [in] FloatingAvgData_t
* \return   uint16_t
*/
uint16_t adc_calc_float_avg(floating_avg_data_t *float_avg_data);

#endif
