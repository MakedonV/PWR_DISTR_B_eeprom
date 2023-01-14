/*----------------------------------------------------------------------------*/
/**
* \file         role.h
* \brief        Implementation of ROLE base functionality.
*
* \date         20180509
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2018
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	role
* \brief 		Enumerations and interface functions used by role objects
*               This header file should be included last after all other role
*               related headers are included.
* \details
*/
/*----------------------------------------------------------------------------*/


#ifndef MRS_ROLE_H_
#define MRS_ROLE_H_

#if defined(__cplusplus)
extern "C" {
#endif

// ===================================================================================================
// Defines and typedefs
// ===================================================================================================

#define ROLE_PARAM_NONE             ((uint32_t)0u)              ///< An empty parameter



#ifndef ROLE_DO_PARAM_LIST_ANY
#define ROLE_DO_PARAM_LIST_ANY
#endif // ROLE_DO_PARAM_LIST_ANY

#ifndef ROLE_DO_PARAM_LIST_BOARD
#define ROLE_DO_PARAM_LIST_BOARD
#endif // ROLE_DO_PARAM_LIST_BOARD

#ifndef ROLE_DO_PARAM_LIST_SYS
#define ROLE_DO_PARAM_LIST_SYS
#endif // ROLE_DO_PARAM_LIST_SYS

#ifndef ROLE_DO_PARAM_LIST_TICK
#define ROLE_DO_PARAM_LIST_TICK
#endif // ROLE_DO_PARAM_LIST_TICK

#ifndef ROLE_DO_PARAM_LIST_FLASH
#define ROLE_DO_PARAM_LIST_FLASH
#endif // ROLE_DO_PARAM_LIST_FLASH

#ifndef ROLE_DO_PARAM_LIST_CAN
#define ROLE_DO_PARAM_LIST_CAN
#endif // ROLE_DO_PARAM_LIST_CAN

#ifndef ROLE_DO_PARAM_LIST_SPI
#define ROLE_DO_PARAM_LIST_SPI
#endif // ROLE_DO_PARAM_LIST_SPI

#ifndef ROLE_DO_PARAM_LIST_BL_PROTOCOL
#define ROLE_DO_PARAM_LIST_BL_PROTOCOL
#endif // ROLE_DO_PARAM_LIST_BL_PROTOCOL

#ifndef ROLE_DO_PARAM_LIST_BL_WBX
#define ROLE_DO_PARAM_LIST_BL_WBX
#endif // ROLE_DO_PARAM_LIST_BL_WBX

// ===================================================================================================
// Public function prototypes
// ===================================================================================================

/*----------------------------------------------------------------------------*/
/**
* \ingroup		role
* \brief		Universal function to perform a given operation on a
* 				given role. Accepts one additional parameter.
* \details		NEVER CALL this function directly.
* 				ALWAYS CALL role_do function instead.
*
* \pre
*
* \param id              [in] role_id_t      role identifier
* \param typeid          [in] role_do_id_t   action to perform by the role
* \param type_num        [in] uint8_t		 numeric representation of type of variadic argument passed to role_do
* \return enum_CODE                          error code
*/
enum_CODE role_dothis(role_id_t id, role_do_id_t typeid, role_do_param_t type_num, ...);

/*----------------------------------------------------------------------------*/
/**
* \ingroup		role
* \brief		Calls in a loop the respective play functions of the initialized roles.
* 				Should be called as frequently as possible.
* \details
*
* \return void
*/
void role_play(void);

// ===================================================================================================
// Defines and typedefs for role
// ===================================================================================================

/**
 * This preprocessor magic that replaces a function call with the exact same name
 * is only done to make sure application
 * authors pass exactly four arguments to these function.
 * NEVER USE role_dothis directly as it doesn't do type checking!
 * ALWAYS USE role_do instead
 */
#define role_dothis(handle, opt, type_num, param) role_dothis(handle, opt, type_num, param)

/**
 * C11 way of differentiating types at compile time.
 * Used to convert actual parameters passed to variadic argument of
 * role_do to numerical equivalents for static and dynamic type checking.
 */

#if __STDC_VERSION__ >= 201112L // >=C11
#define ROLE_PARAM_TO_NUM(param) _Generic( (param), \
        ROLE_DO_PARAM_LIST_ANY \
        ROLE_DO_PARAM_LIST_BOARD \
        ROLE_DO_PARAM_LIST_SYS \
        ROLE_DO_PARAM_LIST_TICK \
        ROLE_DO_PARAM_LIST_FLASH \
        ROLE_DO_PARAM_LIST_CAN \
		ROLE_DO_PARAM_LIST_SPI \
        ROLE_DO_PARAM_LIST_BL_PROTOCOL \
        ROLE_DO_PARAM_LIST_BL_WBX \
        )

#ifdef __GNUC__
/**
 * Universal macro to perform a given operation on a particular role.
 * ALWAYS USE role_do macro and never call role_dothis function directly.
 */

/**
 * In this form we benefit from making a compile time check of passing
 * a valid parameter type. This is based on GCC extension which allows returning
 * a value from a multi-statement macro.
 */
#define role_do(role, opt, param) __extension__ ({ \
                            _Static_assert((uint32_t)(ROLE_DO_PARAM(opt)) == (uint32_t)(ROLE_PARAM_TO_NUM(param)), \
                                            "Role action ID does not match the parameter type"); \
                            role_dothis(role, opt, ROLE_PARAM_TO_NUM(param), param); \
                                    })
#else
/**
 * For other compilers we define the macro version without
 * compile-time checking (leaving only runtime checking via ASSERT).
 * See role_dothis function.
 */
#define role_do(role, opt, param) role_dothis(role, opt, ROLE_PARAM_TO_NUM(param), param)

#endif // Compiler

#else // <C11

#error "C11 standard is needed. Try to compile with -std=c11 flag."

#endif // C11

#ifdef __cplusplus
}
#endif

#endif /* MRS_ROLE_H_ */
