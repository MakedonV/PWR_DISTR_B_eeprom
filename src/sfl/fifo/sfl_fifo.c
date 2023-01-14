/*----------------------------------------------------------------------------*/
/**
* \file         sfl_fifo.c
* \brief        Implements the sfl- fifo.
* \details      This implementation is used for the First-In-First-Out ( order-saving ringbuffer) Buffer.
* \date         20161130
* \author       FSetz
*
*/
/*----------------------------------------------------------------------------*/
#include "sfl_fifo.h"
#include "hal_sys.h"
#include <string.h>

// we are using a ring buffer. For this, we only rotate in one direction.


// USE AS BUFF SIZE THE SAME NUMBER AS ARRAY SIZE!!!!




/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*
* \test STATUS: *** TESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20161201   | Author  | FSetz
*/

enum_SFL_FIFO_ERROR_CODES sfl_fifo_init(SFL_FIFO_CONFIG_TYPE* config, uint32_t element_count, uint32_t element_size)
{
    enum_SFL_FIFO_ERROR_CODES retval = SFL_FIFO_ERROR_NONE;
    if( (config == 0)  || (element_count < 1) || (element_size < 1) )
    {
        retval = SFL_FIFO_ERROR_PARAM;
    }
    else
    {
        config->count = 0;
        config->max = element_count;
        config->next_write = 0;
        config->next_read = 0;
        config->write_in_progress = 0;
        config->read_in_progress = 0;
        config->read_lock = SFL_FIFO_READLOCK_DISABLED;
        config->element_size = element_size;
    }
    return retval;
}



// return: convert this to the a datatype that fits to your fifo size. Max size is 2^32 -1
/*----------------------------------------------------------------------------*/
/**
* \internal
*   // No need to check if actual are in write mode, because the value is never too high.
    // The count is first counted up when already valid data inserted.
* \endinternal
*
*
* \test STATUS: *** TESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20161201   | Author  | FSetz
*/
uint32_t sfl_fifo_get_count(SFL_FIFO_CONFIG_TYPE* config)
{
    uint32_t ret = 0;
    ret = config->count;
    return ret;
}




// This is for getting a lock, that nobody can perform a get( and remove an item from list)
/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*
* \test STATUS: *** TESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20161201   | Author  | FSetz
*/
void sfl_fifo_lock_read(SFL_FIFO_CONFIG_TYPE* config)
{
    hal_sys_disable_all_interrupts();
    config->read_lock = SFL_FIFO_READLOCK_ENABLED;
    hal_sys_enable_all_interrupts();
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*
* \test STATUS: *** TESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20161201   | Author  | FSetz
*/
void sfl_fifo_unlock_read(SFL_FIFO_CONFIG_TYPE* config)
{
    hal_sys_disable_all_interrupts();
    config->read_lock = SFL_FIFO_READLOCK_DISABLED;
    hal_sys_enable_all_interrupts();
}




/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*
* \test STATUS: *** TESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20161201   | Author  | FSetz
*/
enum_SFL_FIFO_READLOCK_TYPE sfl_fifo_get_read_lock(SFL_FIFO_CONFIG_TYPE* config)
{
    enum_SFL_FIFO_READLOCK_TYPE ret;
    hal_sys_disable_all_interrupts();
    ret = config->read_lock;
    hal_sys_enable_all_interrupts();
    return  ret;
}



/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*
* \test STATUS: *** TESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20161201   | Author  | FSetz
*/
enum_SFL_FIFO_ERROR_CODES sfl_fifo_put(SFL_FIFO_CONFIG_TYPE* config, uint8_t* new_data, uint8_t* buff )
{
    enum_SFL_FIFO_ERROR_CODES retval = SFL_FIFO_ERROR_UNKNOWN;
    // check if another thread already write to this buffer. And avoid this.
    hal_sys_disable_all_interrupts();
    if(config->write_in_progress == 0 )
    {
        //multithreading lock
        config->write_in_progress = 1;

        // check if queue is full
        if( (config->count +1) > config->max )
        {
            retval = SFL_FIFO_ERROR_OVERFLOW;
        }
        else
        {

            // copy the new content
            uint8_t* tempbuffer=buff;
            tempbuffer+=config->next_write * config->element_size;
            memcpy(tempbuffer, new_data, config->element_size);

            //manage next write position and reset when at end
            (config->next_write)++;
            if(config->next_write >= config->max)
            {
               (config->next_write) = 0;
            }
            else
            {
                // do not reset the write pointer
            }
            // increment element counter
            (config->count)++;

            retval = SFL_FIFO_ERROR_NONE;
        }
        // deactivate lock
        config->write_in_progress = 0;
    }
    else
    {
        retval = SFL_FIFO_ERROR_BUSY;
    }
    hal_sys_enable_all_interrupts();
    return retval;
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*
* \test STATUS: *** TESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20161201   | Author  | FSetz
*/
enum_SFL_FIFO_ERROR_CODES sfl_fifo_get(SFL_FIFO_CONFIG_TYPE* config, uint8_t* result, uint8_t* buff )
{
    enum_SFL_FIFO_ERROR_CODES retval = SFL_FIFO_ERROR_UNKNOWN;
    // check in another thread is actually reading
    hal_sys_disable_all_interrupts();
    if( config->read_in_progress == 0 )
    {
        // set lock for singleton read
        config->read_in_progress = 1;
        // Check if data in buffer
        if( (config->count) > 0 )
        {

            // get the data
        	uint8_t* tempbuffer=buff;
			tempbuffer+=config->next_read * config->element_size;
			memcpy(result, tempbuffer, config->element_size);

            // adapt next read pos
            (config->next_read)++;
            // reset read position if max reached.
            if(config->next_read >= config->max )
            {
                config->next_read = 0;
            }
            else
            {
                // do not reset read position
            }
            (config->count)--;

            retval = SFL_FIFO_ERROR_NONE;
        }
        else
        {
            // no data available to read. User should have used sfl_fifo_get_count() before.
            retval = SFL_FIFO_ERROR_EMPTY;
        }
        // release singleton read lock.
        config->read_in_progress = 0;
    }
    else
    {
        retval = SFL_FIFO_ERROR_BUSY;
    }
    hal_sys_enable_all_interrupts();
    return retval;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*
* \test STATUS: *** TESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20200204   | Author  | FZeller
*/
enum_SFL_FIFO_ERROR_CODES sfl_fifo_get_preview(SFL_FIFO_CONFIG_TYPE* config, uint8_t* result, uint8_t* buff )
{
    enum_SFL_FIFO_ERROR_CODES retval = SFL_FIFO_ERROR_UNKNOWN;
    // check in another thread is actually reading
    if( config->read_in_progress == 0 )
    {
        // set lock for singleton read
        config->read_in_progress = 1;
        // Check if data in buffer
        if( (config->count) > 0 )
        {
            // get the data
        	uint8_t* tempbuffer=buff;
            tempbuffer+=config->next_read * config->element_size;
        	memcpy(result, tempbuffer, config->element_size);

            retval = SFL_FIFO_ERROR_NONE;
        }
        else
        {
            // no data available to read. User should have used sfl_fifo_get_count() before.
            retval = SFL_FIFO_ERROR_EMPTY;
        }
        // release singleton read lock.
        config->read_in_progress = 0;
    }
    else
    {
        retval = SFL_FIFO_ERROR_BUSY;
    }
    return retval;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*
* \test STATUS: *** TESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20170130  | Author  | FSetz
*/
enum_SFL_FIFO_ERROR_CODES sfl_fifo_search(uint8_t* val, SFL_FIFO_CONFIG_TYPE* config, int32_t* result, uint8_t* buff )
{
    enum_SFL_FIFO_ERROR_CODES retval = SFL_FIFO_ERROR_NONE;
    uint32_t act_pos;   ///< actual position that is searched.
    uint32_t read_index;    ///< Read index of searched position
    uint32_t i;         ///< Counter variable
    uint32_t j;         ///< Counter variable
    uint8_t bytes_different = 0;
    uint32_t tmp_cnt; // = config->count; ///< Used for not every time access to var ( went crazy when interrupt happens between execution)
    uint32_t e_size;
    uint32_t max;
    tmp_cnt = config->count; ///< Used for not every time access to var ( went crazy when interrupt happens between execution)
    read_index = config->next_read;
    e_size = config->element_size;
    act_pos = read_index * config->element_size;
    max = config->max;

    *result = -1;

    if( tmp_cnt == 0)
    {
        retval =SFL_FIFO_ERROR_EMPTY;

    }
    else
    {
        // walk through all elements in buffer to check if is the same content
        for( i = 0; i < tmp_cnt; ++i )
        {
            // compare for each value
            bytes_different = 0;
            for( j = 0; j < (e_size); ++j )
            {
                if( buff[act_pos + j] != val[j] )
                {
                    bytes_different = 1;
                    j = (e_size);
                }
                else
                {
                    // they are identical
                }
            }

            // handle the checking result
            if( bytes_different == 0 )
            {
                // Yes, value found. Stop search
                *result = read_index;  // return the position of the first found element
                i = tmp_cnt; // stop searching
            }
            else
            {
                // not found
                read_index ++;

                if( read_index >= max )
                {
                    read_index = 0;
                }
                else
                {
                    //normally increment act pos
                }

                // count up act_pos
                act_pos = read_index * e_size;

            }// end value check
        }// end searching loop
    }// end count check
    return retval;
}





enum_SFL_FIFO_ERROR_CODES sfl_fifo_remove(SFL_FIFO_CONFIG_TYPE* config, uint32_t count)
{
    enum_SFL_FIFO_ERROR_CODES retval = SFL_FIFO_ERROR_NONE;
    uint32_t tmp_count;

    if( count > (config->count) )
    {
        retval = SFL_FIFO_ERROR_PARAM;
    }
    else
    {
        hal_sys_disable_all_interrupts();
        config->count -= count;
        config->read_in_progress = 1;

        tmp_count = (config->next_read) + count;

        if( tmp_count < (config->next_read) )
        {
            retval = SFL_FIFO_ERROR_UNKNOWN;
        }
        else
        {
            if(tmp_count >= config->max )
            {
                tmp_count = tmp_count - config->max;
            }
            else
            {
            }
            config->next_read = tmp_count;
        }
        hal_sys_enable_all_interrupts();

        config->read_in_progress = 0;
        retval = SFL_FIFO_ERROR_NONE;

    }
    return retval;
}


uint32_t sfl_fifo_get_distance(uint32_t actual, uint32_t destination, uint32_t max)
{
    uint32_t count = 0;
    if( destination > actual )
    {
        count = destination - actual;
    }
    else
    {
        count = max - actual + destination;
    }
    return count;
}




/*----------------------------------------------------------------------------*/
/**
* \internal
*   Set all pointers to zero
* \endinternal
*
*/
enum_SFL_FIFO_ERROR_CODES sfl_fifo_clear(SFL_FIFO_CONFIG_TYPE* config)
{
    enum_SFL_FIFO_ERROR_CODES retval = SFL_FIFO_ERROR_NONE;
    
    if ( (config->write_in_progress == 0) && (config->read_in_progress == 0) )
    {
        hal_sys_disable_all_interrupts();

        config->count = 0;
        config->next_write = 0;
        config->next_read = 0;
        config->write_in_progress = 0;
        config->read_in_progress = 0;
        
        hal_sys_enable_all_interrupts();
    }
    else
    {
        retval = SFL_FIFO_ERROR_BUSY;
    }
    
    return retval;
}

