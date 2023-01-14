#ifndef SFL_MATH_H
#define SFL_MATH_H

/*----------------------------------------------------------------------------*/
/**
* \file         sfl_math.h
* \brief        Definitions for common mathmatic functionalities.
*
*/
/*----------------------------------------------------------------------------*/
/**
* \addtogroup sfl_math SFL Mathematic [MATH]
* \{
* \brief    Mathematic functions.
* \details  With these functions you are able to do a mapping between different units e.g. Digits to Temperature.
*           Basic for this function is a linear interpolation between two points of a straight line. The points are part of a translation table (2D-Array or 2 Arrays with same length)
*           which contain the x values and the corresponding y values.
*
*           Example:
*           --------
*           Here calculate the temperature from raw ADC digits (fixed used 1553).
* \code{.c}
*    #define CALIB_POINTS 5
*  // (1) Create a 2D mapping table
*  uint16_t adc_temp_map[2][CALIB_POINTS] =
*  {
*  //    20°C, 25°C, 30°C, 35°C, 40°C
*      { 1090, 1260, 1430, 1610, 1792 }, // x-values (ADC-Digits)
*      {  200,  250,  300,  350,  400 }  // y-values (Temperature in 0,1°C)
*  };
*
*    uint16_t adc_in = 1520u;
*    uint16_t temperature;
*    temperature = os_util_lookup1D(&adc_temp_map[0],&adc_temp_map[1],CALIB_POINTS,adc_in,LUT_MODE_EXTRAPOLATION);
*    // 1520 <=> 32,5°C = 325
* \endcode
*   For version information see file sfl_math_version.h
*/
/*----------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "hal_data_types.h"

// ---------------------------------------------------------------------------------------------------
// defines
// ---------------------------------------------------------------------------------------------------
#define NR_MAX_INT16    32767       ///< Maximum positive number of an 16 bit signed integer value.
#define NR_MIN_INT16   -32768       ///< Minimum negative number if an 16 bit signed integer value.

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------
/** This enum lists all available modes for the lookup functions.
* For detailed desciption of what to do see the implementation description of os_util_lookup1D().
*
*/
typedef enum
{
    LUT_MODE_EXTRAPOLATION =0,      ///< Use Linear interpolation for values that exceed the min and max val of the lookup table. (Use this if unsure!)
    LUT_MODE_LIMIT           ,      ///< When value is < min or > max limit the value to the maximum values from lookup table.
    LUT_MODE_CALIBRATION     ,      ///< Internal used to calibrate different values.
    LUT_MODE_EXTRAPOLATION_POS,     ///< (Positive) Same as LUT_MODE_EXTRAPOLATION but return just values >= 0 (Low level limit to 0).
    LUT_MODE_MAX                    ///< Last element in enum. Do not use.
}enum_LUT_MODE;

// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------


/*----------------------------------------------------------------------------*/
/**
* \brief    Lookup table (LUT) 1D (16-Bit)
* \details  Math form is: y=f(x) [ = m*x+b];
*   This function checks between which X-array index the value is. Remember that both X and Y arrays need to have the same size.
*   Then it calculates the gradient and the offset between found index and next one.
*   Depending on this, the Y value for given x(val) is interpolated.
*   When parameter mode is Extrapolation, values that are higher than last element in table are calculated with the same gradient as last index.
*   LUT_MODE_LIMIT forbids outputs that are higher than maximum X array index.
*   IMPORTANT: This function only supports X tables with ascending order
*
*   Remember that this function only works with 16 bit, Don't pass too large numbers here or overruns can happen.
*   \todo In C it is better to first multiply then divide - Don't calculate M and B (y=M*x+B), prefer using M*x. This reduces rounding and cut off errors.
*   \todo Maybe split this function into Interpolate function ( calculate y=mx+b ) and array index verwurstling
*
*
* \param    *table_x     [in] int16_t   Pointer to x-values array. Same Unit as val parameter, Minimum is 2 elements.
* \param    *table_y     [in] int16_t   Pointer to y-values array. Output Unit.
* \param    count_points [in] uint16_t  Number of elements in the x and y-array
* \param    val_x        [in] int16_t   The value for that should be converted to y. Unit same as x-array.
* \param    mode         [in] uint8_t   See #enum_LUT_MODE
*                                   LUT_MODE_EXTRAPOLATION (Standard): away from the array extrapolate
*                                   LUT_MODE_LIMIT: away from the array not extrapolate
*                                   LUT_MODE_KALIBRATION: y-value always >= 0, extrapolating upwards with difference (y_max - x_max)
* \return   int16_t                 The calculated y value (Unit from y-array).
*/
int16_t os_util_lookup1D (int16_t *table_x, int16_t *table_y, uint16_t count_points, int16_t val_x, enum_LUT_MODE mode);


/*----------------------------------------------------------------------------*/
/**
* \brief    Lookup table (LUT) 1D (32-Bit)
* \details  Math form is: y=f(x) [ = m*x+b];
*   This function checks between which X-array index the value is. Remember that both X and Y arrays need to have the same size.
*   Then it calculates the gradient and the offset between found index and next one.
*   Depending on this, the Y value for given x(val) is interpolated.
*   When parameter mode is Extrapolation, values that are higher than last element in table are calculated with the same gradient as last index.
*   LUT_MODE_LIMIT forbids outputs that are higher than maximum X array index.
*   IMPORTANT: This function only supports X tables with ascending order
*
*   Remember that this function only works with 32 bit, Don't pass too large numbers here or overruns can happen.
*   \todo In C it is better to first multiply then divide - Don't calculate M and B (y=M*x+B), prefer using M*x. This reduces rounding and cut off errors.
*   \todo Maybe split this function into Interpolate function ( calculate y=mx+b ) and array index verwurstling
*
*
* \param    *table_x [in] int32_t   Pointer to x-values array. Same Unit as val parameter, Minimum is 2 elements.
* \param    *table_y [in] int32_t   Pointer to y-values array. Output Unit.
* \param    count    [in] uint16_t  Number of elements in the x and y-array
* \param    val      [in] int32_t   The value for that should be converted to y. Unit same as x-array.
* \param    mode     [in] uint8_t   See #enum_LUT_MODE
*                                   LUT_MODE_EXTRAPOLATION (Standard): away from the array extrapolate
*                                   LUT_MODE_LIMIT: away from the array not extrapolate
*                                   LUT_MODE_KALIBRATION: y-value always >= 0, extrapolating upwards with difference (y_max - x_max)
* \return   int32_t                 The calculated y value (Unit from y-array).
*/
int32_t os_util_lookup1D_32(int32_t *table_x, int32_t *table_y, uint16_t count, int32_t val, enum_LUT_MODE mode);



/*----------------------------------------------------------------------------*/
/**
* \brief    Calculates an output for 2 Parameter inputs. (16 Bit)
* \details  Math form would be: y=f(x1,x2). For example you can calculate a
*           PWM dutycycle for a (dynamic) input voltage and an encoder (freq) input.
*           On high frequencys, you need a lower duty, to rotate slower but on low voltages
*           you have to increase the duty to have enough power. This results in a 3D chart from
*           x-Axis: freq input; y Axis: input voltage and 3rd dimension is output duty.
*
*
*
* \param    *table_x [in] int16_t   Pointer to x-values array (1D)
* \param    *table_y [in] int16_t   Pointer to y-values array (1D)
* \param    count_x  [in] uint8_t   Number of elements of x-array
* \param    count_y  [in] uint8_t   Number of elements of y-array
* \param    table_z  [in] int16_t   Pointer z-array (1D with (count_y * count_x) elements)
* \param    val_x    [in] int16_t   x-value input (away from the x-array => extrapolation)
* \param    val_y    [in] int16_t   y-value input(away from the y-array => extrapolation)
* \return   int16_t                 z-value output
*/
int16_t os_util_lookup2D   (int16_t *table_x, int16_t *table_y, uint8_t count_x, uint8_t count_y, int16_t *table_z, int16_t val_x, int16_t val_y);

/*----------------------------------------------------------------------------*/
/**
* \brief    Calculates an output for 2 Parameter inputs. (32 Bit)
* \details  Math form would be: y=f(x1,x2). For example you can calculate a
*           PWM dutycycle for a (dynamic) input voltage and an encoder (freq) input.
*           On high frequencys, you need a lower duty, to rotate slower but on low voltages
*           you have to increase the duty to have enough power. This results in a 3D chart from
*           x-Axis: freq input; y Axis: input voltage and 3rd dimension is output duty.
*
*
*
* \param    *table_x [in] int32_t   Pointer to x-values array (1D)
* \param    *table_y [in] int32_t   Pointer to y-values array (1D)
* \param    count_x  [in] uint8_t   Number of elements of x-array
* \param    count_y  [in] uint8_t   Number of elements of y-array
* \param    table_z  [in] int32_t   Pointer z-array (1D with (count_y * count_x) elements)
* \param    val_x    [in] int32_t   x-value input (away from the x-array => extrapolation)
* \param    val_y    [in] int32_t   y-value input(away from the y-array => extrapolation)
* \return   int32_t                 z-value output
*/
int32_t os_util_lookup2D_32(int32_t *table_x, int32_t *table_y, uint8_t count_x, uint8_t count_y, int32_t *table_z, int32_t val_x, int32_t val_y);


/** \}*/
#endif
