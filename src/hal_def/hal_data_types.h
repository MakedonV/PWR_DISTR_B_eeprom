#ifndef HAL_DATA_TYPES_H
#define HAL_DATA_TYPES_H

/*----------------------------------------------------------------------------*/
/**
* \file         hal_data_types.h
* \brief        Definitions for hardware (and platform) independent data types.
*/
/*----------------------------------------------------------------------------*/
/** 
* \addtogroup   doc_hal_data_types HAL DATT
* \{
* \brief        This documentation part describes the MRS wide standard data types for all hw platforms.
* \details      Main goal is to create a hardware-platform independent layer.
*               Most of this file is platform dependent data type definition.
*/
/*----------------------------------------------------------------------------*/

#define MRS_LITTLE_ENDIAN   1234
#define MRS_BIG_ENDIAN      4321
    // ---------------------------------------------------------------------------------------------------
    // RH850 GHS
    // ---------------------------------------------------------------------------------------------------
    #if defined __V850__ && defined __ghs__

        #define RH850F1L_GHS                 1

        // ---------------------------------------------------------------------------------------------------
        // unsigned data types
        // ---------------------------------------------------------------------------------------------------
        //typedef bool                        _Bool;
        typedef unsigned char                boolean;           ///< boolean is stored as unsigned 8.
        typedef unsigned char               uint8_t;            ///< data type unsigned 8 Bit.
        typedef unsigned short              uint16_t;           ///< data type unsigned 16 Bit.
        typedef unsigned long               uint32_t;           ///< data type unsigned 32 Bit.
        typedef unsigned long long          uint64_t;           ///< data type unsigned 64 Bit.

        // ---------------------------------------------------------------------------------------------------
        // signed data types
        // ---------------------------------------------------------------------------------------------------

        typedef signed char                   int8_t;           ///< data type signed 8 Bit.
        typedef signed short                  int16_t;          ///< data type signed 16 Bit.
        typedef signed long                   int32_t;          ///< data type signed 32 Bit.
        typedef signed long long              int64_t;          ///< data type signed 64 Bit.

        typedef float                       float32_t;          ///< data type float.
        typedef double                      float64_t;          ///< data type double.
        
        typedef uint8_t*  const reg_8bit_t;                     ///< unsigned 8 pointer.
        typedef uint16_t* const reg_16bit_t;                    ///< unsigned 16 pointer.
        typedef uint32_t* const reg_32bit_t;                    ///< unsigned 32 pointer.

        #define INT8_MAX    127                                 ///< Maximum value for a signed 8 bit variable.
        #define INT16_MAX   32767                               ///< Maximum value for a signed 16 bit variable.
        #define INT32_MAX   2147483647                          ///< Maximum value for a signed 32 bit variable.
        #define INT64_MAX   9223372036854775807LL               ///< Maximum value of a signed 64 bit variable.

        #define UINT8_MAX   0xffu                               ///< Maximum value for a unsigned 8 bit variable (255U).
        #define UINT16_MAX  0xffffu                             ///< Maximum value for a unsigned 16 bit variable (65535U).
        #define UINT32_MAX  0xffffffffu                         ///< Maximum value for a unsigned 32 bit variable (4294967295U).
        #define UINT64_MAX  0xffffffffffffffffULL               ///< Maximum value for a unsigned 64 bit variable (18446744073709551615ULL).

        #define ARCHITECTURE_64BIT 1                            ///< Define to allow 64 Bit access.
        #define ARCHITECTURE_BYTE_ORDER     MRS_LITTLE_ENDIAN   ///< RH850 is Little endian

    // ---------------------------------------------------------------------------------------------------
    // standard GCC (also used by the GNU ARM Embedded Toolchain, e.g. for MK10D10)
    // ---------------------------------------------------------------------------------------------------
    #elif defined __GNUC__

        // 64 Bit, e.g. LINUX on PC
        #if __x86_64
            #define ARCHITECTURE_64BIT 1                        ///< Enable 64 bit usage when platform not has ressources.
        #elif __amd64
            #define ARCHITECTURE_64BIT 1                        ///< Enable 64 bit usage when platform not has ressources.
        #endif

        #include <stddef.h>                                     ///< This include provides NULL
        #include <stdint.h>                                     ///< This include provides uintx_t data types.
        #include <stdbool.h>                                    ///< This include provides _Bool datatypes.

        typedef _Bool boolean;                                  ///< define boolean type.

        #if defined __BYTE_ORDER__
        // S32K and Native Linux (sw-Test) has defined __BYTE_ORDER__
            #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
                #define ARCHITECTURE_BYTE_ORDER MRS_BIG_ENDIAN
            #elif __BYTE_ORDER__ ==  __ORDER_LITTLE_ENDIAN__
                #define ARCHITECTURE_BYTE_ORDER MRS_LITTLE_ENDIAN
            #else
                #error "unimplemented endianes detected!"
            #endif
        #else
        // On Windows Mingw env, we need <sys/param.h> and other defines
            #include <sys/param.h>
            #if defined BYTE_ORDER
                // Sw-test MinGW Windows i686
                #if BYTE_ORDER == LITTLE_ENDIAN
                    #define ARCHITECTURE_BYTE_ORDER MRS_LITTLE_ENDIAN
                #elif BYTE_ORDER == BIG_ENDIAN
                    #define ARCHITECTURE_BYTE_ORDER MRS_BIG_ENDIAN
                #else
                    #error "unimplemented endianes detected!"
                #endif
            #endif
        #endif

    // ---------------------------------------------------------------------------------------------------
    // Linux QT
    // ---------------------------------------------------------------------------------------------------
    #elif QT_CORE_LIB

        #include <QtGlobal>
        typedef quint8         uint8_t;                         ///< Datatype unsigned  8 Bit.
        typedef qint16         int16_t;                         ///< Datatype signed   16 Bit.
        typedef quint16        uint16_t;                        ///< Datatype unsigned 16 Bit.
        typedef qint32         int32_t;                         ///< Datatype signed   32 Bit.
        typedef quint32        uint32_t;                        ///< Datatype unsigned 32 Bit.
        //typedef bool          _Bool;

        #ifndef NULL
            #define NULL ((void *)0)                            ///< Define for Nullpointer.
        #endif
    
    
    // ---------------------------------------------------------------------------------------------------
    // none
    // ---------------------------------------------------------------------------------------------------
    #else
        #error "no standard data types for this platform defined! Adapt hal_data_types.h"
    #endif

    // ---------------------------------------------------------------------------------------------------
    // Common
    // ---------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------
    // standard boolean defines
    // ---------------------------------------------------------------------------------------------------
    #ifndef TRUE
       #define TRUE                     1u                          ///< MRS defines true as 1.
    #endif

    #ifndef FALSE
       #define FALSE                    0u                          ///< MRS defiens FALSE as 0.
    #endif

    // ---------------------------------------------------------------------------------------------------
    // standard null pointer
    // ---------------------------------------------------------------------------------------------------
    #ifndef NULL_PTR
       #define NULL_PTR                 (void *) 0                  /// MRS defiens NULL as 0.
    #endif

    // ---------------------------------------------------------------------------------------------------
    // byte order defines
    // ---------------------------------------------------------------------------------------------------
    #define MSB_FIRST                   0u                          ///< Byte order definition MSB.
    #define LSB_FIRST                   1u                          ///< Byte order definition LSB.
 /** \}*/
#endif
