#ifndef HAL_CRC_H
#define HAL_CRC_H
/*----------------------------------------------------------------------------*/
/**
* \file         hal_crc.h
* \brief        Definitions for hardware independent CRC functions.
*/
/*----------------------------------------------------------------------------*/
/** 
* \addtogroup   doc_hal_crc HAL CRC
* @{
* \brief        This documentation part describes the interface function definitions for a hardware-independent and abstracted CRC calculation module.
* \details      Main goal is to create a hardware-platform independent layer, that can be accessed from different HW-platforms.
*               The CRC polynomial is configured at implementation.
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
/** This enum lists all possible errors that can be returned from CRC functions.*/
typedef enum
{
    HAL_CRC_OK                      = 0u,       ///< no error
    HAL_CRC_ERROR_NO_CHANNEL_LEFT       ,       ///< no crc channel left on device
    HAL_CRC_ERROR_WHILE_INITIALIZING    ,       ///< Error in the initializing function

}enum_HAL_CRC_RETURN_VALUE;

// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------

/** This handler structure contains all internal variables for the use of a single CRC module*/
typedef struct
{
    void* ptr_handle;       ///< A Pointer to the handler itself
    uint8_t channel;        ///< Channel of a module that is used. 
    const void* ptr_data;   ///< A pointer to the data where the crc is calculated from.
    uint32_t size;          ///< Length in bytes over that the crc should be calculated (ptr_data length).
    uint32_t crc;           ///< calculated CRC value.

}struct_hal_crc_handle;

// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
*   \brief      Initialize the hardware crc module for given channel.
*   \details    Use this function to initialize the hardware crc functionality.
*               This function can be used to reset the CRC for non continuios blocks. 
*
*   \param    ptr_crc_handle            [in] #struct_hal_crc_handle* Pointer to the empty crc handler. 
*   \param    channel                   [in] uint8_t HW channel number for new handler.
*   \return   Result of the function: Either be zero for success or a positive number from #enum_HAL_CRC_RETURN_VALUE as error code. 
*/
enum_HAL_CRC_RETURN_VALUE hal_crc_init(struct_hal_crc_handle* ptr_crc_handle,
                                       uint8_t                channel);

/*----------------------------------------------------------------------------*/
/**
*   \brief Calculate the CRC for given data.
*   \details    Use hardware for crc calculating.
*
*   \pre The function #hal_crc_init must be called once before. 
*
*   \param    ptr_crc_handle            [in] #struct_hal_crc_handle* Pointer to the handler of the HW crc unit.
*   \param    ptr_data                  [in] void* pointer to data to be generated.
*   \param    size                      [in] uint32_t size of data to be generated.
*   \return   uint32_t caclulated crc
*/
uint32_t hal_crc_calculate_crc(struct_hal_crc_handle* ptr_crc_handle,
                               const void* ptr_data,
                               uint32_t size);
 
 /** @}*/
#endif

