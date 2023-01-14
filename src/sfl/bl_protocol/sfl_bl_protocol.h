#ifndef SFL_BL_PROTOCOL_H
#define SFL_BL_PROTOCOL_H
/*----------------------------------------------------------------------------*/
/**
* \file         sfl_bl_protocol.h
* \brief        Definitions for MRS Bootloader Protocol.
*
* \todo         Move bl_protocol into CFL.
* \todo         Create a Bootloader config file for dynamic setup of (MCU/Baudrate,...).
* \todo         Generate Set and Get Methods for external variables.
* \todo         Make sfl_bl_can_receive() to a private function.
* \todo         Make sfl_bl_get_ascii_ptr() private.
* \todo         Make sfl_bl_set_eeprom_access() private.
* \todo         Make sfl_bl_can_read_eeprom() private.
* \todo         Make sfl_bl_set_program_state() private.
* \todo         Make sfl_bl_check_port_bit() private.
* \todo         Make sfl_bl_check_crc() private.
* \todo         Make sfl_bl_can_receive() private.
* \todo         Make sfl_bl_set_flasher_state() private.
* \todo         Maybe remove sfl_bl_change_can_baudrate(), not used, no handler needed,.
*
*
*/
/*----------------------------------------------------------------------------*/
/**
* \addtogroup  sfl_bl_protocol MRS Bootloader Protocol [BL]
* \{
* \brief    Communicate with the MRS Developer Studio to read/write from/to dflash and for configuration & parameterization.
* \details  This requires a CAN bus interface on your device Hardware.
*           The MRS Bootloader Protocol consists of four parts: The NVM access, application observation, the CAN part with FIFO.
*           Main goal of the protocol is to communicate with the MRS Developer Studio Software, where you are able to download new applications, write and read
*           values or just exchange live data via CAN bus.\n
*
*           The MRS library provides the functionality to detect if a costumer application is executed properly. For this a reset counter is implemented in EEPROM.
*           When application is executed without a problem, this counter is set to 0 after a certain time. When for example a watchdog reset
*           happened and the counter is higher than value X the module is stopped
*           and stays in bootloader to flash a valid application. \n
*
*           IMPORTANT: At this time only one single CAN Interface with MRS BL support is available. \n
*
* \internal
*           Many parameters are stored in a persistent storage. This is normally Flash but here the naming EEPROM is used (historical).
*           Some EEPROM parameters need to be checked before they are applied (eg. bootloaders baudrate). Idea was to create redundancy by inverting the byte and store the inverted byte also.
*           This method is called inverted mirroring (old but works). The BL Protocol uses different access levels to write to EEPROM. DS handles them automatically.
*
*           MRS BLE protocol uses different CAN-IDs to communicate. A base ID is used and depending on the operations, an offset of this ID is used. The protocol uses up to 7 different CAN-IDs.
*           The IDs are dynamically settable in EEPROM to avoid CAN-ID collisions between MRS Protocol and Costumer CAN-IDs.
*
*           A table (st_bl_msg bl_msg) contains all available bootloader commands that are supported.
*           This table includes the action callback handlers and if possible the automatic generated response messages.
*
*           After 10 seconds with no communication, the bootloader access is disabled to protect from invalid access. \n
* \endinternal
*           Example
*           Initializes the CAN0 interface and use this for MRS Bootloader Protocol.
*    \code{.c}
*    struct_hal_can_handle  can_handle;
*
*    void usercode_init(void)
*    {
*        (void)hal_can_init(&can_handle, CAN_0, FALSE);
*        (void)hal_can_set_receive_callback(&can_handle, sfl_bl_can_receive_pre);
*        sfl_bl_init(&can_handle);
*        ...
*    }
*
*    void usercode(void)
*    {
*       sfl_bl_cyclic();
*       ...
*    }
*   \endcode
*
*   For version information see file sfl_bl_protocol_version.h
*/
/*----------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------------------------------
// configuration
// ---------------------------------------------------------------------------------------------------
// change this to switch between different hardware base libraries (for EEPROM, CAN access, etc.)
#define USE_HAL                         1                   ///< Configuration selector define: The HardwareAbstractionLayer is used - use this for MK10D10_BSP and RH850F1L_GHS
#define HCS08_MRS_LIBRARY               0                   ///< Configuration selector define: The HCS08 MRS Library is used


#define MCU_TYPE                        E_MCU_TYPE_RH850    ///< Configuration: The actual used MCU
#define BL_CAN_RX_FIFO_BUFFER_LEN       (16u)               ///< Configuration: Size of the Bootloader message receive FIFO.

// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "hal_data_types.h"

#if USE_HAL == 1

    #include "sfl_timer.h"
    #include "hal_can.h"
    #include "hal_nvm.h"
    #include "hal_sys.h"

#elif HCS08_MRS_LIBRARY == 1

    //nothing to do

#endif

#include "sfl_bl_protocol_s32k.h"

// ---------------------------------------------------------------------------------------------------
// defines
// ---------------------------------------------------------------------------------------------------

//global settings
#define PAR_OFFSET       2u                 ///< EEPROM start address of Factory data offset.

/**
* This enum lists all available Errorcodes that are returned from the MRS Bootloader protocols functions.
*/
typedef enum
{
    ERR_OK          = 0u ,                  ///< OK
    ERR_SPEED       = 1u ,                  ///< This device does not work in the active speed mode.
    ERR_RANGE       = 2u ,                  ///< Parameter out of range.
    ERR_VALUE       = 3u ,                  ///< Parameter of incorrect value.
    ERR_OVERFLOW    = 4u ,                  ///< Timer overflow.
    ERR_MATH        = 5u ,                  ///< Overflow during evaluation.
    ERR_ENABLED     = 6u ,                  ///< Device is enabled.
    ERR_DISABLED    = 7u ,                  ///< Device is disabled.
    ERR_BUSY        = 8u ,                  ///< Device is busy.
    ERR_NOTAVAIL    = 9u ,                  ///< Requested value or method not available.
    ERR_RXEMPTY     = 10u,                  ///< No data in receiver.
    ERR_TXFULL      = 11u,                  ///< Transmitter is full.
    ERR_BUSOFF      = 12u,                  ///< Bus not available.
    ERR_OVERRUN     = 13u,                  ///< Overrun error is detected.
    ERR_FRAMING     = 14u,                  ///< Framing error is detected.
    ERR_PARITY      = 15u,                  ///< Parity error is detected.
    ERR_NOISE       = 16u,                  ///< Noise error is detected.
    ERR_IDLE        = 17u,                  ///< Idle error is detected.
    ERR_FAULT       = 18u,                  ///< Fault error is detected.
    ERR_BREAK       = 19u,                  ///< Break char is received during communication.
    ERR_CRC         = 20u,                  ///< CRC error is detected.
    ERR_ARBITR      = 21u,                  ///< A node losts arbitration. This error occurs if two nodes start transmission at the same time.
    ERR_PROTECT     = 22u,                  ///< Protection error is detected.
    ERR_UNDERFLOW   = 23u,                  ///< Underflow error is detected.
    ERR_UNDERRUN    = 24u,                  ///< Underrun error is detected.
    ERR_COMMON      = 25u,                  ///< Common error of a device.
    ERR_LINSYNC     = 26u,                  ///< LIN synchronization err
    ERR_NOT_OK      = 27u,                  ///< any other error

}enum_MRS_BL_ERROR_CODES_T;

// Default values for bootloader id
#define C_BL_ID             (0x1FFFFFF0u)   ///< CAN ID where MRS Bootlaoder communicates with.
#define C_BL_ID_EXT         (1u)            ///< Flag that indicates if the MRS-BL Can ID is an extended (29-Bit) CAN-ID (1) or a normal (11-Bit) ID.
#define C_BL_ID_NIBBLE      (0u)            ///< MRS BL Protocol CAN-ID base offset.

// ---------------------------------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------------------------------
/*----------------------------------------------------------------------------*/
/**
*   \brief      This macro is used to check if baudrate is in range.
*   \details    Before a baudrate is applied, it is checked if it fits the MRS used baudrates.
*
*   \param  x   The baudrate that will be ckecked. Format must be one of the values from #enum_bl_baudrate_t.
*   \return     TRUE for value is in range or FALSE for invalid baudrate.
*/
#define CHECK_BAUDRATE(x) (((x) & 0xFFu) >= E_BAUD_1MBIT && ((x) & 0xFFu) < E_BAUD_MAX)

/*----------------------------------------------------------------------------*/
/**
*   \brief      This macro is used to generate an inverted and mirrored value of a byte.
*   \details    This is used in EEPROM to make sure that the content is correct.
*               An eight bit value is inverted and shifted to the upper byte of an 16 Bit word.
*               The lower 16 Bit are used as the value as it was. Use this to generate redundancy.
*   \param  x   A Byte that contains an information.
*   \return     Returns a 16 Bit word that contains the inverted input as high byte and the original byte as low-byte.
*/
#define INV_16(x) (uint16_t)(((~x) << 8) + (x))

/*----------------------------------------------------------------------------*/
/**
*   \brief      Check if inversion mirroring is ok.
*   \details    This is the inverted macro of INV_16().
*               It is checked if the Inverted High-Byte of the parameter is same as the low byte.
*   \param  x   16 Bit word that should be ckecked if correctly inverted.
*   \return     Returns TRUE if inverted value fits the original one or FALSE when not.
*/
#define CHECK_INV_16(x) (((x) & 0xFFu) == (~((x) >> 8) & 0xFFu))

/*----------------------------------------------------------------------------*/
/**
*   \brief      Swap the two bytes of a 16 Bit word.
*   \details    Depending on the endianess of a MCU, you may need to use a different byte order.
*               This macro converts a 16 Bit word into the other format (Little-endian, Big-endian).
*   \param x    The 16 Bit word you want to convert.
*   \return     The endianess-converted input.
*/
#define SWAP_16(x) (((x) >>  8) & 0xFFu) | (((x) << 8) & 0xFF00u)

/*----------------------------------------------------------------------------*/
/**
*   \brief      Swap the four bytes of a 32 Bit word.
*   \details    Depending on the endianess of a MCU, you may need to use a different byte order.
*               This macro converts a 32 Bit word into the other format (Little-endian, Big-endian).
*   \param x    The 32 Bit word you want to convert.
*   \return     The endianess-converted input.
*/
#define SWAP_32(x) (((x) >> 24) & 0xFFu) | (((x) << 8) & 0xFF0000u) | (((x) >> 8) & 0xFF00u) | (((x) << 24) & 0xFF000000u)

/*----------------------------------------------------------------------------*/
/**
*   \brief      Merge Bootloader ID offset and extended flag.
*   \details    In EEPROM, the extended can id information flag and the used CAN-ID offset are stored in a single byte to save memory.
*               This macro merges the information into a single byte.
*               Remember that there is no parameter check for this macro.
*   \param a    Extended CAN ID flag.
*   \param b    Bootloader ID nibble (offset).
*   \return     The merged byte
*/
#define NIBBLE_H_L(a,b) ((a)|((b) << 4) & 0xFFu)

/*----------------------------------------------------------------------------*/
/**
*   \brief      Calculate Bootloader ID checksum.
*   \details    Naming is incorrect, this macro generates a checksum, not a CRC.
*               Use this macro only for generating the MRS Bootloaders "CRC" for Extended flag, canID-and Offset(nibble)
*   \param a    Extended CAN-ID used flag.
*   \param b    Bootloader CAN-ID Offset (Nibble)
*   \param x    Bootloader basic CAN-ID
*/
#define CALC_CRC(a,b,x) ((~((((x) >> 24) & 0xFFu)+(((x) >> 16u) & 0xFFu) + (((x) >> 8u) & 0xFFu) + ((x) & 0xFFu) + NIBBLE_H_L((a),(b)))) & 0xFFu)

/*----------------------------------------------------------------------------*/
/**
*   \brief      Read the Size of bootloader ID in EEPROM
*   \return     Length in bytes of the CAN specific EEPROM parts.
*/
#define C_BL_ID_CNT (ext_bl_par_data[PAR_BL_ID_EXT1].length + ext_bl_par_data[PAR_BL_ID1].length + ext_bl_par_data[PAR_BL_ID_CRC1].length)

// caller macros to read/write from/to dflash
#if USE_HAL == 1

    #define BL_EEPROM_GET_DATA(addr,pu8Data,d_size)   hal_nvm_eeprom_read_by_address( addr, d_size, pu8Data)    ///< Function Redirect
    #define BL_EEPROM_SET_DATA(addr,pu8Data,d_size)   hal_nvm_eeprom_write_by_address(addr, d_size, pu8Data)    ///< Function Redirect
    #define BL_EEPROM_GET_BYTE(addr, pu8Data)         hal_nvm_eeprom_read_by_address( addr, 1u,     pu8Data)    ///< Function Redirect
    #define BL_EEPROM_SET_BYTE(addr, pu8Data)         hal_nvm_eeprom_write_by_address(addr, 1u,     pu8Data)    ///< Function Redirect

    // 10s access timeout
    #define TRIGGER_BL_ACCESS() (void)sfl_timer_set_timestamp(&mgl_ti_access, HAL_PRECISION_1S);                ///< Function Redirect

    // jump to bootloader by jumping to illegal address
    #define RESET()         0                                                                                   ///< MCU Reset Function for the use of the HAL

#elif HCS08_MRS_LIBRARY == 1

    #define BL_EEPROM_GET_DATA(addr,pu8Data,d_size)   IEE1_GetData((IEE1_TAddress)addr, pu8Data, d_size)        ///< Function Redirect
    #define BL_EEPROM_SET_DATA(addr,pu8Data,d_size)   IEE1_SetData((IEE1_TAddress)addr, pu8Data, d_size)        ///< Function Redirect
    #define BL_EEPROM_GET_BYTE(addr,data)             IEE1_GetByte((IEE1_TAddress)addr,data)                    ///< Function Redirect
    #define BL_EEPROM_SET_BYTE(addr,data)             IEE1_SetByte((IEE1_TAddress)addr,data)                    ///< Function Redirect

    // 10s access timeout
    #define TRIGGER_BL_ACCESS() os_timestamp(&mgl_ti_access, OS_1000ms);                                        ///< Function Redirect

     // Reset function to jump to bootloader
    #define ILLEGAL_OPCODE 0x9E00                                                                               ///< Define an illegal opcode to reset the MCU, just for HCS12 MCU
    #define RESET()  __asm DCW ILLEGAL_OPCODE                                                                   ///< MCU Reset Function for the use of the HCS12 MCU

#endif

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------

/**
*   This enum list all available EEPROM write access rights
*/
typedef enum
{
    E_ACCESS_NONE                = 0u,      ///< Not allowed to write to EEPROM.
    E_ACCESS_BL                      ,      ///< Write access to modify bootloader and factory data (EEPROM Addr < 200[0xC8]).
    E_ACCESS_CAL                     ,      ///< Write access to modify calibration values (EEPROM Addr in range 200[0xC8] to 512[0x200]).
    E_ACCESS_USER                    ,      ///< Write access to modify user eeprom values (EEPROM Addr in range 512[0x200] to 2048[0x800]).
    E_ACCESS_ALL                     ,      ///< Master write Access. Allowance to write to all parts of the EEPROM.
    E_ACCESS_MAX                            ///< Last element in enum. Do not use.
}t_access;


// Bootloader sub-ids
/**
*   This enum lists all CAN Sub-IDs (Offset) that are used by MRS protocol base CAN-ID as offset.
*/
typedef enum
{
    E_SUB_BL_ADDR                = 0u,      ///< Scan ID. Used for scan all devices or to answer on scan requests.
    E_SUB_PC_CMD                     ,      ///< Remote (PC) sends a command to MCU
    E_SUB_BL_RESP                    ,      ///< Response of a normal command. Nearly all commands use this eg. EEPROM write access answer (Former E_SUB_BL_FLASH_ACK)
    E_SUB_PC_FLASH_DATA              ,      ///< Remote Write data to MCUs Code flash
    E_SUB_BL_EEP_DATA                ,      ///< Data content of EEPROM read data command (not command response).
    E_SUB_PC_EEP_DATA                ,      ///< Remote have send new data for EEPROM.
    E_SUB_BL_HW_RESP                 ,      ///< Unused?
    E_SUB_PC_HW_CMD                  ,      ///< Unused?
    E_SUB_MAX                               ///< Do not use. Leave at last position in enum.
}t_bl_sub;


/**
*   This enum lists all available checks that should be performed to a received can-messages data-content.
*   The protocol needs for some commands special checks eg. the identification of a module.
*   The data content must be at a fixed position in can message.
*/
typedef enum
{
    E_CHECK_NONE                 = 0u,      ///< No further data content check needed.
    E_CHECK_SN                       ,      ///< Serial number check requested (E.g. Check if the requested command is for this MCU).
    E_CHECK_DATA                     ,      ///< Data value check requested. Some commands must have a special data content (like eeprom write access). This data should be checked (located in .d_in2 part of message)
    E_CHECK_MAX                             ///< Maximum enum value. Do not use and leave at last position.
}t_check;


/**
*   This enum lists the available automatic answers for bl protocol.
*   Those are needed where automatic answers are possible or maybe no function callback for user handling is needed.
*/
typedef enum
{
    E_SEND_SN_SCAN               = 0u,      ///< Send scan response.
    E_SEND_SN_DATA                   ,      ///< Send serial number.
    E_SEND_DATA                      ,      ///< Send the requested data.
    E_SEND_SPECIFIC                  ,      ///< Send a specific response, not categorized in the upper values.
    E_SEND_MAX                              ///< Last element in enum. Do not use.
}t_send;

// Bootloader CAN baudrates
/**
*   This enum contains all for MRS BL available CAN Baudrates.
*   Other baudrates are not supported.
*/
typedef enum
{
    E_BAUD_1MBIT                 = 1u,      ///< Bootloader Baudrate is 1MBIT
    E_BAUD_800KBIT                   ,      ///< Bootloader Baudrate is 800KBIT
    E_BAUD_500KBIT                   ,      ///< Bootloader Baudrate is 500KBIT
    E_BAUD_250KBIT                   ,      ///< Bootloader Baudrate is 250KBIT
    E_BAUD_125KBIT                   ,      ///< Bootloader Baudrate is 125KBIT
    E_BAUD_50KBIT                    ,      ///< Bootloader Baudrate is 50KBIT
    E_BAUD_20KBIT                    ,      ///< Bootloader Baudrate is 20KBIT
    E_BAUD_10KBIT                    ,      ///< Bootloader Baudrate is 10KBIT
    E_BAUD_100KBIT                   ,      ///< Bootloader Baudrate is 100KBIT
    E_BAUD_83KBIT                    ,      ///< Bootloader Baudrate is 83KBIT
    E_BAUD_33KBIT                    ,      ///< Bootloader Baudrate is 33KBIT
    E_BAUD_MAX                              ///< Last element in enum. Do not use.
} enum_bl_baudrate_t;

/**
*   This enum lists the available states of the user application.
*   Normal running mode is OK. In all other cases, the application is not active and device stays in bootloader until value set valid.
*/
enum E_PROG_STATE
{
    E_PROG_OK                    = 0u,      ///< Program running
    E_PROG_STOP_PARAMETER            ,      ///< Program automatically stopped due to parameter error: a) Portbyte invalid b) Baudrate invalid c)
    E_PROG_STOP_RESET                ,      ///< Program automatically stopped due to erroneous resets
    E_PROG_STOP_USER                 ,      ///< Program stopped by user
    E_PROG_STOP_MASTER               ,      ///< Program stopped by MRS master
    E_PROG_MAX                              ///< Last element in enum. Do not use.
};


/**
*   EEPROM parameter type.
*   Each parameter in EEPROM has a special type. Depending on this type, the behavior of handing is different.
*   A raw number can be interpreted, a ASCII value must be handled as pointer,...
*/
typedef enum
{
    T_NUMBER                     = 0u,      ///< Parameter is from type INTEGER.
    T_ASCII                          ,      ///< Parameter has type String.
    T_RESERVE                        ,      ///< Reserved parameters are unused.
    T_MAX                                   ///< Maximum element in enum. Do not use.

}t_par_datatype;

/**
*   This enum lists all available Bootloader CAN commands
*/
typedef enum E_BL_MSG
{
    E_BL_SCAN                    =0u,       ///< Command scan all modules on the CAN bus (Remote, Master command).
    E_BL_ADDRESSING                 ,       ///<
    E_BL_PROG_STOP_MASTER           ,       ///< Command Stop application and jump to Bootloader with MRS access.
    E_BL_PROG_START_MASTER          ,       ///< Command (re)start application (soft reset) with MRS access.
    E_BL_PROG_STOP_USER             ,       ///< Command Stop application and jump to Bootloader.
    E_BL_PROG_START_USER            ,       ///< Command (re)start application (soft reset).
    E_BL_EEPROM_WRITE_BL            ,       ///< Command get eeprom write security access for bl parameter.
    E_BL_EEPROM_WRITE_CAL           ,       ///< Command get eeprom write security access for calibration values.
    E_BL_EEPROM_WRITE_USER          ,       ///< Command get eeprom write security access for user area.
    E_BL_EEPROM_WRITE_ALL           ,       ///< Command get eeprom write security access for all areas.
    E_BL_EEPROM_WRITE_STOP          ,       ///< Command to clear the security access rights.
    E_BL_EEPROM_WRITE               ,       ///< ?
    E_BL_EEPROM_READ                ,       ///< ?
#ifdef BOOTLOADER_FLASH_ACTIVE
    E_BL_FLASH_REQUEST              ,       ///< In Application: command for getting security access rights
    E_BL_FLASH_CLEAR_APPL           ,       ///< In Application: command for getting security access rights
    E_BL_FLASH_WRITE                ,       ///< In Application: command for getting security access rights
#else
    E_BL_FLASH_REQUEST              ,       ///< In Application: command for getting security access rights
#endif
    E_BL_MAX                                ///< Last element in list. Do not use.

}t_bl_msg;

// All EEPROM parameters
/**
*   EEPROM factory data storage enumeration.
*   The bootloader data (also called factory data) stores the most important tings in eeprom.
*   Here for example the serial number for addressing the module or can baudrate on which the device communicates is stored.
*   Each ID represents an eeprom element with different lengths.
*/
enum E_PAR_DATA
{
  // ------------------------------------------- Factory data -------------------------------------------
    PAR_ID                      = 0u,       ///< ID  0: Factory Data start marker. Indicates if valid data is written.
    PAR_SERIALNUMBER                ,       ///< ID  1: 32-Bit Serial number of the device. Serial number is not unique, each partnumber has a new serial number value.
    PAR_PARTNUMBER                  ,       ///< ID  2: Part number of a MRS module.
    PAR_DRAWINGUMBER                ,       ///< ID  3: Drawing number.
    PAR_NAME                        ,       ///< ID  4: ?
    PAR_ORDERNUMBER                 ,       ///< ID  5: Ordering number of module.
    PAR_TEST_DATE                   ,       ///< ID  6: The date the device was produced/tested.
    PAR_HW_VERSION                  ,       ///< ID  7: Hardware revision / index.
    PAR_RESETCOUNTER                ,       ///< ID  8: Number of resets since since last operational Application.
    PAR_LIB_VERSION                 ,       ///< ID  9: Used library version number.
    PAR_RESETREASON_LVD             ,       ///< ID 10: Count for reset reason was Low voltage detection.
    PAR_RESETREASON_LOC             ,       ///< ID 11: ?
    PAR_RESETREASON_ILAD            ,       ///< ID 12: ?
    PAR_RESETREASON_ILOP            ,       ///< ID 13: ?
    PAR_RESETREASON_COP             ,       ///< ID 14: Count for reset reason was watchdog .
    PAR_MCU_TYPE                    ,       ///< ID 15: MCU that is present on the System.
    PAR_HW_CAN_ACTIVE               ,       ///< ID 16: Bit coded information which can mus on hardware is physically available.
    PAR_RESERVE1                    ,       ///< ID 17: Reserved for future use.
    // ------------------------------------------- Bootloader data -------------------------------------------
    PAR_BL_VERSION                  ,       ///< ID 18: Bootloader's version information.
    PAR_PROG_STATE                  ,       ///< ID 19: Application running/ stopped information.
    PAR_PORTBYTE1                   ,       ///< ID 20: Bootloader mode (HW/SW bootloader) Indicates the Port and Pin information on which GPIO the HW bootloader reacts.
    PAR_PORTBYTE2                   ,       ///< ID 21: Copy of the first portbyte.
    PAR_BL_BAUDRATE1                ,       ///< ID 22: For bootloader communication used baudrate.
    PAR_BL_BAUDRATE2                ,       ///< ID 23: Copy of the bootloaders used baudrate.
    PAR_BL_ID_EXT1                  ,       ///< ID 24: Flag indicating if BL uses extended or normal CAN IDs.
    PAR_BL_ID1                      ,       ///< ID 25: Bootloaders CAN ID.
    PAR_BL_ID_CRC1                  ,       ///< ID 26: Checksum for Bootloaders CAN setup.
    PAR_BL_ID_EXT2                  ,       ///< ID 27: Copy of PAR_BL_ID_EXT1.
    PAR_BL_ID2                      ,       ///< ID 28: Copy of PAR_BL_ID1.
    PAR_BL_ID_CRC2                  ,       ///< ID 29: Copy of PAR_BL_ID_CRC1.
    PAR_SW_VERSION                  ,       ///< ID 30: User defined software version.
    PAR_MODULENAME                  ,       ///< ID 31: User defined Name of the module.
    PAR_BL_CANBUS                   ,       ///< ID 32: CAN-Bus index which is used for Bootloader communication.
    PAR_WD_TIMEOUT                  ,       ///< ID 33: Watchdog timeout time (ms).
    PAR_RESERVE2                    ,       ///< ID 34: Reserved for future use.
    PAR_MAX                                 ///< Last value in enum. Do not use.
};

/**
*   This enum lists all available MCUs that are actually supoorted by MRS Bootlaoder Protocol.
*   The protocol itself is MCU independent but the implementation for eg. flash write must be handled
*   separately. To select a dedicated MCU or flash flow, this different MCu defiens are used.
*/
enum E_MCU_TYPE
{
    E_MCU_TYPE_NONE = 0,                    ///< No MCU is selected
    E_MCU_TYPE_HCS08,                       ///< NXP (Freescale) HCS08 (DZ60) is used
    E_MCU_TYPE_HCS12,                       ///< NXP (Freescale) HCS12 (XEQ384, XEP, XEG,...) is used
    E_MCU_TYPE_HCS08DZ128,                  ///< NXP (Freescale) HCS08 (DZ128) is used
    E_MCU_TYPE_MK10xx,                      ///< NXP (Freescale) MK series MCU is used
    E_MCU_TYPE_RH850                        ///< Renesas RH850 MCU is used
};

/**
*   This enum specifies the different reset modes.
*/
enum E_RESET_TYPE
{
    E_RESET_TYPE_NONE = 0,                  ///< No Reset
    E_RESET_TYPE_BL,                        ///< Jump into MRS bootloader (hard reset).
    E_RESET_TYPE_APP                        ///< Jump into application startup (soft reset).
};




// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------


/**
* CAN frame format structure
*/
typedef struct
{
        uint32_t    id;                     ///< Bit 0-28: CAN identifier, Bit 30: rtr, Bit 31: Frame format (0=11bit, 1=29bit)
        uint8_t     dlc;                    ///< Length of message
        uint8_t     data[8];                ///< 8 data bytes
        uint16_t    can_timestamp;          ///< Timestamp

}struct_bl_can_frame;

/**
* CAN RX FIFO structure
*/
typedef struct
{
    uint8_t write_ptr;                      ///< Next write.
    uint8_t read_ptr;                       ///< next read.
    uint8_t overrun;                        ///< Fifo full flag.
    struct_bl_can_frame buffer[BL_CAN_RX_FIFO_BUFFER_LEN]; ///< Can frame buffer
} struct_bl_can_rx_fifo;


/**
*   Structure of bootloader commands table
*   This struct contains all elements that are used for a bootloader command table.
*/
struct st_bl_msg
{
    t_bl_sub d_rx_sub;                      ///< Can-ID offset of base BL can id that is used for this command
    uint8_t  d_in1[2];                      ///< The first 2 Bytes of CAN data. They contain the command that should be performed.
    t_check  d_check;                       ///< Indication if a special check is needed: eg. is the requested serial number ours.
    uint8_t  d_rx_dlc;                      ///< Can DLC of received data.
    uint8_t  d_in2[6];                      ///< The rest of the CAN data bytes. Contain further information for the command.
    uint8_t (*d_ptr_callback)(uint8_t, struct_bl_can_frame*, struct_bl_can_frame*);     ///< Function that is called when command arrives
    t_send   d_send;                        ///< Information which answer type should be sent.
    t_bl_sub d_tx_sub;                      ///< CAN-ID offset of base BL id that is used for the commands response.
    uint8_t  d_tx_dlc;                      ///< Data length in bytes for the commands response
    uint8_t  d_send_data[8];                ///< The Data for the commands response
};


/**
*   ASCII table structure.
*   All ASCII values that are stored in EEPROM must be identified due to its identification.
*/
struct st_ascii
{
    uint8_t  par_name;                      ///< Identification of the string. Use values from #E_PAR_DATA.
    uint8_t  str[30];                       ///< String content.
};

/**
*   EEPROM parameter map.
*   This structure maps the EEPROM IDs to its byte length, data type and default value.
*/
struct struc_par_data
{
    uint8_t length;                         ///< Length in bytes of the storage element.
    t_par_datatype data_type;               ///< Type of the storage element.
    uint32_t val_default;                   ///< Default value for uninitialized parameters.
};

// ---------------------------------------------------------------------------------------------------
// externals
// ---------------------------------------------------------------------------------------------------
extern uint8_t ext_bl_flasher_active;
extern uint32_t ext_bl_sub_fr[E_SUB_MAX];
extern uint8_t ext_bl_par_addr[PAR_MAX];
extern const struct struc_par_data ext_bl_par_data[PAR_MAX];
extern uint8_t ext_bl_access;
extern uint8_t ext_flag_trigger_app_reset;

// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------

#if USE_HAL == 1

    /*----------------------------------------------------------------------------*/
    /**
    * \brief    Initialize bootloader protocol
    * \details
    *
    * \param    handle [in] struct_hal_can_handle* Pointer to (an initialized) can handle structure where bootloader protocol should communicate
    * \return   void
    */
    void sfl_bl_init(struct_hal_can_handle* handle);

#elif HCS08_MRS_LIBRARY == 1

    /*----------------------------------------------------------------------------*/
    /**
    * \brief    Initialize bootloader protocol
    * \details
    *
    * \return   void
    */
    void sfl_bl_init(void);

#endif

/*----------------------------------------------------------------------------*/
/**
* \brief    Cyclic caller for bootloader protocol.
* \details  This is used for cyclic checks e.g. timeout ...
*
* \return   void
*/
 void sfl_bl_cyclic(void);

/*----------------------------------------------------------------------------*/
/**
* \brief    Set eeprom/dflash access
* \details  Depending on the received message (Bootloader CAN ID-Offset) the EEPROM rights are set.
*
* \param    p_step    [in] uint8_t                 Use a value from #E_BL_MSG to set the eeprom access rights.
* \param    p_msg_in  [in] struct_bl_can_frame*    Unused. Reserved for future use.
* \param    p_msg_out [in] struct_bl_can_frame*    Unused. Reserved for future use.
* \return   uint8_t   Feedback of function. Uses ERR_VALUE for failed or ERR_OK for success.
*/
uint8_t sfl_bl_set_eeprom_access(uint8_t p_step, struct_bl_can_frame* p_msg_in, struct_bl_can_frame* p_msg_out);

/*----------------------------------------------------------------------------*/
 /**
* \brief    Read from eeprom/dflash
* \details  Result is given by a CAN message in MRS BL format. The data is not sent, just prepared.
*           When data is received on BL-ID + #E_SUB_BL_RESP (2), the action has failed. Then the following data contains further information.
*           data[2]=0x0E => invalid DLC for received CAN frame.
*           data[2]=0x01 => Read failed at position data[3]. Errorcode data[4].
*           On a successful action, the read data is transmitted on BL-CAN-ID + E_SUB_BL_EEP_DATA(4).
*
* \param    p_step    [in] uint8_t                 Unused. Reserved for future use.
* \param    p_msg_in  [in] struct_bl_can_frame*    Pointer to received can message. Data[0]/[1]= Received Command, [2]/[3]=Address to read, [4]=Byte length (DLC min=5)
* \param    p_msg_out [out] struct_bl_can_frame*   Pointer to send message. Data[0...n] contain the read data. Remember to check the response CAN-ID!
* \return   uint8_t   Feedback of function. #ERR_OK for success or a positive error code.
*/
 uint8_t sfl_bl_can_read_eeprom(uint8_t p_step, struct_bl_can_frame* p_msg_in, struct_bl_can_frame* p_msg_out);

/*----------------------------------------------------------------------------*/
 /**
* \brief    Write to eeprom/dflash
* \details  This function writes the given values into nvm.
*           Also checks if portbyte and inverted mirrors are also correct.
*           An acknowledge is given on BL-CAN-ID + #E_SUB_BL_RESP (2). The following error codes are provided:
*           data[2]=0x0F => No access rights given.
*           data[2]=0x0E => invalid DLC.
*           data[2]=0x0C => Wrong access rights given.
*           data[2]=0x0B => Pre-checks not passed (BL dependent baudrate, portbyte, checksum).
*           data[2]=0x01 => Writing failed.
*
* \param    p_step    [in] uint8_t                 Unused. Reserved for future use.
* \param    p_msg_in  [in] struct_bl_can_frame*    Pointer to received can message. Data[0]/[1]=Address to write, [2]/[3]=Address to read, [4]=Byte length (DLC min=5)
* \param    p_msg_out [in] struct_bl_can_frame*    Pointer to send message.
* \return   uint8_t   Feedback of function
*/
 uint8_t sfl_bl_can_write_eeprom(uint8_t p_step, struct_bl_can_frame* p_msg_in, struct_bl_can_frame* p_msg_out);

/*----------------------------------------------------------------------------*/
 /**
* \brief    Set program state of bootloader protocol process
* \details  Write the programs state into EEPROM. Later the Bootloader can select if the application will start or module is stopped.
*
* \param    p_step    [in] uint8_t                 The command that changes the program state. A value from #E_BL_MSG (eg. E_BL_PROG_STOP_MASTER)
* \param    p_msg_in  [in] struct_bl_can_frame*    Unused. Reserved for future use.
* \param    p_msg_out [in] struct_bl_can_frame*    Unused. Reserved for future use.
* \return   uint8_t   Feedback of function.
*/
uint8_t sfl_bl_set_program_state(uint8_t p_step, struct_bl_can_frame* p_msg_in, struct_bl_can_frame* p_msg_out);

/*----------------------------------------------------------------------------*/
 /**
* \brief    Read default EEPROM string content.
* \details  This function returns the address of an ASCII string that is used as the default values.
*           When invalid index as given, the value "n/a" is returned.
*
* \param    p_index [in] uint8_t Index variable of EEPROM id. Use a value from #E_PAR_DATA where ext_bl_par_data[x].data_type == T_ASCII.
* \return   Pointer to first char of string.
*/
uint8_t* sfl_bl_get_ascii_ptr(uint8_t p_index);

/*----------------------------------------------------------------------------*/
 /**
* \brief    Check port byte settings
* \details  Checks if the port bit is valid  (0 or a valid port)
* \todo     Port bit seems to be hardware dependent. Add a define in configuration that allows more than 6 Ports.
*
* \param    p_byte  [in] uint8_t   byte value to be checked
* \return   uint8_t  feedback of function
*/
uint8_t sfl_bl_check_port_bit(uint8_t p_byte);

/*----------------------------------------------------------------------------*/
 /**
* \brief    Restore (reload) the default value for given EEPROM element.
* \details  The default values are type dependent: #ext_bl_par_data says which type to use. Then the values either from variable .val_default or strings from #par_ascii.
*
* \param    i       [in] uint8_t Index of EEPROM ID that will be reloaded. Use values from #E_PAR_DATA.
* \return   uint8_t Feedback of function.
*/
uint8_t sfl_bl_write_default_value(uint8_t i);

/*----------------------------------------------------------------------------*/
 /**
* \brief    Build and compare given data's checksum (called "crc")
* \details  This is the S19 checksum. All bytes in an S19 line are added. The checksum fills the value to 0xFF.
*
* \param    *p_data [in] uint8_t Pointer to data where checksum have to be checked.
* \param    p_start [in] uint8_t Offset of where in array to start.
* \param    p_cnt   [in] uint8_t Number of bytes that are used (from p_start).
* \return   uint8_t Returns TRUE if checksum for given data is valid(=0xFF), FALSE when not.
*/
 uint8_t sfl_bl_check_crc(uint8_t *p_data, uint8_t p_start, uint8_t p_cnt);

/*----------------------------------------------------------------------------*/
/**
* \brief    Send a message on CAN bus.
* \details  Differs in the implementation for HAL or non HAL feature set.
*
* \param    p_id    [in] uint32_t can id
* \param    p_dlc   [in] uint8_t can dlc
* \param    p_b0    [in] uint8_t can data byte 0
* \param    p_b1    [in] uint8_t can data byte 1
* \param    p_b2    [in] uint8_t can data byte 2
* \param    p_b3    [in] uint8_t can data byte 3
* \param    p_b4    [in] uint8_t can data byte 4
* \param    p_b5    [in] uint8_t can data byte 5
* \param    p_b6    [in] uint8_t can data byte 6
* \param    p_b7    [in] uint8_t can data byte 7
* \return   uint8_t feedback of function
*/
uint8_t sfl_bl_can_send(uint32_t p_id, uint8_t p_dlc, uint8_t p_b0, uint8_t p_b1, uint8_t p_b2, uint8_t p_b3, uint8_t p_b4, uint8_t p_b5, uint8_t p_b6, uint8_t p_b7);

/*----------------------------------------------------------------------------*/
/**
* \brief    Parses and processes a can frame.
* \details  Executes the callback that was configured in bl_msg table and sends the generated or automatic answer.
*           This is an internal function, do not use it directly.
*
* \param    msg  [in] struct_bl_can_frame* Pointer to can message structure that was received.
* \return   void
*/
void sfl_bl_can_receive(struct_bl_can_frame* msg);

#if USE_HAL == 1

    /*----------------------------------------------------------------------------*/
    /**
    * \brief   CAN-RX Message handler for BL protocol (Using HAL).
    * \details Use this function in your RX-interrupt or on the function that handles it.
    *          Internally this function puts the data into a FIFO that is handled later at the cyclic call.
    *
    * \param    ptr_can_msg               [in] struct_hal_can_frame* Pointer to received can message structure.
    */
    void sfl_bl_can_receive_pre(struct_hal_can_frame* ptr_can_msg);

#elif HCS08_MRS_LIBRARY == 1

    /*----------------------------------------------------------------------------*/
    /**
    * \brief    CAN-RX Message handler for BL protocol (Using HCS12XE).
    * \details  Use this function in your RX-interrupt or on the function that handles it.
    *          Internally this function puts the data into a FIFO that is handled later at the cyclic call.
    *
    * \param    msg  [in] bios_can_msg_typ* Pointer to received can message structure.
    * \return   void
    */
    void sfl_bl_can_receive_pre(bios_can_msg_typ* msg);

#endif

/*----------------------------------------------------------------------------*/
/**
* \brief    Set bootloader state
* \details  Prepares the CAN-message answer for actual state request (dont send) and prepare for jump to bootloader.
*
* \param    p_step      [in] uint8_t                step
* \param    p_msg_in    [in] struct_bl_can_frame*   Unused. Reserved for future use.
* \param    p_msg_out   [in] struct_bl_can_frame*   Pointer to can message prepared for answer.
* \return   Feedback of function. Actually unused.
*/
uint8_t sfl_bl_set_flasher_state(uint8_t p_step, struct_bl_can_frame* p_msg_in, struct_bl_can_frame* p_msg_out);

/*----------------------------------------------------------------------------*/
/**
* \brief    Change the baudrate for bootloader can interface.
* \details  This function does not update the values in eeprom, just a local change. When an invalid value is given, the baudrate is set to 125K.
*
* \param    handle   [in] struct_hal_can_handle*    A Pointer to the can handle. Normally this parameter is used as #bl_protocol_handle and could be internal.
* \param    baudrate [in] enum_bl_baudrate_t        The new baudrate.
* \return   void
*/
void sfl_bl_change_can_baudrate(struct_hal_can_handle* handle, enum_bl_baudrate_t baudrate);

/*----------------------------------------------------------------------------*/
/**
* \brief    Reads the hw_version string from eeprom.
* \details  Copies the version into output parameter.
*
* \param    *hw_version [out] uint8_t Pointer where result is stored.
* \return   void
*/
void sfl_bl_get_hw_version(uint8_t *hw_version);

/** \} */
#endif // SFL_BL_PROTOCOL_H
