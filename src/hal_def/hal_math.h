#ifndef HAL_MATH_H
#define HAL_MATH_H
/*----------------------------------------------------------------------------*/
/** 
* \file        hal_math.h
* \brief       Definitions for hardware independent Math functionality.
*/
/*----------------------------------------------------------------------------*/
/** 
* \defgroup     doc_hal_math HAL MATH
* \brief        Marked for removal.
* \details      Do not use.
*/
/*----------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------------------------------
// --------------------------------------------- Includes --------------------------------------------
// ---------------------------------------------------------------------------------------------------
#include "hal_data_types.h"

// ---------------------------------------------------------------------------------------------------
// --------------------------------------------- defines ---------------------------------------------
// ---------------------------------------------------------------------------------------------------
#define CCITT_MASK 0x1021

#define CRC_MASK CCITT_MASK // Polynomial(Mask) for the CRC to use

// ---------------------------------------------------------------------------------------------------
// --------------------------------------------- Typedefs --------------------------------------------
// ---------------------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------------------
// -------------------------------------------- Prototypes -------------------------------------------
// ---------------------------------------------------------------------------------------------------


/*----------------------------------------------------------------------------*/
/** 
* \ingroup  doc_hal_math
* \brief    initialise math functions/hardware units
* \return   void      
*/
void hal_math_init(void);


/*----------------------------------------------------------------------------*/
/** 
* \ingroup  doc_hal_math
* \brief    calculate a 16 Bit CRC checksum (CCIT) of ptr_buffer until length u32_byte_count and with seed u16_start_value
*
* \pre      call hal_math_init() once
*
* \param    ptr_buffer      [in] uint8_t*   pointer to start of input buffer
* \param    u16_start_value [in] uint16_t   start/seed value (0 = none)
* \param    u32_byte_count  [in] uint32_t   length of input buffer/crc iterations
* \return   uint16_t                        16 bit crc checksum
*/
uint16_t hal_math_get_16bitcrc(const uint8_t* ptr_buffer, uint16_t u16_start_value, uint32_t u32_byte_count);


/*----------------------------------------------------------------------------*/
/** 
* \ingroup  doc_hal_math
* \brief    calculate a 16 Bit CRC checksum (AC-style) of ptr_buffer until length u32_byte_count and with seed u16_start_value    
* \details  there was a mixup in the CRC implementation in the AC code base. This function is needed to preserve comptability, when interfacing with legacy code
*
* \pre      call hal_math_init() once   
*
* \param    ptr_buffer      [in] uint8_t*   pointer to start of input buffer
* \param    u16_start_value [in] uint16_t   start/seed value (0 = none)
* \param    u32_byte_count  [in] uint32_t   length of input buffer/crc iterations
* \return   uint16_t                        16 bit crc checksum    
*/
uint16_t hal_math_get_ac_16bitcrc(const uint8_t* ptr_buffer, uint16_t u16_start_value, uint32_t u32_byte_count);


/** @}*/
#endif // _HAL_MATH_H_
