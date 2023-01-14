/*----------------------------------------------------------------------------*/
/**
* \file         defines_general.h
* \brief        Defines handful macros
*
* \date         20180509
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2018
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	bl_defines
* \brief        Contains general purpose macros and constants
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef MRS_SRC_INTERNALS_DEFINES_H_
#define MRS_SRC_INTERNALS_DEFINES_H_

#if defined(__cplusplus)
extern "C" {
#endif


// ===================================================================================================
// Types
// ===================================================================================================
typedef signed int   int_t;
typedef unsigned int uint_t;

typedef uint8_t      len_t;

// ===================================================================================================
// Defines for return codes
// ===================================================================================================

typedef enum
{
    CODE_OK                   = 0,
    CODE_FAIL                    ,

    CODE_S19_CRC_FAIL,                         ///< S19 CRC Error
    CODE_APP_CRC_FAIL,                         ///< Application CRC Error
    CODE_RANGE_FAIL,                           ///< Address Range Error
    CODE_S19_FORMAT_FAIL,                      ///< S19 Format Error (unsupported S19 Line Header S1/S2)
    CODE_INCOMPATIBLE_FLASH_HEADER,            ///< incompatible App Flash Header
    CODE_WRONG_SIGNATURE,                      ///< wrong application signature

    CODE_INVALID_PARAM           ,
    CODE_INVALID_ROLE            ,
    CODE_UNKNOWN_FLAG            ,
    CODE_TYPE_MISMATCH           ,
    CODE_POOL_EMPTY              ,
    CODE_POOL_FULL               ,
    CODE_TOO_MANY_TOP_ROLES      ,
    CODE_ROLE_TYPE_MISMATCH      ,
    CODE_ROLE_INIT_FAIL          ,
    CODE_DEPEND_ROLE_FAIL        ,
    CODE_TYPE_CHECKING_OMITTED   ,
    CODE_ROLE_ACCESS_DENIED      ,
    CODE_ROLE_ALREADY_CONSTRUCTED,
    CODE_ROLE_CMD_FAILED         ,
    CODE_ROLE_OUT_OF_RANGE       ,
    CODE_ROLE_INACTIVE           ,

    CODE_MEMORY_INVALID
} enum_CODE;


// ===================================================================================================
// General useful defines
// ===================================================================================================
#ifndef NULL
    #define NULL    ((void*)0)      ///< Defining a NULL if not yet defined
#endif // NULL

#ifndef STATIC_ARR_ELEMENTS_COUNT
    #define STATIC_ARR_ELEMENTS_COUNT(x)    ( sizeof(x) / sizeof(x[0]) ) ///< Calculates number of elements in a static array.
#endif // STATIC_ARR_ELEMENTS_COUNT

#define SIZEOF_MEMBER(type, member)         ((uint16_t) sizeof(((type *)0)->member))

#ifndef TRUE
    #define TRUE    (1==1)
#endif // TRUE

#ifndef FALSE
    #define FALSE   (!TRUE)
#endif // FALSE

#ifndef MAX
#define MAX(a,b)    ((a) > (b) ? (a) : (b))
#endif // MAX

#ifndef MIN
#define MIN(a,b)    ((a) < (b) ? (a) : (b))
#endif // MIN

#define ABS_DIFF(a,b) ( MAX(a,b) - MIN(a,b) )

#define DO_ONCE()                \
    {                            \
        static bool once = TRUE; \
        if (once)                \
        {                        \
            once = FALSE;

#define END_ONCE()               \
        }                        \
    }

#define NOT_ONCE()               \
        once = TRUE;

// ===================================================================================================
// Repeating macros
// ===================================================================================================
#define FLASH_EMPTY_CELL 0xFFu
#define FLASH_RPT2 FLASH_EMPTY_CELL, FLASH_EMPTY_CELL
#define FLASH_RPT3 FLASH_EMPTY_CELL, FLASH_RPT2
#define FLASH_RPT4 FLASH_RPT2, FLASH_RPT2
#define FLASH_RPT8 FLASH_RPT4, FLASH_RPT4
#define FLASH_RPT5 FLASH_RPT3, FLASH_RPT2
#define FLASH_RPT10 FLASH_RPT5, FLASH_RPT5
#define FLASH_RPT12 FLASH_RPT2, FLASH_RPT10
#define FLASH_RPT14 FLASH_RPT4, FLASH_RPT10
#define FLASH_RPT20 FLASH_RPT10, FLASH_RPT10
#define FLASH_RPT30 FLASH_RPT10, FLASH_RPT20
#define FLASH_RPT40 FLASH_RPT10, FLASH_RPT30
#define REPEAT_FLASH_EMPTY(n) FLASH_RPT ## n

// ===================================================================================================
// Assertion related defines
// ===================================================================================================

#define STRINGIFY(x)    #x
#define TOSTRING(x)     STRINGIFY(x)

#if __STDC_VERSION__ >= 201112L // >=C11
    #define ASSERT_STATIC(expr, str) _Static_assert(expr, str)
#else
    #define ASSERT_STATIC(expr, str) \
        extern char assertion[(expr) ? 1 : -1] __attribute__((unused))
#endif // Cxx version

#define SUPPRESS_UNUSED_WARNING(f) (void)(f)

#define FILENUM(num) \
    enum { FILE_N = num }; \
    void _dummy##num(void) {}

// Two assert variants. The failure1 variant is more size efficient
#if 1
#define ASSERT(expr)        if (!(expr)) failure1(FILE_N, __LINE__)

#else
#define AT                  __FILE__ ":" TOSTRING(__LINE__)
#define ASSERT(expr)        if (!(expr)) failure2("Assertion failure at " AT)

#endif // 1/0

#define ASSERT_FORCE        failure3(FILE_N, __LINE__)

#ifdef NDEBUG
#define ASSERT_DEBUG(expr, code) ((void)0)
#define SOFTWARE_BREAKPOINT() ((void)0)

#else // !NDEBUG
#define ASSERT_DEBUG(expr) ASSERT(expr)
#define SOFTWARE_BREAKPOINT() __asm("BKPT #0\n\t")
#endif // NDEBUG

#if __STDC_VERSION__ >= 201112L // >=C11
#define LIMIT_MAX(var) ((uint32_t)_Generic( (var), \
                                    uint8_t     : 0xFFu, \
                                    uint16_t    : 0xFFFFu, \
                                    uint32_t    : 0xFFFFFFFFu, \
                                    int8_t      : 0x7F, \
                                    int16_t     : 0x7FFFF, \
                                    int32_t     : 0x7FFFFFFF))

#define ASSERT_OVERFLOW_STATIC(var, max_val) ASSERT_STATIC( max_val <= LIMIT_MAX(var), "Overflow error" )

#else // <C11
#define LIMIT_MAX(var) ((void)0)
#define ASSERT_OVERFLOW_STATIC(var, max_val) ((void)0)
#endif // C11

// ===================================================================================================
// Function prototypes
// ===================================================================================================
void failure1(int file_n, int line_n);
void failure2(const char* ptr_info);
uint32_t failure3(int file_n, int line_n);

#ifdef __cplusplus
}
#endif

#endif /* MRS_SRC_INTERNALS_DEFINES_H_ */
