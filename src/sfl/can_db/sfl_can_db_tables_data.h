#ifndef SFL_CAN_DB_TABLES_DATA_H
#define SFL_CAN_DB_TABLES_DATA_H
/*----------------------------------------------------------------------------*/
/**
* \file         sfl_can_db_tables_data.h
* \brief        this is the sfl_can_db_tables_data header file which handles can db data
* \date         20160822
* \author       riegel
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup
* \brief
* \details
*/
/*----------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "hal_data_types.h"
#include "hal_can.h"
#include "can_db_tables.h"
#include "sfl_timer.h"
#include "string.h"
#include "sfl_fifo.h"
#include "flexcan_driver.h"

typedef struct
{
    uint32_t can_clock;
    uint32_t arbitr_baud;
    uint32_t data_baud;
    uint8_t  arbitr_smp;
    uint8_t  data_smp;
} __attribute__((packed)) struct_ROLE_CAN_EXT_BAUD_INPUT_DEMO;

typedef struct
{
    uint16_t  presc;
    uint8_t   rjw;
    uint8_t   propseg;
    uint8_t   pseg1;
    uint8_t   pseg2;
    uint16_t  tolerance;
    uint8_t   smp;
} __attribute__((packed)) struct_ROLE_CAN_EXT_BAUD_OUTPUT_PHASE_DEMO;

typedef struct
{
    // phase 1 - arbitration, phase 2 - data
    // for classical CAN only phase 1 is valid
    // i.e. the data_baud in the input should be 0
    struct_ROLE_CAN_EXT_BAUD_OUTPUT_PHASE_DEMO phase[2];
} __attribute__((packed)) struct_ROLE_CAN_EXT_BAUD_OUTPUT_DEMO;

typedef struct
{
    struct_ROLE_CAN_EXT_BAUD_INPUT_DEMO inp;
    struct_ROLE_CAN_EXT_BAUD_OUTPUT_DEMO out;
} __attribute__((packed)) struct_ROLE_CAN_EXT_BAUD_DEMO;

typedef struct
{
    uint8_t                  enable;
    struct_ROLE_CAN_EXT_BAUD_DEMO baud;
    uint8_t                  crc8;
} __attribute__((packed)) struct_EE_CAN_EXT_DEMO;

/**
 *  CAN frame arbitration and data parts
 */
typedef enum
{
    CAN_FD_FRAME_PHASE_ARBITRATION,
    CAN_FD_FRAME_PHASE_DATA
} enum_PHY_CAN_FRAME_PHASE_DEMO;

// ---------------------------------------------------------------------------------------------------
// defines
// ---------------------------------------------------------------------------------------------------
#define dyn_CAN_DP_MAX                  can_db.DP_MAX
#define dyn_CAN_BUS_MAX                 can_db.BUS_MAX
#define dyn_CAN_BLOCK_MAX               can_db.BLOCK_MAX
#define dyn_CAN_BAUDRATE                can_db.BAUDRATE
#define dyn_CAN_BUS_1                   can_db.BUS_1
#define dyn_OS_PORT_INPUT_PIN_MAX       os_io->PORT_INPUT_PIN_MAX
#define dyn_OS_PORT_PIN_MAX             os_io->PORT_PIN_MAX
#define dyn_OS_ANALOG_PIN_MAX           os_io->PORT_ANALOG_PIN_MAX
#define BIOS_CAN_BAUDRATE_NAME_LEN      10


typedef enum
{
    NONE                      =23,       ///< legacy: CAN HS
    BIOS_CAN_EXT_BAUDRATE     =0,        ///< legacy: CAN HS
    BIOS_CAN_10KBIT           =8,
    BIOS_CAN_20KBIT           =7,
    BIOS_CAN_33_33KBIT        =11,
    BIOS_CAN_50KBIT           =6,
    BIOS_CAN_83_33KBIT        =10,
    BIOS_CAN_100KBIT          =9,
    BIOS_CAN_125KBIT          =5,
    BIOS_CAN_250KBIT          =4,
    BIOS_CAN_500KBIT          =3,
    BIOS_CAN_800KBIT          =2,
    BIOS_CAN_1MBIT            =1,
    BIOS_CAN_FD_START_ID      =30,       ///< Selected Baudrate is external
    BIOS_CAN_FD_EXT_BAUDRATE  =31,       ///< Selected Baudrate is external
    BIOS_CAN_FD_125_250       =32,       ///< 125 KBit/s arbitration and 250 KBit/s data
    BIOS_CAN_FD_125_500       =33,       ///< 125 KBit/s arbitration and 500 KBit/s data
    BIOS_CAN_FD_125_800       =34,       ///< 125 KBit/s arbitration and 800 KBit/s data
    BIOS_CAN_FD_125_1000      =35,       ///< 125 KBit/s arbitration and 1000 KBit/s data
    BIOS_CAN_FD_125_2000      =36,       ///< 125 KBit/s arbitration and 2000 KBit/s data
    BIOS_CAN_FD_125_3000      =37,       ///< 125 KBit/s arbitration and 3000 KBit/s data
    BIOS_CAN_FD_125_4000      =38,       ///< 125 KBit/s arbitration and 4000 KBit/s data
    BIOS_CAN_FD_125_5000      =39,       ///< 125 KBit/s arbitration and 5000 KBit/s data
    BIOS_CAN_FD_250_500       =40,       ///< 250 KBit/s arbitration and 500 KBit/s data
    BIOS_CAN_FD_250_800       =41,       ///< 250 KBit/s arbitration and 800 KBit/s data
    BIOS_CAN_FD_250_1000      =42,       ///< 250 KBit/s arbitration and 1000 KBit/s data
    BIOS_CAN_FD_250_2000      =43,       ///< 250 KBit/s arbitration and 2000 KBit/s data
    BIOS_CAN_FD_250_3000      =44,       ///< 250 KBit/s arbitration and 3000 KBit/s data
    BIOS_CAN_FD_250_4000      =45,       ///< 250 KBit/s arbitration and 4000 KBit/s data
    BIOS_CAN_FD_250_5000      =46,       ///< 250 KBit/s arbitration and 5000 KBit/s data
    BIOS_CAN_FD_500_800       =47,       ///< 500 KBit/s arbitration and 800 KBit/s data
    BIOS_CAN_FD_500_1000      =48,       ///< 500 KBit/s arbitration and 1000 KBit/s data
    BIOS_CAN_FD_500_2000      =49,       ///< 500 KBit/s arbitration and 2000 KBit/s data
    BIOS_CAN_FD_500_3000      =50,       ///< 500 KBit/s arbitration and 3000 KBit/s data
    BIOS_CAN_FD_500_4000      =51,       ///< 500 KBit/s arbitration and 4000 KBit/s data
    BIOS_CAN_FD_500_5000      =52,       ///< 500 KBit/s arbitration and 5000 KBit/s data
    BIOS_CAN_FD_800_1000      =53,       ///< 800 KBit/s arbitration and 1000 KBit/s data
    BIOS_CAN_FD_800_2000      =54,       ///< 800 KBit/s arbitration and 2000 KBit/s data
    BIOS_CAN_FD_800_3000      =55,       ///< 800 KBit/s arbitration and 3000 KBit/s data
    BIOS_CAN_FD_800_4000      =56,       ///< 800 KBit/s arbitration and 4000 KBit/s data
    BIOS_CAN_FD_800_5000      =57,       ///< 800 KBit/s arbitration and 5000 KBit/s data
    BIOS_CAN_FD_1000_2000     =58,       ///< 1000 KBit/s arbitration and 2000 KBit/s data
    BIOS_CAN_FD_1000_3000     =59,       ///< 1000 KBit/s arbitration and 3000 KBit/s data
    BIOS_CAN_FD_1000_4000     =60,       ///< 1000 KBit/s arbitration and 4000 KBit/s data
    BIOS_CAN_FD_1000_5000     =61,       ///< 1000 KBit/s arbitration and 5000 KBit/s data
    BIOS_CAN_BAUD_MAX                    ///< Maximum Baudrate Identifier. Do not use this value and leave at last position in enum.
}enum_BIOS_CAN_FD_BAUDRATE;


#define DF_INTEL                        0
#define DF_MOTOROLA                     1

#define CAN_HANDLE_MAX_NR               3

#define SFL_CAN_CLASSIC_PAYLOAD_SIZE 8
#define SFL_CAN_FD_PAYLOAD_SIZE 64

//--------------------- CAN Filter-Settings: --------------------------
#define BIOS_CAN_ID_NORM_A_AND_B  0 // Beide Masken 11bit
#define BIOS_CAN_ID_NORM_A_EXT_B  1 // Eine Maske 11bit, die andere 29bit
#define BIOS_CAN_ID_EXT_A_AND_B   2 // Beide Maksen 29bit
#define BIOS_CAN_ID_ALL_CLOSED    3 // Keine Maske setzen
#define BIOS_CAN_ID_ALL_OPEN      4 // Alle Nachrichten durchlassen

// ---------------------------------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------------------------------
#define MACRO_DEFINE_TO_STR(arg) #arg

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------

/** This enum lists both values for CAN id types.*/
typedef enum
{
    STAND_ID  = 0,  //< 11 Bit Identifier
    EXT_ID,          //< 29 Bit Identifier
    NO_ID           // no identifier allowed
}enum_type_can_id;

// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------

/* A buffer struct for CAN frames which contain a "variable" data payload and the actual frame header data.
 * Payload size is determined on compile time by the CAN_FD_GLOBAL_ENABLE macro.
 */
typedef struct
{
    struct_hal_can_frame    header;
#if CAN_FD_GLOBAL_ENABLE == 1
    uint8_t                 data[SFL_CAN_FD_PAYLOAD_SIZE];
#else
    uint8_t                 data[SFL_CAN_CLASSIC_PAYLOAD_SIZE];
#endif
} struct_sfl_can_fifo_frame;

typedef struct
{
    uint32_t id;
    uint32_t timestamp;
    uint16_t id_ext    :1;
    uint16_t remote_tx :1;
    uint8_t data[64];
    uint8_t len;
    uint8_t prty;
    uint8_t can_fd;
    uint8_t can_fd_brs;

} bios_can_msg_typ;

typedef struct
{
    uint8_t name[BIOS_CAN_BAUDRATE_NAME_LEN+1];
    uint8_t sj;
    uint8_t pre;
    uint8_t t_seg1;
    uint8_t t_seg2;
    uint8_t clk_src;
    uint8_t prop;
} bios_can_baudrate_typ;

typedef struct
{
  uint8_t bus_id_in;
  uint8_t bus_id_out;

} can_gateway_db_const_typ;

/** Holds the configured filter IDs and masks defined in can_db_tables for one CAN bus*/
#define NUMBER_OF_CAN_FILTERS	2u
#define NUMBER_OF_CAN_MASKS		2u

#if ((NUMBER_OF_CAN_FILTERS > 2u) || (NUMBER_OF_CAN_MASKS > 2u))
#error "Max number of available  CAN filters and/or masks is 2 per CAN bus"
#endif

typedef struct
{
    uint32_t  filter[NUMBER_OF_CAN_FILTERS];	// number of filters in can_db_tables, number represents filter A and filter B
    uint32_t  mask[NUMBER_OF_CAN_MASKS];        // same as filter[], number represents mask A and mask B
    uint8_t   mask_division;    				// holds information on how the filters are split up, see "CAN Filter-Settings" further up.

} can_db_filter_typ;

typedef struct
{
    uint16_t   DP_MAX;
    uint8_t    BUS_MAX;
    uint16_t   BLOCK_MAX;
    uint8_t    GATEWAY_DB_MAX;  //DR20100908 Achtung! CAN_GATEWAY_DB_MAX wurde bisher als #define in der vorkompilierten Lib verwendet!!!
    uint16_t   BAUDRATE;
    uint8_t    BUS_1;
    uint8_t    user_filter_active;
    uint8_t    sa_active;         // DR20140715
    uint8_t    sa_db;             // DR20140715
    uint8_t    sa_val;            // DR20140715
    uint16_t   sa_db_rx;          // DR20140715   "<< 8" vorberechnen
    uint16_t   sa_val_rx;         // DR20140715   "<< 8" vorberechnen
    can_db_filter_typ f_set[5];

} can_db_typ;

typedef struct {
    uint8_t  hw_module_id;                  /// Hardware CAN Modul Index
    uint8_t  hw_module_active;              /// 0: MSCAN wird nicht initialisiert, 1: MSCAN wird initialisiert und benutzt
    uint8_t  bios_baudrate_id;              /// BIOS Baudrate Define (#BIOS_CAN_125KBIT, #BIOS_CAN_500KBIT, #BIOS_CAN_EXT_BAUDRATE usw.)
    uint8_t  gw_input;                      /// 0: Filter werden verwendet. 1: Filter werden ignoriert, Gateway-Funktion wird beim Empfang angesprungen
    bios_can_baudrate_typ baudrate;         /// Bei #BIOS_CAN_EXT_BAUDRATE werden Segment/Jump-With Register diese Werte verwendet
    bios_can_baudrate_typ baudrate_fd_data; /// Bei #BIOS_CAN_FD_EXT_BAUDRATE werden Segment/Jump-With Register füre CAN-FD data diese Werte verwendet
    uint8_t  can_fd_capability;             /// CAN bus is can-fd capably
} can_bus_db_const_typ;

typedef struct {
    uint8_t  bus_id;                  /// CAN Bus Index
    uint32_t can_id;                  /// CAN Identifier fï¿½r diesen Block
    uint8_t  can_id_ext;              /// CAN Extended ID Flag
    uint32_t zykluszeit_ms_max;       /// wenn zykluszeit_max <= zykluszeit_min, dann zyklisch senden, sonst bei ï¿½nderung
    uint32_t zykluszeit_ms_min;       /// wenn zykluszeit_max = 0, dann nie senden
    uint8_t  msg_len_dlc;             /// uint8_t DLC Msg-Lï¿½nge
    uint8_t  tx;                    /// uint8_t
    uint8_t  can_bus_gw;              /// DR20120424 Damit wird ein Bus angegeben, an welchen die Nachricht ohne Verï¿½nderung weitergeleitet werden soll
    uint32_t can_id_mask;             /// Manuelle Maske, welcher der User wï¿½nscht DR040309
    uint8_t  mux_start;               /// DR20120808 Multiplexer in CAN-Block mï¿½glich
    uint8_t  mux_length;              /// DR20120808 Multiplexer in CAN-Block mï¿½glich
    uint8_t  mux_val;                 /// DR20120808 Multiplexer in CAN-Block mï¿½glich

} can_block_db_const_typ;

typedef struct {
    can_block_id nr_can_block;        /// Nummer des CAN-Blocks als DB-Schlï¿½ssel zur Zuordnung von CAN_block zu CAN_signal
    uint16_t  pos_bit_0;              /// Position von Bit0 innerhalb der 64 Bit (8 Bytes) des CAN Blockes
    uint16_t  bit_laenge;             /// Lï¿½nge in Bit im CAN Block, wenn 0, dann ist der Datenpunkt kein CAN-Variable
    uint8_t  datentyp;                /// uint8_t......float
    uint8_t  data_format;

} can_datenpunkt_db_const_typ;

typedef struct {
#if CAN_FD_GLOBAL_ENABLE == 1
    uint8_t  last_data[SFL_CAN_FD_PAYLOAD_SIZE];		/// alte Daten zum Vergleich, wenn nur bei Aenderung gesendet werden soll
#else
    uint8_t  last_data[SFL_CAN_CLASSIC_PAYLOAD_SIZE];	/// alte Daten zum Vergleich, wenn nur bei Aenderung gesendet werden soll
#endif
    uint32_t time_stamp_read;
    uint32_t time_stamp_write;
    uint32_t time_stamp_transmit;
    uint8_t  received         :1;
    uint8_t  transmit         :1; //  Flag : 1 => CAN_Meldung wird beim naechsten Druchlauf einmal gesendet => Flag wird dabei geloescht
    uint8_t  transmit_stop    :1; // 0 = Alle Nachrichten kï¿½nnen gesendet werden, 1 = Es wird keine Nachricht mehr gesendet  DR151009
    boolean  stop_gw_known_ids:1; // if the message is configured to send on an other as gateway this feature can be stopped during runtime with #sfl_can_db_stop_gateway_for_known_ids
    bios_can_msg_typ msg;

} can_block_db_ram_typ;

/** This struct contains everything for the CAN RX/TX config.*/
typedef struct
{
    uint32_t                        rx_fifo_size;
    uint32_t                        tx_fifo_size;
    struct_sfl_can_fifo_frame*      ptr_tx_fifo_buffer;
    struct_sfl_can_fifo_frame*      ptr_rx_fifo_buffer;
    SFL_FIFO_CONFIG_TYPE*           tx_fifo_config;
    SFL_FIFO_CONFIG_TYPE*           rx_fifo_config;
} struct_can_fifo_config;


#ifndef MAX_CAN_DEFAULT_SET
#define MAX_CAN_DEFAULT_SET     3       // max CAN supported
#endif

#ifndef CAN0_TX_FIFO_SIZE
#define CAN0_TX_FIFO_SIZE 5        // CAN 0 TX fifo size (1 entry = 16 byte)
#endif

#ifndef CAN1_TX_FIFO_SIZE
#define CAN1_TX_FIFO_SIZE 5        // CAN 1 TX fifo size (1 entry = 16 byte)
#endif

#ifndef CAN2_TX_FIFO_SIZE
#define CAN2_TX_FIFO_SIZE 5        // CAN 2 TX fifo size (1 entry = 16 byte)
#endif

#ifndef CAN0_RX_FIFO_SIZE
#define CAN0_RX_FIFO_SIZE 5        // CAN 0 RX fifo size (1 entry = 16 byte)
#endif

#ifndef CAN1_RX_FIFO_SIZE
#define CAN1_RX_FIFO_SIZE 5        // CAN 1 RX fifo size (1 entry = 16 byte)
#endif

#ifndef CAN2_RX_FIFO_SIZE
#define CAN2_RX_FIFO_SIZE 5        // CAN 2 RX fifo size (1 entry = 16 byte)
#endif

extern struct_sfl_can_fifo_frame fifo_msg_tx_can0[CAN0_TX_FIFO_SIZE];
extern struct_sfl_can_fifo_frame fifo_msg_tx_can1[CAN1_TX_FIFO_SIZE];
extern struct_sfl_can_fifo_frame fifo_msg_tx_can2[CAN2_TX_FIFO_SIZE];

extern struct_sfl_can_fifo_frame fifo_msg_rx_can0[CAN0_RX_FIFO_SIZE];
extern struct_sfl_can_fifo_frame fifo_msg_rx_can1[CAN1_RX_FIFO_SIZE];
extern struct_sfl_can_fifo_frame fifo_msg_rx_can2[CAN2_RX_FIFO_SIZE];

extern SFL_FIFO_CONFIG_TYPE fifo_tx_can0_config_default;
extern SFL_FIFO_CONFIG_TYPE fifo_tx_can1_config_default;
extern SFL_FIFO_CONFIG_TYPE fifo_tx_can2_config_default;

extern SFL_FIFO_CONFIG_TYPE fifo_rx_can0_config_default;
extern SFL_FIFO_CONFIG_TYPE fifo_rx_can1_config_default;
extern SFL_FIFO_CONFIG_TYPE fifo_rx_can2_config_default;

extern struct_can_fifo_config can_fifo_config_default[];

extern struct_can_fifo_config* can_fifo_config_actual[MAX_CAN_DEFAULT_SET];

extern uint8_t ext_sfl_can_stop_gw_unknown_ids[CAN_BUS_MAX];

// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Initializing CAN baudrates of can_db_tables
* \details
*
* \pre
*
* \return   void
*/
void sfl_can_db_tables_data_init(void);

uint32_t calculate_can_baudrate(struct_ROLE_CAN_EXT_BAUD_DEMO* ptr_io);

uint32_t set_can_baudrate_timing(hal_can_time_segment *ptr_tseg, const struct_ROLE_CAN_EXT_BAUD_OUTPUT_PHASE_DEMO *params, enum_PHY_CAN_FRAME_PHASE_DEMO iphase);

void main_here();


/*----------------------------------------------------------------------------*/
/**
* \brief    Convert the CAN baud rates between the different formats
* \details  This function converts the CAN baud rate enum/define, depending on the state of the ident parameter,
* 			from HAL to BIOS format for ident = 1 and from BIOS to HAL for ident = 0.
*
* \pre
*
* \param    baudrate [in] uint8_t const		Baud rate which needs to be converted, can either be BIOS or HAL baud rate
* \param    ident    [in] uint8_t			Select the conversion mode. "1" for HAL to BIOS. "0" for BIOS to HAL.
* \return   uint8_t  						The converted baud rate enum/define
*/
uint8_t sfl_can_db_tables_convert_baud(uint8_t const baudrate, uint8_t ident);

/*----------------------------------------------------------------------------*/
/**
* \brief    A initialization function for setting the baudrate and bit timings for the given CAN bus
* \details  This function accepts a can_handle for the desired bus with the associated config settings found in can_db_tables.
*           The config should set up with the desired baudrate as well as CAN bit timings and individual can_db flags. please refer to can_db_tables.c
*
* \param    can_bus_handle [in] struct_hal_can_handle           a CAN handle with the desired CAN bus index
* \param    can_config       [in] const can_bus_db_const_typ    the can_db config with baudrates, bit timings and CAN flags
* \return   void
*/
void sfl_can_db_tables_data_init_baudrate(struct_hal_can_handle can_bus_handle, const can_bus_db_const_typ can_config);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Initialization of the message buffer and filters for each CAN bus.
* \details  This functions initializes the message buffer depending on the configured flags in CAN DB.
*           If the CAN bus is a gateway input, one message buffer is configured for the Standard-IDs and one for Extended-IDs,
*           no mask will be set meaning all CAN IDs are accepted.
*           If there is no CAN block configured as "RX", no message buffer will be initialized as receive with its filter and mask.
*           Depending on the FILTER and MASK macros defined in can_db_tables.h, the filters will be set accordingly.
*           The partition of the message buffer for Standard-ID or Extended-ID is depended of the CANi_MASK_DIVISION macro.
*           ATTENTION: the given mask configuration is later inverted and should be considered when setting this in the code.
*
*           Message buffer occupation:
*           MB 0-1 RX
*          (MB nr. 2 is free)
*           MB 3 reserved for bl RX
*           MB 4-5 TX
*           MB 6 reserved for bl TX
*
* \pre
*
* \param    can_db_filter_config    [in] can_db_filter_typ* struct that holds the defined filter and masks from can_db_tables
* \return   void
*/
void sfl_can_db_init(can_db_filter_typ* can_db_filter_config);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*
* \pre
*
* \param    void
* \return   void
*/
void sfl_can_db_fifo_init(void);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*
* \pre
*
* \param    instance   [in] uint8_t
* \param    buffIdx    [in] uint32_t
* \return   void
*/
void sfl_can_rx_int_enable(uint8_t instance, uint32_t buffIdx);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Returns the pointer to the can handle.
* \details  The can handle contains all the current can settings.
*
* \pre
*
* \param    interface  [in]  uint8_t
* \param    ptr_handle [out] struct_hal_can_handle*
* \return   void
*/
void sfl_can_get_handle(uint8_t interface, struct_hal_can_handle* ptr_handle);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Function to configure the message buffer and set its filter and mask
* \details  This function configures a number of message buffers for receive depending on the numbers of
*           defined filter and masks. These filters and masks can be found in can_db_tables
*
* \pre
*
* \param    can_handle              [in] struct_hal_can_handle* const
* \param    can_db_filter_config    [in] const can_db_filter_typ*
* \param    id_type_mb              [in] const uint8_t*
* \return   void
*/
void sfl_can_db_config_mb_set_filter_mask(struct_hal_can_handle* const can_handle, const can_db_filter_typ* can_db_filter_config, const uint8_t id_type_mb[]);

/*----------------------------------------------------------------------------*/
/**
* \brief    A helper getter function to retrieve the configured FIFO size either from the RX or TX FIFO. Created for (better) modular code.
* \details  This function is used to return the RX or TX FIFO size for the given CAN bus. Returns the value
*           of the macro CAN<bus_id>_RX_FIFO_SIZE or CAN<bus_id>_TX_FIFO_SIZE found in can_db_tables.h.
*           Mainly used for the init FIFO functions and to get precompiled values.
*
* \param    bus_id   [in] uint8_t   the bus id for which to retrieve the information
* \param    tx_or_rx [in] uint8_t   0 to retrieve RX FIFO size and 1 to retrieve the TX FIFO sizes
* \return   uint8_t                 the configured RX or TX FIFO size for the given CAN bus
*/
uint8_t sfl_can_db_get_fifo_size(uint8_t bus_id, uint8_t tx_or_rx);

uint8_t sfl_can_db_tables_convert_baud(uint8_t const baudrate, uint8_t ident);

#endif
