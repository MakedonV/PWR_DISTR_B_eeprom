/*----------------------------------------------------------------------------*/
/**
* \file         bl_ee_helper.h
* \brief        Implementation of EEPROM read and write functions.
*
* \date         20180509
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2018
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	bl_ee_helper
* \brief 		Contains helper routines for EEPROM management.
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_ROLE_PROTOCOL_ROLE_PROTOCOL_EE_H_
#define SRC_ROLE_PROTOCOL_ROLE_PROTOCOL_EE_H_

// Starting from v12 the EEPROM structure is made dependent
// from extended CAN baud structures defined in role_phy_can.h
// meaning that the BL protocol whenever included should first
// include the ee_helper.h which on its turn will depend on role_phy_can.h.
// For this reason we automatically include role_phy_can.h here, although
// such practice of inclusion of one header file in another header file
// is discouraged.
#include "role_phy_can.h"

// ===================================================================================================
// Defines and Typedefs
// ===================================================================================================
//#if (!defined(ROLE_MINIMAL) && !defined(BL_UPDATE_APP)) || defined(STANDALONE_APP)
#if ( !defined(BL_UPDATE_APP) )

// EE_HELPER.H: COMMENT_1
// For the bootloader the EE_VERSION should hold the latest supported EEPROM version
// For the debugging purposes it is possible to deploy an older EEPROM structures (for the sake of
// testing with the application program which are supposed to work with the older BL versions or EEPROM structures)
// by setting EE_VERSION to smaller values.

// For standalone application we also define the EE_VERSION together with the BL protocol and bootloader version.
// This is in case if the EEPROM does not contain any previous records and for the standalone application
// to determine which versions of EEPROM, BL protocol to emulate.

// Never decrement EE_VERSION if you are not sure what you are doing.
// Never decrement for production build
#define EE_VERSION                   14u // Supports 0 (BL v3), 13 (BL v13) and 14 (BL v14+), (v1=v14 obsolete value)

#if (EE_VERSION < 14)
// For EE_VERSION < 14 only the BL protocol v0 is a valid choice.
#define C_BL_PROTOCOL_VERSION        0u

#else
// For the same reason given above the bootloader should, normally, define the latest available BL protocol version
// However, for debugging purposes it should be still possible to use older BL protocol versions.
// Never decrement C_BL_PROTOCOL_VERSION if you are not sure what you are doing
#define C_BL_PROTOCOL_VERSION        1u
#endif // EE_VERSION

#else

// The following is for BL updater application. Here we assume that ROLE_MINIMAL is not defined
#define EE_VERSION_LATEST            14u
#define C_BL_PROTOCOL_VERSION        1u

#endif // !BL_UPDATE_APP

#if !defined(ROLE_MINIMAL)
// Bootloader version
#define C_BL_VERSION                 ((uint16_t)26u)

#endif // !ROLE_MINIMAL


typedef struct
{
    uint8_t                  enable;
    struct_ROLE_CAN_EXT_BAUD baud;
    uint8_t                  crc8;
} __attribute__((packed)) struct_EE_CAN_EXT;

#if defined(ROLE_MINIMAL) || (defined(EE_VERSION) && (EE_VERSION < 14)) || defined(BL_UPDATE_APP)
struct struct_EEPROM_OLD	// HCS EEPROM structure
{
    uint16_t  eeprom_version;
    uint16_t  id;
    uint32_t  serial_number;
    uint8_t   part_number[12u];
    uint8_t   drawing_number[12u];
    uint8_t   name[20u];
    uint8_t   order_number[8u];
    uint8_t   test_date[8u];
    uint8_t   hw_version[2u];
    uint8_t   reset_counter;
    uint16_t  lib_version;
    uint8_t   reset_reason_lvd;
    uint8_t   reset_reason_loc;
    uint8_t   reset_reason_ilad;
    uint8_t   reset_reason_ilop;
    uint8_t   reset_reason_cop;
    uint8_t   mcu_type;
    uint8_t   hw_can_active;
    uint8_t   reserve1[3u];
    uint16_t  bl_version;
    uint16_t  prog_state;
    uint16_t  port_byte1;
    uint16_t  port_byte2;
    uint16_t  baudrate1;
    uint16_t  baudrate2;
    uint8_t   bl_id_ext1;
    uint32_t  bl_id1;
    uint8_t   bl_id_crc1;
    uint8_t   bl_id_ext2;
    uint32_t  bl_id2;
    uint8_t   bl_id_crc2;
    uint8_t   sw_version[20u];
    uint8_t   module_name[30u];
    uint8_t   bl_canbus;
    uint16_t  wd_timeout;
    uint16_t  startup_time;
    uint8_t   reserve2[7u];

    // dummy fields to make the program compile
    // not used
    uint8_t   bl_protocol_version;
	struct_EE_CAN_EXT can_ext_bt;

} __attribute__((packed));

typedef struct struct_EEPROM_OLD struct_EEPROM_OLD;

typedef struct
{
    uint16_t addr_old;
    uint16_t addr_new;
    uint8_t field_len_old;
    uint8_t field_len_new;
    uint_t def_val;
    uint8_t min_len;
    bool is_string;
} struct_EE_VERSION_MAPPING;
#endif // ROLE_MINIMAL || EE_VERSION < 14

#if defined(ROLE_MINIMAL) || (defined(EE_VERSION) && (0 == EE_VERSION)) || defined(BL_UPDATE_APP)
struct struct_EEPROM0	// used by BL v3
{
    uint16_t  eeprom_version;
    uint16_t  id;
    uint32_t  serial_number;
    uint32_t  device_number;
    uint8_t   part_number[20u];
    uint8_t   drawing_number[20u];
    uint8_t   name[20u];
    uint8_t   order_number[20u];
    uint8_t   test_date[12u];
    uint16_t  mcu_type;
    uint8_t   hw_version[2u];
    uint16_t  hw_can_active;
    uint16_t  bl_version;

    uint32_t  trace_code;
    uint8_t   factory_data_reserve[20u];
    uint16_t  reset_counter;
    uint8_t   reset_reason_lvd;
    uint8_t   reset_reason_loc;
    uint8_t   reset_reason_ilad;
    uint8_t   reset_reason_ilop;
    uint8_t   reset_reason_cop;
    uint8_t   reset_reason_reserved;

    uint16_t  lib_version;
    uint16_t  prog_state;
    uint8_t   module_data_reserve[20u];
    uint16_t  port_byte1;
    uint16_t  port_byte2;
    uint16_t  baudrate1;
    uint16_t  baudrate2;
    uint8_t   bl_id_ext1;
    uint32_t  bl_id1;
    uint8_t   bl_id_crc1;
    uint8_t   bl_id_ext2;
    uint32_t  bl_id2;
    uint8_t   bl_id_crc2;
    uint8_t   sw_version[20u];
    uint8_t   module_name[30u];
    uint8_t   bl_canbus;
    uint16_t  wd_timeout;
    uint8_t   config_data_reserve[20u];

    uint8_t   reserve1[3u];				///< This field seems to be obsolete but added for the sake of mapping
    uint16_t  startup_time;				///< This field seems to be obsolete but added for the sake of mapping
    uint8_t   reserve2[7u];				///< This field seems to be obsolete but added for the sake of mapping

    // dummy fields to make the program compile
    // not used
    uint8_t   bl_protocol_version;
    struct_EE_CAN_EXT can_ext_bt;

} __attribute__((packed));

typedef struct struct_EEPROM0 struct_EEPROM0;
#endif // ROLE_MINIMAL || 0 == EE_VERSION

#if defined(ROLE_MINIMAL) || (defined(EE_VERSION) && (13 == EE_VERSION)) || defined(BL_UPDATE_APP)
struct struct_EEPROM13
{
    uint16_t  eeprom_version;
    uint16_t  id;
    uint32_t  serial_number;
    uint32_t  device_number;
    uint8_t   part_number[20u];
    uint8_t   drawing_number[20u];
    uint8_t   name[20u];
    uint8_t   order_number[20u];
    uint8_t   test_date[12u];
    uint16_t  mcu_type;
    uint8_t   hw_version[2u];
    uint16_t  hw_can_active;
    uint16_t  bl_version;
    uint16_t  bl_protocol_version;

    uint32_t  trace_code;
    uint8_t   factory_data_reserve[20u];
    uint16_t  reset_counter;
    uint8_t   reset_reason_lvd;
    uint8_t   reset_reason_loc;
    uint8_t   reset_reason_ilad;
    uint8_t   reset_reason_ilop;
    uint8_t   reset_reason_cop;
    uint8_t   reset_reason_reserved;

    uint16_t  lib_version;
    uint16_t  prog_state;
    uint8_t   module_data_reserve[20u];
    uint16_t  port_byte1;
    uint16_t  port_byte2;
    uint16_t  baudrate1;
    uint16_t  baudrate2;
    uint8_t   bl_id_ext1;
    uint32_t  bl_id1;
    uint8_t   bl_id_crc1;
    uint8_t   bl_id_ext2;
    uint32_t  bl_id2;
    uint8_t   bl_id_crc2;
    uint8_t   sw_version[20u];
    uint8_t   module_name[30u];
    uint8_t   bl_canbus;
    uint16_t  wd_timeout;
    uint8_t   config_data_reserve[20u];

    uint8_t   factory_reset;

    // The following baudrate fields are
    // to enable the baudrate extended configuration
    // This allows to either bootloader or the application
    // to dynamically configure the CAN(FD) baudrate
    struct_EE_CAN_EXT can_ext_bt;

    uint8_t   reserve1[3u];				///< This field seems to be obsolete but added for the sake of mapping
    uint16_t  startup_time;				///< This field seems to be obsolete but added for the sake of mapping
    uint8_t   reserve2[7u];				///< This field seems to be obsolete but added for the sake of mapping
} __attribute__((packed));

typedef struct struct_EEPROM13 struct_EEPROM13;
#endif // ROLE_MINIMAL || 13 == EE_VERSION

#if defined(ROLE_MINIMAL) || (defined(EE_VERSION) && (14 == EE_VERSION)) || defined(BL_UPDATE_APP)

#define EE1_RO_LIST \
	    uint16_t  eeprom_version; \
	    uint16_t  id; \
	    uint8_t   bl_protocol_version; \
	    uint16_t  bl_version; \
	    uint32_t  trace_code; \
	    uint32_t  uidl; \
	    uint32_t  uidml; \
	    uint32_t  uidmh; \
	    uint32_t  uidh; \
	    uint32_t  serial_number; \
	    uint8_t   reserved_ro[8]; \
	    uint8_t   part_number[14]; \
	    uint8_t   drawing_number[30u]; \
	    uint8_t   name[40u]; \
	    uint8_t   hw_version[2u]; \
	    uint16_t  mcu_type; \
	    uint8_t   order_number[12u]; \
	    uint8_t   test_date[10u]; \
	    uint8_t   hw_can_active; \
	    uint8_t   factory_data_reserve[20u]

struct struct_EEPROM14_RO
{
	EE1_RO_LIST;
} __attribute__((packed));

struct struct_EEPROM14
{
	EE1_RO_LIST;

    uint16_t  reset_counter;
    uint8_t   reset_reason_lvd;
    uint8_t   reset_reason_loc;
    uint8_t   reset_reason_ilad;
    uint8_t   reset_reason_ilop;
    uint8_t   reset_reason_cop;
    uint8_t   reset_reason_reserved;
    uint16_t  prog_state;
    uint8_t   factory_reset;
    uint8_t   module_data_reserve[20u];

    uint16_t  wd_timeout;
    uint8_t   bl_canbus;
    uint16_t  port_byte1;
    uint16_t  port_byte2;
    uint16_t  baudrate1;
    uint16_t  baudrate2;
    uint8_t   bl_id_ext1;
    uint32_t  bl_id1;
    uint8_t   bl_id_crc1;
    uint8_t   bl_id_ext2;
    uint32_t  bl_id2;
    uint8_t   bl_id_crc2;
    uint8_t   sw_version[20u];
    uint8_t   module_name[30u];
    uint16_t  startup_time;
    uint8_t   config_data_reserve[20u];

    // The following baudrate fields are
    // to enable the baudrate extended configuration
    // This allows to either bootloader or the application
    // to dynamically configure the CAN(FD) baudrate
    struct_EE_CAN_EXT can_ext_bt;
} __attribute__((packed));

typedef struct struct_EEPROM14_RO struct_EEPROM14_RO;
typedef struct struct_EEPROM14 struct_EEPROM14;

typedef struct
{
    uint16_t addr;
    uint8_t field_len;
    uint_t def_val;
    uint8_t min_len;
    bool is_string;
} struct_EE_MEMB_DEFAULT;

#elif defined(EE_VERSION) && (EE_VERSION > 14)
#error "Unsupported EEPROM version. The new EEPROM structure needs to be present."

#endif // ROLE_MINIMAL || (14 == EE_VERSION)

struct struct_PROGRAM_ONCE
{
    uint8_t ee_addr_format;
} __attribute__((packed));
typedef struct struct_PROGRAM_ONCE struct_PROGRAM_ONCE;


typedef struct
{
    uint16_t addr;
    uint16_t size;
} struct_EE_FIELD_INFO;

#if defined(BL_UPDATE_APP)
// The following structure is used to port data from old to new EEPROM
typedef struct
{
    uint16_t addr_old;
    uint16_t addr_new;
    uint8_t  len_old;
    uint8_t  len_new;
} struct_EE_DATA_PORTING;
#endif // BL_UPDATE_APP

enum E_MCU_TYPE
{
    E_MCU_TYPE_NONE = 0,
    E_MCU_TYPE_HCS08,
    E_MCU_TYPE_HCS12,
    E_MCU_TYPE_HCS08DZ128,
    E_MCU_TYPE_MK10xx,
    E_MCU_TYPE_RH850,
    E_MCU_TYPE_S32K144,
    E_MCU_TYPE_SKEAZ64AMLH,
    E_MCU_TYPE_S32K148,
    E_MCU_TYPE_MKE18F
};

typedef enum
{
    EEPROM_ADDRESSING_OLD,
    EEPROM_ADDRESSING_NEW,
} enum_EEPROM_ADDRESSING;

typedef enum
{
    PROGRAM_ONCE_READ,
    PROGRAM_ONCE_WRITE
} enum_PROGRAM_ONCE_OP;

// ===================================================================================================
// Macros
// ===================================================================================================
#define SWAP8(x)    ((uint8_t) x)
#define SWAP16(x)   ((uint16_t) ((((uint16_t)x) >>  8) & 0xFFu) | ((((uint16_t)x) << 8) & 0xFF00u))
#define SWAP32(x)   ((uint32_t) ((((uint32_t)x) >> 24) & 0xFFu) | ((((uint32_t)x) << 8) & 0xFF0000u) \
                    | ((((uint32_t)x) >> 8) & 0xFF00u) | ((((uint32_t)x) << 24) & 0xFF000000u))


// ===================================================================================================
// EEPROM specific
// ===================================================================================================
#if !defined(ROLE_MINIMAL) && !defined(BL_UPDATE_APP) // Bootloader

#if (0 == EE_VERSION)
#define EE_MEMB_ADDR_NEW(m) 	offsetof(struct_EEPROM0, m)
#define EE_MEMB_SIZE_NEW(m)		SIZEOF_MEMBER(struct_EEPROM0, m)
#define EE0_MEMB_MAP(m, val)	EE_MEMB_MAP(m, val)

#elif (13 == EE_VERSION)
#define EE_MEMB_ADDR_NEW(m) 	offsetof(struct_EEPROM13, m)
#define EE_MEMB_SIZE_NEW(m)		SIZEOF_MEMBER(struct_EEPROM13, m)
#define EE13_MEMB_MAP(m, val)	EE_MEMB_MAP(m, val)

#endif // 0 == EE_VERSION, 13 == EE_VERSION

#if (0 == EE_VERSION || 13 == EE_VERSION)

#define EE_MEMB_ADDR_OLD(m) 	offsetof(struct_EEPROM_OLD, m)
#define EE_MEMB_SIZE_OLD(m)		SIZEOF_MEMBER(struct_EEPROM_OLD, m)

#define EE_MEMB_ADDR_BOTH(m)    ( ( ((uint32_t)EE_MEMB_ADDR_OLD(m)) << 16 ) | EE_MEMB_ADDR_NEW(m) )
#define EE_MEMB_SIZE_BOTH(m)    ( ( ((uint32_t)EE_MEMB_SIZE_OLD(m)) << 16 ) | EE_MEMB_SIZE_NEW(m) )

#define EE_MEMB_ADDR(m)         ( bl_ee_working_addr(EE_MEMB_ADDR_BOTH(m)) )

#define EE_READ_PTR(m) \
        ee_read(EE_MEMB_ADDR_BOTH(m))

#define EE_WRITE(m, d) \
        ee_write(EE_MEMB_ADDR_BOTH(m), EE_MEMB_SIZE_BOTH(m), d)

#define EE_WRITE_MAX32(m, d) \
        ee_write_max32(EE_MEMB_ADDR_BOTH(m), EE_MEMB_SIZE_BOTH(m), d)

#define EE_WRITE_DEFAULT(m) \
        ee_set_defaults(EE_MEMB_ADDR_BOTH(m))

#define EE_MEMB_MAP(m, val)         {EE_MEMB_ADDR_OLD(m), EE_MEMB_ADDR_NEW(m), EE_MEMB_SIZE_OLD(m), EE_MEMB_SIZE_NEW(m), \
                                    (uint_t)val, MIN(EE_MEMB_SIZE_OLD(m), sizeof(val)), \
                                    _Generic((val), char* : TRUE, default: FALSE) }

#elif (14 == EE_VERSION)

#define EE_MEMB_ADDR(m)				offsetof(struct_EEPROM14, m)
#define EE_MEMB_SIZE(m)				SIZEOF_MEMBER(struct_EEPROM14, m)

#define EE_READ_PTR(m) \
        ee_read(EE_MEMB_ADDR(m))

#define EE_WRITE(m, d) \
        ee_write(EE_MEMB_ADDR(m), EE_MEMB_SIZE(m), d)

#define EE_WRITE_MAX32(m, d) \
        ee_write_max32(EE_MEMB_ADDR(m), EE_MEMB_SIZE(m), d)

#define EE_WRITE_DEFAULT(m) \
        ee_set_defaults(EE_MEMB_ADDR(m))

#define EE_MEMB(m, val)             {EE_MEMB_ADDR(m), EE_MEMB_SIZE(m), \
                                    (uint_t)val, MIN(EE_MEMB_SIZE(m), sizeof(val)), \
                                    _Generic((val), char* : TRUE, default: FALSE) }

#define EE14_MEMB(m, val)			EE_MEMB(m, val)

#else
#error "Missing EE_ macros for the defined EE_VERSION"
#endif // EE_VERSION

#else	// Application
		// Since in Application we determine the EEPROM version at runtime we need to
		// have some possibility to select the correct addresses of the EEPROM fields.
		// So the above EE_READ, EE_WRITE, etc. macros will be more complicated

//////////////////////////////////////////////////////////////////////
#define EE_MEMB_ADDR_OLD(m) 	offsetof(struct_EEPROM_OLD, m)
#define EE_MEMB_SIZE_OLD(m)		SIZEOF_MEMBER(struct_EEPROM_OLD, m)

#define EE0_MEMB_ADDR_NEW(m)    offsetof(struct_EEPROM0, m)
#define EE0_MEMB_SIZE_NEW(m)	SIZEOF_MEMBER(struct_EEPROM0, m)

#define EE0_MEMB_ADDR_BOTH(m)   ( ( ((uint32_t)EE_MEMB_ADDR_OLD(m)) << 16 ) | EE0_MEMB_ADDR_NEW(m) )
#define EE0_MEMB_SIZE_BOTH(m)   ( ( ((uint32_t)EE_MEMB_SIZE_OLD(m)) << 16 ) | EE0_MEMB_SIZE_NEW(m) )

#define EE0_MEMB_ADDR(m)         ( bl_ee_working_addr(EE0_MEMB_ADDR_BOTH(m)) )

#define EE0_READ_PTR(m) \
        ee_read(EE0_MEMB_ADDR_BOTH(m))

#define EE0_WRITE(m, d) \
        ee_write(EE0_MEMB_ADDR_BOTH(m), EE0_MEMB_SIZE_BOTH(m), d)

#define EE0_WRITE_MAX32(m, d) \
        ee_write_max32(EE0_MEMB_ADDR_BOTH(m), EE0_MEMB_SIZE_BOTH(m), d)

#define EE0_WRITE_DEFAULT(m) \
        ee_set_defaults(EE0_MEMB_ADDR_BOTH(m))

#define EE0_MEMB_MAP(m, val)        {EE_MEMB_ADDR_OLD(m), EE0_MEMB_ADDR_NEW(m), EE_MEMB_SIZE_OLD(m), EE0_MEMB_SIZE_NEW(m), \
                                    (uint_t)val, MIN(EE_MEMB_SIZE_OLD(m), sizeof(val)), \
                                    _Generic((val), char* : TRUE, default: FALSE) }

//////////////////////////////////////////////////////////////////////

#define EE13_MEMB_ADDR_NEW(m) 	 offsetof(struct_EEPROM13, m)
#define EE13_MEMB_SIZE_NEW(m)  	 SIZEOF_MEMBER(struct_EEPROM13, m)

#define EE13_MEMB_ADDR_BOTH(m)   ( ( ((uint32_t)EE_MEMB_ADDR_OLD(m)) << 16 ) | EE13_MEMB_ADDR_NEW(m) )
#define EE13_MEMB_SIZE_BOTH(m)   ( ( ((uint32_t)EE_MEMB_SIZE_OLD(m)) << 16 ) | EE13_MEMB_SIZE_NEW(m) )

#define EE13_MEMB_ADDR(m)         ( bl_ee_working_addr(EE13_MEMB_ADDR_BOTH(m)) )

#define EE13_READ_PTR(m) \
        ee_read(EE13_MEMB_ADDR_BOTH(m))

#define EE13_WRITE(m, d) \
        ee_write(EE13_MEMB_ADDR_BOTH(m), EE13_MEMB_SIZE_BOTH(m), d)

#define EE13_WRITE_MAX32(m, d) \
        ee_write_max32(EE13_MEMB_ADDR_BOTH(m), EE13_MEMB_SIZE_BOTH(m), d)

#define EE13_WRITE_DEFAULT(m) \
        ee_set_defaults(EE13_MEMB_ADDR_BOTH(m))

#define EE13_MEMB_MAP(m, val)       {EE_MEMB_ADDR_OLD(m), EE13_MEMB_ADDR_NEW(m), EE_MEMB_SIZE_OLD(m), EE13_MEMB_SIZE_NEW(m), \
                                    (uint_t)val, MIN(EE_MEMB_SIZE_OLD(m), sizeof(val)), \
                                    _Generic((val), char* : TRUE, default: FALSE) }

//////////////////////////////////////////////////////////////////////
#define EE14_MEMB_ADDR(m)				offsetof(struct_EEPROM14, m)
#define EE14_MEMB_SIZE(m)				SIZEOF_MEMBER(struct_EEPROM14, m)

#define EE14_READ_PTR(m) \
        ee_read(EE14_MEMB_ADDR(m))

#define EE14_WRITE(m, d) \
        ee_write(EE14_MEMB_ADDR(m), EE14_MEMB_SIZE(m), d)

#define EE14_WRITE_MAX32(m, d) \
        ee_write_max32(EE14_MEMB_ADDR(m), EE14_MEMB_SIZE(m), d)

#define EE14_WRITE_DEFAULT(m) \
        ee_set_defaults(EE14_MEMB_ADDR(m))

#define EE14_MEMB(m, val)           {EE14_MEMB_ADDR(m), EE14_MEMB_SIZE(m), \
                                    (uint_t)val, MIN(EE14_MEMB_SIZE(m), sizeof(val)), \
                                    _Generic((val), char* : TRUE, default: FALSE) }

//////////////////////////////////////////////////////////////////////

#define EE_MEMB_ADDR(m) \
        (  0 == ee_version_get() ? EE0_MEMB_ADDR(m)  : \
        ( 13 == ee_version_get() ? EE13_MEMB_ADDR(m) : \
		(  1 == ee_version_get() ? EE14_MEMB_ADDR(m) : \
        ( 14 == ee_version_get() ? EE14_MEMB_ADDR(m) : \
        						   ASSERT_FORCE \
		  ) ) ) )

#define EE_MEMB_SIZE(m) \
        (  0 == ee_version_get() ? EE0_MEMB_SIZE_NEW(m)  : \
        ( 13 == ee_version_get() ? EE13_MEMB_SIZE_NEW(m) : \
        (  1 == ee_version_get() ? EE14_MEMB_SIZE(m)     : \
        ( 14 == ee_version_get() ? EE14_MEMB_SIZE(m)     : \
        						   ASSERT_FORCE \
		  ) ) ) )

#define EE_READ_PTR(m) \
        (  0 == ee_version_get() ? EE0_READ_PTR(m)  : \
        ( 13 == ee_version_get() ? EE13_READ_PTR(m) : \
		(  1 == ee_version_get() ? EE14_READ_PTR(m) : \
        ( 14 == ee_version_get() ? EE14_READ_PTR(m) : \
        						   (uint8_t*)ASSERT_FORCE \
		  ) ) ) )

#define EE_WRITE(m, d) \
        (  0 == ee_version_get() ? EE0_WRITE(m, d)  : \
        ( 13 == ee_version_get() ? EE13_WRITE(m, d) : \
		(  1 == ee_version_get() ? EE14_WRITE(m, d) : \
        ( 14 == ee_version_get() ? EE14_WRITE(m, d) : \
        						   ASSERT_FORCE \
		  ) ) ) )

#define EE_WRITE_MAX32(m, d) \
        (  0 == ee_version_get() ? EE0_WRITE_MAX32(m, d)  : \
        ( 13 == ee_version_get() ? EE13_WRITE_MAX32(m, d) : \
		(  1 == ee_version_get() ? EE14_WRITE_MAX32(m, d) : \
        ( 14 == ee_version_get() ? EE14_WRITE_MAX32(m, d) : \
        						   ASSERT_FORCE \
		  ) ) ) )

#define EE_WRITE_DEFAULT(m) \
        (  0 == ee_version_get() ? EE0_WRITE_DEFAULT(m)  : \
        ( 13 == ee_version_get() ? EE13_WRITE_DEFAULT(m) : \
		(  1 == ee_version_get() ? EE14_WRITE_DEFAULT(m) : \
        ( 14 == ee_version_get() ? EE14_WRITE_DEFAULT(m) : \
        						   ASSERT_FORCE \
		  ) ) ) )

//////////////////////////////////////////////////////////////////////
#if defined(BL_UPDATE_APP)
#define EE0_TO_EE14_DATA_PORT(m)       {EE0_MEMB_ADDR_NEW(m), EE14_MEMB_ADDR(m), \
										EE0_MEMB_SIZE_NEW(m), EE14_MEMB_SIZE(m)}

#define EE13_TO_EE14_DATA_PORT(m)      {EE13_MEMB_ADDR_NEW(m), EE14_MEMB_ADDR(m), \
										EE13_MEMB_SIZE_NEW(m), EE14_MEMB_SIZE(m)}

#endif // BL_UPDATE_APP
//////////////////////////////////////////////////////////////////////

#endif // !ROLE_MINIMAL

// ===================================================================================================
// Miscellaneous defines
// ===================================================================================================
#define LITTLE_ENDIAN       0
#define BIG_ENDIAN          1

#define EE_ARCH_FORMAT      LITTLE_ENDIAN
#define EE_IO_FORMAT        BIG_ENDIAN

#define EE_ALL_DEFAULTS     ((uint32_t)-1)

#define BL_PROTOCOL_CAN_ID  ((uint32_t)0x1FFFFFF0u)
#define C_BL_ID             ((uint16_t)2332u) // For BL v14 and higher
#define C_BL_ID_PREV        ((uint16_t)1331u) // Before BL v14

#define C_BL_ID_EXT         (1u)
#define C_BL_ID_NIBBLE      (0u)

//#define C_BL_ID_CNT         (EE_MEMB_SIZE(bl_id_ext1) + EE_MEMB_SIZE(bl_id1) + EE_MEMB_SIZE(bl_id_crc1))
// The following is slightly dangerous, but as far as the fields in
// old and new formats are of the same size this should be fine.
#if !defined(ROLE_MINIMAL) && !defined(BL_UPDATE_APP)

#if (0 == EE_VERSION || 13 == EE_VERSION)
#define C_BL_ID_CNT         (EE_MEMB_SIZE_NEW(bl_id_ext1) + EE_MEMB_SIZE_NEW(bl_id1) + EE_MEMB_SIZE_NEW(bl_id_crc1))

#else
#define C_BL_ID_CNT         (EE_MEMB_SIZE(bl_id_ext1) + EE_MEMB_SIZE(bl_id1) + EE_MEMB_SIZE(bl_id_crc1))
#endif // EE_VERSION

#else

#define C_BL_ID_CNT         (EE_MEMB_SIZE(bl_id_ext1) + EE_MEMB_SIZE(bl_id1) + EE_MEMB_SIZE(bl_id_crc1))

#endif // !ROLE_MINIMAL

#define INV_16(x)           ((uint16_t)(( ((uint16_t)(~x)) << 8) + (x & 0xFF)))
#define INV_8(x)            ((uint8_t) (( ((uint8_t)(~x)) << 4) + (x & 0x0F)))
#define CHECK_INV_8(x)      (((x) & 0x0Fu) == (~((x) >> 4) & 0x0Fu))    ///< Check if high part is the inverse of the low part

#if EE_ARCH_FORMAT != EE_IO_FORMAT

#define SWAP(x)             _Generic((x),   uint16_t: SWAP16(x), \
                                            uint32_t: SWAP32(x), \
                                            int16_t:  SWAP16(x), \
                                            int32_t:  SWAP32(x), \
                                            default:  SWAP8(x))

#else

#define SWAP(x)             (x)

#endif // EE_ARCH_FORMAT != EE_IO_FORMAT

// Macro: Merge high-low-nibble for EEPROM ID_EXT
#define NIBBLE_H_L(a,b)     ((a) | (((b) << 4) & 0xFFu))

// Macro: Calculate CRC of bootloader id
#define CALC_CRC(a,b,x)     ((~((((x) >> 24) & 0xFFu)+(((x) >> 16u) & 0xFFu) + \
                                (((x) >> 8u) & 0xFFu) + ((x) & 0xFFu) + \
                                NIBBLE_H_L((a),(b)))) & 0xFFu)

#if defined(CPU_S32K148)
#define ROLE_BL_PROTOCOL_MCU_TYPE            ((uint8_t)E_MCU_TYPE_S32K148)

//#elif defined(CPU_S32K146)

#elif (defined(CPU_S32K144HFT0VLLT) || defined(CPU_S32K144LFT0MLLT))
#define ROLE_BL_PROTOCOL_MCU_TYPE            ((uint8_t)E_MCU_TYPE_S32K144)

#elif defined(CPU_MK10DN512VLL10)
#define ROLE_BL_PROTOCOL_MCU_TYPE            ((uint8_t)E_MCU_TYPE_MK10xx)

#else
#error "MCU type unknown. Specify the MCU type via SYSTEM_CPU in the makefile."
#endif

#define EE_BAD_ADDRESS      ((uint16_t)-1)

/**
 * Running mode for minimal (application) setup
 * For non-minimal setup (bootloader) the minimal mode parameter is ignored
 */
//#define ROLE_MODE_MINIMAL_STANDARD
//#define ROLE_MODE_MINIMAL_EXTENDED
#define BL_APP_MODE_APP							((uint32_t)0u)
#define BL_APP_MODE_STANDALONE					((uint32_t)1u)

// ===================================================================================================
// Public function prototypes
// ===================================================================================================

/*----------------------------------------------------------------------------*/
/**
* \ingroup		bl_ee_helper
* \brief		Checks the matching of EEPROM id (Startkenner) with the predefined constant(s)
*
* \details
*
* \pre
*
* \return bool								True if match, False otherwise
*/
bool ee_is_id_match(uint32_t app_mode);

#if defined(BL_UPDATE_APP)
/*----------------------------------------------------------------------------*/
/**
* \ingroup		bl_ee_helper
* \brief		Resets the EEPROM version so that the ee_version_get function can be entered again.
*
*
* \details
*
* \pre
*/
void ee_version_reset();
#endif // BL_UPDATE_APP

/*----------------------------------------------------------------------------*/
/**
* \ingroup		bl_ee_helper
* \brief		Returns the EEPROM version which is being used in the bootloader or application.
*
* \details
*
* \pre
*
* \return uint16_t							EEPROM version number
*/
uint16_t ee_version_get();

/*----------------------------------------------------------------------------*/
/**
* \ingroup		bl_ee_helper
* \brief		Checks if EEPROM address mapping is used
*
* \details
*
* \pre
*
* \return bool							true if mapping is used, otherwise false
*/
bool ee_is_address_mapping_used();

/*----------------------------------------------------------------------------*/
/**
* \ingroup		bl_ee_helper
* \brief		Returns a pointer to a data stored in EEPROM memory upon providing an offset
*  				in EEPROM memory structure (data member offset).
*
* \details
*
* \pre
*
* \param offset [in]		uint16_t		offset of data member of EEPROM structure
* \return uint8_t*							pointer to EEPROM data
*/
uint8_t* ee_read(uint32_t offset);

/*----------------------------------------------------------------------------*/
/**
* \ingroup		bl_ee_helper
* \brief		Returns a pointer to a data stored in EEPROM memory upon providing an offset
*  				in EEPROM memory structure (data member offset).
*  				This differs from ee_read function in that it prefers to first
*  				read the value from the backup flash memory. When this attemp is invalid
*  				it calls the normal ee_read function.
*
* \details
*
* \pre
*
* \param offset [in]		uint16_t		offset of data member of EEPROM structure
* \return uint8_t*							pointer to Flash or EEPROM data
*/
#if 0
uint8_t* ee_read_backup(uint32_t offset);
#endif // 0

/*----------------------------------------------------------------------------*/
/**
* \ingroup		bl_ee_helper
* \brief		Writes data pointer by ptr_val of to an EEPROM memory at a requested offset.
*
* \details
*
* \pre
*
* \param offset [in]		uint16_t		offset of data member of EEPROM structure
* \param size [in]			uint16_t		size in bytes of data to write to EEPROM
* \param ptr_val [in]		uint8_t*		pointer to data to write
* \return enum_CODE						CODE_OK - on success, error code otherwise
*/
enum_CODE ee_write(uint32_t offset, uint32_t size, const uint8_t *ptr_val);

/*----------------------------------------------------------------------------*/
/**
* \ingroup		bl_ee_helper
* \brief		This is a wrapper function to make writing of integer values into EEPROM
*  				easy. In most of the cases this function is used indirectly via EE_WRITE_MAX32
*  				macro.
*
* \details
*
* \pre
*
* \param offset [in]		uint16_t		offset of data member of EEPROM structure
* \param size [in]			uint16_t		size in bytes of data to write to EEPROM
* \param val [in]		    uint32_t		value to write
* \return enum_CODE						CODE_OK - on success, error code otherwise
*/
enum_CODE ee_write_max32(uint32_t offset, uint32_t size, uint32_t val);

/*----------------------------------------------------------------------------*/
/**
* \ingroup		bl_ee_helper
* \brief		Sets the default values of either all members (when EE_ALL_DEFAULTS is specified)
*  				of EEPROM structure or to the requested member via its offset (in older EEPROM
*  				format). The data member is found via iteration over mgl_ee_map to find the
* 				member whose address matches the provided offset.
*
* \details
*
* \pre
*
* \param offset [in]		uint16_t		offset of data member of EEPROM structure
* \return enum_CODE                         CODE_OK - on success, error code otherwise
*/
enum_CODE ee_set_defaults(uint32_t offset);

#if defined(BL_UPDATE_APP)
enum_CODE ee_port_data();
#endif // BL_UPDATE_APP

#if 0
/*----------------------------------------------------------------------------*/
/**
* \ingroup      bl_ee_helper
* \brief        Sets the EEPROM factory reset counter value
*
* \details
*
* \pre
*
* \param val [in]        uint8_t      A value from 0 to 0x0F
* \return enum_CODE                   Always returns CODE_OK
*/
enum_CODE ee_set_factory_reset_counter(uint8_t val);

/*----------------------------------------------------------------------------*/
/**
* \ingroup      bl_ee_helper
* \brief        Gets the EEPROM factory reset counter value
*
* \details
*
* \pre
*
* \param None
* \return uint8_t                     Returns a value in range from 0 to 0x0F
*/
uint8_t ee_get_factory_reset_counter();
#endif // 0

/*----------------------------------------------------------------------------*/
/**
* \ingroup      bl_ee_helper
* \brief        Returns either old or new address
*               Do not use this function directly but rather via EE_MEMB_ADDR macro.
*
* \details
*
* \pre
*
* \param addr [in]             uint32_t    Contains both uint16_t old and new addresses
* \return uint16_t                         The true working address
*/
uint16_t bl_ee_working_addr(uint32_t addr);


#endif /* SRC_ROLE_PROTOCOL_ROLE_PROTOCOL_EE_H_ */
