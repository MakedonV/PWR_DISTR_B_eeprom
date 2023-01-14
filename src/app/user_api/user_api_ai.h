#ifndef SRC_USER_API_AI_H_
#define SRC_USER_API_AI_H_
/*----------------------------------------------------------------------------*/
/**
* \file         user_api_ai.h
* \brief        Declaration of analog input functionality
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup     user_api_ai ANALOG INPUT
* \{
* \brief
* \details
*/
/*----------------------------------------------------------------------------*/
#include "hal_data_types.h"
#include "io_tables.h"
#include "adc_app.h"


/*----------------------------------------------------------------------------*/
/**
* \brief    Function to select the measurement range of a AI input
* \details  If a AI input supports measurement range select this function will
*           set the AI configuration to support the desired measurement range
*
* \pre
*
* \param    pin   [in] enum_adc_pin_name    select a AI pin from io_tables.h (enum_adc_pin_name enumeration)
* \param    range [in] uint8_t              select the desired measurement range [AI_RANGE_16V] or [AI_RANGE_32V]
*
*/
void user_ai_set_measurement_range(enum_adc_pin_name pin, uint8_t range);


/*----------------------------------------------------------------------------*/
/**
* \brief    Get the value of analog input in digits
*
* \param    pin     [in] enum_adc_pin_name  Ports & Interfaces: Digital Output Pin
*
* \return   uint16_t                        Value of the analog input in digits
*/
uint16_t user_ai_get(enum_adc_pin_name pin);
#define user_ai_get_digits( pin ) user_ai_get( pin )


/*----------------------------------------------------------------------------*/
/**
* \brief    Get the value of analog input in millivolts [mV]
*
* \param    pin     [in] enum_adc_pin_name  Ports & Interfaces: Digital Output Pin
*
* \return   uint16_t                        Value of the analog input in mV
*/
uint16_t user_ai_get_mv(enum_adc_pin_name pin);


/*----------------------------------------------------------------------------*/
/**
* \brief    Get the value of analog inputs with calibration in millivolts [mV]
*           Note: this is limited to select channels on certain hardware.
*
* \param    pin     [in] enum_adc_pin_name  Ports & Interfaces: Digital Output Pin
*
* \return   uint16_t                        Value of the analog input in mV
*/
uint16_t user_ai_get_cal(enum_adc_pin_name pin);

/*----------------------------------------------------------------------------*/
/**
* \brief    Get the filtered value of an analog input.
*           Note: this is limited to select channels on certain hardware.
*
* \param    pin     [in] enum_adc_pin_name  Ports & Interfaces: Digital Output Pin
*
* \return   uint16_t                        Value of the analog input in mV
*/
uint16_t user_ai_get_filtered(enum_adc_pin_name pin);


#endif /* SRC_USER_API_AI_H_ */
/** \} */
