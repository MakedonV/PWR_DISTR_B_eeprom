#ifndef HAL_NVM_H
#define HAL_NVM_H
/*----------------------------------------------------------------------------*/
/**
* \file         hal_nvm.h
* \brief        Definitions for hardware independent non volatile memory (NVM) interface (CFLASH, DFLASH or EEPROM).
*/
/*----------------------------------------------------------------------------*/
/** 
* \addtogroup   doc_hal_nvm HAL NVM
* @{
* \brief        This documentation part describes the interface function definitions for a hardware-independent and abstracted NVM interface.
* \details      Main goal is to create a hardware-platform independent layer, that can be accessed from different HW-platforms.
*              
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
#define HAL_NVM_ERASED_BYTE             0x0u

// ---------------------------------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------
/** This enum lists all possible error codes that NVM functions can return.*/
typedef enum
{
    HAL_NVM_OK                      = 0u,     ///< No error.
    HAL_NVM_ERROR_GENERAL               ,     ///< Common error.
    HAL_NVM_ERROR_INIT_FAILED           ,     ///< Initialization failed.
    HAL_NVM_ERROR_WHILE_READING         ,     ///< Error during read operation.
    HAL_NVM_ERROR_WHILE_WRITING         ,     ///< Error during write operation.
    HAL_NVM_ERROR_WHILE_CLEARING        ,     ///< Error during clear operation.
    HAL_NVM_ERROR_BLOCK_INVALID         ,     ///< Error block structure invalid.
    HAL_NVM_ERROR_BLOCK_NO_INVALID      ,     ///< Error block number invalid.
    HAL_NVM_ERROR_DATA_LEN_INVALID      ,     ///< Error data length invalid.
    HAL_NVM_ERROR_DATA_ADDR_INVALID     ,     ///< Error data address invalid.
    HAL_NVM_ERROR_NOT_IMPLEMENTED             ///< Error function not implemented.

} enum_HAL_NVM_RETURN_VALUE;

// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
*   \brief      Initialize NVM memory handling.
*   \details    
*
*   \return     This function return zero for success or an error code from #enum_HAL_NVM_RETURN_VALUE.
*/
enum_HAL_NVM_RETURN_VALUE hal_nvm_init(void);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Initialize a block of memory.
*   \details    Before using EEPROM/DFLASH memory via block number, the block must be initialized first.
*
*   \pre        The function #hal_nvm_init must be called once before.
*
*   \param      block_no        [in] uint16_t Number of FEE block.
*   \return     This function return zero for success or an error code from #enum_HAL_NVM_RETURN_VALUE.
*/
enum_HAL_NVM_RETURN_VALUE hal_nvm_eeprom_init_block(uint16_t block_no);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Check if the NVM block is writable.  
*   \details    Before a write of a block can be performed, it must be checked if it is erased.
*
*   \pre        The function #hal_nvm_init must be called once before.
*
*   \param      block_no        [in] uint16_t Number of FEE block.
*   \return     This function return zero for success or an error code from #enum_HAL_NVM_RETURN_VALUE.
*/
enum_HAL_NVM_RETURN_VALUE hal_nvm_eeprom_check_block(uint16_t block_no);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Cyclic caller for NVM manager. 
*   \details    This function is only needed when non blocking functions #hal_nvm_eeprom_write_by_block_no or #hal_nvm_eeprom_read_by_block_no are used.
*               "When new data is present store them".
*
*   \return     void
*/
void hal_nvm_eeprom_process_cyclic(void);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Non blocking NVM write by block number.
*   \details    IMPORTANT! This function only works if hal_nvm_eeprom_process_cyclic() is called cyclic.
*               Each block size is defined in the corresponding FEE configuration.
*
*   \param      block_no        [in] uint16_t Number of FEE block.
*   \param      ptr_data        [in] uint8_t* Pointer to data that should be written.
*   \return     This function return zero for success or an error code from #enum_HAL_NVM_RETURN_VALUE.
*/
enum_HAL_NVM_RETURN_VALUE hal_nvm_eeprom_write_by_block_no(uint16_t block_no, const uint8_t* ptr_data);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Non blocking NVM read by block number. 
*   \details    IMPORTANT! This function only works if hal_nvm_eeprom_process_cyclic() is called cyclic.
*
*   \pre
*
*   \param      block_no        [in] uint16_t Number of FEE block.
*   \param      len             [in] uint32_t Length of data to be read.
*   \param      ptr_data        [in] uint8_t* Pointer to read data. 
*   \return     This function return zero for success or an error code from #enum_HAL_NVM_RETURN_VALUE.
*/
enum_HAL_NVM_RETURN_VALUE hal_nvm_eeprom_read_by_block_no(uint16_t block_no, uint32_t len, uint8_t* ptr_data);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Blocking NVM write by block number.
*   \details    IMPORTANT! Please keep in mind not to write a block with too much data.
*               This could lead to timing issues and watchdog errors.
*
*   \pre 
*
*   \param      block_no        [in] uint16_t Number of FEE block
*   \param      ptr_data        [in] uint8_t* Pointer to data
*   \return     This function return zero for success or an error code from #enum_HAL_NVM_RETURN_VALUE.
*/
enum_HAL_NVM_RETURN_VALUE hal_nvm_eeprom_write_by_block_no_immediate(uint16_t block_no, const uint8_t* ptr_data);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Blocking NVM read by block number.
*   \details    IMPORTANT! Please keep in mind not to read a block with too much data.
*               This could lead to timing issues and watchdog errors.
*
*   \pre
*
*   \param      block_no        [in] uint16_t Number of FEE block
*   \param      len             [in] uint32_t Length of data to be read
*   \param      ptr_data        [in] uint8_t* Pointer to data
*   \return     This function return zero for success or an error code from #enum_HAL_NVM_RETURN_VALUE.
*/
enum_HAL_NVM_RETURN_VALUE hal_nvm_eeprom_read_by_block_no_immediately(uint16_t block_no, uint32_t len, uint8_t* ptr_data);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Blocking NVM write by address. 
*   \details    IMPORTANT! Please keep in mind not to write a block with too much data.
*               This could lead to timing issues and watchdog errors.
*
*   \pre
*
*   \param      addr            [in] uint32_t Address offset relative to base address.
*   \param      len             [in] uint32_t Length of data to be written.
*   \param      ptr_data        [in] uint8_t* Pointer to data.
*   \return     This function return zero for success or an error code from #enum_HAL_NVM_RETURN_VALUE.
*/
enum_HAL_NVM_RETURN_VALUE hal_nvm_eeprom_write_by_address(uint32_t addr, uint32_t len, const uint8_t* ptr_data);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Blocking NVM read by address.  
*   \details    IMPORTANT! Please keep in mind not to read a block with too much data.
*               This could lead to timing issues and watchdog errors.
*
*   \pre
*
*   \param      addr            [in] uint32_t Address offset relative to base address
*   \param      len             [in] uint32_t Length of data to be read
*   \param      ptr_data        [in] uint8_t* Pointer to data
*   \return     This function return zero for success or an error code from #enum_HAL_NVM_RETURN_VALUE.
*/
enum_HAL_NVM_RETURN_VALUE hal_nvm_eeprom_read_by_address(uint32_t addr, uint32_t len, uint8_t* ptr_data);
 /** @}*/
#endif
