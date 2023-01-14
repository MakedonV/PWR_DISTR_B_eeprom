#ifndef SRC_USER_API_IO_H_
#define SRC_USER_API_IO_H_
/*----------------------------------------------------------------------------*/
/**
* \file         user_api_io.h
* \brief        Declaration of IO functionality
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup     user_api_io IO
* \{
* \brief        This handles the IO functionality
* \details      All functions for working with IO are listed here. Functions to configure, read status and set the IO pins.
*               Pay attention to the configuration of PINS. Some pins could be preconfigured with pwm functionality and may need to be reconfigured.
*
*/
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include "hal_io.h"
#include "io_tables.h"


/*----------------------------------------------------------------------------*/
/**
* \brief    Set digital output pin
*
* \param    pin     [in] uint16_t       Ports & Interfaces: Digital Output Pin
* \param    state   [in] uint8_t        Boolean
*
*/
void user_do_set(uint16_t pin, uint8_t state);
#define user_io_set_do(pin ,state) user_do_set(pin ,state)

/*----------------------------------------------------------------------------*/
/**
* \brief    Read back the status of digital output pin
*
* \param    pin       [in] uint16_t     Ports & Interfaces: digital output pin
*
* \return   uint8_t                     Return the value of the digital output pin
*/
uint8_t user_do_readback(uint16_t pin);
#define user_io_readback_do(pin) user_do_readback(pin)

/*----------------------------------------------------------------------------*/
/**
* \brief    Get the status digital input pin
*
* \param    pin       [in] uint16_t     Ports & Interfaces: digital output pins
*
* \return   uint8_t                     Return the value of the digital input pin
*/
uint8_t user_di_get(uint16_t pin);
#define user_io_get_di(pin) user_di_get(pin)

/*----------------------------------------------------------------------------*/
/**
* \brief    Find the corresponding adc pin name for a given MCU pin
*
* \param    pin       [in] uint16_t     Ports & Interfaces: digital output pin
*
* \return   enum_adc_pin_name           The corresponding adc pin, or ADC_MAX if the given pin is not an ADC pin.
*/
enum_adc_pin_name user_di_find_adc_pin(uint16_t pin);

/** \} */
#endif /* SRC_USER_API_IO_H_ */
