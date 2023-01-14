/*
 * bit_acess_macros.h
 *
 *  Created on: 20.02.2020
 *      Author: mmueller2
 */

#ifndef SOURCE_HCS12_XEQ384_CAN_GATEWAY_CANGRAPH_CODE_BIT_ACCESS_MACROS_H_
#define SOURCE_HCS12_XEQ384_CAN_GATEWAY_CANGRAPH_CODE_BIT_ACCESS_MACROS_H_

/*
 * 		MACROS FOR BIT ACCESS
 */

/*	This macro is used for getting the size of an byte array that is used to store bit values
 *
 * 	e.g. you need to store 13 bits:
 * 	required size [byte] = 13 bit div 8 bit + 1 byte = 1 + 1 byte = 2 byte
 *
 * 	Note: div means integer division
 *
 * 	uint8_t bitfield[CONVERT_BIT_TO_BYTE_SIZE(13)];
 * 	  results to
 * 	uint8_t bitfield[2];
 */
#define CONVERT_BIT_TO_BYTE_SIZE(bit_size)	( ((bit_size-1u)/8u) + 1u )

//																calculate byte index		calculate bit index in selected byte
//																		  |								|
//																		  |								|
//																		  |		  clear bit by			|
//																		  |		  bit-wise "and"		|
//																		  |		with inverted value		|
//																		  |			 |					|
// Clear a bit in a byte array that is used as bit field	   		  	  |			 |					|
#define BYTE_ARRAY_CLEAR_BIT( byte_array, bit_index )   ( byte_array[bit_index/8u]  &= ~( 1u << (bit_index & 7u) ) )
//																			  			   |
//																						set bit

// Set a bit in a byte array that is used as bit field
#define BYTE_ARRAY_SET_BIT( byte_array, bit_index )    	( byte_array[bit_index/8u] |= (1u << (bit_index & 7u)) )

// Read a bit in a byte array that is used as bit field
#define BYTE_ARRAY_GET_BIT( byte_array, bit_index )		( byte_array[bit_index/8u]  & (1u << (bit_index & 7u)) )


#endif /* SOURCE_HCS12_XEQ384_CAN_GATEWAY_CANGRAPH_CODE_BIT_ACCESS_MACROS_H_ */
