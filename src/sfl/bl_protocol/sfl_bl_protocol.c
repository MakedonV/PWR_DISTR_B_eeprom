/*----------------------------------------------------------------------------*/
/**
 * \file         sfl_bl_protocol.c
 * \brief        Implementation of MRS Bootloader Protocol.
 */
/*----------------------------------------------------------------------------*/
/**
 * \addtogroup  sfl_bl_protocol
 * \{
*/
// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "sfl_bl_protocol.h"

#ifdef BOOTLOADER_FLASH_ACTIVE
    #include "bl_flash.h"
#endif
// ---------------------------------------------------------------------------------------------------
// tables
// ---------------------------------------------------------------------------------------------------

/**
*   This is the global command lookup table where automatic response and callbacks are located.
*/
const struct st_bl_msg bl_msg[E_BL_MAX] = {
//   d_rx_sub      d_in1[2]     d_check      d_rx_dlc  d_in2[6]                               *d_ptr_callback                 d_send       d_tx_sub  d_tx_dlc   d_send_data[8];
{   E_SUB_PC_CMD ,{0x00, 0x00}  ,E_CHECK_NONE   ,2  ,{0x00,}                                ,NULL_PTR                   ,E_SEND_SN_SCAN ,E_SUB_BL_ADDR  ,8  ,{0x00,}                        },  // Scan module
{   E_SUB_PC_CMD ,{0x20, 0x10}  ,E_CHECK_SN     ,6  ,{0x00,}                                ,NULL_PTR                   ,E_SEND_SN_DATA ,E_SUB_BL_RESP  ,8  ,{0x21, 0x10,}                  },  // Address module
{   E_SUB_PC_CMD ,{0x60, 0x22}  ,E_CHECK_DATA   ,8  ,{0x57, 0xF0, 0xCA, 0xEA, 0xF2, 0xF5}   ,&sfl_bl_set_program_state  ,E_SEND_SN_DATA ,E_SUB_BL_RESP  ,8  ,{0x61, 0x22,}                  },  // Stop  program master
{   E_SUB_PC_CMD ,{0x70, 0x22}  ,E_CHECK_DATA   ,8  ,{0x75, 0x0F, 0xAC, 0xAE, 0x2F, 0x5F}   ,&sfl_bl_set_program_state  ,E_SEND_SN_DATA ,E_SUB_BL_RESP  ,8  ,{0x71, 0x22,}                  },  // Start program master
{   E_SUB_PC_CMD ,{0x40, 0x22}  ,E_CHECK_DATA   ,8  ,{0x44, 0xEF, 0xBA, 0xAA, 0xE2, 0xF1}   ,&sfl_bl_set_program_state  ,E_SEND_SN_DATA ,E_SUB_BL_RESP  ,8  ,{0x41, 0x22,}                  },  // Stop  program user
{   E_SUB_PC_CMD ,{0x50, 0x22}  ,E_CHECK_DATA   ,8  ,{0x45, 0xEE, 0xBB, 0xAB, 0xE3, 0xF2}   ,&sfl_bl_set_program_state  ,E_SEND_SN_DATA ,E_SUB_BL_RESP  ,8  ,{0x51, 0x22,}                  },  // Start program user
{   E_SUB_PC_CMD ,{0x20, 0x01}  ,E_CHECK_DATA   ,5  ,{0x3F, 0x33, 0xFA,}                    ,&sfl_bl_set_eeprom_access  ,E_SEND_DATA    ,E_SUB_BL_RESP  ,5  ,{0x20, 0xF0, 0x01, 0x00, 0x00,}},  // EEPROM write access bootloader
{   E_SUB_PC_CMD ,{0x20, 0x04}  ,E_CHECK_DATA   ,5  ,{0x4F, 0x44, 0xFB,}                    ,&sfl_bl_set_eeprom_access  ,E_SEND_DATA    ,E_SUB_BL_RESP  ,5  ,{0x20, 0xF0, 0x01, 0x00, 0x00,}},  // EEPROM write access calibration
{   E_SUB_PC_CMD ,{0x20, 0x08}  ,E_CHECK_DATA   ,5  ,{0x8F, 0x88, 0xFB,}                    ,&sfl_bl_set_eeprom_access  ,E_SEND_DATA    ,E_SUB_BL_RESP  ,5  ,{0x20, 0xF0, 0x01, 0x00, 0x00,}},  // EEPROM write access user data
{   E_SUB_PC_CMD ,{0x20, 0x11}  ,E_CHECK_DATA   ,5  ,{0xF3, 0x33, 0xAF,}                    ,&sfl_bl_set_eeprom_access  ,E_SEND_DATA    ,E_SUB_BL_RESP  ,5  ,{0x21, 0x11, 0x01, 0x00, 0x00,}},  // EEPROM write access all
{   E_SUB_PC_CMD ,{0x20, 0x02}  ,E_CHECK_NONE   ,2  ,{0x00,}                                ,&sfl_bl_set_eeprom_access  ,E_SEND_DATA    ,E_SUB_BL_RESP  ,5  ,{0x20, 0xF0, 0x02, 0x00, 0x00,}},  // EEPROM write access stop
{   E_SUB_PC_EEP_DATA ,{0, 0 }  ,E_CHECK_NONE   ,0  ,{0x00,}                                ,&sfl_bl_can_write_eeprom   ,E_SEND_SPECIFIC,E_SUB_BL_RESP  ,0  ,{0x00,}                        },  // EEPROM write
{   E_SUB_PC_CMD ,{0x20, 0x03}  ,E_CHECK_NONE   ,5  ,{0x00,}                                ,&sfl_bl_can_read_eeprom    ,E_SEND_SPECIFIC,E_SUB_BL_RESP  ,0  ,{0x00,}                        },  // EEPROM read
// Bootloader Flashfunction
#ifdef BOOTLOADER_FLASH_ACTIVE
{   E_SUB_PC_CMD ,{0x20, 0x00}        ,E_CHECK_NONE   ,5  ,{0x00,}                                ,&sfl_bl_set_flasher_state  ,E_SEND_DATA    ,E_SUB_BL_RESP   ,8  ,{0x21, 0x0,}                  },  // BL_Flash Write / PC-flash-question
{   E_SUB_PC_CMD ,{0x02, 0x02}        ,E_CHECK_NONE   ,5  ,{0x00,}                                ,&sfl_bl_can_clear_flash    ,E_SEND_SPECIFIC,E_SUB_BL_RESP   ,8  ,{0x00}                        },  // BL_Flash APPL_Clear
{   E_SUB_PC_FLASH_DATA ,{0x0, 0x0}   ,E_CHECK_NONE   ,0  ,{0x00,}                                ,&sfl_bl_can_write_flash    ,E_SEND_SPECIFIC,E_SUB_BL_RESP   ,0  ,{0x00,}                       },  // BL_Flash Write
#else
{   E_SUB_PC_CMD ,{0x20, 0x00}        ,E_CHECK_NONE   ,5  ,{0x00,}                                ,&sfl_bl_set_flasher_state  ,E_SEND_SPECIFIC    ,E_SUB_BL_RESP   ,8  ,{0x2F, 0xFF,}                  },  // BL_Flash Write / PC-flash-question
#endif
};

/**
*   This table contains all Factory / Bootloader eeprom data that is from type ASCII.
*/
const struct st_ascii par_ascii[] = {
{   PAR_PARTNUMBER      ,""             },
{   PAR_DRAWINGUMBER    ,""             },
{   PAR_NAME            ,""             },
{   PAR_ORDERNUMBER     ,""             },
{   PAR_TEST_DATE       ,""             },
{   PAR_HW_VERSION      ,""             },
{   PAR_SW_VERSION      ,"NO SOFTWARE"  },
{   PAR_MODULENAME      ,"Modulname"    },
};

/**
*   This table contains the EEPROM description of the factory and bootloader data. 
*   Each eeprom index element has a length and a type. This is another lookup table to quickly interpret the EEPROM types and lengths.
*/
const struct struc_par_data ext_bl_par_data[PAR_MAX] = {
// ----------------------------------------------------------------------------------------------------------------
//  Len     DataType   Default
// ----------------------------------------------------------------------------------------------------------------
{  2       ,T_NUMBER  ,1331                                            },  // PAR_ID
{  4       ,T_NUMBER  ,0                                               },  // PAR_SERIALNUMBER
{  12      ,T_ASCII   ,0                                               },  // PAR_PARTNUMBER
{  12      ,T_ASCII   ,0                                               },  // PAR_DRAWINGUMBER
{  20      ,T_ASCII   ,0                                               },  // PAR_NAME
{  8       ,T_ASCII   ,0                                               },  // PAR_ORDERNUMBER
{  8       ,T_ASCII   ,0                                               },  // PAR_TEST_DATE
{  2       ,T_ASCII   ,0                                               },  // PAR_HW_VERSION
{  1       ,T_NUMBER  ,0                                               },  // PAR_RESETCOUNTER
{  2       ,T_NUMBER  ,0                                               },  // PAR_LIB_VERSION
{  1       ,T_NUMBER  ,0                                               },  // PAR_RESETREASON_LVD
{  1       ,T_NUMBER  ,0                                               },  // PAR_RESETREASON_LOC
{  1       ,T_NUMBER  ,0                                               },  // PAR_RESETREASON_ILAD
{  1       ,T_NUMBER  ,0                                               },  // PAR_RESETREASON_ILOP
{  1       ,T_NUMBER  ,0                                               },  // PAR_RESETREASON_COP
{  1       ,T_NUMBER  ,MCU_TYPE                                        },  // PAR_MCU_TYPE
{  1       ,T_NUMBER  ,0                                               },  // PAR_HW_CAN_ACTIVE
{  3       ,T_RESERVE ,0                                               },  // PAR_RESERVE1
{  2       ,T_NUMBER  ,0                                               },  // PAR_BL_VERSION
{  2       ,T_NUMBER  ,INV_16(0x00)                                    },  // PAR_PROG_STATE
{  2       ,T_NUMBER  ,INV_16(0x00)                                    },  // PAR_PORTBYTE1
{  2       ,T_NUMBER  ,INV_16(0x00)                                    },  // PAR_PORTBYTE2
{  2       ,T_NUMBER  ,INV_16(E_BAUD_125KBIT)                          },  // PAR_BL_BAUDRATE1
{  2       ,T_NUMBER  ,INV_16(E_BAUD_125KBIT)                          },  // PAR_BL_BAUDRATE2
{  1       ,T_NUMBER  ,NIBBLE_H_L(C_BL_ID_EXT,C_BL_ID_NIBBLE)          },  // PAR_BL_ID_EXT1
{  4       ,T_NUMBER  ,C_BL_ID                                         },  // PAR_BL_ID1
{  1       ,T_NUMBER  ,CALC_CRC(C_BL_ID_EXT, C_BL_ID_NIBBLE, C_BL_ID)  },  // PAR_BL_ID_CRC1
{  1       ,T_NUMBER  ,NIBBLE_H_L(C_BL_ID_EXT,C_BL_ID_NIBBLE)          },  // PAR_BL_ID_EXT2
{  4       ,T_NUMBER  ,C_BL_ID                                         },  // PAR_BL_ID2
{  1       ,T_NUMBER  ,CALC_CRC(C_BL_ID_EXT, C_BL_ID_NIBBLE, C_BL_ID)  },  // PAR_BL_ID_CRC2
{  20      ,T_ASCII   ,0                                               },  // PAR_SW_VERSION
{  30      ,T_ASCII   ,0                                               },  // PAR_MODULENAME
{  1       ,T_NUMBER  ,0                                               },  // PAR_BL_CANBUS
{  2       ,T_NUMBER  ,INV_16(0x03)                                    },  // PAR_WD_TIMEOUT
{  7       ,T_RESERVE ,0                                               },  // PAR_RESERVE2
};

const uint8_t par_ascii_max = sizeof(par_ascii) / sizeof(par_ascii[0]); ///< This constant holds the maximum number of EEPROM parameters stored by bl protocol.


// ---------------------------------------------------------------------------------------------------
//  globals
// ---------------------------------------------------------------------------------------------------
uint8_t  ext_bl_flasher_active = 0;                 ///< Global flag that indicates if communication on the MRS BL Protocol is active or not.
uint32_t ext_bl_sub_fr[E_SUB_MAX];                  ///< Global table that holds the CAN-IDs for bootloader protocols CAN communication.
uint8_t  ext_bl_access = FALSE;                     ///< Global flag that indicates if MRS BL communication is allowed or not.

// ---------------------------------------------------------------------------------------------------
// module globals
// ---------------------------------------------------------------------------------------------------
uint8_t  mgl_bl_id_ext = C_BL_ID_EXT;               ///< Indicates if external can ID is used
uint8_t  mgl_bl_id_nibble = C_BL_ID_NIBBLE;         ///< Holds the MRS BL protocol base CAN ID offset.
uint8_t  mgl_prog_state;                            ///< Holds the programming state. Used to identify if application is started or not.
uint32_t mgl_bl_id = C_BL_ID;                       ///< Holds the bootloader CAN ID.
uint16_t mgl_bl_version = 0u;                       ///< Identify the mrs bl protocols version. (Dont use SFL_BL_PROTOCOL_VERSION)
uint32_t mgl_bl_sn = 0u;                            ///< Serial number
uint32_t mgl_ti_access = 0u;                        ///< Timestamp for last protocol usage.
uint32_t mgl_ti_reset_delay = 0u;                   ///< Delay for resetting the access rights.

struct_bl_can_rx_fifo mgl_bl_can_rx_fifo;           ///< CAN-RX fifo for MRS BL protocol com.

// --------------------------------------------------------------------------

t_access mgl_eeprom_access;                         ///< EEPROM access level
uint8_t ext_bl_par_addr[PAR_MAX];                   ///< EEPROM Address to ID MAP. Each index holds the address of the given EEPROM element. This is initialized at startup.

#if USE_HAL == 1
    struct_hal_can_handle bl_protocol_handle;       ///< CAN handle for tx messages.
#endif

uint8_t ext_flag_trigger_app_reset = E_RESET_TYPE_NONE; ///< Flag that indicates if a reset of the Application or a jump into bootloader is required.

/*----------------------------------------------------------------------------*/
/**
* \internal
* \endinternal
*/
#if USE_HAL == 1
    void sfl_bl_init(struct_hal_can_handle* handle)
    {
#elif HCS08_MRS_LIBRARY == 1
    void sfl_bl_init(void)
    {
#endif

    uint16_t p_id, p_prog_state;
    const uint8_t bl_id_cnt = C_BL_ID_CNT;
    uint8_t  p_arr1[8], p_arr2[8];
    uint8_t  check1, check2;
    uint16_t p_pb1, p_pb2;
    uint8_t mcu_type = 0;
    uint8_t i;

#ifdef BOOTLOADER_FLASH_ACTIVE
#else
    // Reset the Access Key
    hal_sys_set_bl_access_key(0u);
#endif
    // Get EEPROM identifier (1331)
    (void)BL_EEPROM_GET_DATA(PAR_OFFSET, (uint8_t*)&p_id, ext_bl_par_data[PAR_ID].length);

	#if USE_HAL == 1

        // Apply handle
        bl_protocol_handle = *handle;

        // Swap motorola -> intel
        p_id = SWAP_16(p_id);

    #endif

    // All available EEPROM values...
    for (i=0;i<PAR_MAX;i++)
    {
        // Initialize absolute addresses
        if (i == 0)
        {
            ext_bl_par_addr[i] = PAR_OFFSET;
        }
        else
        {
            ext_bl_par_addr[i] = ext_bl_par_addr[i-1] + ext_bl_par_data[i-1].length;
        }

        // Initialize default values?
        if (p_id != ext_bl_par_data[PAR_ID].val_default)
        {
            (void)sfl_bl_write_default_value(i);
        }
        // Check existing values?
        else
        {
            if (i == PAR_SERIALNUMBER)
            {
                (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_SERIALNUMBER], (uint8_t*)&mgl_bl_sn, ext_bl_par_data[PAR_SERIALNUMBER].length);

                //RH8501FL is little endian -> intel format
                //so change to big endian
#if USE_HAL == 1
                mgl_bl_sn = SWAP_32(mgl_bl_sn);
#endif
            }
            else if (i == PAR_PROG_STATE)
            {
                (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_PROG_STATE], (uint8_t*)&p_prog_state, ext_bl_par_data[PAR_PROG_STATE].length);

#if USE_HAL == 1
                p_prog_state = SWAP_16(p_prog_state);
#endif

                mgl_prog_state = (uint8_t)p_prog_state; // Use low-byte


            }
            else
            {
                //do nothing
            }
        }
    }

    // Check further EEPROM values
    // a) Bootloader ID:       Redundant + global variables
    // b) Port-Byte:           Redundant (for HW-Bootloader)
    // c) Bootloader Baudrate: Redundant
    if (p_id == ext_bl_par_data[PAR_ID].val_default)
    {
        // ---------------------------------------
        // Check redundant bootloader id
        // ---------------------------------------

        // Get BL-ID 1+2
        (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_BL_ID_EXT1], p_arr1, bl_id_cnt);
        (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_BL_ID_EXT2], p_arr2, bl_id_cnt);

        // Check BL-ID 1+2
        check1 = sfl_bl_check_crc(p_arr1, 0, bl_id_cnt);
        check2 = sfl_bl_check_crc(p_arr1, 0, bl_id_cnt);

        // 1 OK, 2 NOK => Copy 1 -> 2
        if (check1 && !check2)
        {
            (void)BL_EEPROM_SET_DATA(ext_bl_par_addr[PAR_BL_ID_EXT2], p_arr1, bl_id_cnt);
        }
        // 1 NOK, 2 OK => Copy 2 -> 1
        else if (!check1 && check2)
        {
            (void)BL_EEPROM_SET_DATA(ext_bl_par_addr[PAR_BL_ID_EXT1], p_arr2, bl_id_cnt);
        }
        // 1 NOK, 2 NOK => Set default values
        else if (!check1 && !check2)
        {
            (void)sfl_bl_write_default_value(PAR_BL_ID_EXT1);
            (void)sfl_bl_write_default_value(PAR_BL_ID_EXT2);
            (void)sfl_bl_write_default_value(PAR_BL_ID1);
            (void)sfl_bl_write_default_value(PAR_BL_ID2);
            (void)sfl_bl_write_default_value(PAR_BL_ID_CRC1);
            (void)sfl_bl_write_default_value(PAR_BL_ID_CRC2);
        }
        else
        {
            //do nothing
        }

        // In the end read (maybe updated) values because they are needed for global variables
        (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_BL_ID_EXT1], p_arr1, bl_id_cnt);
        mgl_bl_id_ext = p_arr1[0] & 0x0Fu;
        mgl_bl_id_nibble = (p_arr1[0] >> 4u) & 0x0Fu;
        mgl_bl_id_nibble = mgl_bl_id_nibble * 4u;
        mgl_bl_id = *(uint32_t*)&p_arr1[1];

#if USE_HAL == 1
        mgl_bl_id = SWAP_32(mgl_bl_id);
#endif
        // add mgl_bl_id_ext to the ID
        mgl_bl_id |= ((uint32_t)(mgl_bl_id_ext & 0x01u) << 31u);

        // ---------------------------------------
        // Check redundant port-byte (for HW bootloader)
        // ---------------------------------------

        // Get Port-Byte 1+2
        (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_PORTBYTE1], (uint8_t*)&p_pb1, ext_bl_par_data[PAR_PORTBYTE1].length);
        (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_PORTBYTE2], (uint8_t*)&p_pb2, ext_bl_par_data[PAR_PORTBYTE2].length);

#if USE_HAL == 1
        p_pb1 = SWAP_16(p_pb1);
        p_pb2 = SWAP_16(p_pb2);
#endif

        // Check Port-Byte 1+2
        check1 = CHECK_INV_16(p_pb1);
        check2 = CHECK_INV_16(p_pb2);

        // 1 OK, 2 NOK => Copy 1 -> 2
        if (check1 && !check2)
        {
#if USE_HAL == 1
            p_pb1 = SWAP_16(p_pb1);
#endif
            (void)BL_EEPROM_SET_DATA(ext_bl_par_addr[PAR_PORTBYTE2], (uint8_t*)&p_pb1, ext_bl_par_data[PAR_PORTBYTE2].length);
        }
        // 1 NOK, 2 OK => Copy 2 -> 1
        else if (!check1 && check2)
        {
#if USE_HAL == 1
            p_pb2 = SWAP_16(p_pb2);
#endif
            (void)BL_EEPROM_SET_DATA(ext_bl_par_addr[PAR_PORTBYTE1], (uint8_t*)&p_pb2, ext_bl_par_data[PAR_PORTBYTE1].length);
        }
        // 1 NOK, 2 NOK => Set default values
        else if (!check1 && !check2)
        {
            (void)sfl_bl_write_default_value(PAR_PORTBYTE1);
            (void)sfl_bl_write_default_value(PAR_PORTBYTE2);
        }
        else
        {
            //do nothing
        }

        // ---------------------------------------
        // Check redundant bootloader baudrate
        // ---------------------------------------

        // Get bootloader baudrate 1+2
        (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_BL_BAUDRATE1], (uint8_t*)&p_pb1, ext_bl_par_data[PAR_BL_BAUDRATE1].length);
        (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_BL_BAUDRATE2], (uint8_t*)&p_pb2, ext_bl_par_data[PAR_BL_BAUDRATE2].length);

#if USE_HAL == 1
        p_pb1 = SWAP_16(p_pb1);
        p_pb2 = SWAP_16(p_pb2);
#endif
        // Check bootloader baudrate 1+2
        check1 = (CHECK_INV_16(p_pb1)) && (CHECK_BAUDRATE(p_pb1));
        check2 = (CHECK_INV_16(p_pb2)) && (CHECK_BAUDRATE(p_pb2));

        // 1 OK, 2 NOK => Copy 1 -> 2
        if (check1 && !check2)
        {
#if USE_HAL == 1
            p_pb1 = SWAP_16(p_pb1);
#endif
           (void)BL_EEPROM_SET_DATA(ext_bl_par_addr[PAR_BL_BAUDRATE2], (uint8_t*)&p_pb1, ext_bl_par_data[PAR_BL_BAUDRATE2].length);
        }
        // 1 NOK, 2 OK => Copy 2 -> 1
        else if (!check1 && check2)
        {
#if USE_HAL == 1
            p_pb2 = SWAP_16(p_pb2);
#endif
            (void)BL_EEPROM_SET_DATA(ext_bl_par_addr[PAR_BL_BAUDRATE1], (uint8_t*)&p_pb2, ext_bl_par_data[PAR_BL_BAUDRATE1].length);
        }
        // 1 NOK, 2 NOK => Set default values
        else if (!check1 && !check2)
        {
            (void)sfl_bl_write_default_value(PAR_BL_BAUDRATE1);
            (void)sfl_bl_write_default_value(PAR_BL_BAUDRATE2);
        }
        else
        {
            //do nothing
        }
    }


    // Initialize bootloader SUB-IDs
    for (i=0;i<E_SUB_MAX;i++)
    {
        //mgl_bl_sub[i] = mgl_bl_id + ((uint32_t)i << mgl_bl_id_nibble);
        ext_bl_sub_fr[i] = mgl_bl_id + ((uint32_t)i << mgl_bl_id_nibble);
    }

    // ---------------------------------------
    // Read the MCU TYPE
    // ---------------------------------------
    (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_MCU_TYPE], (uint8_t*)&mcu_type, ext_bl_par_data[PAR_MCU_TYPE].length);

    if(mcu_type != MCU_TYPE)
    {
        mcu_type = MCU_TYPE;

        (void)BL_EEPROM_SET_DATA(ext_bl_par_addr[PAR_MCU_TYPE], (uint8_t*)&mcu_type, ext_bl_par_data[PAR_MCU_TYPE].length);
    }


    // ---------------------------------------
    // Read the Bootloader version
    // ---------------------------------------
    (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_BL_VERSION], (uint8_t*)&mgl_bl_version, ext_bl_par_data[PAR_BL_VERSION].length);

#if USE_HAL == 1
    mgl_bl_version = SWAP_16(mgl_bl_version);
#endif

#ifdef BOOTLOADER_FLASH_ACTIVE
    (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_BL_BAUDRATE1], (uint8_t*)&p_pb1, ext_bl_par_data[PAR_BL_BAUDRATE1].length);

#if USE_HAL == 1
    p_pb1 = SWAP_16(p_pb1);
#endif
    // Check bootloader baudrate 1 and change it
    // TODO: may cause problems because the BSP has initialized the CAN already with another baudrate
    check1 = (CHECK_INV_16(p_pb1)) && (CHECK_BAUDRATE(p_pb1));
    if(check1)
    {
        sfl_bl_change_can_baudrate(&bl_protocol_handle, (enum_bl_baudrate_t) (p_pb1 & 0xFF));
    }

    // check the BL Version
    if(mgl_bl_version != C_BL_VERSION)
    {
        mgl_bl_version = C_BL_VERSION;
    #if USE_HAL == 1
        mgl_bl_version = SWAP_16(mgl_bl_version);
    #endif
    (void)BL_EEPROM_SET_DATA(ext_bl_par_addr[PAR_BL_VERSION], (uint8_t*)&mgl_bl_version, ext_bl_par_data[PAR_BL_VERSION].length);

    }
    mgl_bl_version = C_BL_VERSION;

    // Check if a Software or a Hardware Bootloader was selected
    (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_PORTBYTE1], (uint8_t*)&p_pb1, ext_bl_par_data[PAR_PORTBYTE1].length);

#if USE_HAL == 1
    p_pb1 = SWAP_16(p_pb1);
#endif

    // Check port config
    check1 = CHECK_INV_16(p_pb1);

    // Send Startup Message to Start the Download process if a SW bootloader is selected or if the bootloader was requested by the application
    if( ( (check1) && ((p_pb1 & 0xFFu) == 0u) ) || (hal_sys_get_bl_access_key() == HAL_SYS_BOOTLOADER_ACCESS_KEY) )
    {
        (void)sfl_bl_can_send(ext_bl_sub_fr[E_SUB_BL_ADDR], 8, 1, (uint8_t)(mgl_bl_sn>>24), (uint8_t)(mgl_bl_sn>>16), (uint8_t)(mgl_bl_sn>>8), (uint8_t)mgl_bl_sn, mgl_prog_state, mgl_bl_id_nibble, mgl_bl_version);
    }


#else
    // fall back for the application if the bootloader version is not set (0) then set it temporary to 1, because the developers studio can't detect a BL Version 0
    if(mgl_bl_version == 0)
    {
        mgl_bl_version = 1;
    }
#endif

}

/*----------------------------------------------------------------------------*/
/**
* \internal
* Get pointer of ASCII string of EEPROM default values
* \endinternal
*
*/
uint8_t* sfl_bl_get_ascii_ptr(uint8_t p_index)
{
    uint8_t i;
    
    for(i=0;i<par_ascii_max;i++)
    {
        if (par_ascii[i].par_name == p_index)
        {
            return (uint8_t*)par_ascii[i].str;
        }
    }
    
    return (uint8_t*)"n/a";
}

/*----------------------------------------------------------------------------*/
/** 
* \internal
*   Set eeprom/dflash access
* \endinternal
*
*/
uint8_t sfl_bl_set_eeprom_access(uint8_t p_step, struct_bl_can_frame* p_msg_in, struct_bl_can_frame* p_msg_out)
{
    uint8_t ret = ERR_OK;
    
    // Suppress warning
    (void)p_msg_in; (void)p_msg_out;
    
    // Set EEPROM write access
    switch (p_step)
    {
        case E_BL_EEPROM_WRITE_BL:      mgl_eeprom_access = E_ACCESS_BL;    break;
        case E_BL_EEPROM_WRITE_CAL:     mgl_eeprom_access = E_ACCESS_CAL;   break;
        case E_BL_EEPROM_WRITE_USER:    mgl_eeprom_access = E_ACCESS_USER;  break;
        case E_BL_EEPROM_WRITE_ALL:     mgl_eeprom_access = E_ACCESS_ALL;   break;
        case E_BL_EEPROM_WRITE_STOP:    mgl_eeprom_access = E_ACCESS_NONE;  break;
        default: ret = ERR_VALUE;   break;
    }
    
    return ret;
}

/*----------------------------------------------------------------------------*/
/** 
* \internal
*   Read from eeprom/dflash
* \endinternal
*
*   \todo Check if return value for error should not be overwritten.
*
*/
uint8_t sfl_bl_can_read_eeprom(uint8_t p_step, struct_bl_can_frame* p_msg_in, struct_bl_can_frame* p_msg_out)
{
    uint8_t  ret = ERR_OK;
    uint16_t p_addr = (((uint16_t)p_msg_in->data[2] << 8) | (uint16_t)p_msg_in->data[3]);
    uint8_t  p_len  = p_msg_in->data[4];
    uint8_t  i;
    
    // Suppress warning
    (void)p_step;
    
    // Prepare default response
    p_msg_out->id = ext_bl_sub_fr[E_SUB_BL_RESP];
    p_msg_out->dlc = 5;
    p_msg_out->data[0] = p_msg_in->data[0];
    p_msg_out->data[1] = p_msg_in->data[1];
    
    // At least 5 bytes necessary
    if (p_msg_in->dlc >= 5)
    {
        // Limit range from 1 to 8 bytes to read
        if (p_len > 8)
        {
            p_len = 8;
        }
        else if (p_len <= 0)
        {
            p_len = 1;
        }
        
        else
        {
            //do nothing
        }

        // Read every byte separately (to be able to response with erroneous index)
        for (i=0;i<p_len;i++)
        {
            ret = BL_EEPROM_GET_BYTE((p_addr+i), &p_msg_out->data[i]);
            if (ret != ERR_OK)
            {
                break;
            }
        }

        // Read successful or not?
        if (ret == ERR_OK)
        {
            p_msg_out->id = ext_bl_sub_fr[E_SUB_BL_EEP_DATA];
            p_msg_out->dlc = p_len;
        }
        else
        {
            // Is it okay to leave byte 0 and 1 with the read EEPROM data?
            // I think it should be better to overwrite with p_msg_in->data[0/1];
            p_msg_out->data[2] = 0x01;
            p_msg_out->data[3] = i;
            p_msg_out->data[4] = ret;
        }
    }
    else
    {
        p_msg_out->data[2] = 0x0E;
        p_msg_out->data[3] = 0x00;
        p_msg_out->data[4] = 0x00;
    }
    
    return ret;
}

/*----------------------------------------------------------------------------*/
/** 
* \internal
* \endinternal
*
*/
uint8_t sfl_bl_check_port_bit(uint8_t p_byte)
{
    uint8_t p_nr = (p_byte >> 4) & 0x0F;
    uint8_t b_nr = p_byte & 0x0F;
    
    if ((p_nr == 0 && b_nr == 0) || (p_nr > 0 && p_nr <= 6 && b_nr > 0 && b_nr <= 8))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*----------------------------------------------------------------------------*/
/** 
* \internal
* implementation of sfl_bl_check_crc function to check crc
* \endinternal
*/
uint8_t sfl_bl_check_crc(uint8_t *p_data, uint8_t p_start, uint8_t p_cnt)
{
    uint8_t i, crc_sum=0;
 
 
    for(i=p_start;i<p_cnt;i++)  
    {
        crc_sum += p_data[i];
    }
   
    // CRC ok?
    if (crc_sum == 0xFF)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
} 

/*----------------------------------------------------------------------------*/
/** 
* \internal
* \endinternal
*
*/
uint8_t sfl_bl_can_write_eeprom(uint8_t p_step, struct_bl_can_frame* p_msg_in, struct_bl_can_frame* p_msg_out)
{
    uint8_t ret = ERR_OK;
    uint8_t ok = 0;
    uint16_t p_addr = ((uint16_t)p_msg_in->data[0] << 8) | (uint16_t)p_msg_in->data[1];
    uint16_t p_len  = p_msg_in->dlc-2;
    uint16_t p_addr_end = p_addr + p_len - 1;   // -1, end address is also included to write
    const uint8_t  bl_id_cnt = C_BL_ID_CNT;
    
    // Suppress warning
    (void)p_step;
    // Prepare default response
    p_msg_out->id = ext_bl_sub_fr[E_SUB_BL_RESP];
    p_msg_out->dlc = 5;
    p_msg_out->data[0] = 0x20;
    p_msg_out->data[1] = 0xE8;
    p_msg_out->data[2] = 0x0;
    p_msg_out->data[3] = 0x0;
    p_msg_out->data[4] = 0x0;

    // EPPROM write access previously activated?
    if (mgl_eeprom_access != E_ACCESS_NONE)
    {
        // At least 3 bytes received (2 x address + at least 1 databyte
        if (p_msg_in->dlc >= 3)
        {
            // EEPROM write access valid for desired address range?
            if ((mgl_eeprom_access == E_ACCESS_BL   && p_addr >= ext_bl_par_addr[PAR_PORTBYTE1]    && p_addr_end <   200)  // Originally PAR_BL_BAUDRATE1, but PAR_PORTBYTE1 needs a copy, too
             || (mgl_eeprom_access == E_ACCESS_CAL  && p_addr >=   200                      && p_addr_end < 0x200)
             || (mgl_eeprom_access == E_ACCESS_USER && p_addr >= 0x200                      && p_addr_end < 0x800)
             || (mgl_eeprom_access == E_ACCESS_ALL))
            {
                // Create parameter copy in range PAR_PORTBYTE1 (originally PAR_BL_BAUDRATE1) to 200
                if ((mgl_eeprom_access == E_ACCESS_BL || mgl_eeprom_access == E_ACCESS_ALL) && p_addr >= ext_bl_par_addr[PAR_PORTBYTE1] && p_addr_end <   200)
                {
                    // Set to enable parameter write for the other BL parameter
                    ok = 1;

                    // Create Portbyte1 copy if address applies
                    if (p_addr <= ext_bl_par_addr[PAR_PORTBYTE1] && p_addr_end >= ext_bl_par_addr[PAR_PORTBYTE1])
                    {
                        ok = 0;
                        
                        // Exactly write Portbyte1 (with its inverting)?
                        if (p_addr == ext_bl_par_addr[PAR_PORTBYTE1] && p_len == 2)
                        {
                            if (p_msg_in->data[3] == (uint8_t)(~p_msg_in->data[2]))
                            {
                                if (sfl_bl_check_port_bit(p_msg_in->data[3]))
                                {
                                    ret = BL_EEPROM_SET_DATA(ext_bl_par_addr[PAR_PORTBYTE2], &p_msg_in->data[2], ext_bl_par_data[PAR_PORTBYTE2].length);
                                    ok = 1;
                                }
                                else
                                {
                                    ok = 2;
                                }
                            }
                        }
                    }
                    
                    // Create Baudrate1 copy if address applies
                    if (p_addr <= ext_bl_par_addr[PAR_BL_BAUDRATE1] && p_addr_end >= ext_bl_par_addr[PAR_BL_BAUDRATE1])
                    {
                        ok = 0;
                        
                        // Exactly write Baudrate1 (with its inverting)?
                        if (p_addr == ext_bl_par_addr[PAR_BL_BAUDRATE1] && p_len == 2)
                        {
                            if (p_msg_in->data[3] == (uint8_t)(~p_msg_in->data[2]))
                            {
                                if (p_msg_in->data[3] > 0 && p_msg_in->data[3] < E_BAUD_MAX)
                                {
                                    ret = BL_EEPROM_SET_DATA(ext_bl_par_addr[PAR_BL_BAUDRATE2], &p_msg_in->data[2], ext_bl_par_data[PAR_BL_BAUDRATE2].length);
                                    ok = 1; 
                                }
                                else
                                {
                                    ok = 3;
                                }
                            }
                        }
                    }
                    
                    // Create Bootloader1 copy if address applies
                    if (p_addr <= ext_bl_par_addr[PAR_BL_ID_CRC1] && p_addr_end >= ext_bl_par_addr[PAR_BL_ID_EXT1])
                    {
                        ok = 0;
                        
                        // Exactly write Baudrate1 (with its inverting)?
                        if (p_addr == ext_bl_par_addr[PAR_BL_ID_EXT1] && p_len == bl_id_cnt)
                        {
                            if (sfl_bl_check_crc(&p_msg_in->data[2], 0, bl_id_cnt))
                            {
                                ret = BL_EEPROM_SET_DATA(ext_bl_par_addr[PAR_BL_ID_EXT2], &p_msg_in->data[2], bl_id_cnt);
                                ok = 1; 
                            }
                            else
                            {
                                ok = 4;
                            }
                        }
                    }
                }
                else
                {
                    ok = 1;
                }
                
                
                if (ok == 1)
                {
                
                    ret = BL_EEPROM_SET_DATA(p_addr,&p_msg_in->data[2],p_len);
                    if (ret == ERR_OK)
                    {
                        p_msg_out->data[2] = 0x00;  // Finished successful! :-)
                    }
                    else
                    {
                        p_msg_out->data[2] = 0x01;
                        p_msg_out->data[3] = 255;
                        p_msg_out->data[4] = ret;
                    }
                }
                else
                {
                    p_msg_out->data[2] = 0x0B;
                    p_msg_out->data[3] = ok;
                }
                
            }
            else
            {
                // Should have more values:
                // 0 =>  Addr start not in allowed range
                // 1 => E_ACCESS_xx unknown
                // 10 => Addr end not in allowed range with E_ACCESS_BL
                // 11 => Addr end not in allowed range with E_ACCESS_CAL
                // 12 => Addr end not in allowed range with E_ACCESS_USER
                p_msg_out->data[2] = 0x0C;
                p_msg_out->data[3] = ok;
            }
        }
        else
        {
            p_msg_out->data[2] = 0x0E;
        }
    }
    else
    {
        p_msg_out->data[2] = 0x0F;
    }
    return ret;
}

/*----------------------------------------------------------------------------*/
/** 
* \internal
* \endinternal
*/
uint8_t sfl_bl_set_program_state(uint8_t p_step, struct_bl_can_frame* p_msg_in, struct_bl_can_frame* p_msg_out)
{
    uint8_t  ret;
    uint16_t p_state;
    (void)p_msg_in;
    (void)p_msg_out;
    
    // Application only supports PROG_STOP. PROG_START commands will be supported in bootloader.
    if (p_step == E_BL_PROG_STOP_MASTER)
    {
        p_state = INV_16(E_PROG_STOP_MASTER);
    }

    else if (p_step == E_BL_PROG_STOP_USER)
    {
        p_state = INV_16(E_PROG_STOP_USER);
    }

    else if (p_step == E_BL_PROG_START_MASTER)
    {
        p_state = INV_16(E_PROG_OK);
    }

    else if (p_step == E_BL_PROG_START_USER)
    {
        p_state = INV_16(E_PROG_OK);
    }
    else
    {
        //do nothing
    }

    #if USE_HAL == 1
             p_state = SWAP_16(p_state);
    #endif
    
    // Write prog state to EEPROM (reset will be done after this function call)
    ret = BL_EEPROM_SET_DATA(ext_bl_par_addr[PAR_PROG_STATE], (uint8_t*)&p_state, ext_bl_par_data[PAR_PROG_STATE].length);
    
    return ret;
}

/*----------------------------------------------------------------------------*/
/** 
* \internal 
*   This function internally calls sfl_bl_get_ascii_ptr().
* \endinternal
*/
uint8_t sfl_bl_write_default_value(uint8_t i)
{
    uint8_t ret = ERR_OK;
    uint8_t *ptr_start;
    uint8_t *ptr_default;
    uint32_t intel_val;

    // ASCII? Get pointer to string...
    if (ext_bl_par_data[i].data_type == T_ASCII)
    {
        ptr_default = sfl_bl_get_ascii_ptr(i);
    }

    else if (ext_bl_par_data[i].data_type == T_NUMBER)
    {
        // EEPROM Read/Write
        #if USE_HAL == 1

            //RH8501FL is little endian -> intel format
            //so change to big endian
            intel_val = SWAP_32(ext_bl_par_data[i].val_default);
            ptr_start = (uint8_t*)&intel_val;

        #elif HCS08_MRS_LIBRARY == 1

            //HCS12 is big endian -> motorola format#
            ptr_start = (uint8_t*)&ext_bl_par_data[i].val_default;

        #endif

        ptr_default = ptr_start + (4 - ext_bl_par_data[i].length);
    }

    // Reserve etc.? Do nothing.
    else    // T_RESERVE
    {
        ptr_default = NULL_PTR;
    }
    
    if (ptr_default != NULL_PTR)
    {
        ret = BL_EEPROM_SET_DATA(ext_bl_par_addr[i], ptr_default, ext_bl_par_data[i].length);
    }
    
    return ret;
}

/*----------------------------------------------------------------------------*/
/** 
* \internal
* \endinternal
*/
uint8_t sfl_bl_can_send(uint32_t p_id, uint8_t p_dlc, uint8_t p_b0, uint8_t p_b1, uint8_t p_b2, uint8_t p_b3, uint8_t p_b4, uint8_t p_b5, uint8_t p_b6, uint8_t p_b7)
{
#if USE_HAL == 1

	struct_hal_can_frame l_can_msg;
	uint8_t data[8];

	l_can_msg.can_id = p_id;
	l_can_msg.can_dlc = p_dlc;
	data[0] = p_b0;
	data[1] = p_b1;
	data[2] = p_b2;
	data[3] = p_b3;
	data[4] = p_b4;
	data[5] = p_b5;
	data[6] = p_b6;
	data[7] = p_b7;
	l_can_msg.ptr_data = data;

	//send bootloader protocol
	return hal_can_send(&bl_protocol_handle, &l_can_msg);

#elif HCS08_MRS_LIBRARY == 1
	return os_can_send_message(p_id & 0x1FFFFFFF, (p_id >> 31) & 1, p_dlc, p_b0, p_b1, p_b2, p_b3, p_b4, p_b5, p_b6, p_b7);

#endif
}

/*----------------------------------------------------------------------------*/
/** 
* \internal
* \endinternal
*/
void sfl_bl_cyclic(void)
{
#if USE_HAL == 1

    uint8_t elapsed = FALSE;
    uint8_t do_reset_elapsed = FALSE;

    if( ext_bl_access && sfl_timer_time_elapsed( &elapsed, mgl_ti_access, 10u, HAL_PRECISION_1S ) == SFL_TIMER_ERR_OK && elapsed )
    {
        ext_bl_access = FALSE;
    }

    if( mgl_bl_can_rx_fifo.read_ptr != mgl_bl_can_rx_fifo.write_ptr )
    {
        sfl_bl_can_receive( &mgl_bl_can_rx_fifo.buffer[mgl_bl_can_rx_fifo.read_ptr] );
        mgl_bl_can_rx_fifo.read_ptr++;
        if( mgl_bl_can_rx_fifo.read_ptr >= BL_CAN_RX_FIFO_BUFFER_LEN )
        {
            mgl_bl_can_rx_fifo.read_ptr = 0;
        }

    }

    // Handle Reset Request
    // give the system some time to send out the CAN Messages before the reset is done.
    (void)sfl_timer_time_elapsed( &do_reset_elapsed, mgl_ti_reset_delay, 100u, HAL_PRECISION_1MS );
    if( ext_flag_trigger_app_reset == E_RESET_TYPE_APP )
    {

        if(do_reset_elapsed == TRUE)
        {
            hal_sys_reset(HAL_SYS_RESET_TYPE_START_APP);
        }
    }
    else if( ext_flag_trigger_app_reset == E_RESET_TYPE_BL )
    {
        if(do_reset_elapsed == TRUE)
        {
            hal_sys_reset(HAL_SYS_RESET_TYPE_SOFTRESET_BL);
        }
    }
    else
    {
        (void)sfl_timer_set_timestamp(&mgl_ti_reset_delay, HAL_PRECISION_1MS);
    }

#elif HCS08_MRS_LIBRARY == 1
    if (ext_bl_access && os_time_past(mgl_ti_access, 10, OS_1000ms))
    {
        ext_bl_access = FALSE;
    }
#endif
}

/*----------------------------------------------------------------------------*/
/** 
* \internal
* \endinternal
*/
void sfl_bl_can_receive(struct_bl_can_frame* msg)
{
    uint8_t i,k;
    uint8_t rx_check_ok = FALSE;
    uint8_t ret=ERR_OK;
    struct_bl_can_frame tmp_msg;
    uint8_t exit = FALSE;

    // Length OK?
    if (msg->dlc > 0)
    {
        // Check all commands...
        for (i = 0u;((i < E_BL_MAX) && (exit == FALSE));i++)
        {
            // Identifier + MsgType OK and if necessary access granted?
            if (msg->id == ext_bl_sub_fr[bl_msg[i].d_rx_sub] && (i <= E_BL_ADDRESSING || ext_bl_access == TRUE))
            {
                // First 2 Data-Bytes match?
                if ((msg->data[0] == bl_msg[i].d_in1[0] && msg->data[1] == bl_msg[i].d_in1[1]) || bl_msg[i].d_rx_dlc == 0)
                {
                    // Check received message for serial number or data
                    rx_check_ok = TRUE;
                    if (bl_msg[i].d_check == E_CHECK_SN)
                    {
                        if (msg->data[2] != (uint8_t)(mgl_bl_sn>>24) || msg->data[3] != (uint8_t)(mgl_bl_sn>>16) || msg->data[4] != (uint8_t)(mgl_bl_sn>>8) || msg->data[5] != (uint8_t)mgl_bl_sn)
                        {
                            rx_check_ok = FALSE;
                            // if the serial number doesn't match, the bl access flag must be cleared
                            ext_bl_access = FALSE;
                        }
                    }
                    else if (bl_msg[i].d_check == E_CHECK_DATA)
                    {
                        for (k=0;k<(bl_msg[i].d_rx_dlc-2);k++)
                        {
                            if (bl_msg[i].d_in2[k] != msg->data[2+k])
                            {
                                rx_check_ok = FALSE;
                            }
                        }
                    }
                    
                    else
                    {
                        //do nothing
                    }

                    // Received message OK? Continue and send answer.
                    if (rx_check_ok == TRUE)
                    {
                        // Re-Trigger 10s timeout access for this module
                        TRIGGER_BL_ACCESS();
                        
                        // Callback function?
                        if (bl_msg[i].d_ptr_callback != NULL_PTR)
                        {
                            ret = bl_msg[i].d_ptr_callback(i, msg, &tmp_msg);
                        }
                        
                        // Address-Command? Grant access to more commands
#ifdef BOOTLOADER_FLASH_ACTIVE
                        if (i == E_BL_ADDRESSING || i == E_BL_FLASH_REQUEST || i == E_BL_FLASH_CLEAR_APPL || i == E_BL_FLASH_WRITE  )
#else
                        if (i == E_BL_ADDRESSING || i == E_BL_FLASH_REQUEST)
#endif
                        {
                            ext_bl_access = TRUE;
                        }
                        
                        // Send specific response?
                        if (bl_msg[i].d_send == E_SEND_SPECIFIC)
                        {
                            (void)sfl_bl_can_send(tmp_msg.id, tmp_msg.dlc, tmp_msg.data[0], tmp_msg.data[1], tmp_msg.data[2], tmp_msg.data[3], tmp_msg.data[4], tmp_msg.data[5], tmp_msg.data[6], tmp_msg.data[7]);
                        }
                        
                        // Successful? => Send response
                        else if (ret == ERR_OK)
                        {
                            if (bl_msg[i].d_send == E_SEND_SN_SCAN)
                            {
                                (void)sfl_bl_can_send(ext_bl_sub_fr[bl_msg[i].d_tx_sub], bl_msg[i].d_tx_dlc, 0, (uint8_t)(mgl_bl_sn>>24), (uint8_t)(mgl_bl_sn>>16), (uint8_t)(mgl_bl_sn>>8), (uint8_t)mgl_bl_sn, mgl_prog_state, mgl_bl_id_nibble, mgl_bl_version);
                            }
                            else if (bl_msg[i].d_send == E_SEND_SN_DATA)
                            {
                                (void)sfl_bl_can_send(ext_bl_sub_fr[bl_msg[i].d_tx_sub], bl_msg[i].d_tx_dlc, bl_msg[i].d_send_data[0], bl_msg[i].d_send_data[1], (uint8_t)(mgl_bl_sn>>24), (uint8_t)(mgl_bl_sn>>16), (uint8_t)(mgl_bl_sn>>8), (uint8_t)(mgl_bl_sn), 0, mgl_bl_version);
                            }
                            else if (bl_msg[i].d_send == E_SEND_DATA)
                            {
                                (void)sfl_bl_can_send(ext_bl_sub_fr[bl_msg[i].d_tx_sub], bl_msg[i].d_tx_dlc, bl_msg[i].d_send_data[0], bl_msg[i].d_send_data[1], bl_msg[i].d_send_data[2], bl_msg[i].d_send_data[3], bl_msg[i].d_send_data[4], bl_msg[i].d_send_data[5], bl_msg[i].d_send_data[6], bl_msg[i].d_send_data[7]);
                            }
                            else
                            {
                                //do nothing
                            }

                            // After setting PROG_STOP goto bootloader?
                            if (i == E_BL_PROG_STOP_MASTER || i == E_BL_PROG_STOP_USER)
                            {
                                //TODO: include RESET here
                                //sfl_timer_wait(50,PRECISION_1MS);    // Short delay for sending above response message
                                //RESET();        // Proceed reset and stay in bootloader
                                ext_flag_trigger_app_reset = E_RESET_TYPE_BL;
                            }

                            if (i == E_BL_PROG_START_MASTER || i == E_BL_PROG_START_USER)
                            {
                                //TODO: include RESET here
                                //sfl_timer_wait(50,PRECISION_1MS);    // Short delay for sending above response message
                                //RESET();        // Proceed reset and stay in bootloader
                                ext_flag_trigger_app_reset = E_RESET_TYPE_APP;
                            }
                        }
                        else
                        {
                            //do nothing
                        }
                    }
                    exit = TRUE;
                }
            }
        }
    }
}

#if USE_HAL == 1

    /*----------------------------------------------------------------------------*/
    /**
    * \internal
    * \endinternal
    */
    void sfl_bl_can_receive_pre( struct_hal_can_frame* ptr_can_msg )
    {
        struct_bl_can_frame l_can_msg;
        uint8_t i;

        // Transfer CAN frame format from hal to bl msg format
        l_can_msg.id = ptr_can_msg->can_id;
        l_can_msg.dlc = ptr_can_msg->can_dlc;
        // allow only the bootloader relevant messages
        if((l_can_msg.id >= ext_bl_sub_fr[E_SUB_BL_ADDR]) && (l_can_msg.id <= ext_bl_sub_fr[E_SUB_MAX-1]))
        {
            for( i = 0; i < 8; i++ )
            {
                l_can_msg.data[i] = ptr_can_msg->data[i];
            }

            // Store the BL can frames in a FIFO
            mgl_bl_can_rx_fifo.buffer[mgl_bl_can_rx_fifo.write_ptr] = l_can_msg;

            mgl_bl_can_rx_fifo.write_ptr++;
            if(mgl_bl_can_rx_fifo.write_ptr >= BL_CAN_RX_FIFO_BUFFER_LEN)
            {
                mgl_bl_can_rx_fifo.write_ptr = 0;
            }
            if(mgl_bl_can_rx_fifo.write_ptr == mgl_bl_can_rx_fifo.read_ptr)
            {
                mgl_bl_can_rx_fifo.read_ptr++;
                if(mgl_bl_can_rx_fifo.read_ptr >= BL_CAN_RX_FIFO_BUFFER_LEN)
                {
                    mgl_bl_can_rx_fifo.read_ptr = 0;
                }
                mgl_bl_can_rx_fifo.overrun++;
            }
        }
    }

#elif HCS08_MRS_LIBRARY == 1

    /*----------------------------------------------------------------------------*/
    /**
    * \internal
    * implemenation of function sfl_bl_can_receive_pre to prepare received can data for HCS12XE
    * \endinternal
    */
    void sfl_bl_can_receive_pre(bios_can_msg_typ* msg)
    {
        struct_bl_can_frame l_can_msg;
        uint8_t i;

        l_can_msg.id = msg->id | ((uint32_t)msg->id_ext << 31);
        l_can_msg.dlc = msg->len;

        for (i=0;i<8;i++)
        {
            l_can_msg.data[i] = msg->data[i];
        }

        sfl_bl_can_receive(&l_can_msg);
    }

#endif

/*----------------------------------------------------------------------------*/
/**
 * \internal
 * \endinternal
 */
uint8_t sfl_bl_set_flasher_state( uint8_t p_step, struct_bl_can_frame* p_msg_in, struct_bl_can_frame* p_msg_out )
{
    uint8_t ret = ERR_OK;
    (void) p_msg_in;

    ext_bl_flasher_active = 1u;
#ifdef BOOTLOADER_FLASH_ACTIVE
    // No special Message needed
    (void) p_msg_out;
#else
    // Prepare the special Application to Bootloader Message
    p_msg_out->id = ext_bl_sub_fr[bl_msg[p_step].d_tx_sub];
    p_msg_out->dlc = bl_msg[p_step].d_tx_dlc;
    p_msg_out->data[0] = bl_msg[p_step].d_send_data[0];
    p_msg_out->data[1] = bl_msg[p_step].d_send_data[1];
    p_msg_out->data[2] = (uint8_t)(mgl_bl_sn>>24);
    p_msg_out->data[3] = (uint8_t)(mgl_bl_sn>>16);
    p_msg_out->data[4] = (uint8_t)(mgl_bl_sn>>8);
    p_msg_out->data[5] = (uint8_t)(mgl_bl_sn);
    p_msg_out->data[6] = 0;
    p_msg_out->data[7] = 0;

    // Proceed reset and stay in bootloader
    ext_flag_trigger_app_reset = E_RESET_TYPE_BL;
#endif
    return ret;
}


/*----------------------------------------------------------------------------*/
/**
* \internal
* \endinternal
*
* \test STATUS: *** TESTED ***
*/
void sfl_bl_change_can_baudrate(struct_hal_can_handle* handle, enum_bl_baudrate_t baudrate)
{
    enum_HAL_CAN_BAUDRATE baudrate_hal = HAL_CAN_BAUD_125;
    enum_HAL_CAN_RETURN_VALUE ret = HAL_CAN_OK;
    switch(baudrate)
    {
        case E_BAUD_1MBIT:
            baudrate_hal = HAL_CAN_BAUD_1000;
            break;

        case E_BAUD_800KBIT:
            baudrate_hal = HAL_CAN_BAUD_INVALID;
            break;

        case E_BAUD_500KBIT:
            baudrate_hal = HAL_CAN_BAUD_500;
            break;

        case E_BAUD_250KBIT:
            baudrate_hal = HAL_CAN_BAUD_250;
            break;

        case E_BAUD_125KBIT:
            baudrate_hal = HAL_CAN_BAUD_125;
            break;

        case E_BAUD_50KBIT:
            baudrate_hal = HAL_CAN_BAUD_50;
            break;

        case E_BAUD_20KBIT:
            baudrate_hal = HAL_CAN_BAUD_20;
            break;

        case E_BAUD_10KBIT:
            baudrate_hal = HAL_CAN_BAUD_10;
            break;

        case E_BAUD_100KBIT:
            baudrate_hal = HAL_CAN_BAUD_100;
            break;

        case E_BAUD_83KBIT:
            baudrate_hal = HAL_CAN_BAUD_83;
            break;

        case E_BAUD_33KBIT:
            baudrate_hal = HAL_CAN_BAUD_33;
            break;

        default:
            baudrate_hal = HAL_CAN_BAUD_125;
            break;
    }

    ret = hal_can_set_baudrate(handle, baudrate_hal);

    // set a default baudrate in case an invalid baudrate was selected
    if(ret == HAL_CAN_ERROR_BAUDRATE)
    {
        ret = hal_can_set_baudrate(handle, HAL_CAN_BAUD_125);
    }
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 * \endinternal
 */
void sfl_bl_get_hw_version(uint8_t *hw_version)
{
    (void)BL_EEPROM_GET_DATA(ext_bl_par_addr[PAR_HW_VERSION], hw_version, ext_bl_par_data[PAR_HW_VERSION].length);
}


/** \} */
