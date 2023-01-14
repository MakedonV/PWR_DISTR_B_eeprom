/*----------------------------------------------------------------------------*/
/**
* \file         role_types.h
* \brief        Contains important macros and enumerations used by roles
*
* \date         20180509
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2018
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	role_types
* \brief        Contains important macros and enumerations
*               This header file should be included first before any other
*               role header file is included.
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_ROLE_TYPES_H_
#define SRC_ROLE_TYPES_H_

#if defined(__cplusplus)
extern "C" {
#endif

// ===================================================================================================
// Defines and Typedefs
// ===================================================================================================
typedef struct struct_ROLE struct_ROLE;

typedef uint16_t     role_id_t;
typedef uint16_t     role_do_param_t;
typedef uint32_t     role_do_id_t;

#define ROLE_ID_TYPE(id)            ((uint8_t)( id >> 8u ))     ///< Type part of a role id
#define ROLE_ID_IND(id)             ((uint8_t)( id ))           ///< Index part of a role id

#define ROLE_DO_TYPE(id)            ((uint8_t)( id >> 24u ))     ///< Role type part of a role do id
#define ROLE_DO_ID(id)              ((uint16_t)( id >> 16u ))    ///< Unique id part of a role do id (includes role type)
#define ROLE_DO_PARAM(id)           ((role_do_param_t)( id ))    ///< Parameter part of a role do id (includes role type where parameter is defined)

/**
 *  Composes role id from given type and index
 */
#define ROLE_COMPOSE_ID(type, ind) \
  ( (((role_id_t)type) << 8u) | ((uint8_t)ind) )

/**
 *  Composes a numerical value for start of a parameter list
 */
#define ROLE_COMPOSE_DO_PARAM_BASE(type) \
  (((role_do_param_t)(type & (uint8_t)(-1))) << 8u)

/**
 *  Composes role do id from given parameter type (which includes role type) and index
 */
#define ROLE_COMPOSE_DO_ID(type, ind, param_id) \
  ( (((role_do_id_t)(type & (uint8_t)(-1))) << 24u) \
  | (((role_do_id_t)(ind & (uint8_t)(-1))) << 16u) \
  | ((role_do_id_t)(param_id & (role_do_param_t)(-1))) )

/**
 *  Maps a parameter type to a numerical constant used in _Generic. See role.h
 */
#define ROLE_MAP_REAL_PARAM_TO_NUM(param, num) \
  param : num


/**
 * Enumeration of existing roles
 */
typedef enum
{
    ROLE_TYPE_ANY,

    ROLE_TYPE_BOARD,

    ROLE_TYPE_PHY,

    ROLE_TYPE_SYS = ROLE_TYPE_PHY,
    ROLE_TYPE_TICK,
    ROLE_TYPE_FLASH,
    ROLE_TYPE_CAN,
    ROLE_TYPE_SPI,

    ROLE_TYPE_PROTOCOL,
    ROLE_TYPE_PROTOCOL_BOOTLOADER = ROLE_TYPE_PROTOCOL,
    ROLE_TYPE_PROTOCOL_COMMON,
    ROLE_TYPE_WALLBOX_BOOTLOADER

} enum_ROLE_TYPE;

/**
 * ROLE ids. The id is composed of two fields the type field and the index field.
 * Although the type field changes from one role to another the index should be
 * incremented by 1 as it is also used to statically allocate array of roles
 * and in a constant time find the pointer to a role when referenced by id.
 * Otherwise, the set of role ids for each role type could be defined in their
 * own *.h files and finding of a particular role would be in O(log(N)) time.
 */
typedef enum
{
    ROLE_ID_BOARD = ROLE_COMPOSE_ID(ROLE_TYPE_BOARD, 0u),

    ROLE_ID_PHY_SYS = ROLE_COMPOSE_ID(ROLE_TYPE_SYS, 1u),
    ROLE_ID_PHY_TICK = ROLE_COMPOSE_ID(ROLE_TYPE_TICK, 2u),
    ROLE_ID_PHY_FLASH = ROLE_COMPOSE_ID(ROLE_TYPE_FLASH, 3u),

    ROLE_ID_PHY_CAN0 = ROLE_COMPOSE_ID(ROLE_TYPE_CAN, 4u),
    ROLE_ID_PHY_CAN1 = ROLE_COMPOSE_ID(ROLE_TYPE_CAN, 5u),
    ROLE_ID_PHY_CAN2 = ROLE_COMPOSE_ID(ROLE_TYPE_CAN, 6u),

    ROLE_ID_PHY_SPI = ROLE_COMPOSE_ID(ROLE_TYPE_CAN, 7u),

	ROLE_ID_PROTOCOL_BOOTLOADER = ROLE_COMPOSE_ID(ROLE_TYPE_PROTOCOL_BOOTLOADER, 8u),
    ROLE_ID_PROTOCOL_COMMON = ROLE_COMPOSE_ID(ROLE_TYPE_PROTOCOL_COMMON, 9u),
    ROLE_ID_WALLBOX_BOOTLOADER = ROLE_COMPOSE_ID(ROLE_TYPE_WALLBOX_BOOTLOADER, 10u),

    ROLE_ID_DUMMY_END
} enum_ROLE_ID;

/**
 *  Represents numerical equivalents of parameter types
 *  which are passed to role_do function-like macro to
 *  perform particular action on a role. The following
 *  constants participate in definition of role do ids
 *  as well as in corresponding ROLE_DO_PARAM_LIST* macros
 *  of each role type.
 */
typedef enum
{
    ROLE_ANY_PARAM_UINT32 = ROLE_COMPOSE_DO_PARAM_BASE(ROLE_TYPE_ANY),
    ROLE_ANY_PARAM_UINT32PTR
} enum_ROLE_ANY_DO_PARAM;

/**
 * The following enum lists all DO action ids composed of role type,
 * command index (used just as a unique value) and the parameter type.
 * The parameter type by itself also includes the role type where the
 * parameter is defined to keep it also unique which is needed
 * for both compile-time and runtime checking. Each of the following
 * DO action ids contains also the parameter numerical type which makes
 * the parameter type validity checking convenient.
 * Use only the following constants with role_do function.
 */
typedef enum
{
    ROLE_INIT = ROLE_COMPOSE_DO_ID(ROLE_TYPE_ANY, 0u, ROLE_ANY_PARAM_UINT32),
    ROLE_START = ROLE_COMPOSE_DO_ID(ROLE_TYPE_ANY, 1u, ROLE_ANY_PARAM_UINT32),
    ROLE_STOP = ROLE_COMPOSE_DO_ID(ROLE_TYPE_ANY, 2u, ROLE_ANY_PARAM_UINT32)
} enum_ROLE_ANY_DO_ID;

/**
 * The following list is assumed to be the first in the
 * list of the _Generic qualifier as it is not
 * starting with symbol ','.
 * See role_do function-like macro and ROLE_PARAM_TO_NUM macro
 * for more information.
 */
#define ROLE_DO_PARAM_LIST_ANY \
          ROLE_MAP_REAL_PARAM_TO_NUM(uint32_t, ROLE_ANY_PARAM_UINT32) \
        , ROLE_MAP_REAL_PARAM_TO_NUM(uint32_t*, ROLE_ANY_PARAM_UINT32PTR)


typedef void (*pf_role_constructor)(struct_ROLE *ptr_role);     ///< Parent constructor prototype, ptr_role is a pointer to a derived role

/**
 * Enumeration of states a role can have
 */
typedef enum
{
    ROLE_STATE_DORMANT,                 ///< All the functionality of a role is stopped
    ROLE_STATE_ACTIVE                   ///< Normal operation state
} enum_ROLE_STATE;

#ifdef __cplusplus
}
#endif

#endif /* SRC_ROLE_TYPES_H_ */
