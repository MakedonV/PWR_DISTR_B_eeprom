#ifndef SRC_APP_USER_API_USER_API_UTIL_H_
#define SRC_APP_USER_API_USER_API_UTIL_H_
/*----------------------------------------------------------------------------*/
/**
* \file         user_api_util.h
* \brief        Declaration LookUp functionality
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup  user_api_util LOOKUP TABLES
* \{
* \brief     Working with lookup tables
* \details
*/
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include "sfl_math.h"


/*----------------------------------------------------------------------------*/
/**
* \brief    16 bit 1-D Lookup Table
* \details  To approximate one-dimensional function
*
* \param    *table_x [in] int16_t       Pointer to an x-array
* \param    *table_y [in] int16_t       Pointer to an y-array
* \param    count    [in] uint8_t       Counts of x-array elements
* \param    val      [in] int16_t       x-value
* \param    mode     [in] enum_LUT_MODE LUT_MODE
*
* \return   uint16_t                Returns the approximate value
*/
uint16_t user_lookup_1D_16(int16_t *table_x, int16_t *table_y, uint8_t count, int16_t val, enum_LUT_MODE mode);
#define user_util_get_lookup_value_1D_16( table_x,table_y,count,val,mode ) user_lookup_1D_16( table_x,table_y,count,val,mode )


/*----------------------------------------------------------------------------*/
/**
* \brief    32 bit 1-D Lookup Table
* \details  To approximate one-dimensional function
*
* \param    *table_x [in] int32_t       Pointer to an x-array
* \param    *table_y [in] int32_t       Pointer to an y-array
* \param    count    [in] uint8_t       Counts of x-array elements
* \param    val      [in] int32_t       x-value
* \param    mode     [in] enum_LUT_MODE LUT_MODE
* \return   uint32_t                Returns the approximate value
*/
uint32_t user_lookup_1D_32 (int32_t *table_x, int32_t *table_y, uint8_t count, int32_t val, enum_LUT_MODE mode);
#define user_util_get_lookup_value_1D_32( table_x,table_y,count,val,mode ) user_lookup_1D_32( table_x,table_y,count,val,mode )


/*----------------------------------------------------------------------------*/
/**
* \brief    16 bit 2-D Lookup Table
* \details  To approximate two-dimensional function
*
* \param    *table_x [in] int16_t   Pointer to an x-array
* \param    *table_y [in] int16_t   Pointer to an y-array
* \param    count_x  [in] uint8_t   Counts of x-array elements
* \param    count_y  [in] uint8_t   Counts of y-array elements
* \param    *table_z [in] int16_t   Pointer to an z-array
* \param    val_x    [in] int16_t   x-value
* \param    val_y    [in] int16_t   y-value
*
* \return   uint16_t                Returns the approximate value
*/
uint16_t user_lookup_2D_16(int16_t *table_x, int16_t *table_y, uint8_t count_x, uint8_t count_y, int16_t *table_z, int16_t val_x, int16_t val_y);
#define user_util_get_lookup_value_2D_16( table_x,table_y,count_x,count_y,table_z,val_x,val_y ) user_lookup_2D_16( table_x,table_y,count_x,count_y,table_z,val_x,val_y )


/*----------------------------------------------------------------------------*/
/**
* \brief    32 bit 2-D Lookup Table
* \details  To approximate two-dimensional function
*
* \param    *table_x [in] int32_t   Pointer to an x-array
* \param    *table_y [in] int32_t   Pointer to an y-array
* \param    count_x  [in] uint8_t   Counts of x-array elements
* \param    count_y  [in] uint8_t   Counts of y-array elements
* \param    *table_z [in] int32_t   Pointer to an z-array
* \param    val_x    [in] int32_t   x-value
* \param    val_y    [in] int32_t   y-value
*
* \return   uint32_t                Returns the approximate value
*/
uint32_t user_lookup_2D_32(int32_t *table_x, int32_t *table_y, uint8_t count_x, uint8_t count_y, int32_t *table_z, int32_t val_x, int32_t val_y);
#define user_util_get_lookup_value_2D_32( table_x,table_y,count_x,count_y,table_z,val_x,val_y ) user_lookup_2D_32( table_x,table_y,count_x,count_y,table_z,val_x,val_y )


/** \} */
#endif /* SRC_APP_USER_API_USER_API_UTIL_H_ */
