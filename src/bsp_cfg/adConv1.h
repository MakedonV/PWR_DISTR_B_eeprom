#ifndef adConv1_H
#define adConv1_H
/* MODULE adConv1. */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced
 * There are global macros defined to be used by the integrator and another one used as include guard.
 */

/* Include inherited beans */
#include "adc_driver.h"
#include "io_tables.h"


/*! @brief Device instance number */
#define INST_ADCONV1 (0U)
#define INST_ADCONV2 (1U)

/*! @brief Configuration declaration */
extern const adc_converter_config_t adConv0_ConvConfig0;


/*! adConv2 configuration structure */
extern const adc_converter_config_t adConv1_ConvConfig0;

/*! @brief ADC init helping structure
 * Implements : struct_adc_init_config_t
 */
typedef struct
{
        const adc_chan_config_t channel_cfg;   // Channel configuration
        enum_pin_name io_pin_name;              // resembling pin of the io table
}struct_adc_init_config_t;

/*! @brief Declaration of the ADC init config structure */
extern struct_adc_init_config_t adc_init_config_tbl[];

#endif
