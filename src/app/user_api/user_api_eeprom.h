#ifndef SRC_USER_API_EEPROM_H_
#define SRC_USER_API_EEPROM_H_
/*----------------------------------------------------------------------------*/
/**
* \file         user_api_eeprom.h
* \brief        Declaration of EEPROM functionality
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup     user_api_eeprom EEPROM
* \{
* \brief        This handles the EEPROM functionality
* \details      Provides functions for writing and reading the EEPROM
*/
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include "hal_nvm.h"

// user data EEPROM start address and size
#define EE_FACTORY_DATA_START	0								  //< start address of the EEPROM
#define EE_USER_START 			2048                              //< start address of user EEPROM
#define EE_USER_END   			4095                              //< end address of user EEPROM
#define EE_USER_SIZE  			(EE_USER_END - EE_USER_START + 1) //< total size of user EEPROM (+1 because byte 2048 is also part of it)

// return codes and errors are defined in hal_nvm.h

typedef enum
{
	PART_NR=0,
	DRAWING_NR,
	HW_TYPE,
	ORDER_NR,
	TEST_DATE,
	HW_VERSION,
	APP_NAME,
	APP_VERSION,

} enum_USER_ASCII_EEPROM_FIELD_NAME;

typedef enum
{
	EEPROM_VERSION=0,
	ID,
	MCU,
	HW_ACTIVE,
	BL_VERSION,
	RESET_COUNTER,
	PROG_STATUS,
	COP_WD_TIMEOUT,
	BL_CAN_BUS
} enum_USER_UINT16_EEPROM_FIELD_NAME;

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPORM read raw (blocking)
* \details  Read EEPROM data to ptr_data from address (ee_addr) with length (len)
*           32 bit aligned access is preferred, but 16 and 8 bit aligned also works
*           use typecasting e.g. (uint16_t*) ptr_data if your data has a different size.
*
* \param    ee_addr   [in] uint32_t         start address
* \param    len       [in] uint32_t         Length of data (Bytes)
* \param    *ptr_data [out] uint8_t         Pointer to where read data is saved
*
* \return   enum_HAL_NVM_RETURN_VALUE       Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_raw(uint32_t const ee_addr, uint32_t const len, uint8_t *const ptr_data);


/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM write (blocking)
* \details  Write EEPROM data (ptr_data) to EEPROM address (ee_addr) with the length (len)
*           32 bit aligned access is preferred, but 16 and 8 bit aligned also works
*           user writeable from EE_USER_START until EE_USER_START + EE_USER_SIZE
*           use typecasting e.g. (uint16_t*) ptr_data if your data has a different size
*
* \param    ee_addr   [in] uint32_t         Start address
* \param    len       [in] uint32_t         Length of data (Bytes)
* \param    *ptr_data [in] const uint8_t    Pointer to the write data
*
* \return   enum_HAL_NVM_RETURN_VALUE       Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_write_raw(uint32_t const ee_addr, uint32_t const len, uint8_t const *const ptr_data);


/*----------------------------------------------------------------------------*/
/**
* \brief    EEPORM read (blocking)
* \details  Read EEPROM data to ptr_data from address (ee_addr) with length (len)
*           32 bit aligned access is preferred, but 16 and 8 bit aligned also works
*           use typecasting e.g. (uint16_t*) ptr_data if your data has a different size.
*           This function will only read data from the user area of the EEPROM.
*
* \param    ee_addr   [in] uint32_t         start address
* \param    len       [in] uint32_t         Length of data (Bytes)
* \param    *ptr_data [out] uint8_t         Pointer to where read data is saved
*
* \return   enum_HAL_NVM_RETURN_VALUE       Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read(uint32_t const ee_addr, uint32_t const len, uint8_t *const ptr_data);


/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM write (blocking)
* \details  Write EEPROM data (ptr_data) to EEPROM address (ee_addr) with the length (len)
*           32 bit aligned access is preferred, but 16 and 8 bit aligned also works
*           user writeable from EE_USER_START until EE_USER_START + EE_USER_SIZE
*           use typecasting e.g. (uint16_t*) ptr_data if your data has a different size
*           This function will only write data to the user area of the EEPROM.
*
* \param    ee_addr   [in] uint32_t         Start address
* \param    len       [in] uint32_t         Length of data (Bytes)
* \param    *ptr_data [in] const uint8_t    Pointer to the write data
*
* \return   enum_HAL_NVM_RETURN_VALUE       Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_write(uint32_t const ee_addr, uint32_t const len, uint8_t const *const ptr_data);


/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read for 32 Bit targets (blocking)
* \details  this function behaves the same as user_eeprom_read() except that
*           it takes uint32_t data pointers
*           it can be used to avoid having to typecast yourself
*
* \param    ee_addr   [in] uint32_t const   start address
* \param    len       [in] uint32_t const   Length of data (Bytes)
* \param    *ptr_data [out] uint32_t const  Pointer to where read data is saved
* \return   enum_HAL_NVM_RETURN_VALUE       Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_32bit(uint32_t const ee_addr, uint32_t const len, uint32_t *const ptr_data);


/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM write for 32 Bit targets (blocking)
* \details  this function behaves the same as user_eeprom_write() except that
*           it takes uint32_t data pointers
*           it can be used to avoid having to typecast yourself
*
* \param    ee_addr   [in] uint32_t const   start address
* \param    len       [in] uint32_t const   Length of data (Bytes)
* \param    *ptr_data [in] uint32_t const   Pointer to where read data is saved
* \return   enum_HAL_NVM_RETURN_VALUE       Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_write_32bit(uint32_t const ee_addr, uint32_t const len, uint32_t const *const ptr_data);


/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM write for 32 Bit values (blocking)
* \details  similar to user_eeprom_read() this function directly reads values
*           instead of pointers
*           different to user_eeprom_read it returns the actual value not a
*           status code
*
* \param    ee_addr [in] uint32_t const     EEPROM address
* \return   uint32_t                        value read from ee_addr
*/
uint32_t user_eeprom_read_value_32bit(uint32_t const ee_addr);


/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM write for 32 Bit values (blocking)
* \details  similar to user_eeprom_write() this function directly writes values
*           instead of pointers
*           use typecasts if you want to use other data structures,
*           e.g. (int32_t) for signed integers
*
* \param    ee_addr [in] uint32_t const     EEPROM address
* \param    value   [in] uint32_t const     32 Bit value to be written
* \return   enum_HAL_NVM_RETURN_VALUE       Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_write_value_32bit(uint32_t const ee_addr, uint32_t const value);

//##################################-----------------------------------------------------------------------------------
//#                                #-----------------------------------------------------------------------------------
//#   factory data                 #
//#                                #-----------------------------------------------------------------------------------
//##################################-----------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the eeprom version of the module
* \details  This function will read the eeprom version from the
* 			EEPROM and return it as uint16_t value.
*
* \return   uint16_t          Returns the eeprom version of the module
*/
uint16_t user_eeprom_read_module_eeprom_version(void);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the id of the module
* \details  This function will read the id from the
* 			EEPROM and return it as uint16_t value.
*
* \return   uint16_t          Returns the id of the module
*/
uint16_t user_eeprom_read_module_id(void);


/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the serial number of the module
* \details  This function will read the serial number from the
* 			EEPROM and return it as uint32_t value.
*
* \return   uint32_t          Returns the serial number of the module
*/
uint32_t user_eeprom_read_module_serial_nr(void);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the device type number of the module
* \details  This function will read the device type number from the
* 			EEPROM and return it as uint32_t value.
*
* \return   uint32_t          Returns the device type number of the module
*/
uint32_t user_eeprom_read_module_device_type(void);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the part number of the module and remove unnecessary characters
* \details  This function will read the part number from the
* 			EEPROM and write it into the given buffer. Buffer should be an array with at least 21 elements.
*
* \param    *buffer     [in] uint8_t     		buffer to write the data into
* \param    size_buffer [in] uint8_t            size of the buffer, must be at least 21 bytes
* \param    filter      [in] bool     			FALSE to read the raw data or set TRUE to filter unnecessary characters (whitespace)
* \return   enum_HAL_NVM_RETURN_VALUE           Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_part_nr(uint8_t buffer[], uint8_t const size_buffer, bool const filter);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the drawing number of the module
* \details  This function will read the drawing number from the
* 			EEPROM and write it into the given buffer. Buffer should be an array with at least 21 elements.
*
* \param    *buffer     [in] uint8_t     		buffer to write the data into, size must be at least 21 bytes
* \param    size_buffer [in] uint8_t            size of the buffer, must be at least 21 bytes
* \param    filter      [in] bool               FALSE to read the raw data or set TRUE to filter unnecessary characters (whitespace)
* \return   enum_HAL_NVM_RETURN_VALUE           Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_drawing_nr(uint8_t buffer[], uint8_t const size_buffer, bool const filter);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the module name of the device
* \details  This function will read the module name from the
* 			EEPROM and write it into the given buffer. Buffer must be an array with at least 41 elements.
*
* \param    *buffer     [in] uint8_t     		buffer to write the data into
* \param    size_buffer [in] uint8_t            size of the buffer, must be at least 41 bytes
* \param    filter      [in] bool               FALSE to read the raw data or set TRUE to filter unnecessary characters (whitespace)
* \return   enum_HAL_NVM_RETURN_VALUE           Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_name(uint8_t buffer[], uint8_t const size_buffer, bool const filter);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the production order number of the module
* \details  This function will read the production order number from the
* 			EEPROM and write it in the given buffer. Buffer must be an array with at least 21 elements.
*
* \param    *buffer     [in] uint8_t     		buffer to write the data into
* \param    size_buffer [in] uint8_t            size of the buffer, must be at least 21 bytes
* \param    filter      [in] bool               FALSE to read the raw data or set TRUE to filter unnecessary characters (whitespace)
* \return   enum_HAL_NVM_RETURN_VALUE           Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_order_nr(uint8_t buffer[], uint8_t const size_buffer, bool const filter);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the test date of the module
* \details  This function will read the test date from the
* 			EEPROM and write it in the given buffer. Buffer must be an array with at least 13 elements.
*
* \param    *buffer     [in] uint8_t     		buffer to write the data into
* \param    size_buffer [in] uint8_t            size of the buffer, must be at least 13 bytes
* \param    filter      [in] bool               FALSE to read the raw data or set TRUE to filter unnecessary characters (whitespace)
* \return   enum_HAL_NVM_RETURN_VALUE           Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_test_date(uint8_t buffer[], uint8_t const size_buffer, bool const filter);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the mcu type of the module
* \details  This function will read the mcu type from the
* 			EEPROM and return it as uint16_t value.
*
* \return   uint16_t          Returns the mcu_type number of the module
*/
uint16_t user_eeprom_read_module_mcu_type(void);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the hardware version of the module
* \details  This function will read the hardware version from the
* 			EEPROM and write it in the given buffer. Buffer must be an array with at least 3 elements.
*
* \param    *buffer     [in] uint8_t     		buffer to write the data into
* \param    size_buffer [in] uint8_t            size of the buffer, must be at least 3 bytes
* \param    filter      [in] bool               FALSE to read the raw data or set TRUE to filter unnecessary characters (whitespace)
* \return   enum_HAL_NVM_RETURN_VALUE           Return code, see enum
*/

enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_hw_version(uint8_t buffer[], uint8_t const size_buffer, bool const filter);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the available can interfaces of the module
* \details  This function will read the available can interfaces from the
* 			EEPROM and return it as uint16_t value.
*
* \return   uint16_t          Returns the available can interfaces of the module
*/
uint16_t user_eeprom_read_module_hw_can_active(void);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the bootloader version of the module
* \details  This function will read the bootloader version from the
* 			EEPROM and return it as uint16_t value.
*
* \return   uint16_t          Returns the bootloader version of the module
*/
uint16_t user_eeprom_read_module_bootloader_version(void);

//##################################-----------------------------------------------------------------------------------
//#                                #-----------------------------------------------------------------------------------
//#   module data                  #
//#                                #-----------------------------------------------------------------------------------
//##################################-----------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the reset counter of the module
* \details  This function will read the reset counter from the
* 			EEPROM and return it as uint16_t value.
*
* \return   uint16_t          Returns the reset counter of the module
*/
uint16_t user_eeprom_read_module_reset_counter(void);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the reset reason of the module
* \details  This function will read the reset counter from the
* 			EEPROM and return it as uint16_t value. See the Applics Studio reference manual for further information.
*
* \return   uint16_t          Returns the reset reason of the module (coded in binary).
*/
uint8_t user_eeprom_read_module_reset_reason(void);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the program status of the module
* \details  This function will read the program status from the
* 			EEPROM and return it as uint16_t value. See the Applics Studio reference manual for further information.
*
* \return   uint16_t          Returns the program status of the module
*/
uint16_t user_eeprom_read_module_prog_status(void);

//##################################-----------------------------------------------------------------------------------
//#                                #-----------------------------------------------------------------------------------
//#   conifg data                  #
//#                                #-----------------------------------------------------------------------------------
//##################################-----------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read for app name
* \details  Read the application info app_name from EEPROM.
*           Usually done in usercode_init with data from dsl_cfg.h
*
* \param    *buffer     [in] uint8_t 		    buffer for the application name, size must be at least 21 bytes
* \param    size_buffer [in] uint8_t            size of the buffer, must be at least 21 bytes
* \param    filter      [in] bool               FALSE to read the raw data or set TRUE to filter unnecessary characters (whitespace)
* \return   enum_HAL_NVM_RETURN_VALUE       	Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_app_name(uint8_t* buffer, uint8_t const size_buffer, bool const filter);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read for app version.
* \details  Read the application info app_version from EEPROM.
*           Usually done in usercode_init with data from dsl_cfg.h
*
* \param    *buffer     [in] uint8_t            buffer for the application version
* \param    size_buffer [in] uint8_t            size of the buffer, must be at least 31 bytes
* \param    filter      [in] bool               FALSE to read the raw data or set TRUE to filter unnecessary characters (whitespace)
* \return   enum_HAL_NVM_RETURN_VALUE           Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_app_version(uint8_t* buffer, uint8_t const size_buffer, bool const filter);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM write for app info (name, version)
* \details  Write the app info (app_name and app_version) to EEPROM.
*           Usually done in usercode_init with defines from dsl_cfg.h
*
* \param    *app_name    [in] const uint8_t Pointer to app name (30 chars)
* \param    *app_version [in] const uint8_t Pointer to app version (20 chars)
* \return   enum_HAL_NVM_RETURN_VALUE       Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_write_app_info(const uint8_t *const app_name, const uint8_t *const app_version);

/*----------------------------------------------------------------------------*/
/**
* \brief    this manually resets the reset_counter to 0
* \details  The reset counter is normally automatically reset after a predefined time (60s)
*           With this function it can be manually reset. It can be used if your application
*           runs for a shorter time than the predefined time or if you want to implement your own
*           reset_counter handling.
*           If reset_counter reaches 51 the bootloader will stop the application and stay in bootloader.
*           Depending on the bootloader version, the reset_counter is incremented for different reasons.
*           Please refer to applics.dev or contact support for details.
* \return   enum_HAL_NVM_RETURN_VALUE       Return code, see enum
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_reset_reset_counter(void);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the cop watchdog timeout value of the module
* \details  This function will cop watchdog timeout value from the
*           EEPROM and return it as uint16_t value. See the Applics Studio reference manual for further information.
*
* \return   uint16_t          Returns the program status of the module
*/
uint16_t user_eeprom_read_module_cop_wd_timeout(void);

/*----------------------------------------------------------------------------*/
/**
* \brief    EEPROM read the instance of the bootloader CAN bus
* \details  This function will return the bootloader CAN bus instance from the
*           EEPROM. See the Applics Studio reference manual for further information.
*
* \return   uint16_t          Returns the EEPROM value for the bootloader CAN bus interface
*/
uint16_t user_eeprom_read_bl_can_bus(void);

//##################################-----------------------------------------------------------------------------------
//#                                #-----------------------------------------------------------------------------------
//#   helper functions             #
//#                                #-----------------------------------------------------------------------------------
//##################################-----------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
* \brief    Helper function that returns a read pointer to the given field name.
* \details  This function is used in order to get a read pointer for the appropriate
* member variable of the EEPORM structure. Specifically the members only with uint16 values
*
* \param	field_name [in] enum_USER_UINT16_EEPROM_FIELD_NAME	which field value should it point to
* \return   uint16_t*				      						Pointer to the struct member
*/
uint16_t* user_eeprom_ptr_to_uint16_field_member(enum_USER_UINT16_EEPROM_FIELD_NAME field_name);

/*----------------------------------------------------------------------------*/
/**
* \brief    Helper function that returns a read pointer to the given field name.
* \details  This function is used in order to get a read pointer for the appropriate
* member variable of the EEPORM structure. Specifically the members only with ASCII values
*
* \param	field_name [in] enum_USER_UINT16_EEPROM_FIELD_NAME	which field value should it point to
* \return   uint8_t*				      						Pointer to the struct member
*/
uint8_t* user_eeprom_ptr_to_char_field_member(enum_USER_ASCII_EEPROM_FIELD_NAME field_name);

/*----------------------------------------------------------------------------*/
/**
* \brief    Helper function that returns the length of an array member from the EEPROM structure
*
* \param	field_name [in] enum_USER_UINT16_EEPROM_FIELD_NAME	which field value should it point to
* \return   uint8_t					      						length of the array from the struct member
*/
uint8_t user_eeprom_length_of_field_member(enum_USER_ASCII_EEPROM_FIELD_NAME field_name);

/*----------------------------------------------------------------------------*/
/**
* \brief    Get the pointer to the struct member and fill the buffer with its value.
* \details  This helper function accepts the given buffer to write the value thats written
* in the EEPROM for the desired field name. Buffer should be sized accordingly, the function also
* appends an null termination control character to the buffer and filters unnecessary characters (whitespace) if desired.
*
* \param	field_name 	[in] enum_USER_UINT16_EEPROM_FIELD_NAME		which field value should it point to
* \param	*buffer		[in] uint8_t								buffer to write the data into, size must be appropriate for the field value
* \param    size_buffer [in] uint8_t                                size of the given buffer
* \param    filter      [in] bool                                   FALSE to read the raw data or set TRUE to filter unnecessary characters (whitespace)
* \return   uint16_t*				      							Pointer to the struct member
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_ascii_field(enum_USER_ASCII_EEPROM_FIELD_NAME field_name, uint8_t *buffer, uint8_t size_buffer, bool filter);

/** @} */ // end of doxygen group

#endif /* SRC_USER_API_EEPROM_H_ */

