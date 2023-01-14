/*
 * user_api_util.c
 *
 *  Created on: 15.10.2019
 *      Author: VGoertz
 */

#include "user_api_util.h"
#include "sfl_math.h"


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t user_lookup_1D_16(int16_t *table_x, int16_t *table_y, uint8_t count, int16_t val, enum_LUT_MODE mode)
{
    return os_util_lookup1D(table_x, table_y, count, val, mode);
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_lookup_1D_32 (int32_t *table_x, int32_t *table_y, uint8_t count, int32_t val, enum_LUT_MODE mode)
{
    return os_util_lookup1D_32(table_x, table_y, count, val, mode);
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t user_lookup_2D_16(int16_t *table_x, int16_t *table_y, uint8_t count_x, uint8_t count_y, int16_t *table_z, int16_t val_x, int16_t val_y)
{
    return os_util_lookup2D(table_x, table_y, count_x, count_y, table_z, val_x, val_y);
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_lookup_2D_32(int32_t *table_x, int32_t *table_y, uint8_t count_x, uint8_t count_y, int32_t *table_z, int32_t val_x, int32_t val_y)
{
    return os_util_lookup2D_32(table_x, table_y, count_x, count_y, table_z, val_x, val_y);
}

