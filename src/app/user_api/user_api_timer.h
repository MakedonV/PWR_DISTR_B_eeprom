#ifndef SRC_USER_API_TIMER_H_
#define SRC_USER_API_TIMER_H_
/*----------------------------------------------------------------------------*/
/**
* \file         user_api_timer.h
* \brief        Declaration of TIMER functionality
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup     user_api_timer TIMER
* \{
* \brief        Provides functions to work with timers
* \details
*/
/*----------------------------------------------------------------------------*/

#include <stdint.h>

/** This enum lists all available resolutions that the system tick manager can handle.*/
typedef enum
{
    PRECISION_1US    = 0u,   ///< Precision is 1us.
    PRECISION_10US       ,   ///< Precision is 10us.
    PRECISION_100US      ,   ///< Precision is 100us.
    PRECISION_1MS        ,   ///< Precision is 1ms.
    PRECISION_10MS       ,   ///< Precision is 10ms.
    PRECISION_100MS      ,   ///< Precision is 100ms.
    PRECISION_1S             ///< Precision is 1s.
}enum_PRECISION;


/*----------------------------------------------------------------------------*/
/**
* \brief    Set the time at which the counting should start and with what precision
*
* \param    timestamp [in] uint32_t*        Pointer the the variable to store timer data
* \param    precision [in] enum_PRECISION   With what precision should be counted
*
* \return   uint8_t                         Return code
*/
uint8_t user_set_timestamp(uint32_t* timestamp, enum_PRECISION precision);
#define user_timer_set_timestamp( timestamp, precision ) user_set_timestamp( timestamp, precision )


/*----------------------------------------------------------------------------*/
/**
* \brief    Measure whether the desired time has passed
*
* \param    timestamp_t0 [in] uint32_t          Variable in wich current state of time is saved
* \param    span         [in] uint32_t          Time range to be measured
* \param    precision    [in] enum_PRECISION    With what precision should be counted,
*                                               precision should be the same as the one in user_set_timestamp()
*
* \return   uint8_t                             Return code
*/
uint8_t user_time_past(uint32_t timestamp_t0, uint32_t span, enum_PRECISION precision);
#define user_timer_time_elapsed( timestamp_t0,span,precision ) user_time_past( timestamp_t0,span,precision )


/*----------------------------------------------------------------------------*/
/**
* \brief    Measure the time interval between calls to this function
*
* \param    roundtrip_time [in] uint32_t*       Pointer the the variable to store timer data
* \param    precision      [in] enum_PRECISION  With what precision should be counted,
*                                               precision should be the same as the one in user_set_timestamp()
* \param    reset_start    [in] uint8_t         If a reset is carried out, measurements are taken again from this point in time
*
* \return   uint8_t                             Return code
*/
uint8_t user_get_roundtrip_time(uint32_t* roundtrip_time, enum_PRECISION precision, uint8_t reset_start);
#define user_timer_get_prog_cycletime( roundtrip_time, precision,reset_start ) user_get_roundtrip_time( roundtrip_time, precision,reset_start )


#endif /* SRC_USER_API_TIMER_H_ */
/** \} */
