#ifndef SFL_TIMER_H
#define SFL_TIMER_H
/*----------------------------------------------------------------------------*/
/**
* \file         sfl_timer.h
* \brief        Definitions for software driven time span calculations. 
*
*/
/*----------------------------------------------------------------------------*/
/**
* \addtogroup   sfl_timer SFL Timer [TIME]
* \{
* \brief        Provides Timestamps, Timespans and and cyclic caller time distance checks.
* \details      The functions are for checking if a given time is over or to save points of time.
*               This functionality is not based on a RTC, just timer ticks of local oscillator.
*               It is just a hardware (OSC-Freq, PLL. Temperature,...) and config Dependant "timer ticks" value.
*               So it is possible to drift and to float to environmental changes.
*   Example:
*   -------
*   Here a timer is used to check if 1000ms is over since last run.
*   The code in the if statement is executed every second once.
* \code{.c}
*    
*    uint32_t time_elapsed = FALSE;
*    uint32_t timestamp;
*    
*    // preset timestamp
*    (void)sfl_timer_set_timestamp(&timestamp, HAL_PRECISION_1MS);
*    
*    while(1)
*    {
*        // Check if time is over
*        (void)sfl_timer_time_elapsed(&time_elapsed, timestamp, 1000u, HAL_PRECISION_1MS);
*        if( time_elapsed == TRUE )
*        {
*            // Time is over, do something
*            ...
*            
*            // Dont forget to reset the value, or event happens periodic
*            (void)sfl_timer_set_timestamp(&timestamp, HAL_PRECISION_1MS);
*        }
*    }
* \endcode
*   For version information see file sfl_timer_version.h    
*/
/*----------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "hal_tick.h"

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------
/** 
*   This enum lists all possible errorcodes that can happen at sfl_timer functions.
*/
typedef enum
{
    SFL_TIMER_ERR_OK									=0u,    ///< No error
	SFL_TIMER_ERR_PRECISION_INVALID,						    ///< Presicion invalid
	SFL_TIMER_ERR_OVERFLOW,                                     ///< Information that an overflow of timer might happened. Then the factor is unknown but the modulo 2^32 value fits

    SFL_TIMER_ERR_GENERAL
} enum_SFL_TIMER_ERR;

// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
* \brief    Initializes the timer module
* \details  This is just needed for roundtrip calculations.
*
* \return   void
*/
void sfl_timer_init(void);

/*----------------------------------------------------------------------------*/
/**
* \brief    Uninitializes the timer module.
* \details  Unimplemented at this time.
*
* \return   void
*/
void sfl_timer_deinit(void);

/*----------------------------------------------------------------------------*/
/**
* \brief    Save actual timestamp
* \details  Save the actual time-resolution depending (precision) timestamp into a given variable.
*           Remember that a timestamp does not use physical units like ms. 
*
* \param    timestamp          [out] uint32_t* Pointer to variable which saves the actual timestamp.
* \param    precision          [in] enum_HAL_PRECISION Used time resolution.
* \return   enum_SFL_TIMER_ERR feedback of function
*/
enum_SFL_TIMER_ERR sfl_timer_set_timestamp(uint32_t* timestamp, enum_HAL_PRECISION precision);

/*----------------------------------------------------------------------------*/
/**
* \brief    Reads the time span between now and given timestamp_t0. 
* \details  Calculates the difference between given timestamp_t0 and a temporary actual read timestamp.
*           The difference is written into time_elapsed variable. 
*           On cyclic actions, remember to reset the timestamp, or event will just happen once.
*
* \param    time_elapsed       [out] uint32_t*   Pointer to calculated difference between timestamp_t0 and now. Resolution is given at param precision.
* \param    timestamp_t0       [in] uint32_t    Timestamp_t0, which was previous saved. You can get this value from sfl_timer_set_timestamp() function.
* \param    precision          [in] enum_HAL_PRECISION Used time resolution. Remember to use same value for timestamp_t0 and here.
* \return   enum_SFL_TIMER_ERR Feedback of function.
*/
enum_SFL_TIMER_ERR sfl_timer_get_time_elapsed(uint32_t* time_elapsed, uint32_t timestamp_t0, enum_HAL_PRECISION precision);

/*----------------------------------------------------------------------------*/
/**
* \brief    Checks if a given timespan is elapsed.
* \details  Calculates the difference between timestamp_t0 and now. Then checks if the difference is greater than span.
*           The result is written into elapsed variable. On cyclic actions, remember to reset the timestamp, or event will just happen once.
*
* \param    elapsed            [out] uint8_t* Pointer result TRUE: time is elapsed, FALSE: time is not elapsed
* \param    timestamp_t0       [in] uint32_t timestamp_t0, which was previous saved
* \param    span               [in] uint32_t time span to be waited for
* \param    precision          [in] enum_HAL_PRECISION Used time resolution.
* \return   enum_SFL_TIMER_ERR Feedback of function
*/
enum_SFL_TIMER_ERR sfl_timer_time_elapsed(uint8_t* elapsed, uint32_t timestamp_t0, uint32_t span, enum_HAL_PRECISION precision);

/*----------------------------------------------------------------------------*/
/**
* \brief    Internal Function. DO NOT USE =)
* \details  This is a blocking wait function to handle very short delays. Do not use in your code.
*
* \param    wait_time          [in] uint32_t Time that have to be waited.
* \param    precision          [in] enum_HAL_PRECISION Resolution of time units given from wait_time.
* \return   enum_SFL_TIMER_ERR Feedback of function
*/
enum_SFL_TIMER_ERR sfl_timer_wait(uint32_t wait_time, enum_HAL_PRECISION precision);

/*----------------------------------------------------------------------------*/
/**
* \brief    Calculate the roundtrip time.
* \details  Calculates the time span between last and actual call. 
*
* \param    roundtrip_time     [out] uint32_t* Pointer to variable that holds the calculated time. Resolution is given at param precision.
* \param    precision          [in] enum_HAL_PRECISION enum_HAL_PRECISION Used time resolution.
* \return   enum_SFL_TIMER_ERR Feedback of function
*/
enum_SFL_TIMER_ERR sfl_timer_get_roundtrip_time(uint32_t* roundtrip_time, enum_HAL_PRECISION precision);

/** \} */
#endif
