#ifndef HAL_IO_H
#define HAL_IO_H
/*----------------------------------------------------------------------------*/
/**
* \file         hal_io.h
* \brief        Definitions for hardware independent IO functions.
*/
/*----------------------------------------------------------------------------*/
/** 
* \addtogroup   doc_hal_io HAL IO
* @{
* \brief        This documentation part describes the interface function definitions for a hardware-independent and abstracted IO interface.
* \details      Main goal is to create a hardware-platform independent layer, that can be accessed from different HW-platforms.
*               IOs handle the main access of digital and analog inputs and outputs of a system. So the following functionalities are given:
*               - Read digital inputs
*               - Read analog inputs
*               - Write digital outputs 
*               - Write analog outputs
*/
/*----------------------------------------------------------------------------*/





// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "hal_data_types.h"
#include "pins_driver.h"

// ---------------------------------------------------------------------------------------------------
// defines
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------
/** This enum lists all possible errorcodes for HAL IO functions that can be returned.*/
typedef enum
{
    HAL_IO_OK                       = 0u,   ///< No error.
    HAL_IO_ERROR_GENERAL                ,   ///< Common error.
    HAL_IO_ERROR_INIT_FAILED            ,   ///< Initialization failed.
    HAL_IO_ERROR_DEINIT_FAILED          ,   ///< De-initialization failed.
    HAL_IO_ERROR_CHANNEL_INVALID        ,   ///< Channel invalid e.g. channel not opened yet.
    HAL_IO_ERROR_NO_CHANNEL_LEFT        ,   ///< No channel left on device.
    HAL_IO_ERROR_NO_DO                  ,   ///< Pin is not configured as digital output.
    HAL_IO_ERROR_NO_DI                  ,   ///< Pin is not configured as digital input.
    HAL_IO_ERROR_NOT_IMPLEMENTED        ,   ///< Error function not implemented.
	HAL_IO_ERROR_SET_PIN				,	///< Value for set pin is undefined.
	HAL_IO_ERROR_SET_DIRECTION			,	///< Value direction is undefined.
    HAL_IO_ERROR_ADC_MAX_NOT_DEFINED        ///< Adc max value is undefined.

} enum_HAL_IO_RETURN_VALUE;


typedef enum
{
    PORT_A      = 0,
    PORT_B         ,
    PORT_C         ,
    PORT_D         ,
    PORT_E         ,
    PORT_MAX
}enum_port_name;


// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
*   \brief      Initialize the IO management
*   \details    This function has to be called once after init to use the io functionality of a system.
*
*   \param    num_pins [in] Number of pins to initialize. This must be smaller or equal to the length of
*                           g_pin_mux_InitConfigArr defined in io_tables.h
*       
*   \return   Result of the function: Either be zero for success or a positive number from #enum_HAL_IO_RETURN_VALUE as error code. 
*/
enum_HAL_IO_RETURN_VALUE hal_io_init(const uint16_t num_pins);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Get the number of pins that have been configured by hal_io_init.
*   \details    When hal_io_init was not called before, this function will simply return 0.
*
*   \param    num_pins [out] the number of initialized pins will be written here
*       
*   \return   nothing, this cannot fail
*/
void hal_io_get_pin_count(uint16_t *const num_pins);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Change configuration of a single pin.
*   \details    This will also update g_pin_mux_InitConfigArr in io_tables.h
* 
*   \param    ptr_pins [in] pointer to a single pin configuration struct declared in src\bsp\sdk\platform\drivers\inc\pins_driver.h
*   \param    num_pins [in] index of the pin to configure with the given config
*
*   \return   Result of the function: Either be zero for success or a positive number from #enum_HAL_IO_RETURN_VALUE as error code.
*/
enum_HAL_IO_RETURN_VALUE hal_io_init_pin(pin_settings_config_t const *const ptr_pin, const uint16_t pin_ind);

/*----------------------------------------------------------------------------*/
/**
*   \details    Release the IO functionality.
*   \brief      Use this function to clean up all used ressources.
*   \details    Use this function to de-initialize the io functionality of the system.
*
*   \return   Result of the function: Either be zero for success or a positive number from #enum_HAL_IO_RETURN_VALUE as error code. 
*/
enum_HAL_IO_RETURN_VALUE hal_io_deinit(void);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Read a digital port input pin.
*   \details    Use this function to perform a read from a given input pin. Depending on the implementation this can be direct register access or a value that was sampled before.
*
*   \pre  The function #hal_io_init must be called once before you can use this.
*
*   \param    pin       [in] uint16_t Port pin identifier from which to read. Mostly they are defined in /config/Dio_Cfg.h
*   \param    ptr_state [in] uint8_t* Pointer to returned state of port pin:  1: corresponding port pin state is logical '1';
*                                                                               0: corresponding port pin state is logical '0'
*   \return   Result of the function: Either be zero for success or a positive number from #enum_HAL_IO_RETURN_VALUE as error code. 
*/
enum_HAL_IO_RETURN_VALUE hal_io_di_get(const uint16_t pin, uint8_t *const ptr_state);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Read the actual state of the output port pin. 
*   \details    Use this function to see if the given output pin is actually enabled (high; 1) or disabled ( low, 0).
*
*   \pre The function #hal_io_init must be called once before you can use this.
*
*   \param    pin       [in] uint16_t Port pin identifier from which to read. Mostly they are defined in /config/Dio_Cfg.h
*   \param    ptr_state [in] uint8_t* Pointer to returned state of port pin: 1: corresponding port pin state is logical '1';
*                                                                              0: corresponding port pin state is logical '0'
*   \return   Result of the function: Either be zero for success or a positive number from #enum_HAL_IO_RETURN_VALUE as error code. 
*/
enum_HAL_IO_RETURN_VALUE hal_io_do_readback(const uint16_t pin, uint8_t *const ptr_state);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Write a digital output pin.
*   \details    Use this function to enable(1,high) or disable(0,low) a digital output port pin. 
*
*   \pre The function #hal_io_init must be called once before you can use this.
*
*   \param    pin       [in] uint16_t Port pin identifier from which to read. Mostly they are defined in /config/Dio_Cfg.h
*   \param    state     [in] uint8_t      1: corresponding port pin state is logical '1';
*                                       0: corresponding port pin state is logical '0'
*   \return   Result of the function: Either be zero for success or a positive number from #enum_HAL_IO_RETURN_VALUE as error code. 
*/
enum_HAL_IO_RETURN_VALUE hal_io_do_set(const uint16_t pin, const uint8_t state);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Change (toggle) the state of a digital output port pin.
*   \details    When output is off, this function turns it on. Otherwise when it's on, it will be turned off.
*
*   \pre The function #hal_io_init must be called once before you can use this.
*
*   \param    pin       [in] uint16_t Port pin identifier from which to read. Mostly they are defined in /config/Dio_Cfg.h
*   \return   Result of the function: Either be zero for success or a positive number from #enum_HAL_IO_RETURN_VALUE as error code. 
*/
enum_HAL_IO_RETURN_VALUE hal_io_do_toggle(const uint16_t pin);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Read an analog input port pin in raw adc digits.
*   \details    This function performs an ADC input pin read for given channel. 
*               If this function is a direct adc sample or reads from a pre sampled value depends on the implementation.
*
*   \pre The function #hal_io_init must be called once before you can use this.
*
*   \param    pin       [in] uint16_t ADC channel identifier from where to read. Mostly they are defined in /config/Adc_Cfg.h.
*   \param    ptr_val   [in] uint16_t* Pointer to returned value of analogue pin in digits
*   \return   Result of the function: Either be zero for success or a positive number from #enum_HAL_IO_RETURN_VALUE as error code. 
*/
enum_HAL_IO_RETURN_VALUE hal_io_ai_get_digits(const uint16_t pin, uint16_t *const ptr_val);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Read an analog input port pin in mV.
*   \details    This function performs an ADC input pin read for given channel. 
*               If this function is a direct adc sample or reads from a pre sampled value depends on the implementation.
*
*   \pre The function #hal_io_init must be called once before you can use this.
*
*   \param    pin       [in] uint16_t ADC channel identifier from where to read. Mostly they are defined in /config/Adc_Cfg.h.
*   \param    ptr_val   [in] uint16_t* Pointer to returned value of analog pin in mV.
*   \return  Result of the function: Either be zero for success or a positive number from #enum_HAL_IO_RETURN_VALUE as error code. 
*/
enum_HAL_IO_RETURN_VALUE hal_io_ai_get_mV(const uint16_t pin, uint16_t *const ptr_val);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Set analog output value in digits.
*   \details    This function performs a write on digital-analog-converter for given channel. 
*
*   \pre The function #hal_io_init must be called once before you can use this.
*
*   \param    pin   [in] uint16_t DAC channel identifier from where to read. Mostly they are defined in /config/Dac_Cfg.h.
*   \param    val   [in] uint16_t Analog output value in digits.
*   \return   Result of the function: Either be zero for success or a positive number from #enum_HAL_IO_RETURN_VALUE as error code. 
*/
enum_HAL_IO_RETURN_VALUE hal_io_ao_set_digits(const uint16_t pin, const uint16_t val);


/*----------------------------------------------------------------------------*/
/**
*   \brief      Set analog output value in mV.
*   \details    This function performs a write on digital-analog-converter for given channel. 
*
*   \pre The function #hal_io_init must be called once before you can use this.
*
*   \param    pin   [in] uint16_t DAC channel identifier from where to read. Mostly they are defined in /config/Dac_Cfg.h.
*   \param    val   [in] uint16_t Analog output value in mV.
*   \return   Result of the function: Either be zero for success or a positive number from #enum_HAL_IO_RETURN_VALUE as error code. 
*/
enum_HAL_IO_RETURN_VALUE hal_io_ao_set_mV(const uint16_t pin, const uint16_t val);


/*----------------------------------------------------------------------------*/
/**
* \brief    Function to set direction of a single pin
* \details
*
* \pre
*
* \param    pin                      [in] uint16_t	pin name of the pin which direction is changed
* \param    direction                [in] uint8_t	input or output direction
* \return   enum_HAL_IO_RETURN_VALUE
*/
enum_HAL_IO_RETURN_VALUE hal_io_pin_set_direction(const uint16_t pin, const uint8_t direction);


/*----------------------------------------------------------------------------*/
/**
* \brief    Function to set the direction of a port
* \details	This function sets the direction configuration for all pins
 * 			in a port. Pins corresponding to bits with value of '1' will be configured as
 * 			output and pins corresponding to bits with value of '0' will be configured as
 * 			input.
 *
*
* \pre
*
* \param    port                     [in] uint16_t	port name of the port which direction is changed
* \param    pins_direction           [in] uint32_t	bit coded direction of the pins.
* 													0 = will be configured as input
* 													1 = will be configured as output
* \return   enum_HAL_IO_RETURN_VALUE
*/
enum_HAL_IO_RETURN_VALUE hal_io_port_set_direction(const uint16_t port, const uint32_t pins_direction);


/*----------------------------------------------------------------------------*/
/**
* \brief    Function to write a port
* \details  This function writes all pins configured as output with the values given in
* 			the parameter data. '0' represents LOW, '1' represents HIGH.
*
* \pre
*
* \param    port                     [in] uint16_t	port name of the port which is written
* \param    data                     [in] uint32_t	desired values of the pins
* \return   enum_HAL_IO_RETURN_VALUE
*/
enum_HAL_IO_RETURN_VALUE hal_io_do_port_write(const uint16_t port, const uint32_t data);


/*----------------------------------------------------------------------------*/
/**
* \brief    Function to read a port
* \details  This function returns the current input values from a port. Only pins
* 			configured as input will have meaningful values.
*
* \pre
*
* \param    port                     [in] uint16_t	port name of the port which is read
* \param    ptr_state                [in] uint32_t*	value of the port
* \return   enum_HAL_IO_RETURN_VALUE
*/
enum_HAL_IO_RETURN_VALUE hal_io_di_port_read(const uint16_t port, uint32_t *const ptr_state);


 /** @}*/
#endif
