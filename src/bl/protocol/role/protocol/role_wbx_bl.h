/*----------------------------------------------------------------------------*/
/**
* \file         role_wbx_bl.h
* \brief        Implementation of Wallbox bootloader role.
*
* \date         20201208
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2020
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	role_wbx_bl
* \brief 		Implements a role for Wallbox bootloader
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_ROLE_BL_WBX_H_
#define SRC_ROLE_BL_WBX_H_

#if defined(SPECIFICS_WALLBOX)

#if defined(__cplusplus)
extern "C" {
#endif

// ===================================================================================================
// Defines, typedefs
// ===================================================================================================

/**
 * The following enum lists all DO action ids composed of role type,
 * command index (used just as a unique value) and the parameter type.
 * Use only the following constants with role_do function.
 */
typedef enum
{
    ROLE_BL_WBX_CHECK_AND_JUMP_APP = ROLE_COMPOSE_DO_ID(ROLE_TYPE_WALLBOX_BOOTLOADER, 0u, ROLE_ANY_PARAM_UINT32)

} enum_ROLE_BL_WBX_DO_ID;


#define BASE_SW_VERSION_MAJOR       0
#define BASE_SW_VERSION_MINOR       0
#define BASE_SW_VERSION_BUILD       3

#define STRINGIZE2(s)               #s
#define STRINGIZE(s)                STRINGIZE2(s)

#define BASE_SW_STRING  (STRINGIZE(BASE_SW_VERSION_MAJOR) "." STRINGIZE(BASE_SW_VERSION_MINOR) "." STRINGIZE(BASE_SW_VERSION_BUILD))
#define BASE_SW_FULL    ((BASE_SW_VERSION_MAJOR << 16) + (BASE_SW_VERSION_MINOR << 8) + BASE_SW_VERSION_BUILD)

// HW version
#ifndef WALLBOX_HW_REV_NUM
#error "HW version (WALLBOX_HW_REV_NUM) not defined"
#endif

#pragma message "HW version: " STRINGIZE(WALLBOX_HW_REV_STR)
#pragma message "HW version num: " STRINGIZE(WALLBOX_HW_REV_NUM)

#define BTLDR_FLASH_RESERVE_SIZE    64

#define APP_NORM_RESET_REQUEST_FG_MAGIC_NUM (0xa5) // magic number to signal the normal requested by Application reset request.
#define MAX_ALLOWED_APP_WDT_CNT     255

typedef enum
{
    first_fw,
    second_fw,
    no_fw
} firmware_num_t;

typedef struct __attribute__ ((packed, aligned (BTLDR_FLASH_RESERVE_SIZE)))
//typedef struct __attribute__ ((packed))
{
    const uint16_t fw_num;
}boot_data_t;


#define BOOTLOADER_APP_FIRMWARE_REV_SIZE (32)
typedef const struct __attribute__ ((packed))
{
    const boot_data_t boot_data;                // at app build this part will be filled with 0xFF and used by bootloader to store RO data
    const uint32_t crc;                         // crc starts from app_size till app end (aligned?)
    const uint32_t app_size;                    // full app size with header, excluding crc
    const unsigned char app_sw_revision[BOOTLOADER_APP_FIRMWARE_REV_SIZE];    // string with app version (w/o '0' termination)
#if (!defined(ROLE_MINIMAL) && !defined(BL_UPDATE_APP))  // At app build linker adds these fields directly
    const uint32_t * const vtor_table_ptr;      // pointer to app vtor table in flash
    const uint32_t * const got_flash_ptr;       // pointer to app got table in flash
    uint32_t * const got_ram_ptr;               // got table destination pointer, in RAM. Will be copied here with offset adding at startup
    const uint32_t got_size;                    // got size
#endif  // !ROLE_MINIMAL

} btldr_app_header;

typedef struct
{
    uint32_t (*Wdt_Reset_ptr)(void);
    uint32_t (*Get_Version_ptr)(void);
    unsigned char *(*Get_CurrApp_Version_ptr)(void);
    unsigned char *(*Get_AlternateApp_Version_ptr)(void);
    uint32_t (*Get_User_EEPROM_Region_ptr)(uint32_t *start, uint32_t *length);
    uint32_t (*Get_User_DataFlash_Region_ptr)(uint32_t *startAddr, uint32_t *length, uint16_t *write_granularity, uint16_t *erase_granularity);
    uint32_t (*Get_AppImage_Region_Parameters_ptr)(uint32_t *maxSize, uint32_t *write_chunk_size, uint32_t *erase_sector_size);
    uint32_t (*Erase_AppImage_Sector_ptr)(uint32_t sector_num);
    uint32_t (*Write_AppImage_Chunk_ptr)(uint32_t chunkIndex, uint8_t *data);
    uint32_t (*Validate_AppImage_ptr)(void);
    uint32_t (*Reset_Application_ptr)(void);

    uint32_t (*Write_User_EEPROM_Region_ptr)   ( uint32_t dest_addr, void * pData, uint32_t size);
    uint32_t (*Erase_User_DataFlash_Region_ptr)( uint32_t dest_addr, uint32_t size);
    uint32_t (*Write_User_DataFlash_Region_ptr)( uint32_t dest_addr, void * pData, uint32_t size);

    uint32_t (*Get_CurrApp_Statistics_ptr)(void *pData, uint32_t dataSize);        //(since rev. 0.0.3)
    uint32_t (*Get_AlternateApp_Statistics_ptr)(void *pData, uint32_t dataSize);   //(since rev. 0.0.3)

} btldr_functions_t;

/**
* \ingroup      bootloader_storage
* Resets count for every FW
**/
typedef struct __attribute__((packed))
{
    uint8_t padding[2];      ///< reserved
    uint8_t wdt_reset_count; ///< Count of watchdog resets
    uint8_t sw_unexpected_reset_count;  ///< Count of software unexpected resets
} firmware_configs_t;

/**
* \ingroup      bootloader_storage
* All data, stored by bootloader at EEPROM
**/
typedef struct __attribute__((packed))
{
    firmware_configs_t fw[2];    ///< Structs with data related to each firmware
    uint8_t last_loaded_fw;      ///< Which FW was started at last time
    uint8_t init_key;            ///< Storage for magic number. Check if eeprom is not empty (at first boot)
    uint8_t reserved0 [6];          ///< Reserved for future use
} eeprom_config_t;

typedef struct __attribute__ ((packed))
{
    uint32_t wdtRstCnt;                // number of Watchdog resets for the Application
    uint32_t swUnexpRstCnt;            // number of Unexpected SW resets for the Application
} btldr_app_statistics_t;


// ===================================================================================================
// Public function prototypes
// ===================================================================================================

/*----------------------------------------------------------------------------*/
/**
* \ingroup		role_wbx_bl
* \brief		This function is called by the role functionality internally to construct a
* 				Wallbox bootloader role and save it into the role pool.
* 				MUST NOT be called by a user.
* \details
*
* \pre
*
* \param id [in]		role_id_t		role id to be constructed
* \return void
*/
void role_wbx_bl_constructor(role_id_t id, pf_role_constructor parent);

#ifdef __cplusplus
}
#endif

#endif // SPECIFICS_WALLBOX

#endif /* SRC_ROLE_BL_WBX_H_ */
