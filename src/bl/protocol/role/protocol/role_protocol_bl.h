/*----------------------------------------------------------------------------*/
/**
* \file         role_protocol_bl.h
* \brief        Implementation of bootloader protocol role according to MRS
* 				communication protocol standard.
*
* \date         20180509
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2018
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	role_protocol_bl
* \brief 		Implements a role for bootloader communication protocol according
* 				to MRS bootloader protocol specification
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_ROLE_BL_PROTOCOL_H_
#define SRC_ROLE_BL_PROTOCOL_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "ee_helper.h"

// ===================================================================================================
// Defines, typedefs
// ===================================================================================================
typedef void (*bl_protocol_prereset_callback_t)(void);

/**
 * The following macro is used for compile-time type checking of the
 * parameter passed to role_do function-like macro.
 * See role.h for more information.
 */
#define ROLE_DO_PARAM_LIST_BL_PROTOCOL \
        , ROLE_MAP_REAL_PARAM_TO_NUM(struct_EE_CAN_EXT*, ROLE_BL_PROTOCOL_PARAM_CAN_EXT_CALC_STORE) \
        , ROLE_MAP_REAL_PARAM_TO_NUM(const struct_hal_sys_app_header_t**, ROLE_BL_PROTOCOL_PARAM_APP_HEADER_PTR) \
        , ROLE_MAP_REAL_PARAM_TO_NUM(bl_protocol_prereset_callback_t, ROLE_BL_PROTOCOL_PRE_RESET_CALLBACK)


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
    ROLE_BL_PROTOCOL_PARAM_CAN_EXT_CALC_STORE = ROLE_COMPOSE_DO_PARAM_BASE(ROLE_TYPE_PROTOCOL_BOOTLOADER),
    ROLE_BL_PROTOCOL_PARAM_APP_HEADER_PTR,
    ROLE_BL_PROTOCOL_PRE_RESET_CALLBACK

} enum_ROLE_BL_PROTOCOL_DO_PARAM;

/**
 * The following enum lists all DO action ids composed of role type,
 * command index (used just as a unique value) and the parameter type.
 * Use only the following constants with role_do function.
 */
typedef enum
{
    ROLE_BL_PROTOCOL_CAN_EXT_UPDATE = ROLE_COMPOSE_DO_ID(ROLE_TYPE_PROTOCOL_BOOTLOADER, 0u, ROLE_BL_PROTOCOL_PARAM_CAN_EXT_CALC_STORE),

    ROLE_BL_PROTOCOL_GET_APP_HEADER_PTR = ROLE_COMPOSE_DO_ID(ROLE_TYPE_PROTOCOL_BOOTLOADER, 1u, ROLE_BL_PROTOCOL_PARAM_APP_HEADER_PTR),
    ROLE_BL_PROTOCOL_GET_BL_ACCESS_KEY = ROLE_COMPOSE_DO_ID(ROLE_TYPE_PROTOCOL_BOOTLOADER, 2u, ROLE_ANY_PARAM_UINT32PTR),
    ROLE_BL_PROTOCOL_SET_BL_ACCESS_KEY = ROLE_COMPOSE_DO_ID(ROLE_TYPE_PROTOCOL_BOOTLOADER, 3u, ROLE_ANY_PARAM_UINT32),

    ROLE_BL_PROTOCOL_SET_RESET_CALLBACK = ROLE_COMPOSE_DO_ID(ROLE_TYPE_PROTOCOL_BOOTLOADER, 4u, ROLE_BL_PROTOCOL_PRE_RESET_CALLBACK),

    ROLE_BL_PROTOCOL_GET_APP_STARTUP_DELAY = ROLE_COMPOSE_DO_ID(ROLE_TYPE_PROTOCOL_BOOTLOADER, 5u, ROLE_ANY_PARAM_UINT32PTR),
    ROLE_BL_PROTOCOL_SET_APP_STARTUP_DELAY = ROLE_COMPOSE_DO_ID(ROLE_TYPE_PROTOCOL_BOOTLOADER, 6u, ROLE_ANY_PARAM_UINT32PTR)

} enum_ROLE_BL_PROTOCOL_DO_ID;


// Macro for inverting bytes
#define CHECK_BAUDRATE(x) (((x) & 0xFFu) >= E_BAUD_1MBIT && ((x) & 0xFFu) < E_BAUD_MAX)

#define CALIBR_EE_TABLE_START_ADDR	(0x220u)
#define CALIBR_EE_TABLE_MODULE_ADDR	CALIBR_EE_TABLE_START_ADDR
#define CALIBR_EE_TABLE_DATA_ADDR	(CALIBR_EE_TABLE_MODULE_ADDR + 2u)
#define CALIBR_EE_TABLE_END_ADDR    (0x342u)

#define SIGNATURE_LENGTH            256u ///< Set the used signature length 256 Byte = 2048Bit RSA Key, 512 Byte = 4096Bit RSA Key

#define RESET_MAX_CNT               (50u)

/**
 *  Do not enable the hardware CRC calculation (CRC_CALC_HARD) as it produces a wrong CRC
 *  This is because S32K144 seems not to support reverse polynomials
 *  This is yet an assumption and would need further checking.
 */
//#define CRC_CALC_HARD

#define HAL_SYS_APP_HEADER_VERSION              (1u)            ///< Application Header Version

#define HAL_SYS_APP_HEADER_KEY                  (0x12345678u)   ///< Application Header Key

#define HAL_SYS_BOOTLOADER_ACCESS_KEY           (0x4D525321u)   ///< Bootloader Access Key
#define HAL_SYS_BOOTLOADER_APP_START_KEY        (0x07412807u)   ///< Application Start Key
#define HAL_SYS_BOOTLOADER_ACCESS_KEY_UPDATE    (0x1AF7356B)    ///< Applicatio firmare update key

#define REQUIRED_APP_HEADER_VERSION             (1u)            ///< Required Version of the Flash Header

//#define BOOTLOADER_FASTSTART_ACTIVE
//#define BOOTLOADER_ENABLE_SIGNATURE_CHECK                       ///< enable this define to enable the RSA signature check

#define BL_STARTUP_TIME_MIN                     (10u)           ///< 10ms minimal startup delay
#define BL_STARTUP_TIME_MAX                     (1000u)         ///< 1000ms maximal startup delay
//#define BL_STARTUP_TIME_DEFAULT (150u)  ///<  150ms default startup delay for the IUG Bootloader (to short for a Scan / Address Cycle but "Start Download" is working if the Startup message is available!)
#define BL_STARTUP_TIME_DEFAULT                 (1000u)         ///<  1000ms default startup delay for the S32K Bootloader

#define FLASH_BLOCK_OFFSET                      (0u)            ///< start at 0x10000

#define FLASH_APPL_HEADER_OFFSET                (0x8u)          ///< Offset to skip header key and header CRC
#define FLASH_APPL_HEADER_LENGTH                (0x1000u)      ///< Total header size which comes before the application code

extern uint32_t __APP_HEADER_ADDR;

// "Application Image" address defines
#define FLASH_APPL_START_ALL                    ((uint32_t)((uint8_t*)&__APP_HEADER_ADDR))  ///< Start address of application header

#if defined(CPU_S32K148)
#define FLASH_APPL_END_ALL                      (0x17FFFFu)     ///< End address of application code, all available Flash space is used
#define FLASH_SECTOR_SIZE                       (4096u)         ///< Sector size
#define FLASH_BLOCK_AVAILABLE                   (uint8_t)(0xFFu)    ///< X x 4k Blocks (384u - 16u) exceeds uint8_t, therefore set to 0xFF

#elif defined(CPU_S32K146)
#define FLASH_APPL_END_ALL                      (0xFFFFFu)      ///< End address of application code, all available Flash space is used
#define FLASH_SECTOR_SIZE                       (4096u)         ///< Sector size
#define FLASH_BLOCK_AVAILABLE                   (uint8_t)(256u - 16u)    ///< X x 4k Blocks

#elif (defined(CPU_S32K144HFT0VLLT) || defined(CPU_S32K144LFT0MLLT))
#define FLASH_APPL_END_ALL                      (0x7FFFFu)      ///< End address of application code, all available Flash space is used
#define FLASH_SECTOR_SIZE                       (4096u)         ///< Sector size
#define FLASH_BLOCK_AVAILABLE                   (uint8_t)(128u - 16u)    ///< X x 4k Blocks

#elif defined(CPU_S32K142)
#define FLASH_APPL_END_ALL                      (0x3FFFFu)      ///< End address of application code, all available Flash space is used
#define FLASH_SECTOR_SIZE                       (2048u)         ///< Sector size
#define FLASH_BLOCK_AVAILABLE                   (uint8_t)(128u - 32u)    ///< X x 2k Blocks

#elif defined(CPU_MK10DN512VLL10)
//#define FLASH_APPL_END_ALL                      (0x7FFFFu)      ///< End address of application code, all available Flash space is used
#define FLASH_APPL_END_ALL                      (0x78000u)      ///< End address of application code, the last 32 KB is for EEPROM emulation
#define FLASH_SECTOR_SIZE                       (2048u)         ///< Sector size
#define FLASH_BLOCK_AVAILABLE                   (uint8_t)(256u - (32u + 16u))    ///< X x 2k Blocks

#else
#error "MCU type unknown. Specify the MCU type via SYSTEM_CPU in the makefile."
#endif

#if !defined(BL_UPDATE_APP)
#define FLASH_APPL_START                        ((uint32_t)((uint8_t*)&__APP_HEADER_ADDR) + FLASH_APPL_HEADER_LENGTH) ///< Application start address (after header)
#define FLASH_APPL_SIZE                         ((uint32_t)(FLASH_APPL_END_ALL - FLASH_APPL_START))
#define FLASH_APPL_HEADER_START                 FLASH_APPL_START_ALL

#define FLASH_UPDATE_APPL_START_ALL             (uint32_t)( ((FLASH_APPL_END_ALL + 1u) + FLASH_APPL_START_ALL) / 2)      ///< Start address of new application code stored in update region
#define FLASH_UPDATE_APPL_END_ALL               FLASH_APPL_END_ALL

#define FLASH_UPDATE_APPL_HEADER_START          FLASH_UPDATE_APPL_START_ALL                 ///< Start address of new application code stored in update region
#define FLASH_UPDATE_APPL_START                 (FLASH_UPDATE_APPL_START_ALL + FLASH_APPL_HEADER_LENGTH)    ///< Update application start address

#else
#define FLASH_APPL_START                        ((uint32_t)0u) ///< Bootloader start address (after header)
#define FLASH_APPL_SIZE                         ((uint32_t)0x10000u)
#define FLASH_APPL_HEADER_START                 FLASH_APPL_START_ALL

// The rest of the following defines are invalid for BL updated
// They are left here to avoid compiler errors
// Currently the bootloader does not use them as well
#define FLASH_UPDATE_APPL_START_ALL             (uint32_t)( ((FLASH_APPL_END_ALL + 1u) + FLASH_APPL_START_ALL) / 2)      ///< Start address of new application code stored in update region
#define FLASH_UPDATE_APPL_END_ALL               FLASH_APPL_END_ALL

#define FLASH_UPDATE_APPL_HEADER_START          FLASH_UPDATE_APPL_START_ALL                 ///< Start address of new application code stored in update region
#define FLASH_UPDATE_APPL_START                 (FLASH_UPDATE_APPL_START_ALL + FLASH_APPL_HEADER_LENGTH)    ///< Update application start address

#endif // BL_UPDATE_APP


#define CRC32_OK                                0u              ///< Success
#define CRC32_ERROR                             1u              ///< Failure

#define FLASH_BLOCKSIZE                         FLASH_SECTOR_SIZE                         ///< Block size

#define FLASH_PAGESIZE                          (0x100u)        ///< the flash can only programmed 256byte wise

#define CHECK_INV_16(x)                         (((x) & 0xFFu) == (~((x) >> 8) & 0xFFu))    ///< Check if high part is the inverse of the low part

#define EE_FACTORY_RESET_CNT_VAL                (10u)
#define EE_FACTORY_RESET_CLEAR_TIME_MS          (6000u)

// ---------------------------------------------------------------------------------------------------
// Software Version Name
// ---------------------------------------------------------------------------------------------------

#ifdef BOOTLOADER_ENABLE_SIGNATURE_CHECK
    //                      "                    " exact 20 characters, filled up with spaces.
    #define SW_VERSION_NAME_DEFAULT "S32K BL DEFAULT     "
#else
    // development bootloader without signature check
    //                      "                    " exact 20 characters, filled up with spaces.
#if !defined(BL_UPDATE_APP)
	#define SW_VERSION_NAME_DEFAULT "S32K BL DEFAULT X   "
#else
	#define SW_VERSION_NAME_DEFAULT "S32K BL UPDATER"
#endif // BL_UPDATE_APP

#endif

// ===================================================================================================
// Typedefs, enums
// ===================================================================================================

typedef enum
{
    STATE_SIGN_SHA_INIT = 0u,   ///< initialize the SHA256 algorithm
    STATE_SIGN_SHA_UPDATE,      ///< update the SHA256 hash with data
    STATE_SIGN_VERIFY           ///< finalize the SHA256 hash and verify the RSA signature
} enum_SIGN_STATE;

// Bootloader sub-ids
typedef enum
{
    E_SUB_BL_ADDR                = 0u,
    E_SUB_PC_CMD                     ,
    E_SUB_BL_RESP                    ,  // = E_SUB_BL_FLASH_ACK,
    E_SUB_PC_FLASH_DATA              ,
    E_SUB_BL_EEP_DATA                ,
    E_SUB_PC_EEP_DATA                ,
    E_SUB_BL_HW_RESP                 ,
    E_SUB_PC_HW_CMD                  ,
    E_SUB_MAX
}t_bl_sub;

// Bootloader EEPROM access modes
typedef enum
{
    E_ACCESS_NONE                = 0u,
    E_ACCESS_BL                      ,
    E_ACCESS_CAL                     ,
    E_ACCESS_USER                    ,
    E_ACCESS_ALL                     ,
    E_ACCESS_MAX
}t_access;

// Bootloader commands
typedef enum
{
    E_BL_SCAN                    =0u,
    E_BL_ADDRESSING                 ,
    E_BL_ADDRESSING_NEW             ,
    E_BL_PROG_STOP_MASTER           ,
    E_BL_PROG_STOP_MASTER_NEW       ,
    E_BL_PROG_START_MASTER          ,
    E_BL_PROG_START_MASTER_NEW      ,
    E_BL_PROG_STOP_USER             ,
    E_BL_PROG_STOP_USER_NEW         ,
    E_BL_PROG_START_USER            ,
    E_BL_PROG_START_USER_NEW        ,
    E_BL_EEPROM_WRITE_BL            ,
    E_BL_EEPROM_WRITE_CAL           ,
    E_BL_EEPROM_WRITE_USER          ,
    E_BL_EEPROM_WRITE_ALL           ,
    E_BL_EEPROM_WRITE_STOP          ,
    E_BL_EEPROM_WRITE               ,
    E_BL_EEPROM_READ                ,
    E_BL_EEPROM_CAN_EXT_BAUD_INP_SMP,
    E_BL_EEPROM_CAN_EXT_BAUD_INP    ,
    E_BL_EEPROM_CAN_EXT_BAUD_INP_AND_CHANGE,
#if !defined(ROLE_MINIMAL)
    E_BL_FLASH_REQUEST              ,
    E_BL_FLASH_CLEAR_APPL           ,
    E_BL_FLASH_WRITE                ,
    E_BL_READ_COMMIT_HASH           ,

#if defined(IO_TEST_ENABLE)
	E_BL_HW_IO_TEST					,
#endif // IO_TEST_ENABLE

#else
    E_BL_FLASH_REQUEST              ,
#endif
    E_BL_MAX

} enum_BL_CMD;

typedef enum
{
    E_PC_HW_INIT_DIG_IN           =1u,
    E_PC_HW_INIT_ANA_IN           =2u,
    E_PC_HW_INIT_DIG_OUT          =3u,
    E_PC_HW_READ_DIG              =4u,
    E_PC_HW_READ_ANA              =5u,
    E_PC_HW_WRITE_DIG             =6u,
    E_PC_HW_GOTO_SLEEP            =7u,
    E_PC_HW_SET_CAN               =8u,   // Bei HCS12XEQ einen anderen CAN-Bus aktivieren und dann �ber diesen kommunizieren
    E_PC_HW_INIT_SHIFT_REG        =15u,
    E_PC_HW_WRITE_SHIFT_REG       =16u,
    E_PC_HW_INIT_SCI              =20u,
    E_PC_HW_INIT_SPI              =25u,
    E_PC_HW_WRITE_SPI             =26u,
    E_PC_HW_TEST_WATCHDOG         =30u,  //DR20160715 Beim CAN-Interface sollte ab jetzt immer der Watchdog getestet werden => Endlosschleife
    E_PC_HW_GET_MORE_TIME         =50u  //DR20101028 Normalerweise nur f�r 10s m�glich auf diese Befehle zuzugreifen, damit auch l�nger (z.B. wegen Debuggen)
} enum_BL_HW_CMD;

// CAN receive check rx message
typedef enum
{
    E_CHECK_NONE                 = 0u,
    E_CHECK_SN                       ,
    E_CHECK_DATA                     ,
    E_CHECK_MAX
}t_check;

enum E_RESET_TYPE
{
    E_RESET_TYPE_NONE = 0,
    E_RESET_TYPE_BL,
    E_RESET_TYPE_APP
};

typedef enum
{
    E_SEND_SN_SCAN               = 0u,
    E_SEND_SN_DATA                   ,
    E_SEND_DATA                      ,
    E_SEND_SPECIFIC                  ,
    E_SEND_MAX
}t_send;

enum E_FLASH_STATE
{
  E_FLASH_RECORD_WAIT_START_CHAR = 0u   ,
  E_FLASH_RECORD_READ                  ,
  E_FLASH_RECORD_PROCESS               ,
};

/** enum for different system resets */
typedef enum
{
    HAL_SYS_RESET_TYPE_NONE = 0,        ///< No Reset
    HAL_SYS_RESET_TYPE_SOFTRESET_APP,   ///< do a soft reset to restart the application
    HAL_SYS_RESET_TYPE_SOFTRESET_BL,    ///< do a soft reset to restart and enter the bootloader
    HAL_SYS_RESET_TYPE_SOFTRESET_BL_UPDATE_FROM_COPY,    ///< do a soft reset to restart and enter the bootloader to do a firmware update from the update image
    HAL_SYS_RESET_TYPE_HARDRESET_WD,    ///< do a hard reset (reset with the watchdog)
    HAL_SYS_RESET_TYPE_HARDRESET_CPU,   ///< do a hard reset (CPU internal reset)

    HAL_SYS_RESET_TYPE_START_APP        ///< do a jump to start the application
} enum_HAL_SYS_RESET;

typedef enum
{
    BL_EXEC_STATE_UNKNOWN = 0,
    BL_EXEC_STATE_STARTED,
    BL_EXEC_STATE_INITIALIZING,
    BL_EXEC_STATE_INITIALIZED,
    BL_EXEC_STATE_AT_LEAST_ONCE_CONNECTED,
    BL_EXEC_STATE_JUMP_TO_APP,
} enum_BL_EEPROM_STATE;


// flash modes for the flash handler
typedef enum
{
    FLASH_MODE_ERASE = 0u,  ///< erase
    FLASH_MODE_PROG,        ///< programm
} enum_FLASH_MODE;

typedef enum
{
    E_PROG_OK                    = 0u,  // Program running
    E_PROG_STOP_PARAMETER            ,  // Program automatically stopped due to parameter error: a) Portbyte invalid b) Baudrate invalid c)
    E_PROG_STOP_RESET                ,  // Program automatically stopped due to errorneous resets
    E_PROG_STOP_USER                 ,  // Program stopped by user
    E_PROG_STOP_MASTER               ,  // Program stopped by MRS master
    E_PROG_MAX
} enum_PROG_STATE;

typedef struct
{
    enum_FLASH_MODE flash_mode;
    uint32_t flash_block;
    uint32_t flash_addr;
    uint8_t buffer[300u];
    uint8_t header_buffer[300u];
//    enum_FLASH_ERR_CODES_TYPE status;
    enum_CODE status;
} struct_flash_handler_data;

/** This struct defines the application flash header */
typedef struct
{
    uint32_t header_key;            ///< Header Key to identify the header
    uint32_t header_crc;            ///< CRC32 checksum over the header content
    uint32_t app_header_version;    ///< Header version
    uint32_t application_crc;       ///< CRC32 checksum of the application
    uint32_t application_length;    ///< Length of the application, used to calculate the CRC32 checksum
    uint8_t sw_version[32];         ///< Additional SW Version String
#if HAL_SYS_APP_HEADER_VERSION >= 1
    uint8_t reserve[460];
    uint8_t signature_key[512];
#endif
} struct_hal_sys_app_header_t;

/** Hardware bootloader port pin state type
 *  See KSB-724 issue for more details.
 * */
typedef struct
{
	bool active;					///< true when port-pin is non-zero and valid
	bool signal;					///< state of hardware bootloader pin
	bool active_at_startup;			///< state of hardware bootloader port-pin at startup, used only in bootloader
} struct_hw_bl_t;

// ===================================================================================================
// Public function prototypes
// ===================================================================================================

/*----------------------------------------------------------------------------*/
/**
* \ingroup		role_protocol_bl
* \brief		This function is called by the role functionality internally to construct a
* 				bootloader protocol role and save it into the role pool.
* 				MUST NOT be called by a user.
* \details
*
* \pre
*
* \param id [in]		role_id_t		role id to be constructed
* \return void
*/
void role_protocol_bl_constructor(role_id_t id, pf_role_constructor parent);

#ifdef __cplusplus
}
#endif

#endif /* SRC_ROLE_BL_PROTOCOL_H_ */
