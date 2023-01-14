#ifndef SFL_DB_H
#define SFL_DB_H

/*----------------------------------------------------------------------------*/
/**
* \file         sfl_db.h
* \brief        Common container definitions for frame oriented signal access. 
* \date         20170323
* \author       Harter
*
*/
/*----------------------------------------------------------------------------*/
/**
* \addtogroup   sfl_db SFL DB (SignalToFrame) [DB]
* \{ 
* \brief        Signal to Frame management for container usage (CAN, LIN).
* \details      This module can be used to manipulate single data signals within a complete data block. 
*               For example a can-signal within a can-frame or a lin-signal in a lin-frame.
*               This includes network to local conversions (endianess, byte-ordering,...).
*
*   For version information see sfl_db_version.h
*/
/*----------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "hal_data_types.h"


/*----------------------------------------------------------------------------*/
/**
* \brief    Change endianess from intel to motorola or vice versa
* \details	Swap the bytes in a word. Function can be used as
*           motorola_to_intel, too - it is the same mechanism.
*
* \param    val_in   [in] uint32_t   value in intel
* \return   uint32_t value in motorola
*/
uint32_t sfl_db_change_intel_to_motorola_32_bit(uint32_t val_in);

/*----------------------------------------------------------------------------*/
/**
* \brief 	Places the given value into a (can/lin) frame data buffer.
* \details  The given value is shifted to the start position and masked with the given length.
* 			Then it is written into the ptr_to_data_block buffer. The rest of the buffers content
* 			stays unchanged. This feature is internally used for the can-db handling.
* \code{.c}
	uint8_t data_block[8] = {0};
	sfl_db_put_signal_value_to_data_block(0xFA,(uint8_t*) &data_block, 8, 8, 1);
	// data_block[1] == 0xFA;

    sfl_db_put_signal_value_to_data_block(0x01,(uint8_t*) &data_block, 61, 1, 0);
	// data_block[7]= 0x20;
\endcode
*
* \param    value             [in] uint32_t New signal value that have to be put into the frame.
* \param    ptr_to_data_block [out] uint8_t* Pointer where the frame is located.
* \param    startbit          [in] uint8_t Start offset in Bit
* \param    length            [in] uint8_t Length in bit of the new signal
* \param    data_format       [in] uint8_t Data format: Intel = 0 or Motorola = 1
* \return   void
*/
void sfl_db_put_signal_value_to_data_block(uint32_t value, uint8_t* ptr_to_data_block, uint16_t startbit, uint16_t length, uint8_t data_format);

/*----------------------------------------------------------------------------*/
/**
* \brief 	Extract a signal value from a (lin/can) data frame buffer.
* \details	Interprets a signal value from data buffer located at startposition with a dedicated length.
*
*
* Example:
* \code{.c}
	uint8_t data_block[8] = {0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00};
	// Read 18th bit in frame and interpret it as 2 bit sized variable
	ret = sfl_db_get_signal_value_from_data_block((uint8_t*)&data_block, 18, 2, 0);
	// ret = 0x02
\endcode
* \param    ptr_to_data_block [inout] uint8_t* Pointer where the frame is located.
* \param    startbit          [in] uint8_t Start offset in Bit where signal is located
* \param    length            [in] uint8_t Length in bit of the signal
* \param    data_format       [in] uint8_t Data format: Intel = 0 or Motorola = 1
* \return   uint32_t signal   Returns the interpreted signal as 32-Bit variable.
*/
uint32_t sfl_db_get_signal_value_from_data_block(uint8_t* ptr_to_data_block, uint16_t startbit, uint16_t length, uint8_t data_format);

#if ARCHITECTURE_64BIT
/*----------------------------------------------------------------------------*/
/**
* \brief    function to change endianess intel -> motorola or vice versa for 64 bit applications
* \details	Swap the bytes in a word. Function can be used as
*           motorola_to_intel, too - it is the same mechanism.
*
* \param    val_in   [in] uint32_t   value in intel
* \return   uint32_t value in motorola
*/
uint64_t sfl_db_change_intel_to_motorola_64_bit(uint64_t val_in);

/*----------------------------------------------------------------------------*/
/**
* \brief 	See sfl_db_put_signal_value_to_data_block
* \details  Same workwise as upper function but with a single signal size of 64 instead of 32 bit.
*
* \param    value             [in] uint64_t data point
* \param    ptr_to_data_block [in] uint8_t* pointer to data block
* \param    startbit          [in] uint8_t start bit
* \param    length            [in] uint8_t data length
* \param    data_format       [in] uint8_t data format
* \return   void
*/
void sfl_db_put_signal_value_to_data_block_64(uint64_t value, uint8_t* ptr_to_data_block, uint16_t startbit, uint16_t length, uint8_t data_format);

/*----------------------------------------------------------------------------*/
/**
* \brief 	See sfl_db_get_signal_value_from_data_block()
* \details	Same workwise as upper function but with a single signal size of 64 instead of 32 bit.
*
* \param    ptr_to_data_block [in] uint8_t* pointer to data block
* \param    startbit          [in] uint8_t start bit
* \param    length            [in] uint8_t data length
* \param    data_format       [in] uint8_t data format
* \return   uint64_t data point
*/
uint64_t sfl_db_get_signal_value_from_data_block_64(uint8_t* ptr_to_data_block, uint16_t startbit, uint16_t length, uint8_t data_format);

#endif // ARCHITECTURE_64BIT

/** \} */
#endif
