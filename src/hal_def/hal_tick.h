#ifndef HAL_TICK_H
#define HAL_TICK_H
/*----------------------------------------------------------------------------*/
/**
*   \file           hal_tick.h
*   \brief          Definitions for hardware independent system timer functions.
*/
/*----------------------------------------------------------------------------*/
/** 
*   \addtogroup     doc_hal_tick HAL TICK
*   @{
*   \brief          This documentation part describes the interface function definitions for a hardware-independent and abstracted system timer.
*   \details        Main goal is to create a hardware-platform independent layer, that can be accessed from different HW-platforms.
*                   Only task is to provide a system wide time base for time dependent features in applications. There is no direct timer access.
*                   You can use this features to implement your own cyclic calls, wait conditions or timeout behavior.
*                 
*/
/*----------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "hal_data_types.h"

// ---------------------------------------------------------------------------------------------------
// defines
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------
/** This enum lists all possible errors for hal system tick functions.*/
typedef enum
{
    HAL_TICK_OK                         = 0u,   ///< No error.
    HAL_TICK_ERROR_GENERAL                  ,   ///< Common error.
    HAL_TICK_ERROR_INIT_FAILED              ,   ///< Initialization failed.
    HAL_TICK_ERROR_DEINIT_FAILED            ,   ///< De-initialization failed.
    HAL_TICK_ERROR_PRECISION_INVALID        ,   ///< Precision invalid.
    HAL_TICK_ERROR_NOT_IMPLEMENTED              ///< Error function not implemented.

} enum_HAL_TICK_RETURN_VALUE;


/** This enum lists all available resolutions that the system tick manager can handle.*/
typedef enum
{
    HAL_PRECISION_1US                   = 0u,   ///< Precision is 1us.
    HAL_PRECISION_10US                      ,   ///< Precision is 10us.
    HAL_PRECISION_100US                     ,   ///< Precision is 100us.
    HAL_PRECISION_1MS                       ,   ///< Precision is 1ms.
    HAL_PRECISION_10MS                      ,   ///< Precision is 10ms.
    HAL_PRECISION_100MS                     ,   ///< Precision is 100ms.
    HAL_PRECISION_1S                            ///< Precision is 1s.
}enum_HAL_PRECISION;

// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------

/** This struct contains internal used timer tick definitions for all available precisions. */
typedef struct
{
    uint32_t tick_1us;                          ///< System tick for microsecond resolution.
    uint32_t tick_10us;                         ///< System tick for 10 microsecond resolution.
    uint32_t tick_100us;                        ///< System tick for 100 microsecond resolution.
    uint32_t tick_1ms;                          ///< System tick for millisecond resolution.
    uint32_t tick_10ms;                         ///< System tick for 10 millisecond resolution.
    uint32_t tick_100ms;                        ///< System tick for 100 millisecond resolution.
    uint32_t tick_1s;                           ///< System tick for second resolution.
    enum_HAL_PRECISION precision;               ///< System tick precision information.
}struct_hal_tick_handle;

/** this typedef describe function pointer used to execute given function each 1ms. */
typedef void (*callback_timer_1ms_t)(void);

// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
*   \brief      Initialize the system tick functionality.
*   \details    With this function you initialize all important components to use the system timer functionality.
*
*   \return   Returns 0 for success or an error value from #enum_HAL_TICK_RETURN_VALUE.
*/
enum_HAL_TICK_RETURN_VALUE hal_tick_init(void);

/*----------------------------------------------------------------------------*/
/**
*   \brief      De-initialize system timer functionality.
*   \details    Use this to restore defaults to systick.
*
*   \return   Returns 0 for success or an error value from #enum_HAL_TICK_RETURN_VALUE.
*/
enum_HAL_TICK_RETURN_VALUE hal_tick_deinit(void);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Read the actual system timer tick count.
*   \details    Read the local system time tick counter (since reboot).  
*
*   \pre        The function #hal_tick_init should be called before.
*
*   \param    timestamp                  [out] uint32_t* Pointer to timestamp where result is stored.
*   \param    precision                  [in] #enum_HAL_PRECISION Time resolution. 
*   \return   Returns 0 for success or an error value from #enum_HAL_TICK_RETURN_VALUE.
*/
enum_HAL_TICK_RETURN_VALUE hal_get_timestamp(uint32_t* timestamp, enum_HAL_PRECISION precision);


/*----------------------------------------------------------------------------*/
/**
*   \brief      Internal function, do not use. 
*   \details    This function can be used to automatically count up the internal system tick variable handler.
*               This function checks if param 1 > 10. If yes, the second and third params are increased.
*
*   \pre    The function #hal_tick_init should be called before.
*
*   \param    tick_temp        [in] uint32_t* Pointer to originally value.
*   \param    tick_temp_next   [in] uint32_t* Pointer to increased value 1.
*   \param    tick_next        [in] uint32_t* Pointer to increased value 2.
*   \return   void
*/
void hal_tick_set_tick_next(uint32_t* tick_temp, uint32_t* tick_temp_next, uint32_t* tick_next);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Increase the system tick.
*   \details    This function must be called in system timer interrupt or in RTOS task on highest prior interrupt. 
*               This function counts up the minimum system time base counter.  Only use use values from #enum_HAL_PRECISION, also as timer tick.
*
*   \pre    The function #hal_tick_init should be called before.
*
*   \param    precision        [in] #enum_HAL_PRECISION The time resolution / precision that is used in caller function. 
*   \return   void
*/
void hal_tick_set_tick(enum_HAL_PRECISION precision);

/*----------------------------------------------------------------------------*/
/**
*   \brief     user callback for 1ms timer
*   \details   call this function in usercode_init()
*
*   \pre
*
*   \param    callback [in] user_callback_timer_1ms_t
*   \return   void
*/
void set_callback_timer_1ms(callback_timer_1ms_t callback);

void set_callback_timer_lin_1ms(callback_timer_1ms_t callback);

/** @}*/
#endif


