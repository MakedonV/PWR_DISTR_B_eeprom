/*----------------------------------------------------------------------------*/
/**
* \file         role_phy_flash.h
* \brief        Implementation of FLASH/EEPROM low-level role.
*
* \date         20180509
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2018
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	role_phy_flash
* \brief 		Handles read/write into FLASH and EEPROM
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_ROLE_PHY_FLASH_H_
#define SRC_ROLE_PHY_FLASH_H_

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
#define ROLE_DO_PARAM_LIST_FLASH \
        , ROLE_MAP_REAL_PARAM_TO_NUM(struct_ROLE_EEPROM_RDD*, ROLE_FLASH_PARAM_EEPROM_RDD) \
        , ROLE_MAP_REAL_PARAM_TO_NUM(struct_ROLE_EEPROM_WRD*, ROLE_FLASH_PARAM_EEPROM_WRD) \
        , ROLE_MAP_REAL_PARAM_TO_NUM(struct_ROLE_FLASH_ERASE*, ROLE_FLASH_PARAM_FLASH_ERASE) \
        , ROLE_MAP_REAL_PARAM_TO_NUM(struct_ROLE_FLASH_WR_VFD*, ROLE_FLASH_PARAM_FLASH_WR_VFD) \
        , ROLE_MAP_REAL_PARAM_TO_NUM(struct_ROLE_PROGRAM_ONCE_BLOCK*, ROLE_FLASH_PARAM_PROGRAM_ONCE_BLOCK)



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
    ROLE_FLASH_PARAM_EEPROM_RDD = ROLE_COMPOSE_DO_PARAM_BASE(ROLE_TYPE_FLASH),
    ROLE_FLASH_PARAM_EEPROM_WRD,
    ROLE_FLASH_PARAM_FLASH_ERASE,
    ROLE_FLASH_PARAM_FLASH_WR_VFD,
    ROLE_FLASH_PARAM_PROGRAM_ONCE_BLOCK

} enum_ROLE_FLASH_DO_PARAM;

/**
 * The following enum lists all DO action ids composed of role type,
 * command index (used just as a unique value) and the parameter type.
 * Use only the following constants with role_do function.
 */
typedef enum
{
    ROLE_FLASH_EEPROM_READ = ROLE_COMPOSE_DO_ID(ROLE_TYPE_FLASH,    0u, ROLE_FLASH_PARAM_EEPROM_RDD),
    ROLE_FLASH_EEPROM_WRITE = ROLE_COMPOSE_DO_ID(ROLE_TYPE_FLASH,   1u, ROLE_FLASH_PARAM_EEPROM_WRD),
    ROLE_FLASH_ERASE = ROLE_COMPOSE_DO_ID(ROLE_TYPE_FLASH,          2u, ROLE_FLASH_PARAM_FLASH_ERASE),
    ROLE_FLASH_WRITE = ROLE_COMPOSE_DO_ID(ROLE_TYPE_FLASH,          3u, ROLE_FLASH_PARAM_FLASH_WR_VFD),
    ROLE_FLASH_VERIFY = ROLE_COMPOSE_DO_ID(ROLE_TYPE_FLASH,         4u, ROLE_FLASH_PARAM_FLASH_WR_VFD),
    ROLE_FLASH_EEPROM_REFORMAT = ROLE_COMPOSE_DO_ID(ROLE_TYPE_FLASH,         5u, ROLE_ANY_PARAM_UINT32),
#if 0
	ROLE_FLASH_PROGRAM_ONCE_READ = ROLE_COMPOSE_DO_ID(ROLE_TYPE_FLASH,    6u, ROLE_FLASH_PARAM_PROGRAM_ONCE_BLOCK),
    ROLE_FLASH_PROGRAM_ONCE_WRITE = ROLE_COMPOSE_DO_ID(ROLE_TYPE_FLASH,   7u, ROLE_FLASH_PARAM_PROGRAM_ONCE_BLOCK)
#endif // 0

} enum_ROLE_FLASH_DO_ID;

#define USE_INTERNAL_EEPROM (1)                         ///< Instructs whether internal EEPROM should be used
#define USE_EXTERNAL_EEPROM (0)                         ///< Instructs whether external EEPROM should be used

#define FLASH_TARGET                                    ///< When defined indicates that the code is executed from FLASH, otherwise it is executed from RAM

typedef struct
{
  uint32_t address;
  uint8_t *ptr_to_val;
} struct_ROLE_EEPROM_RDD;

typedef struct
{
  uint32_t address;
  uint16_t size;
  uint8_t const *ptr_val;
} struct_ROLE_EEPROM_WRD;

typedef struct
{
  uint32_t address;
  uint32_t size;
} struct_ROLE_FLASH_ERASE;

typedef struct
{
  uint32_t address;
  uint16_t size;
  uint8_t const *ptr_val;
} struct_ROLE_FLASH_WR_VFD;

typedef struct
{
  uint8_t index;
  uint8_t *ptr_to_val;
} struct_ROLE_PROGRAM_ONCE_BLOCK;

// ===================================================================================================
// Public function prototypes
// ===================================================================================================

/*----------------------------------------------------------------------------*/
/**
* \ingroup		role_phy_flash
* \brief		This function is called by the role functionality internally to construct a
* 				FLASH role and save it into the role pool.
* 				MUST NOT be called by a user.
* \details
*
* \pre
*
* \param id [in]		role_id_t		role id to be constructed
* \return void
*/
void role_phy_flash_constructor(role_id_t id, pf_role_constructor parent);


#ifdef __cplusplus
}
#endif

#endif /* SRC_ROLE_PHY_FLASH_H_ */
