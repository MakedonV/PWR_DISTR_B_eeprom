#ifndef SFL_FIFO
#define SFL_FIFO
/*----------------------------------------------------------------------------*/
/**
* \file         sfl_fifo.h
* \brief        Defines a c based dynamical first-in-first-out buffer(fifo)
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup     sfl_fifo SFL FIFO
* @{
* \brief        Software Function Library (First-In-First-Out) FIFO management.
* \details      A fifo is a ringbuffer / queue that manages the sequencing insertion and restorage of a value ( called element ).
*               Idea is to provide a mechanism that manages a simple (structure-) array as a fifo. 
*               Remember that this structure does not create a storage buffer. Just a management structure.
*                
*               
* This is a non priorized queue implementation. Please use it this way:
* \code{.c}
    #define FIFO_SIZE 10
    
    typedef struct
    {
        uint8_t data[8];
        uint32_t id;
    } struct_MY_STRUCT;
    
    struct_MY_STRUCT fifo[FIFO_SIZE];
    SFL_FIFO_CONFIG_TYPE fifo_cfg;
    enum_SFL_FIFO_ERROR_CODES retval;
    struct_MY_STRUCT my_value = {0,1,2,3,4,5,6,7,0};

    sfl_fifo_init(&fifo_cfg,FIFO_SIZE, 12u ); // 12 = SizeOf(struct_MY_STRUCT)
    ...
    retval = sfl_fifo_put(&fifo_cfg, my_value, fifo );
    if( retval != SFL_FIFO_ERROR_NONE )
    {
        ...
    }
    else
    {
        // everything ok
    }

    if( sfl_fifo_get_read_lock(&fifo_cfg) != 0 )
    retval = sfl_fifo_get(&fifo_cfg, &my_value, fifo );
    if( retval != SFL_FIFO_ERROR_NONE )
    {
        ...
    }
    else
    {
        // everything ok
    }


\endcode
* Create an array and use exactly its size to initialize the fifo.
* Then use put() to place an element into the queue and get() to load and remove it from queue.
*
*
* POSSIBLE EXTENSIONS are:
* sfl_fifo_contains(value, &config & buff); // checks if a value is inside the queue ( may used by following values, so you can check if val[0]=a && val[1]=b )
* sfl_fifo_position_of(value, &config, &buff); // returns the fifo index of the first found given value
* sfl_fifo_clear(&config, &buff); // clears the bugger content to "x"
*
*
*
*/
/*----------------------------------------------------------------------------*/


#include "hal_data_types.h"




/** 
 *  Errorcodes of the sfl fifo functions. Please check every (this type returning) function for errors, or your queue could get into unwished states.
 */
typedef enum
{
    SFL_FIFO_ERROR_NONE = 0u,               ///< No error happened.
    SFL_FIFO_ERROR_PARAM,                   ///< Invalid parameter given for this operation.
    SFL_FIFO_ERROR_BUSY,                    ///< Actual another action of this fifo is performed.
    SFL_FIFO_ERROR_OVERFLOW,                ///< Operation failed becauseo of overflow.
    SFL_FIFO_ERROR_EMPTY,                   ///< Operation failed because of too less elements in queue.
    SFL_FIFO_ERROR_UNKNOWN,                 ///< Dramatic error, unknown reason, normally not possible.
    SFL_FIFO_ERROR_MAX                      ///< Last position in namespace.
}enum_SFL_FIFO_ERROR_CODES;


/** 
 * States for the user managed read-lock information ( flag)
 */
typedef enum
{
    SFL_FIFO_READLOCK_DISABLED = 0,         ///< The readlock is turned off.
    SFL_FIFO_READLOCK_ENABLED               ///< The readlock is turned on.
}enum_SFL_FIFO_READLOCK_TYPE;



/** 
* Main managing structure for handling an integer based fifo. Please use the read lock for your applications lock state.
*/
typedef struct
{
    uint32_t count;                         ///< Actual number of elements that are stored in the current fifo.
    uint32_t next_write;                    ///< Position of the next free bufferplace to write.
    uint32_t next_read;                     ///< Position of the next valid and filled bufferplace.
    uint32_t element_size;                  ///< Value that indicates how many bytes a single user structure entry has. ( uint8_t = 1, uint16_t = 2, uint32_t = 4, ...)
    uint32_t max;                           ///< Number of the elements in fifo, buffer size. This is only changed in the init function. Do not modify at runtime or you write over the buffers array size!!!
    uint8_t  write_in_progress;             ///< flag that indicates that actually a write is performed (for interrupt usage)
    uint8_t  read_in_progress;              ///< flag that indicates that actually another (thread) reads from the queue.
    enum_SFL_FIFO_READLOCK_TYPE read_lock;  ///< Flag that indicates that someone want to read multiple elements from queue and should not be interrupted. Please use this internal flag when you are working multithreaded by using the sfl_fifo_lock_read(), write and get functions.
}SFL_FIFO_CONFIG_TYPE;


/*----------------------------------------------------------------------------*/
/**
* \brief    This function initializes the fifo.
* \details  This function resets the setup for the fifo. After calling this function, the position variables are cleared.
* The content of the queue is not modified! You can call this at any time during runtime to reset / initialize the queue/FIFO.
*
* \param    config        [in] SFL_FIFO_CONFIG_TYPE* A pointer to a (empty) structure variable of the type #SFL_FIFO_CONFIG_TYPE. This variable don't need to be filled.
* \param    element_count [in] uint32_t Number of buffer elements (count). Use the same value/define as you use for the fifo array size.
* \param    element_size  [in] uint32_t Number of bytes that a single buffer element has. Please give here a function to copy a value from a buffer place to another (implement yourself in sfl_fifo_user_cfg when not 8,16 or 32 bit values).
* \return   void
*/
enum_SFL_FIFO_ERROR_CODES sfl_fifo_init(SFL_FIFO_CONFIG_TYPE* config, uint32_t element_count, uint32_t element_size);

/*----------------------------------------------------------------------------*/
/**
* \brief    This function returns the number of elements that are actually stored in the buffer.
* \details  Use this function to check how many elements are actually valid and stored in the given buffer. This is for checking if the buffer
* contains more than the needed elements.
*
* \pre     #sfl_fifo_init() must be called before.
*
* \param    config   [in] SFL_FIFO_CONFIG_TYPE* A pointer to the fifos configuration.
* \return   uint32_t Returns the number of elements that are actually valid stored in buffer. This number cant be negative, so cast it to your size. Remember for your datatype, that this value can be as high as your buffer-size.
*/
uint32_t sfl_fifo_get_count(SFL_FIFO_CONFIG_TYPE* config);

/*----------------------------------------------------------------------------*/
/**
* \brief    Set Read lock.
* \details  Information for other users of the fifo that someone want to read more than one element in a sequence.
* Because every read is a deleting read and the sequence of incomming is relevant, the element cant be put back.
* So you need a lock to tell other users of a queue that you are expecting multiple elements. Remember to release
* the lock as soon as you have received all your data.
*
* \pre      #sfl_fifo_init() must be called before.
*
* \param    config [in] SFL_FIFO_CONFIG_TYPE*  A pointer to the fifos configuration.
* \return   void      Does not return any values.
*/
void sfl_fifo_lock_read(SFL_FIFO_CONFIG_TYPE* config);


/*----------------------------------------------------------------------------*/
/**
* \brief    Get readlock information.
* \details  Check this before every (multi-user) fifo access! This check is needed for other queue listeners, that wait for sequenced data.
*
* \pre      #sfl_fifo_init() must be called before.
*
* \param    config  [in] SFL_FIFO_CONFIG_TYPE*  A pointer to the fifos configuration.
* \return  enumSFL_FIFO_READLOCK_TYPE
*/
enum_SFL_FIFO_READLOCK_TYPE sfl_fifo_get_read_lock(SFL_FIFO_CONFIG_TYPE* config);


/*----------------------------------------------------------------------------*/
/**
* \brief    Delock Read.
* \details  Information to other users that the application now not is waiting for a sequenced element in buffer.
* Use this when your sequence is over.
*
* \pre      #sfl_fifo_init() must be called before.
*
* \param    config [in] SFL_FIFO_CONFIG_TYPE* A pointer to the fifos configuration.
* \return   void Does not return a value.
*/
void sfl_fifo_unlock_read(SFL_FIFO_CONFIG_TYPE* config);


/*----------------------------------------------------------------------------*/
/**
* \brief    Puts an element into the fifo-buffer.
* \details  This function adds the given element into the buffer or returns an errorcode from #enum_SFL_FIFO_ERROR_CODES.
* For this, use an external element-buffer and a config struct that contains all important information for fifo handling.
*
* \pre      #sfl_fifo_init() must be called before.
*
* \param    config                    [in] SFL_FIFO_CONFIG_TYPE* A pointer to the fifos configuration.
* \param    new_data                  [in] uint8_t* a pinter to the new data element to insert into fifo. Use a typecast to your needs.
* \param    buff                      [in] void* A pointer to (the first element) of the fifo buffer.
* \return   enum_SFL_FIFO_ERROR_CODES This function returns #SFL_FIFO_ERROR_NONE for valid values or a value unequal to 0 for function failed.
*/
enum_SFL_FIFO_ERROR_CODES sfl_fifo_put(SFL_FIFO_CONFIG_TYPE* config, uint8_t* new_data, uint8_t* buff );




/*----------------------------------------------------------------------------*/
/**
* \brief    Reads and removes one element from the fifo buffer.
* \details  Before the get function is called, you should have checked if the number of available elements in buffer
* is higher or same as the number of elements you want to get. In this case, you should use the lock mechanism to make sure, that
* no other fifo client receives "your" elements.
*
* \pre    #sfl_fifo_init() must be called before. Its needed to use the function #sfl_fifo_put8() before, too or function fails with error empty.
*
* \param    config                    [in] SFL_FIFO_CONFIG_TYPE* A pointer to the fifos configuration.
* \param    result                    [out] uint8_t* Into this pointer, the next fifo value is copied to.
* \param    buff                      [in] uint8_t* A pointer to (the first element) of the fifo buffer.
* \return   enum_SFL_FIFO_ERROR_CODES This function returns #SFL_FIFO_ERROR_NONE for valid values or a value unequal to 0 for function failed.
*/
enum_SFL_FIFO_ERROR_CODES sfl_fifo_get(SFL_FIFO_CONFIG_TYPE* config, uint8_t* result, uint8_t* buff );

/*----------------------------------------------------------------------------*/
/**
* \brief    Reads and one element from the fifo buffer.
* \details  Before the get function is called, you should have checked if the number of available elements in buffer
* is higher or same as the number of elements you want to get. In this case, you should use the lock mechanism to make sure, that
* no other fifo client receives "your" elements.
*
* \pre    #sfl_fifo_init() must be called before. Its needed to use the function #sfl_fifo_put8() before, too or function fails with error empty.
*
* \param    config                    [in] SFL_FIFO_CONFIG_TYPE* A pointer to the fifos configuration.
* \param    result                    [out] uint8_t* Into this pointer, the next fifo value is copied to.
* \param    buff                      [in] uint8_t* A pointer to (the first element) of the fifo buffer.
* \return   enum_SFL_FIFO_ERROR_CODES This function returns #SFL_FIFO_ERROR_NONE for valid values or a value unequal to 0 for function failed.
*/
enum_SFL_FIFO_ERROR_CODES sfl_fifo_get_preview(SFL_FIFO_CONFIG_TYPE* config, uint8_t* result, uint8_t* buff );


/*----------------------------------------------------------------------------*/
/**
* \brief    Looks in the given fifo if the value is presentt.
* \details  This function starts working at the last read datapoint.
*
* \pre
*
* \param    val                       [in] uint8_t  A Pointer to the value that is searched for.
* \param    config                    [in] SFL_FIFO_CONFIG_TYPE* A pointer to the fifos configuration.
* \param    result                    [out] int32_t* -1 for failed or not in buffer and a positive number or 0 for the first found position.
* \param    buff                      [in] uint8_t* The buffer that has to be searched.
* \return   enum_SFL_FIFO_ERROR_CODES This function returns #SFL_FIFO_ERROR_NONE for valid values or a value unequal to 0 for function failed.
*/
enum_SFL_FIFO_ERROR_CODES sfl_fifo_search(uint8_t* val, SFL_FIFO_CONFIG_TYPE* config, int32_t* result,uint8_t* buff );



/*----------------------------------------------------------------------------*/
/**
* \brief    This function removes the given number of elements from the fifo buffer.
* \details  The data is not deleted just config is adapted. Handle like calling fifo_get multiple times.
*
* \param    config                    [in] SFL_FIFO_CONFIG_TYPE* A pointer to the fifos configuration.
* \param    count                     [in] The number of elements to delete.
* \return   enum_SFL_FIFO_ERROR_CODES This function returns #SFL_FIFO_ERROR_NONE for valid values or a value unequal to 0 for function failed.
*/
enum_SFL_FIFO_ERROR_CODES sfl_fifo_remove(SFL_FIFO_CONFIG_TYPE* config, uint32_t count);


uint32_t sfl_fifo_get_distance(uint32_t actual, uint32_t destination, uint32_t max);


/*----------------------------------------------------------------------------*/
/**
* \brief    This function clears the fifo.
* \details  The function just set all internal pointers to initial states.
*
* \param    config                    [in] SFL_FIFO_CONFIG_TYPE* A pointer to the fifos configuration.
* \return   enum_SFL_FIFO_ERROR_CODES This function returns #SFL_FIFO_ERROR_NONE for valid values or a value unequal to 0 for function failed.
*/
enum_SFL_FIFO_ERROR_CODES sfl_fifo_clear(SFL_FIFO_CONFIG_TYPE* config);


/** @} */
#endif// __SFL__FIFO__
