/*----------------------------------------------------------------------------*/
/**
* \file         sfl_math.c
* \brief        Implementation for common mathmatic functionalities.
* \date         20170504
* \author       Riegel
*
*/
/*----------------------------------------------------------------------------*/
/**
* \addtogroup  sfl_math
* \{
*/
//--------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------
#include "sfl_math.h"

// ---------------------------------------------------------------------------------------------------
// tables
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// module globals
// ---------------------------------------------------------------------------------------------------


/*----------------------------------------------------------------------------*/
/**
* \internal
*   Lookup table (LUT) 1D.
*   On invalid param, this function returns 0
* \endinternal
*/
int16_t os_util_lookup1D(int16_t *table_x, int16_t *table_y, uint16_t count_points, int16_t val_x, enum_LUT_MODE mode)
{

    /*
    Example arrays:

    int16_t arr_x[] = {136,200,311,444,666};
    int16_t arr_y[] = {-10,5,16,27,39};

    1. Specify between which points of arr_x[] "val_x" is located (x1,x2)

    2. Calculate straight line and calculate y - value

      y  = y2 - dy;
      dy = ((y2-y1)/(x2-x1))*(x2-x);
    */

    uint16_t    i,k = 0;    // k = array index for lower point of straight line pitch calculation.
    int32_t     dy;
    int32_t     dx = 0;
    int32_t     ret;
    uint8_t     val_inside;
    int16_t     x2;
    int16_t     y2 = 0;
    int16_t     *tab_x;
    int16_t     *tab_y;

    // Preparation.
    ret        = 0L;
    val_inside = 0u; // 0 = below; 1 = inside; 2 = special processing (calibration).
    
    
    // Valid parameters?
    if( (2u <= count_points ) &&
        ((LUT_MODE_MAX) > mode) )
    {
        // Yes. Preparations for extrapolation.
        x2 = table_x[count_points - 1u];

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // Is the "x value" above "x-values array[max]"?
        if( val_x > x2 )
        {
            // Yes. Extrapolate upwards.
            // Use the last two points for extrapolation.
            k = count_points - 2u;
            
            // Set "dx" to negative value (i.e. less than val_x) because of
            // extrapolation for "val_x" overspending "x2"!
            // (i.e. in contrast to extrapolation for "val_x" underspending "x1")
            dx = x2 - val_x;
            
            // Set "y2" for calculation.
            y2 = table_y[count_points - 1u];

            // Special calibration mode?
            if( mode == LUT_MODE_CALIBRATION )
            {
                // Yes.
                // Caution: The following formula is taken over from older implementation.
                //          It is not quite clear, what is the intention/function of this formula!
                
                // Spezial beim Kalibrierung: Differenz x-y vom letzten Stützpunkt beibehalten (z.B. 700 digits -> 1000mA, 900 digits -> 1200mA)
                ret = (int32_t)val_x - ((int32_t)table_x[k] - table_y[k]);
                // Set special processing code.
                val_inside = 2u;
            }
        }
        else
        {
            // It is the normal case for "extrapolation downwards/upwards" or "interpolation downwards/upwards".
            
            // Set special processing code.
            // i.e. "val_x" is inside range of "table_x[0...max.]".
            val_inside = 1u;
            
            // Find next 2 data points.
            tab_x = table_x + 1u;
            for( i = 1u; i < count_points; i++ )
            {
                // Preparation.
                x2 = *tab_x;
                
                // Is "val_x" less than (or equal to) min. "table_x[i]" value.
                if( val_x <= x2 )
                {
                    // Yes. Normal case for extrapolation/interpolation.
                    // Use this point and the previous point for extra-/interpolation.
                    k  = i - 1u;
                    y2 = *(table_y + i);
                    
                    // Did "val_x" underspend "x1"?
                    if( val_x < *table_x )
                    {
                        // Yes. Preparation for "extrapolation downwards".
                        x2 = *table_x;
                        y2 = *table_y;
                    }   
                    
                    // Set "dx" to value if "val_x" is below "x2"
                    // (i.e. normal case for extrapolation)
                    dx = x2 - val_x;
                    break;
                }
                else
                {
                    // do nothing
                }
                
                // Increment the pointer at very end of the loop!
                tab_x++;
                
            } // for( i = 1u; i < (count_points - 1u); i++ )
        }
            
        // ----------------------------------------------------------------
        // ----------------------------------------------------------------
        // No special processing code?
        if( 2u != val_inside )
        {
            // No special processing code.
            
            // Limitation to min. request?
            if( mode == (LUT_MODE_LIMIT) )
            {
                // Yes. Is the "x value" below "x-values array[0]"?
                x2 = table_x[0u];
                if( val_x < x2 )
                {
                    // Yes. Return value is limited to "y-values array[0]".
                    ret = table_y[0u];
                    // Set special processing code.
                    val_inside = 2u;
                }
            }
            // Limitation to min. request?
            else if( mode == (LUT_MODE_MAX) )
            {
                // Yes. Is the "x value" above "x-values array[max]"?
                x2 = table_x[count_points - 1u];
                if( val_x > x2 )
                {
                    // Yes. Return value is limited to "y-values array[max]".
                    ret = table_y[count_points - 1u];
                    // Set special processing code.
                    val_inside = 2u;
                }
            }
        }
            
        // ----------------------------------------------------------------
        // ----------------------------------------------------------------
        
        // No special processing code?
        if( 2u != val_inside )
        {
            // No special processing code.
            
            // This is the normal "extrapolation" or "interpolation".
            // Set pointers to points.
            tab_x = table_x + k;
            tab_y = table_y + k;
            
            // y  = y2 - dy;
            // dy = ((y2-y1)/(x2-x1))*(x2-x);
            //
            // Implement the calculation in optimized sequence.
            dy  = (int32_t)(*(tab_y+1u) - *tab_y) * (int32_t)dx;
            dy /= (int32_t)(*(tab_x+1u) - *tab_x);
            ret = y2 - dy;

        }
            
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // Limit value range to signed integer
        if( ret > NR_MAX_INT16 )
        {
            ret = NR_MAX_INT16;
        }
        else if( ret < NR_MIN_INT16 )
        {
            ret = NR_MIN_INT16;
        }
        else
        {
            // do nothing
        }

        // Limitation to zero on special cases.
        if( ((mode == LUT_MODE_CALIBRATION) || (mode == LUT_MODE_EXTRAPOLATION_POS)) && (ret < 0L) )
        {
            ret = 0L;
        }
        else
        {
            // do nothing
        }
        
    } // if( 2u > count_points )

    return ret;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*   Lookup table (LUT) 1D 32bit signed long values
* \endinternal

*/
int32_t os_util_lookup1D_32(int32_t *table_x, int32_t *table_y, uint16_t count, int32_t val, enum_LUT_MODE mode)
{

    /*
    Example arrays:

    int32_t arr_x[] = {136,200,311,444,666};
    int32_t arr_y[] = {-10,5,16,27,39};

    1. Specify between which points of arr_x[] "val" is located (x2,x1)

    2. Calculate straight line and calculate y - value

       m = (y2-y1)/(x2-x1)
       b = y1 - m * x1;
       y = m * val + b;
    */

    uint16_t i,k = 0;
    int32_t m, b;
    int32_t ret;
    uint8_t val_inside=0;

    // Extrapolate downwards?
    if( val <= table_x[0] )
    {
        k = 0;
    }
    // Extrapolate upwards?
    else if( val >= table_x[count - 1] )
    {
        if( (mode == LUT_MODE_EXTRAPOLATION) || (mode == LUT_MODE_EXTRAPOLATION_POS) )
        {
            k = count-2;
        }
        else
        {
            k = count-1;
        }

        if( mode == LUT_MODE_CALIBRATION )
        {
            // Spezial beim Kalibrierung: Differenz x-y vom letzten Stützpunkt beibehalten (z.B. 700 digits -> 1000mA, 900 digits -> 1200mA)
            ret = val - (table_x[k] - table_y[k]);
            val_inside = 2;
        }
        else
        {
            // do nothing
        }
    }
    // Else find next 2 data points
    else
    {
        for( i = 0; i < (count - 1); i++ )
        {
            if( val < table_x[i + 1] )
            {
                k = i;
                break;
            }
            else
            {
                // do nothing
            }
        }

        val_inside = 1;
    }

    // At limit and value out of range => use next y - value (min or max)
    if( (mode == LUT_MODE_LIMIT) && (val_inside == 0) )
    {
        ret = table_y[k];
    }
    // Otherwise if no skip of MODE_KALIBRATION calculate normally
    else if( val_inside != 2 )
    {
        // y = mx+b;
        //      m = (y2-y1)/(x2-x1);
        //      b = y-mx;
        m = ((table_y[k+1] - table_y[k]) * 1000L) / (table_x[k+1] - table_x[k]);
        b = table_y[k] - ((m * table_x[k]) / 1000L);
        ret = (m * val) / 1000L + b;
    }
    else
    {
        // do nothing
    }

    if( ((mode == LUT_MODE_CALIBRATION) || (mode == LUT_MODE_EXTRAPOLATION_POS)) && (ret < 0) )
    {
        ret = 0;
    }
    else
    {
        // do nothing
    }

    return ret;
}


/*----------------------------------------------------------------------------*/
/** 
* \internal
* \endinternal

*/
int16_t os_util_lookup2D(int16_t *table_x, int16_t *table_y, uint8_t count_x, uint8_t count_y, int16_t *table_z, int16_t val_x, int16_t val_y)
{
    /*
    Example field:

    int16_t arr_x_2d[] = {10,20,30,40,50};
    int16_t arr_y_2d[] = {100,200,300};

    // y * x Matrix
    int16_t arr_z[] = {
        10,  20,  50, 90,150,
        25,  70, 150,250,400,
        50, 120, 240,400,860
    };

    1. Specify between which points of table_x[] "val_x" is located (x2,x1)

    2. Specify between which points of table_y[] "val_y" is located (y2,y1)

    3. Calculate 2 lines similar to Lookup1D, first at location y (a), second at location y+1 (b) and calculate for each z-point z1,z2

       m1 = (z2a-z1a)/(x2-x1)
       b1 = z1a - m1 * x1;
       z1 = m1 * val_x + b1;

       m2 = (z2b-z1b)/(x2-x1)
       b2 = z1b - m2 * x1;
       z2 = m2 * val_x + b2;

    4. Calculate third line orthogonal to them with 2 calculated points z1,z2 and the 2 points on y-axis y1,y2 => This results in endpoint z

       m3 = (z2-z1)/(y2-y1)
       b3 = z1 - m3 * y1
       z  = m3 * val_y + b3
    */

    uint8_t x,y,i,x_1,y_1,y_raw;
    int32_t m1,m2,m3, b1,b2,b3, z1,z2,z;

    // 1. Specify between which points of table_x[] "val_x" is located (x2,x1)

    x = count_x-2;  // By default last line part

    for( i = 0; i < (count_x - 1); i++ )
    {
        if( val_x < table_x[i + 1] )
        {
            x = i;
            break;
        }
        else
        {
            // do nothing
        }
    }

    // 2. Specify between which points of table_y[] "val_y" is located (y2,y1)

    y_raw = count_y-2;  // By default last line part
    y = y_raw * count_x;

    for( i = 0; i < (count_y - 1); i++ )
    {
        if( val_y < table_y[i + 1] )
        {
            y_raw = i;              // Raw value for third line
            y = y_raw * count_x;    // Because of 1D array jump to proper point
            break;
        }
        else
        {
            // do nothing
        }
    }

    // Backup
    x_1 = x + 1;
    y_1 = y + count_x;

    // Line 1 at position y
    m1 = ((int32_t)(table_z[y+x_1] - table_z[y+x]) * 1000L) / (int32_t)(table_x[x_1] - table_x[x]);   // m=(y2-y1)/(x2-x1)
    b1 = table_z[y+x] - (m1 * table_x[x]) / 1000L; // b = y-mx
    z1 = (m1 * val_x) / 1000L + b1;

    // Line 2 at position y+1
    m2 = ((int32_t)(table_z[y_1+x_1] - table_z[y_1+x]) * 1000L) / (int32_t)(table_x[x_1] - table_x[x]);
    b2 = table_z[y_1+x] - (m2 * table_x[x]) / 1000L;
    z2 = (m2 * val_x) / 1000L + b2;

    // Third line orthogonal with 2 new points y1,y2
    m3 = ((z2-z1) * 1000L) / (table_y[y_raw+1] - table_y[y_raw]); // 2 z-points of z1,z2 and 2 y-values of table_y
    b3 = z1 - (m3 * table_y[y_raw]) / 1000L;
    z  = (m3 * val_y) / 1000L + b3;

    // Limit value range to signed integer
    if( z > NR_MAX_INT16 )
    {
        z = NR_MAX_INT16;
    }
    else if( z < NR_MIN_INT16 )
    {
        z = NR_MIN_INT16;
    }
    else
    {
        // do nothing
    }

    return (int16_t)z;
}

/*----------------------------------------------------------------------------*/
/** 
* \internal
*   
* \endinternal

*/
int32_t os_util_lookup2D_32(int32_t *table_x, int32_t *table_y, uint8_t count_x, uint8_t count_y, int32_t *table_z, int32_t val_x, int32_t val_y)
{
    uint8_t x,y,i,x_1,y_1,y_raw;
    int32_t m1,m2,m3, b1,b2,b3, z1,z2,z;

    // 1. Specify between which points of table_x[] "val_x" is located (x2,x1)

    x = count_x-2;  // By default last line part

    for (i=0;i<count_x-1;i++)
    {
        if (val_x < table_x[i+1])
        {
            x = i;
            break;
        }
    }

    // 2. Specify between which points of table_y[] "val_y" is located (y2,y1)

    y_raw = count_y-2;  // By default last line part
    y = y_raw * count_x;

    for (i=0;i<count_y-1;i++)
    {
        if (val_y < table_y[i+1])
        {
            y_raw = i;              // Raw value for third line
            y = y_raw * count_x;    // Because of 1D array jump to proper point
            break;
        }
    }

    // Backup
    x_1 = x + 1;
    y_1 = y + count_x;

    // Line 1 at position y
    m1 = ((int32_t)(table_z[y+x_1] - table_z[y+x]) * 1000L) / (int32_t)(table_x[x_1] - table_x[x]);   // m=(y2-y1)/(x2-x1)
    b1 = table_z[y+x] - (m1 * table_x[x]) / 1000L; // b = y-mx
    z1 = (m1 * val_x) / 1000L + b1;

    // Line 2 at position y+1
    m2 = ((int32_t)(table_z[y_1+x_1] - table_z[y_1+x]) * 1000L) / (int32_t)(table_x[x_1] - table_x[x]);
    b2 = table_z[y_1+x] - (m2 * table_x[x]) / 1000L;
    z2 = (m2 * val_x) / 1000L + b2;

    // Third line orthogonal with 2 new points y1,y2
    m3 = ((z2-z1) * 1000L) / (table_y[y_raw+1] - table_y[y_raw]); // 2 z-points of z1,z2 and 2 y-values of table_y
    b3 = z1 - (m3 * table_y[y_raw]) / 1000L;
    z  = (m3 * val_y) / 1000L + b3;

    return z;
}

/** \} */
