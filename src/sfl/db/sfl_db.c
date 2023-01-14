/*----------------------------------------------------------------------------*/
/**
* \file         sfl_db.c
* \brief        Common container implementation for frame oriented signal access. 
* \date         20170323
* \author       Harter
*
*/
/*----------------------------------------------------------------------------*/
/**
* \addtogroup  sfl_db
* \{
*/
//--------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------
#include "sfl_db.h"

// ---------------------------------------------------------------------------------------------------
// tables
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// module globals
// ---------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
 * \internal
 * implementation of sfl_db_invert_bitorder_in_byte function to invert the bitorder in a given byte
 * \endinternal
 *
 *
 */
uint8_t private_sfl_db_invert_bitorder_in_byte(uint8_t val_in)
{
    uint8_t val_out = val_in;

    val_out = ( (val_out & 0xF0) >> 4 ) | ( (val_out & 0x0F) << 4 ); // swap nibbles
    val_out = ( (val_out & 0xCC) >> 2 ) | ( (val_out & 0x33) << 2 ); // swap pairs of 2 bit in every nibble 0xCC = %11001100   0x33 = %00110011
    val_out = ( (val_out & 0xAA) >> 1 ) | ( (val_out & 0x55) << 1 ); // swap each bit with neighbour        0xAA = %10101010   0x55 = %01010101

    return val_out;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* implementation of sfl_db_change_intel_to_motorola_32_bit function to change endianess intel -> motorola
* \endinternal
*
*/
uint32_t sfl_db_change_intel_to_motorola_32_bit(uint32_t val_in)
{
    uint32_t val_out;

    val_out = ((val_in & 0x000000fful) << 24) + ((val_in & 0x0000ff00ul) << 8)
            + ((val_in & 0x00ff0000ul) >> 8) + ((val_in & 0xff000000ul) >> 24);

    return val_out;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* implementation of sfl_db_put_signal_value_to_data_block function to set data point into data block
* \endinternal
*
*/
//                                                                                          start bit       bit length
void sfl_db_put_signal_value_to_data_block(uint32_t value, uint8_t* ptr_to_data_block, uint16_t startbit, uint16_t length, uint8_t data_format)
{
    uint16_t index_startbyte;   // uint8_t
    uint16_t sh_left;           // uint8_t
    uint16_t counter;           // uint8_t

    uint32_t maske;
    uint32_t value_block;

// startbit = 64
// length = 32

    // startbit in data block
    if( startbit > 511 )
    {
        return;
    }
    // reached max data block length
    if( (startbit + length) > 512 ) // 96 >
    {
        return;
    }

    // check if signal length is bigger then uint32_t
    if( (length > 32) || !length )
    {
        return;
    }

    // check if signal isn't in 4 consecutive bytes
    // 65 & 0x07 = 1, 64 & 0x07 = 0
    if( ((startbit & 0x07) + length) > 32 )
    {
        return;
    }

    maske = 0;

    for(counter = 0; counter < length; counter++)
    {
// @TODO: maske anpassen??
        // len=4 -> 1111
        maske = (maske << 1) | 0x00000001;  // Same as (maske|= 1<<i) but quicker!
    }
    // 16  = 2   0000 0010
    // 32  = 3   0000 0011
    // 48  = 6   0000 0110
    // 64  = 8   0000 1000
    // 96  = 12  0000 1100
    // 256 = 32  0010 0000
    // 480 = 60  0011 1100
///@TODO: should work
    index_startbyte = startbit >> 3;

    // --------------------------------------------------------------------------------
    // at first we save the existing 4-Byte-Block
    // --------------------------------------------------------------------------------

    // 0010 0000 >> 3 = 0100
    value_block = (uint32_t)*(ptr_to_data_block + index_startbyte)
                    + ((uint32_t)*(ptr_to_data_block + index_startbyte + 1) << 8)
                    + ((uint32_t)*(ptr_to_data_block + index_startbyte + 2) << 16)
                    + ((uint32_t)*(ptr_to_data_block + index_startbyte + 3) << 24);

    // --------------------------------------------------------------------------------
    // only the relevant bits
    // --------------------------------------------------------------------------------
    // cut of everything lager than 32 bit
    value &= maske;

    // --------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------
    if (data_format == TRUE)
    {
        // ================================================================================
        // data format MOTOROLA --> Big Endian
        // ================================================================================
        // --------------------------------------------------------------------------------
        // first shift left and then change the bytes direction
        // --------------------------------------------------------------------------------

        // 32 - (64 & 0x07) + 32
        // 32 - 0 + 32
        sh_left = (uint16_t)(32 - ((startbit & 0x07) + length));
        value <<= sh_left; // 64

        // --------------------------------------------------------------------------------
        // change the bytes direction
        // --------------------------------------------------------------------------------
        value = sfl_db_change_intel_to_motorola_32_bit(value);
        maske <<= sh_left;

        // --------------------------------------------------------------------------------
        // change the bytes direction
        // --------------------------------------------------------------------------------
        maske = sfl_db_change_intel_to_motorola_32_bit(maske);
    }
    else
    {
        // << 0
        sh_left = startbit & 0x07;
        value <<= sh_left;
        maske <<= sh_left;
    }

    // --------------------------------------------------------------------------------
    // save the signal different bits
    // --------------------------------------------------------------------------------
    maske = value_block & ~maske;

    // --------------------------------------------------------------------------------
    // bitwise or, signal value with signal different bits
    // --------------------------------------------------------------------------------
    value_block = value | maske;


    // --------------------------------------------------------------------------------
    // write back all 4 byte -> relevant for 32bit datatype
    // --------------------------------------------------------------------------------

    // index_startbyte = 12
    *(ptr_to_data_block + index_startbyte) = (uint8_t)(value_block & 0x000000ffL);
    *(ptr_to_data_block + index_startbyte + 1) = (uint8_t)((value_block & 0x0000ff00L) >> 8);
    *(ptr_to_data_block + index_startbyte + 2) = (uint8_t)((value_block & 0x00ff0000L) >> 16);
    *(ptr_to_data_block + index_startbyte + 3) = (uint8_t)((value_block & 0xff000000L) >> 24);

    return;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* implementation of sfl_db_get_signal_value_from_data_block function to get data point from data block
* \endinternal
*/
uint32_t sfl_db_get_signal_value_from_data_block(uint8_t* ptr_to_data_block, uint16_t startbit, uint16_t length, uint8_t data_format)
{
    uint16_t index_startbyte;
    uint16_t counter;

    uint32_t value;
    uint32_t mask;

    // --------------------------------------------------------------------------------
    // startbit in data block
    // --------------------------------------------------------------------------------
    if( startbit > 511 )
    {
        return 0;
    }
    else
    {
        // do nothing
    }

    // --------------------------------------------------------------------------------
    // reached max data block length
    // --------------------------------------------------------------------------------
    if( (startbit + length) > 512 )
    {
        return 0;
    }
    else
    {
        // do nothing
    }

    // --------------------------------------------------------------------------------
    // check if signal length is bigger then uint32_t
    // --------------------------------------------------------------------------------
    if( (length > 32) || !length )
    {
        return 0;
    }
    else
    {
        // do nothing
    }

    // --------------------------------------------------------------------------------
    // check if signal isn't in 4 consecutive bytes
    // --------------------------------------------------------------------------------
    if( ((startbit & 0x07) + length) > 32 )
    {
        return 0;
    }
    else
    {
        // do nothing
    }

    index_startbyte = startbit >> 3;

    // --------------------------------------------------------------------------------
    // check for the correct data format
    // --------------------------------------------------------------------------------
    if (data_format == TRUE)
    {
        // ================================================================================
        // data format MOTOROLA --> Big Endian
        // ================================================================================
        // Intel (Little Endian): Bitstart=2, Length=12
        // ---- ---- | ---- ---- | --xx xxxx | xxxx xx--
        //                           .. .987   6543 21
        // 0000 0000   0000 0000   0010 0001   1100 1000
        //
        // => 1000 0111 0010 (0x0872)
        //
        //
        // Motorola (Big Endian): Bitstart=2, Length=12
        // ---- ---- | ---- ---- | xxxx xx-- | --xx xxxx
        //                         6543 21       .. .987
        // 0000 0000   0000 0000   1100 1000   0010 0001
        //
        // => 1000 0111 0010 (0x0872)
        // --------------------------------------------------------------------------------
        // Bitstart und Laenge bleiben zwar gleich, aber die maskierten Bits und die Reihenfolge ist anders und zwar:
        // Bei Motorola muessen die Bytes verdreht werden, B0->B3, B1->B2
        // Da wie erwaehnt die Maskierung bereits verdreht ist, Bitstart aber gleich bleibt, passt es schon.
        // Man muss lediglich dran denken, wenn man im Motorola-Format den Bitstart angibt, dass man von links im Byte anfaengt zu zaehlen!
        // ================================================================================
        // --------------------------------------------------------------------------------
        // Byte order changed
        // --------------------------------------------------------------------------------
        value = (uint32_t)*(ptr_to_data_block + index_startbyte + 3) + ((uint32_t)*(ptr_to_data_block + index_startbyte + 2) << 8)
        + ((uint32_t)*(ptr_to_data_block + index_startbyte + 1) << 16) + ((uint32_t)*(ptr_to_data_block + index_startbyte) << 24);

        // --------------------------------------------------------------------------------
        // right shift, bitstart was changed given
        // --------------------------------------------------------------------------------
        value >>= (uint8_t)(32 - ((startbit & 0x07) + length));
    }
    else
    {
        // ================================================================================
        // data format INTEL --> Little Endian
        // ================================================================================
        // --------------------------------------------------------------------------------
        // Bytes normal
        // --------------------------------------------------------------------------------
        value = (uint32_t)*(ptr_to_data_block + index_startbyte) + ((uint32_t)*(ptr_to_data_block + index_startbyte + 1) << 8)
        + ((uint32_t)*(ptr_to_data_block + index_startbyte + 2) << 16) + ((uint32_t)*(ptr_to_data_block + index_startbyte + 3) << 24);

        // --------------------------------------------------------------------------------
        // right shift in range of 0-7
        // --------------------------------------------------------------------------------
        value >>= (startbit & 0x07);
    }

    mask = 0;

    for(counter = 0; counter < length; counter++)
    {
        mask = (mask << 1) | 0x00000001;
    }

    value &= mask;

    return value;
}

#if ARCHITECTURE_64BIT
/*----------------------------------------------------------------------------*/
/**
* \internal
* implementation of sfl_db_change_intel_to_motorola_64_bit function to change endianess intel -> motorola
* \endinternal
*/
uint64_t sfl_db_change_intel_to_motorola_64_bit(uint64_t val_in)
{
    uint64_t val_out;

    val_out = ( ((val_in & 0x00000000000000FF) << 56) + ((val_in & 0x000000000000FF00) << 40)
              + ((val_in & 0x0000000000FF0000) << 24) + ((val_in & 0x00000000FF000000) <<  8)
              + ((val_in & 0x000000FF00000000) >>  8) + ((val_in & 0x0000FF0000000000) >> 24)
              + ((val_in & 0x00FF000000000000) >> 40) + ((val_in & 0xFF00000000000000) >> 56) );

    return val_out;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* implementation of sfl_db_put_signal_value_to_data_block_64 function to set data point into data block
* \endinternal
*
*/
void sfl_db_put_signal_value_to_data_block_64(uint64_t value, uint8_t* ptr_to_data_block, uint16_t startbit, uint16_t length, uint8_t data_format)
{
    uint8_t index_startbyte;
    uint8_t sh_left;
    uint8_t counter;

    uint64_t maske;
    uint64_t value_block;


    // --------------------------------------------------------------------------------
    // startbit in data block
    // --------------------------------------------------------------------------------
    if( startbit > 63 )
    {
        return;
    }

    // --------------------------------------------------------------------------------
    // reached max data block length
    // --------------------------------------------------------------------------------
    if( (startbit + length) > 64 )
    {
        return;
    }

    // --------------------------------------------------------------------------------
    // check if signal length is bigger then uint64_t
    // --------------------------------------------------------------------------------
    if( (length > 64) || !length )
    {
        return;
    }

    maske = 0;

    for(counter = 0; counter < length; counter++)
    {
        maske = (maske << 1) | 0x00000001;
    }

    index_startbyte = startbit >> 3;

    // --------------------------------------------------------------------------------
    // at first we save the existing 8-Byte-Block
    // --------------------------------------------------------------------------------
    value_block =  (uint64_t)*(ptr_to_data_block + index_startbyte)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 1) << 8)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 2) << 16)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 3) << 24)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 4) << 32)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 5) << 40)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 6) << 48)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 7) << 56);

    // --------------------------------------------------------------------------------
    // only the relevant bits
    // --------------------------------------------------------------------------------
    value &= maske;

    // --------------------------------------------------------------------------------
    // check for the correct data format - change value and mask from INTEL to MOTOROLA
    // --------------------------------------------------------------------------------
    if (data_format == TRUE)
    {
        // ================================================================================
        // data format MOTOROLA --> Big Endian
        // ================================================================================
        // --------------------------------------------------------------------------------
        // first shift left and then change the bytes direction
        // --------------------------------------------------------------------------------
        sh_left = (uint8_t)(64 - ((startbit & 0x07) + length));
        value <<= sh_left;

        // --------------------------------------------------------------------------------
        // change the bytes direction
        // --------------------------------------------------------------------------------
        value = sfl_db_change_intel_to_motorola_64_bit(value);
        maske <<= sh_left;

        // --------------------------------------------------------------------------------
        // change the bytes direction
        // --------------------------------------------------------------------------------
        maske = sfl_db_change_intel_to_motorola_64_bit(maske);
    }
    else
    {
        sh_left = startbit & 0x07;
        value <<= sh_left;
        maske <<= sh_left;
    }

    // --------------------------------------------------------------------------------
    // save the signal different bits
    // --------------------------------------------------------------------------------
    maske = value_block & ~maske;

    // --------------------------------------------------------------------------------
    // bitwise or, signal value with signal different bits
    // --------------------------------------------------------------------------------
    value_block = value | maske;


    // --------------------------------------------------------------------------------
    // write back all 4 byte
    // --------------------------------------------------------------------------------
    *(ptr_to_data_block + index_startbyte)     = (uint8_t) (value_block & 0x00000000000000ffL);
    *(ptr_to_data_block + index_startbyte + 1) = (uint8_t)((value_block & 0x000000000000ff00L) >> 8);
    *(ptr_to_data_block + index_startbyte + 2) = (uint8_t)((value_block & 0x0000000000ff0000L) >> 16);
    *(ptr_to_data_block + index_startbyte + 3) = (uint8_t)((value_block & 0x00000000ff000000L) >> 24);
    *(ptr_to_data_block + index_startbyte + 4) = (uint8_t)((value_block & 0x000000ff00000000L) >> 32);
    *(ptr_to_data_block + index_startbyte + 5) = (uint8_t)((value_block & 0x0000ff0000000000L) >> 40);
    *(ptr_to_data_block + index_startbyte + 6) = (uint8_t)((value_block & 0x00ff000000000000L) >> 48);
    *(ptr_to_data_block + index_startbyte + 7) = (uint8_t)((value_block & 0xff00000000000000L) >> 56);

    return;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* implementation of sfl_db_get_signal_value_from_data_block function to get data point from data block
* \endinternal
*
*/
uint64_t sfl_db_get_signal_value_from_data_block_64(uint8_t* ptr_to_data_block, uint16_t startbit, uint16_t length, uint8_t data_format)
{
    uint8_t index_startbyte;
    uint8_t counter;

    uint64_t value;
    uint64_t mask;

    // --------------------------------------------------------------------------------
    // startbit in data block
    // --------------------------------------------------------------------------------
    if( startbit > 63 )
    {
        return 0;
    }
    else
    {
        // do nothing
    }

    // --------------------------------------------------------------------------------
    // reached max data block length
    // --------------------------------------------------------------------------------
    if( (startbit + length) > 64 )
    {
        return 0;
    }
    else
    {
        // do nothing
    }

    // --------------------------------------------------------------------------------
    // check if signal length is bigger then uint64_t
    // --------------------------------------------------------------------------------
    if( (length > 64) || !length )
    {
        return 0;
    }
    else
    {
        // do nothing
    }

    index_startbyte = startbit >> 3;

    // --------------------------------------------------------------------------------
    // check for the correct data format
    // --------------------------------------------------------------------------------
    if (data_format == TRUE)
    {
        // ================================================================================
        // data format MOTOROLA --> Big Endian
        // ================================================================================
        // Intel (Little Endian): Bitstart=2, Length=12
        // ---- ---- | ---- ---- | --xx xxxx | xxxx xx--
        //                           .. .987   6543 21
        // 0000 0000   0000 0000   0010 0001   1100 1000
        //
        // => 1000 0111 0010 (0x0872)
        //
        //
        // Motorola (Big Endian): Bitstart=2, Length=12
        // ---- ---- | ---- ---- | xxxx xx-- | --xx xxxx
        //                         6543 21       .. .987
        // 0000 0000   0000 0000   1100 1000   0010 0001
        //
        // => 1000 0111 0010 (0x0872)
        // --------------------------------------------------------------------------------
        // Bitstart und Laenge bleiben zwar gleich, aber die maskierten Bits und die Reihenfolge ist anders und zwar:
        // Bei Motorola muessen die Bytes verdreht werden, B0->B3, B1->B2
        // Da wie erwaehnt die Maskierung bereits verdreht ist, Bitstart aber gleich bleibt, passt es schon.
        // Man muss lediglich dran denken, wenn man im Motorola-Format den Bitstart angibt, dass man von links im Byte anfaengt zu zaehlen!
        // ================================================================================
        // --------------------------------------------------------------------------------
        // Byte order changed
        // --------------------------------------------------------------------------------
        value =    (uint64_t)*(ptr_to_data_block + index_startbyte + 7)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 6) << 8)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 5) << 16)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 4) << 24)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 3) << 32)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 2) << 40)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 1) << 48)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte)     << 56);

        // --------------------------------------------------------------------------------
        // right shift, bitstart was changed given
        // --------------------------------------------------------------------------------
        value >>= (uint8_t)(64 - ((startbit & 0x07) + length));
    }
    else
    {
        // ================================================================================
        // data format INTEL --> Little Endian
        // ================================================================================
        // --------------------------------------------------------------------------------
        // Bytes normal
        // --------------------------------------------------------------------------------
        value =    (uint64_t)*(ptr_to_data_block + index_startbyte)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 1) << 8)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 2) << 16)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 3) << 24)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 4) << 32)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 5) << 40)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 6) << 48)
                + ((uint64_t)*(ptr_to_data_block + index_startbyte + 7) << 56);

        // --------------------------------------------------------------------------------
        // right shift in range of 0-7
        // --------------------------------------------------------------------------------
        value >>= (startbit & 0x07);
    }

    mask = 0;

    for(counter = 0; counter < length; counter++)
    {
        mask = (mask << 1) | 0x00000001;
    }

    value &= mask;

    return value;
}
#endif // ARCHITECTURE_64BIT

/** \} */
