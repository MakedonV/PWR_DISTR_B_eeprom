/*----------------------------------------------------------------------------*/
/**
* \file         role_board.h
* \brief        Includes board specific header file
*
* \date         20200306
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2020
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	role
* \brief        All roles should include this nominal header file rather
*               than the specific board header file.
*
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_ROLE_BOARD_ROLE_BOARD_H_
#define SRC_ROLE_BOARD_ROLE_BOARD_H_

#if defined(__cplusplus)
extern "C" {
#endif

#if 0
// Bootloader CAN traditional baudrates
typedef enum
{
    E_BAUD_1MBIT                 = 1u,
    E_BAUD_800KBIT                   ,
    E_BAUD_500KBIT                   ,
    E_BAUD_250KBIT                   ,
    E_BAUD_125KBIT                   ,
    E_BAUD_50KBIT                    ,
    E_BAUD_20KBIT                    ,
    E_BAUD_10KBIT                    ,
    E_BAUD_100KBIT                   ,
    E_BAUD_83KBIT                    ,
    E_BAUD_33KBIT                    ,
    E_BAUD_MAX
} enum_bl_baudrate_t;
#endif // 0

#if !defined(ROLE_MINIMAL)
#include "board_variants.h"
#endif // !ROLE_MINIMAL

// ===================================================================================================
// Defines and Typedefs
// ===================================================================================================
#define ROLE_DO_PARAM_LIST_BOARD

/**
 * The following enum lists all DO action ids composed of role type,
 * command index (used just as a unique value) and the parameter type.
 * Use only the following constants with role_do function.
 */
typedef enum
{
    ROLE_BOARD_CAN_TRANSCEIVER_TURN_ON  = ROLE_COMPOSE_DO_ID(ROLE_TYPE_BOARD,   0u, ROLE_ANY_PARAM_UINT32),
    ROLE_BOARD_CAN_TRANSCEIVER_TURN_OFF = ROLE_COMPOSE_DO_ID(ROLE_TYPE_BOARD,   1u, ROLE_ANY_PARAM_UINT32),

    ROLE_BOARD_EXT_WDOG_ENABLE          = ROLE_COMPOSE_DO_ID(ROLE_TYPE_BOARD,   2u, ROLE_ANY_PARAM_UINT32),
    ROLE_BOARD_EXT_WDOG_RESET           = ROLE_COMPOSE_DO_ID(ROLE_TYPE_BOARD,   3u, ROLE_ANY_PARAM_UINT32)

} enum_ROLE_BOARD_DO_ID;


// ===================================================================================================
// Public function prototypes
// ===================================================================================================

/*----------------------------------------------------------------------------*/
/**
* \ingroup      role_board
* \brief        This function is called by the role functionality internally to construct a
*               board specific role and save it into the role pool.
*               MUST NOT be called by a user.
*               This constructor needs not be necessarily implemented is used placed
*               here for possible extensibility in the future.
* \details
*
* \pre
*
* \param id [in]        role_id_t       role id to be constructed
* \return void
*/
void role_board_constructor(role_id_t id, pf_role_constructor parent) __attribute__((weak));



#ifdef __cplusplus
}
#endif

#endif /* SRC_ROLE_BOARD_ROLE_BOARD_H_ */
