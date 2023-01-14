/*----------------------------------------------------------------------------*/
/**
* \file         sfl_timer.c
* \brief        Implementation for software driven time span calculations. 
* \date         20160720
* \author       Harter
*/
/*----------------------------------------------------------------------------*/
/**
* \addtogroup  sfl_timer
* \{
*/
// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------

#include "sfl_timer.h"

// ---------------------------------------------------------------------------------------------------
// module globals
// ---------------------------------------------------------------------------------------------------
static uint8_t mgl_roundtrip_init_flg = FALSE;      ///< Global flag that indicates if initialization for roundtrip already performed.

/*----------------------------------------------------------------------------*/
/**
* \internal
* \endinternal
*
*/
void sfl_timer_init(void)
{
	mgl_roundtrip_init_flg = FALSE;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* \endinternal
*
*/
void sfl_timer_deinit(void)
{
	//write de-initialization code for sfl timer here
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* \endinternal
*
*/
enum_SFL_TIMER_ERR sfl_timer_set_timestamp(uint32_t* timestamp, enum_HAL_PRECISION precision)
{
	//local variables
	volatile enum_SFL_TIMER_ERR ret_val = SFL_TIMER_ERR_OK;

	//get timestamp
	if (hal_get_timestamp(timestamp, precision) != HAL_TICK_OK)
	{
		ret_val = SFL_TIMER_ERR_PRECISION_INVALID;
	}

	else
	{
		//do nothing
	}

	return ret_val;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* implementation of sfl_timer_get_time_elapsed function which calculates time elapsed
* between timestamp_t0 and timestamp_t1
* \endinternal
*
*/
enum_SFL_TIMER_ERR sfl_timer_get_time_elapsed(uint32_t* time_elapsed, uint32_t timestamp_t0, enum_HAL_PRECISION precision)
{
	//local variables
	volatile enum_SFL_TIMER_ERR ret_val = SFL_TIMER_ERR_GENERAL;
	uint32_t timestamp_now = 0;

	//get timestamp
	if (hal_get_timestamp(&timestamp_now, precision) != HAL_TICK_OK)
	{
		ret_val = SFL_TIMER_ERR_PRECISION_INVALID;
	}
	else
	{
	    // if overflow or not: get the difference of timestamp
	    *time_elapsed = timestamp_now - timestamp_t0;

        // check if we had an overflow
        if( timestamp_t0 > timestamp_now )
        {
            // timestamp is from future, we had an overflow
            ret_val = SFL_TIMER_ERR_OVERFLOW;
        }
        else
        {
            ret_val = SFL_TIMER_ERR_OK;
        }
	}

	return ret_val;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* \endinternal
*
*/
enum_SFL_TIMER_ERR sfl_timer_time_elapsed(uint8_t* elapsed, uint32_t timestamp_t0, uint32_t span, enum_HAL_PRECISION precision)
{
	//local variables
	volatile enum_SFL_TIMER_ERR ret_val = SFL_TIMER_ERR_GENERAL;
	uint32_t time_elapsed = 0;

	//get time elapsed
	ret_val = sfl_timer_get_time_elapsed(&time_elapsed, timestamp_t0, precision);
	if( (ret_val == SFL_TIMER_ERR_OK)  || (ret_val == SFL_TIMER_ERR_OVERFLOW) )
	{
		//time elapsed ?
		if( time_elapsed >= span )
		{
			*elapsed = TRUE;
		}

		else
		{
			*elapsed = FALSE;
		}
	}
	else
	{
	    // leave ret_val as it is by native error code
	}

	return ret_val;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* \endinternal
*/
enum_SFL_TIMER_ERR sfl_timer_wait(uint32_t wait_time, enum_HAL_PRECISION precision)
{
    //local variables
    uint32_t act_time = 0;
    uint8_t elapsed = FALSE;

    //get actual time
    sfl_timer_set_timestamp(&act_time, precision);

    //time elapsed ?
    while (elapsed == FALSE)
    {
        (void) sfl_timer_time_elapsed(&elapsed, act_time, wait_time, precision);
    }

    return SFL_TIMER_ERR_OK;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* \endinternal
*
*/
enum_SFL_TIMER_ERR sfl_timer_get_roundtrip_time(uint32_t* roundtrip_time, enum_HAL_PRECISION precision)
{
    //local variables
    volatile enum_SFL_TIMER_ERR ret_val = SFL_TIMER_ERR_OK;
    static uint32_t timestamp_t0 = 0u;
    static uint32_t timestamp_t1 = 0u;

    //init ?
    if (mgl_roundtrip_init_flg == FALSE)
    {
        //get timestamp t0
        if (hal_get_timestamp(&timestamp_t0, precision) != HAL_TICK_OK)
        {
            ret_val = SFL_TIMER_ERR_PRECISION_INVALID;
        }

        else
        {
            //calculate roundtrip time 1st time
            *roundtrip_time = timestamp_t0;

            //set init flg
            mgl_roundtrip_init_flg = TRUE;
        }
    }

    else
    {
        //get timestamp t1
        if (hal_get_timestamp(&timestamp_t1, precision) != HAL_TICK_OK)
        {
            ret_val = SFL_TIMER_ERR_PRECISION_INVALID;
        }

        else
        {
            //over flow ?
            if (timestamp_t0 < timestamp_t1)
            {
                //calculate roundtrip time
                *roundtrip_time = timestamp_t1 - timestamp_t0;
            }

            else
            {
                *roundtrip_time = timestamp_t0 - timestamp_t1;
            }

            //take over
            timestamp_t0 = timestamp_t1;
        }
    }

    return ret_val;
}

/** \} */
