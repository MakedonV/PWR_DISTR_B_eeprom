/*
 * user_api_timer.c
 *
 *  Created on: 10.10.2019
 *      Author: VGoertz
 */

#include "user_api_timer.h"
#include "sfl_timer.h"

// 0 < no error
// 1 < presicion invalid


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_SFL_TIMER_ERR user_set_timestamp(uint32_t *timestamp, enum_PRECISION precision)
{
    return sfl_timer_set_timestamp(timestamp, precision);
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint8_t user_time_past(uint32_t timestamp_t0, uint32_t span, enum_PRECISION precision)
{
    static uint8_t elapsed = 0;

    (void)sfl_timer_time_elapsed(&elapsed, timestamp_t0, span, precision);

    return elapsed;
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_SFL_TIMER_ERR user_get_roundtrip_time(uint32_t *roundtrip_time, enum_PRECISION precision, uint8_t reset_start)
{
    return sfl_timer_get_roundtrip_time(roundtrip_time, precision);

    if(reset_start)
    {
        sfl_timer_init();
    }
}
