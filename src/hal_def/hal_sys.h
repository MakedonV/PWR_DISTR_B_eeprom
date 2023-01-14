#ifndef HAL_SYS_H
#define HAL_SYS_H
/*----------------------------------------------------------------------------*/
/**
* \file         hal_sys.h
* \brief        Definitions for hardware independent system control and information functions. 
*/
/*----------------------------------------------------------------------------*/
/** 
* \addtogroup   doc_hal_sys HAL SYS
* @{
* \brief        This documentation part describes the interface function definitions for a hardware-independent and abstracted application management interface.
* \details      Main goal is to create a hardware-platform independent layer, that can be accessed from different HW-platforms.
*               Main features are:
*               - Reset Application
*               - Jump to bootloader
*               - Manage Application Keys
*
*               Each application image has a header that contains information about length, CRC and so on. 
*               This image header must be written before the application in ROM and is used to detect bit errors during firmware transmission or storage.
*               Additionally the firmware is signed to allow only a trusted generators firmware.  
*               \image html hal_sys.png
*               \image latex hal_sys.png
*               The Bootloader access key is a shared variable from bootloader and application where runtime data is exchanged. 
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
#define HAL_SYS_APP_HEADER_VERSION                  (1u)            ///< Application Header Version
#define HAL_SYS_APP_HEADER_KEY                      (0x12345678u)   ///< Application Header Key
#define HAL_SYS_BOOTLOADER_ACCESS_KEY               (0x4D525321u)   ///< Bootloader Access Key
#define HAL_SYS_BOOTLOADER_APP_START_KEY            (0x07412807u)   ///< Bootloader Access Key
#define HAL_SYS_BOOTLOADER_ACCESS_KEY_UPDATE        (0x1AF7356B)    ///< Bootloader Access Key for a firmware update from the update image

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------
/** This enum contains all available modes for system resets */
typedef enum
{
    HAL_SYS_RESET_TYPE_NONE = 0,                                    ///< No Reset.
    HAL_SYS_RESET_TYPE_START_APP,                                   ///< Do a jump to start the application.
    HAL_SYS_RESET_TYPE_SOFTRESET_APP,                               ///< Do a soft reset to restart the application.
    HAL_SYS_RESET_TYPE_SOFTRESET_BL,                                ///< Do a soft reset to restart and enter the bootloader.
    HAL_SYS_RESET_TYPE_SOFTRESET_BL_UPDATE_FROM_COPY,               ///< Do a soft reset to restart and enter the bootloader to do a firmware update from an update image.
    HAL_SYS_RESET_TYPE_HARDRESET_WD,                                ///< Do a hard reset (reset with the watchdog).
    HAL_SYS_RESET_TYPE_HARDRESET_CPU,                               ///< Do a hard reset (CPU internal reset).

}enum_HAL_SYS_RESET;

// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------
/** This struct defines the application flash header */
typedef struct
{
    uint32_t header_key;            ///< Header key to identify the header.
    uint32_t header_crc;            ///< CRC32 checksum over the header content.
    uint32_t app_header_version;    ///< Header version.
    uint32_t application_crc;       ///< CRC32 checksum of the application.
    uint32_t application_length;    ///< Length of the application, used to calculate the CRC32 checksum.
    uint8_t sw_version[32];         ///< Additional SW Version String.

#if HAL_SYS_APP_HEADER_VERSION >= 1
    uint8_t reserve[460];           ///< Used for later purpose.
    uint8_t signature_key[512];     ///< Signature key, Application is signed to validate that 
#endif

}struct_hal_sys_app_header;

/** 
*   \brief      Function pointer type definition of Reset callback.
*   \details    This function is called, when somewhere a reset command was executed as last hook before shutdown.
*
*   \param      [in] #enum_HAL_SYS_RESET  Reset mode that will be performed.
*   \return     void
*/
typedef void (*hal_sys_pre_reset_callback_t)(enum_HAL_SYS_RESET);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Initialize the hal sysem functionality.
*   \details    Use this function to set the callback for user notification when system reset is performed.
*
*   \param      callback [in] #hal_sys_pre_reset_callback_t pre reset callback
*   \return     void
*/
void hal_sys_init(hal_sys_pre_reset_callback_t callback);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Generate a system reset.
*   \details    This function can generate different kinds of a "RESET".
*               A "SOFTRESET" is only a jump to the Reset Vector
*               A "HARDRESET" can be done by a watchdog reset or a software triggered CPU reset
*               This Function must not be called in an interrupt function!
*
*   \pre        The function #hal_sys_init must be called once before.
*
*   \param    reset_mode [in] #enum_HAL_SYS_RESET The reset mechanism that should be performed. 
*   \return   void
*/
void hal_sys_reset(enum_HAL_SYS_RESET reset_mode);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Read the application header.
*   \details    Read from ROM the applications header and further app information.
*
*   \return     Returns the #struct_hal_sys_app_header structure where information is stored. 
*/
struct_hal_sys_app_header hal_sys_get_app_header(void);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Read the Applications update header.
*   \details    Read from ROM the applications update header.
*
*   \return     Returns the #struct_hal_sys_app_header structure to app update header.
*/
struct_hal_sys_app_header hal_sys_get_app_update_header(void);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Read the bootloader access key value.
*   \details    This key is used to exchange data from Application and vise versa. 
*
*   \return     uint32_t The read bootloader access key.
*/
uint32_t hal_sys_get_bl_access_key(void);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Set the bootloader access key.
*   \details    This key is used to exchange data from Application and vise versa.    
*
*   \param      bl_access_key [in] uint32_t Bootloader access key.
*   \return     void
*/
void hal_sys_set_bl_access_key(uint32_t bl_access_key);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Disable all interrupts.
*   \details    Be careful when using this function. The code after this function must be very short.
*               Use #hal_sys_enable_all_interrupts to enable them again.
*
*   \return     void
*/
void hal_sys_disable_all_interrupts(void);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Enable all interrupts.
*   \details    Be careful with this function. When enabling the interrupts during an interrupt, it 
*               is highly dangerous to produce nested interrupts. You don't want this,- really you don't.
*
*   \return     void
*/
void hal_sys_enable_all_interrupts(void);
 /** @}*/
#endif
