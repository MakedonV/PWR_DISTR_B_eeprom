#ifndef HAL_WATCHDOG_H
#define HAL_WATCHDOG_H
/*----------------------------------------------------------------------------*/
/**
* \file         hal_watchdog.h
* \brief        NO LONGER SUPPORTED
*/
/*----------------------------------------------------------------------------*/


// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "hal_data_types.h"

// ---------------------------------------------------------------------------------------------------
// defines
// ---------------------------------------------------------------------------------------------------
#define WATCHDOG_DEAD_SIGNAL_ITER_MAX	3u

#define WATCHDOG_KICK_REQUEST			( ((wdog_reg_t)1u) << (sizeof(wdog_reg_t)*8u-1) )

// ---------------------------------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------
/** This enum lists all possible errorcodes for HAL IO functions that can be returned.*/
typedef enum
{
    HAL_WATCHDOG_OK                 = 0u,   ///< No error.
	HAL_WATCHDOG_NOT_INITIALIZED        ,   ///< Watchdog not yet initialized.
	HAL_WATCHDOG_NOT_KICKED             ,   ///< Watchdog not kicked.
	HAL_WATCHDOG_INVALID_SIGNAL             ///< Attempt to mask or unmask a reserved signal bit

} enum_HAL_WATCHDOG_RETURN_VALUE;

// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------
//typedef struct
//{
//	uint8_t idx;
//	uint16_t watchdog_no;
//	uint8_t status;
//} watchdog_tbl_t;

typedef enum
{
    HAL_WATCHDOG_INTERNAL           = 0u,   ///< Internal Watchdog
    HAL_WATCHDOG_EXTERNAL               ,   ///< External Watchdog
} enum_HAL_WATCHDOG_TYPE;

typedef uint32_t wdog_reg_t;

typedef struct
{
	wdog_reg_t reg;							///< one signal per slot bit
	wdog_reg_t mask;						///< corresponding reset signal masks
	enum_HAL_WATCHDOG_TYPE wdt_type;	    ///< internal or external watchdog
} struct_watchdog_t;

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------

//void hal_watchdog_reset(uint8_t idx);
//
//
//void hal_watchdog_callback(void);
//void hal_watchdog_check_trigger(void);
//void hal_watchdog_tbl_clear(void);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Initializes watchdog module.
*   \details    Starts the watchdog
*
*   \pre The function #hal_watchdog_start must be called once before you can use this.
*
*   \return   enum_HAL_WATCHDOG_RETURN_VALUE
*/
enum_HAL_WATCHDOG_RETURN_VALUE hal_watchdog_start(enum_HAL_WATCHDOG_TYPE wdog_type, uint16_t pin);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Sets watchdog mask bit(s).
*   \details    The mask bits are used to control the signaled/non-signaled events on those bits.
*   			The watchdog kicking takes place only when all the mask bits become signaled.
*
*   \param      [in] #wdog_reg_t  One or more signal bits to mask
*
*   \return   enum_HAL_WATCHDOG_RETURN_VALUE
*/
enum_HAL_WATCHDOG_RETURN_VALUE hal_watchdog_mask(wdog_reg_t mask_bit);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Removes watchdog mask bit(s).
*   \details    The removed mask bits are no longer checked against their signaled states.
*
*   \param      [in] #wdog_reg_t  One or more signal bits to unmask
*
*   \return   enum_HAL_WATCHDOG_RETURN_VALUE
*/
enum_HAL_WATCHDOG_RETURN_VALUE hal_watchdog_unmask(wdog_reg_t mask_bit);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Signals one or more watchdog event bits.
*   \details	The functions attempts to do a kick if WATCHDOG_KICK_REQUEST bit
*   			is set in the signal_bit. The kick is possible only when all
*   			masked bits are found signaled.
*
*   \param      [in] #wdog_reg_t  Signal one or more bits
*
*   \return   enum_HAL_WATCHDOG_RETURN_VALUE
*/
enum_HAL_WATCHDOG_RETURN_VALUE hal_watchdog_signal(wdog_reg_t signal_bit);

#endif
