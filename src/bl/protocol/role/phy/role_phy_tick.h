/*----------------------------------------------------------------------------*/
/**
* \file         role_phy_tick.h
* \brief        Implementation of Timer low-level role.
*
* \date         20180509
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2018
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	role_phy_tick
* \brief 		Based on DWT unit manages interrupt-free delays of various precision
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_ROLE_PHY_TICK_H_
#define SRC_ROLE_PHY_TICK_H_

#if defined(__cplusplus)
extern "C" {
#endif

// ===================================================================================================
// Defines and Typedefs
// ===================================================================================================

/**
 * The following macro is used for compile-time type checking of the
 * parameter passed to role_do function-like macro.
 * See role.h for more information.
 */
#define ROLE_DO_PARAM_LIST_TICK \
    , ROLE_MAP_REAL_PARAM_TO_NUM(struct_ROLE_TICK_DELAY*,   ROLE_TICK_PARAM_DELAY) \
    , ROLE_MAP_REAL_PARAM_TO_NUM(struct_ROLE_TICK_STAMP*,   ROLE_TICK_PARAM_STAMP) \
	, ROLE_MAP_REAL_PARAM_TO_NUM(struct_ROLE_TICK_TWO_DELAYS*, ROLE_TICK_PARAM_TWO_DELAYS)


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
    ROLE_TICK_PARAM_DELAY = ROLE_COMPOSE_DO_PARAM_BASE(ROLE_TYPE_TICK),
    ROLE_TICK_PARAM_STAMP,
    ROLE_TICK_PARAM_TWO_DELAYS,

} enum_ROLE_TIMER_DO_PARAM;

/**
 * The following enum lists all DO action ids composed of role type,
 * command index (used just as a unique value) and the parameter type.
 * Use only the following constants with role_do function.
 */
typedef enum
{
    ROLE_TIMER_START_DELAY = ROLE_COMPOSE_DO_ID(ROLE_TYPE_TICK,             0u, ROLE_TICK_PARAM_DELAY),
    ROLE_TIMER_CHECK_TIMEOUT = ROLE_COMPOSE_DO_ID(ROLE_TYPE_TICK,           1u, ROLE_TICK_PARAM_DELAY),
    ROLE_TIMER_RECORD_TIMESTAMP = ROLE_COMPOSE_DO_ID(ROLE_TYPE_TICK,        2u, ROLE_TICK_PARAM_STAMP),
    ROLE_TIMER_CHECK_ELAPSED_PERIOD = ROLE_COMPOSE_DO_ID(ROLE_TYPE_TICK,    3u, ROLE_TICK_PARAM_STAMP),
    ROLE_TIMER_ELAPSED_MS_FROM_START = ROLE_COMPOSE_DO_ID(ROLE_TYPE_TICK,   4u, ROLE_ANY_PARAM_UINT32PTR),
    ROLE_TIMER_UPDATE_DELAY_IF_NEW_WILL_LAST_LONGER = ROLE_COMPOSE_DO_ID(ROLE_TYPE_TICK,   5u, ROLE_TICK_PARAM_TWO_DELAYS)
} enum_ROLE_TIMER_DO_ID;


typedef enum
{
    TICK_PRECISION_1US               = 0u,   ///< precision is 1us
    TICK_PRECISION_10US                  ,   ///< precision is 10us
    TICK_PRECISION_100US                 ,   ///< precision is 100us
    TICK_PRECISION_1MS                   ,   ///< precision is 1ms
    TICK_PRECISION_10MS                  ,   ///< precision is 10ms
    TICK_PRECISION_100MS                 ,   ///< precision is 100ms
    TICK_PRECISION_1S                    ,   ///< precision is 1s

    TICK_PRECISION_DUMMY_END
} enum_ROLE_TICK_PRECISION;

typedef struct
{
    uint32_t us_lo;
    uint32_t us_hi;
} struct_ROLE_PHY_TIMESTAMP;

typedef struct
{
        uint16_t delay;
        enum_ROLE_TICK_PRECISION precision;
        bool b_elapsed;
        struct_ROLE_PHY_TIMESTAMP endtime;
} struct_ROLE_TICK_DELAY;

typedef struct
{
        uint16_t period;
        enum_ROLE_TICK_PRECISION precision;
        bool b_elapsed;
        struct_ROLE_PHY_TIMESTAMP timestamp;
} struct_ROLE_TICK_STAMP;

typedef struct
{
	struct_ROLE_TICK_DELAY *running_delay;
	struct_ROLE_TICK_DELAY *new_delay;
} struct_ROLE_TICK_TWO_DELAYS;

// ===================================================================================================
// Public function prototypes
// ===================================================================================================

/*----------------------------------------------------------------------------*/
/**
* \ingroup		role_phy_tick
* \brief		This function is called by the role functionality internally to construct a
* 				Timer role and save it into the role pool.
* 				MUST NOT be called by a user.
* \details
*
* \pre
*
* \param id [in]		role_id_t		role id to be constructed
* \return void
*/
void role_phy_tick_constructor(role_id_t id, pf_role_constructor parent);


#ifdef __cplusplus
}
#endif

#endif /* SRC_ROLE_PHY_TICK_H_ */
