#ifndef _LIN_STACK_H_
#define _LIN_STACK_H_

/*----------------------------------------------------------------------------*/
/**
*   \file         lin_stack.h
*   \brief        Function declarations form MRS LIN Stack.
*/
/*----------------------------------------------------------------------------*/
/** 
*   \addtogroup   doc_lin_main MRS LIN DB
*   \{
*   \brief      Here is the main functionality for MRS LIN DB described.
*   \details    This library provides a user friendly, easy to use LIN bus feature of HCS12.
*               This group contains functions to use LIN communication. There are functions
*               for get and set db values, for schedule handling and more.
* 
*               Example:
*               \code{.c}
*                   void usercode_init(void)
*                   {
*                       // Update the module name from the define
*                       (void)strcpy(EEPROM_Modulname, MODULNAME_DEF);
*                       modulename_change = TRUE;
*
*                       // Set the SW-Version, maximal length=20
*                       (void)strncpy(EEPROM_SW_Version, SW_VERSION, 20);
*   
*                       // set LIN transceiver active
*                       os_digout(DO_LIN_EN, 1);
*   
*                       // set the default value signal values
*                       lin_db_set_value(LIN0_DP_MASTER_ECU_ADDR, 0x00);
*                       lin_db_set_value(LIN0_DP_MASTER_ECU_ADDR_VALUE, 0x00);
*
*                       lin_db_set_value(LIN0_DP_MASTER_SENSOR_CYCLE_REQ, 0x00);
*                       lin_db_set_value(LIN0_DP_SLAVE_SENSOR_ERROR_BIT_VOLT_1, 0x00);
*                       lin_db_set_value(LIN0_DP_SLAVE_SENSOR_ERROR_BIT_VOLT_2, 0x00);
*                   }
*
*                   void usercode(void)
*                   {
*                       uint16_t sensor_raw_value = 0;
*                       uint16_t sensor_value = 0;
*   
*                       // run the selected LIN schedule table
*                       lin_schedule_handling(LIN0_ST_SCHEDULE_TABLE_1, TRUE);
*   
*                       // check if a frame was received - with reset to get a new one
*                       if( lin_check_for_received_frame(LIN_BUS_0, LIN0_FRM_SLAVE_RESP_1, TRUE) == TRUE )
*                       {
*                           // frame was received
*                           // example with lin_db_get_value() to get signal values
*                           sensor_raw_value = (uint16_t)lin_db_get_value(LIN0_DP_SLAVE_SENSOR_RAW_VALUE);
*                           sensor_value = (uint16_t)lin_db_get_value(LIN0_DP_SLAVE_SENSOR_VALUE);
*        
*                           // send the received LIN signal to the CAN bus
*                           (void)os_can_send_message(0x012, 0, 6, (uint8_t)(sensor_raw_value), (uint8_t)(sensor_raw_value >> 8), 0, 0, 0, 0, (uint8_t)(sensor_value), (uint8_t)(sensor_value >> 8));
*        
*       
*                           // example without lin_db_get_value() to get signal values
*                           // uint8_t ext_ary_lin0_master_req_1_data[2];       
*                           // uint8_t ext_ary_lin0_slave_resp_1_data[6];       
*                           // use the data byte array from lin_db_tables.c to get the bytes easier
*       
*                           // send the received LIN signal to the CAN bus
*                           (void)os_can_send_message(0x112, 0, 6, ext_ary_lin0_slave_resp_1_data[0], ext_ary_lin0_slave_resp_1_data[1], ext_ary_lin0_slave_resp_1_data[2], ext_ary_lin0_slave_resp_1_data[3], ext_ary_lin0_slave_resp_1_data[4], ext_ary_lin0_slave_resp_1_data[5], 0, 0);
*                       }
*                   }    
*               \endcode 
*
*
*   \todo Add lin_stack_private_prototypes.h into cw-project
*   \internal
*   \endinternal
*
*/
/*----------------------------------------------------------------------------*/

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------


#include "hal_data_types.h"
#include "bit_access_macros.h"
#include "lin_db_tables.h"
#include "hal_lin.h"
#include "lin_diagnose.h"


// --------------------------------------------------------------------------------
// Software history lin_stack
// --------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------
// END software history
// --------------------------------------------------------------------------------
#define MRS_LIN_STACK_SW_VERSION  "V0.1.0"

#define LIN_SLEEP_TIMEOUT	4000u

#if 0
/** This enum declares used software LIN interfaces.\n
*   The index is used to access IO registers e.g. #lin_ldfc_reg.\n
*   So all used buses must be placed here and according registers must be set. */
typedef enum
{
    LIN30   = 0u, ///< RLIN30 -> located at LIN/Uart Interface (RLIN3)   -> used as UART
    LIN31       , ///< RLIN31 -> located at LIN/Uart Interface (RLIN3)   -> used as LIN
    LIN32       , ///< RLIN32 -> located at LIN/Uart Interface (RLIN3)   -> used as LIN
    LIN33       , ///< RLIN33 -> located at LIN/Uart Interface (RLIN3)   -> used as LIN
    LIN34       , ///< RLIN34 -> located at LIN/Uart Interface (RLIN3)   -> used as LIN
    LIN35       , ///< RLIN35 -> located at LIN/Uart Interface (RLIN3)   -> used as UART
    LIN25       , ///< RLIN25 -> located at LIN Master Interface (RLIN2) -> used as LIN
    LIN_MAX
} enum_LIN_UNIT;
#endif

/// LIN handler state types
typedef enum
{
    LIN_HANDLE_IDLE = 0,									///< LIN handler is idle
    LIN_HANDLE_HEADER_TRANSMISSION_ENABLED,					///< LIN handler transmission is active
    LIN_HANDLE_HEADER_RECEPTION,							///< LIN handler header is received
    LIN_HANDLE_RESPONSE_TRANSMISSION_ENABLED,				///< LIN handler response data transmission is active
    LIN_HANDLE_RESPONSE_RECEPTION_ENABLED,					///< LIN handler response reception is active
    LIN_HANDLE_MASTER_REQUEST_FRAME_TRANSMISSION_ENABLED	///< LIN handler master request frame transmission is active
}enum_lin_handle_state_types;

#if 0
/** \internal LIN_REMOVE_AFTER_ADD_DYN_DATA\n
 *  This structure is used to process data which is in dependency of used LIN interfaces (dynamic). */
typedef struct
{
    uint8_t                     trasmitted_header_id; ///< help variable to save the header ID
    enum_lin_handle_state_types handle_state;         ///< help variable to save the LIN Bus handle state
    uint32_t                    error_timestamp;      ///< timer to handle error
    struct_hal_lin_handle       lin_handle;           ///< see #struct_hal_lin_handle
} struct_lin_dyn_data;
#endif

// --------------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------------

#ifndef NULL_PTR
	#define NULL_PTR ((void*)0)
#endif


#ifndef MRS_FAR
	#define MRS_FAR	// define MRS_FAR to "nothing" if not on HCS platform
#endif

#define LIN_WAKE_SIGNAL     (uint8_t)0xF0    // 0x80 (9600) LIN Wake signal on LIN Bus over UART (Wakeup Impulse 250us  ...5 ms) 19200 = 0xF0 = 262µs
#define LIN_SYNC_SIGNAL     (uint8_t)0x55    // LIN-SYNC -Signal

#define LIN_STATE_ERROR     (uint8_t)0x00
#define LIN_STATE_SUCCESS   (uint8_t)0x01

#define LIN_FRAME_LEN_MAX	((uint8_t)0x08)


// --------------------------------------------------------------------------------
// Enums
// --------------------------------------------------------------------------------

/// LIN RX state machine receive state types
typedef enum
{
    LIN_TX_FRAME_START_INTERRUPT = 0,
    LIN_TX_FRAME_START_IDLE
}enum_lin_tx_frame_start_types;


/// Possible function return values for LIN header received callback, Only in Slave Mode, each header reception create a call
typedef enum
{
    LIN_HEADER_NO_ACK                   = 0u,               ///< No LIN header acknowledge, Message Data will not send
    LIN_HEADER_ACK                      = 1u                ///< LIN header acknowledge, Message Data will send
}enum_header_received_callback;

// --------------------------------------------------------------------------------
// Callback typedefs
// --------------------------------------------------------------------------------
/*----------------------------------------------------------------------------*/
/**
* \ingroup lin
* \brief    Typedef for LIN user callback - Only a header was received
* \details
*
* \pre
*
* \param    lin_module   [in] *lin_header_received_callback_t)(const uint8_t
* \param    rx_header_id [in] const uint8_t
* \param    frame_index  [in] const uint8_t
* \return   typedef
*/
typedef enum_header_received_callback (*lin_header_received_callback_t)(const uint8_t lin_module, const uint8_t rx_header_id, const uint8_t frame_index);

/*----------------------------------------------------------------------------*/
/**
* \ingroup lin
* \brief    Typedef for LIN user callback - Frame was received callback
* \details
*
* \pre
*
* \param    lin_module             [in] *lin_frame_received_callback_t)(const uint8_t
* \param    struct_lin_rx_msg_type [in] const
* \param    frame_index            [in] const uint8_t
* \return   typedef
*/
typedef void (*lin_frame_received_callback_t)(const uint8_t lin_module, const struct_lin_rx_msg_type, const uint8_t frame_index);

/*----------------------------------------------------------------------------*/
/**
* \ingroup lin
* \brief    Typedef for LIN user callback - Schedule table complete callback
* \details
*
* \pre
*
* \param    lin_module               [in] *lin_schedule_complete_callback_t)(const uint8_t
* \param    lin_schedule_table_index [in] const uint8_t
* \return   typedef
*/
typedef void (*lin_schedule_complete_callback_t)(const uint8_t lin_module, const uint8_t lin_schedule_table_index);

/*----------------------------------------------------------------------------*/
/**
* \ingroup lin
* \brief    Typedef for LIN user callback - Frame transmission complete callback
* \details
*
* \pre
*
* \param    lin_module   [in] *lin_frame_transmission_complete_callback_t)(const uint8_t
* \param    tx_header_id [in] const uint8_t
* \param    frame_index  [in] const uint8_t
* \return   typedef
*/
typedef void (*lin_frame_transmission_complete_callback_t)(const uint8_t lin_module, const uint8_t tx_header_id, const uint8_t frame_index);


// --------------------------------------------------------------------------------
// Extern needed variables
// --------------------------------------------------------------------------------

// LIN stack callback handler routines
extern lin_header_received_callback_t 				ext_lin_stack_header_received_cb_handler;
extern lin_frame_received_callback_t 				ext_lin_stack_frame_received_cb_handler;
extern lin_schedule_complete_callback_t 			ext_lin_stack_schedule_complete_cb_handler;
extern lin_frame_transmission_complete_callback_t	ext_lin_stack_frame_transmission_complete_cb_handler;


/*----------------------------------------------------------------------------*/
/**
* \brief    SCI re- and initialization for the correct communication and required LIN settings
* \details
*
*
* \return   void
*/
void lin_init(void);


/*----------------------------------------------------------------------------*/
/**
* \ingroup lin
* \brief Function to setup special callback functions
* \details
* 	- lin_received_header_cb_handler((uint8_t) bus_count, (uint8_t) rx_header_id);
* 	- lin_received_frame_callback_t((uint8_t) bus_count, (struct_lin_rx_msg_type) lin_msg);
*
*
* \pre
*
* \param    ptr_lin_header_received_callback [in]               lin_received_header_callback_t
* \param    ptr_lin_frame_received_callback  [in]               lin_received_frame_callback_t
* \param    ptr_lin_schedule_complete_callback  [in]            lin_schedule_complete_callback_t
* \param    ptr_lin_frame_transmission_complete_callback  [in]  lin_frame_transmission_complete_callback_t
* \return   void
*/
void lin_set_callback(  lin_header_received_callback_t             const ptr_lin_header_received_callback,
                        lin_frame_received_callback_t              const ptr_lin_frame_received_callback,
                        lin_schedule_complete_callback_t           const ptr_lin_schedule_complete_callback,
                        lin_frame_transmission_complete_callback_t const ptr_lin_frame_transmission_complete_callback);


void lin_error_callback(uint8_t module, uint8_t lin_status);

/*----------------------------------------------------------------------------*/
/**
* \brief    help function to disable the LIN stack with all interrupts
* \details  Disable all SCI functions which was needed for the LIN communication.
*           A reinitialization is needed for a reactivation.
*
* \param    lin_module  [in] uint8_t    LIN index number from enum_lin_bus_id.
* \return   void
*/
void lin_module_disable(uint8_t lin_module);


/*----------------------------------------------------------------------------*/
/**
* \brief    LIN-wakeup send
* \details
*
*
* \param    lin_module  [in] uint8_t    LIN index number from enum_lin_bus_id
* \return   void
*/
void lin_send_wakeup(uint8_t lin_module);


/*----------------------------------------------------------------------------*/
/**
* \brief    LIN-sleep send
* \details
*
*
* \param    lin_module  [in] uint8_t    LIN index number from enum_lin_bus_id
* \return   void
*/
void lin_send_go_to_sleep(uint8_t lin_module);


/*----------------------------------------------------------------------------*/
/**
* \brief    LIN receive frame handling in SCI receive interrupt
* \details
*
*
* \param    module [in] uint8_t    LIN SCI HW number eg. BIOS_SCI1.
* \param    data   [in] uint8_t
* \return   void
*/
void lin_rx_frame_handling(uint8_t module, uint8_t data, uint8_t break_received);


/*----------------------------------------------------------------------------*/
/**
* \brief    1ms interrupt callback for LIN timeout handling
* \details
*
*
* \return   void
*/
void lin_ms_timer_handling(void);


/*----------------------------------------------------------------------------*/
/**
* \brief    calculate the CRC (knowing type) and check it
* \details
*
*
* \param    lin_id_with_parity [in] uint8_t     LIN ID with the parity bits
* \param    lin_length         [in] uint8_t     data field length
* \param    lin_data_ptr       [in] uint8_t*    pointer to the data array
* \param    lin_crc_to_check   [in] uint8_t     CRC to check
* \param    lin_crc_type       [in] uint8_t     FALSE = classic CRC, TRUE = enhanced CRC
* \return   uint8_t                             0 = fail, 1 = pass
*/
uint8_t lin_crc_check(uint8_t lin_id_with_parity, uint8_t lin_length, uint8_t* lin_data_ptr, uint8_t lin_crc_to_check, uint8_t lin_crc_type);


/*----------------------------------------------------------------------------*/
/**
* \brief    calculate the CRC and check it
* \details  try first enhanced and then classic CRC calculation
*
*
* \param    lin_id_with_parity [in] uint8_t     LIN ID with the parity bits
* \param    lin_length         [in] uint8_t     data field length
* \param    lin_data_ptr       [in] uint8_t*    pointer to the data array
* \param    lin_crc_to_check   [in] uint8_t     CRC to check
* \return   uint8_t                             0 = no pass, 1 = classic pass, 2 = enhanced pass
*/
uint8_t lin_crc_check_universal(uint8_t lin_id_with_parity, uint8_t lin_length, uint8_t* lin_data_ptr, uint8_t lin_crc_to_check);


/*----------------------------------------------------------------------------*/
/**
* \brief    help function to calculate the CRC value for the correspond LIN frame index
* \details
*
*
* \param    lin_frame_index [in] uint8_t    LIN frame index
* \return   uint8_t                         calculated CRC value
*/
uint8_t lin_calculate_crc_value(uint8_t lin_frame_index);


/*----------------------------------------------------------------------------*/
/**
* \brief    help function to call the interrupt transmit handling only with module input parameter
* \details  get the needed bus information from the LIN frame database.
*           Linked in os_util.c (os_util_init()) to the SCI interrupt function pointer
*
*
* \param    module [in] uint8_t             LIN SCI HW number eg. BIOS_SCI1.
* \return   void
*/
void lin_tx_frame_handling(uint8_t module);


/*----------------------------------------------------------------------------*/
/**
* \brief    Get the data-point value from the LIN database.
* \details  This functions returns the prevously received value of the data-point "datapoint_id"
*
*
* Example Code
*   \code{.c}
*       uint32_t value;
*       value = lin_db_get_value(LIN0_RX_SIGNAL_NAME_15)// Returns the value of LIN0_RX_SIGNAL_NAME_15.
*   \endcode 
*
* \param    datapoint_id    [in] uint32_t   Index of the LIN datapoint. See values from lin_db_tables.c 
* \return   uint32_t                        Returns the data-point value.
*/
uint32_t lin_db_get_value(uint32_t datapoint_id);


/*----------------------------------------------------------------------------*/
/**
* \brief    Write a data-point value to the LIN database.
* \details  This functions sets the value "set_value" to the data-point
*           defined in "datapoint_id".
*   Example Code
*   \code{.c}
*       uint32_t value;
*       lin_db_set_value(LIN0_TX_SIGNAL_NAME_1, 0x0F); // writes "0x0F" to data-point LIN0_TX_SIGNAL_NAME_1.
*   \endcode
*
* \param    datapoint_id    [in] uint32_t   Index of the LIN datapoint. See values from lin_db_tables.c 
* \param    set_value       [in] uint32_t  The new value to write.
* \return   void
*/
void lin_db_set_value(uint32_t datapoint_id, uint32_t set_value);


/*----------------------------------------------------------------------------*/
/**
* \brief    Schedule table handling and bus idle check with automatic go to sleep command if bus was idle for 4s.
* \details  lin_schedule_handling() must be called for each bus schedule table.
*           With lin_st_active parameter is it possible to hold and resume a schedule table.
*
* \param    lin_schedule_table_index [in] uint8_t The schedule table affected by the "lin_st_active" parameter.
* \param    lin_st_active            [in] uint8_t Activate or deactivate schedule table
* \return   void
*
*   Example Code
*   \code{.c}
*       lin_schedule_handling(LIN0_SCHEDULE_TABLE_NR1, TRUE);
*   \endcode

*/
void lin_schedule_handling(uint8_t lin_schedule_table_index, uint8_t lin_st_active);


/*----------------------------------------------------------------------------*/
/**
* \brief    help function to reset a started schedule table, reset the schedule line index
* \details  call this function only one time to reset the schedule line index,
*           other you send only the first frame at the first schedule line
*
* \param    lin_schedule_table_index [in] uint8_t The schedule table affected by the reset.
* \return   void
*
* Example Code
* \code{.c}
*   lin_schedule_handling_reset(LIN0_SCHEDULE_TABLE_NR1);
* \endcode 
*
*/
void lin_schedule_handling_reset(uint8_t lin_schedule_table_index);


/*----------------------------------------------------------------------------*/
/**
* \brief    Handle to evaluate the receive queue.
* \details  This handle must called in the main cycle or user main cycle.\n
*           It actualize the receive LIN message buffer.
*
* \return   void
*/
void lin_queue_in_process(void);


/*----------------------------------------------------------------------------*/
/**
* \brief
* \details
*
*
* \param    lin_module_in [in] uint8_t  LIN index number from enum_lin_bus_id.
* \param    frame_id      [in] uint8_t
* \return   void
*/
void lin_gateway_handling( uint8_t lin_module_in, uint8_t frame_id );


/*----------------------------------------------------------------------------*/
/**
* \brief
* \details
*
*
* \param    frame_dest_index [in] uint8_t
* \param    frame_src_index  [in] uint8_t
* \return   void
*/
void lin_gateway_req_data_process(uint8_t frame_dest_index, uint8_t frame_src_index);


/*----------------------------------------------------------------------------*/
/**
* \brief
* \details
*
*
* \param    frame_dest_index [in] uint8_t
* \param    frame_src_index  [in] uint8_t
* \return   void
*/
void lin_gateway_resp_data_process(uint8_t frame_dest_index, uint8_t frame_src_index);


/*----------------------------------------------------------------------------*/
/**
* \brief    Check for a received response frame
* \details  On Master mode check if a complete LIN Slave response frame data was correct received.\n
*           On Slave mode check if a response frame header was received
*
*
* \param    frame_index [in] uint8_t    frame index number or name from enum_lin_frame_id
* \return   uint8_t                     TRUE (1) received, FALSE (0) nothing received
*/
uint8_t lin_check_for_received_response_frame(uint8_t frame_index);

// Macro "alias" for slave mode with clearer name
#define lin_check_for_received_header(x)		lin_check_for_received_response_frame(x)


/*----------------------------------------------------------------------------*/
/**
* \brief    Check for a received request frame
* \details  Only on Slave mode check if a request frame was received
*
*
* \param    frame_index [in] uint8_t    frame index number or name from enum_lin_frame_id
* \return   uint8_t                     TRUE (1) received, FALSE (0) nothing received
*/
uint8_t lin_check_for_received_request_frame(uint8_t frame_index);


/*----------------------------------------------------------------------------*/
/**
* \brief    Check if LIN frame from the selected lin_module is received.
*           
* \details  This functions checks if a frame from the selected "lin_module" was
*           received, without knowing about request or response frame on master
*           or slave. Set parameter "reset" to TRUE to clear the flag for detection
*           of a new receive, set to FALSE if you want to clear the flag manually.
*           It returns "1" if frame was received an "0" if nothing was received.
*           There is no information about request or response frame on master or slave.
*
*
* \param    lin_module  [in] uint8_t    LIN index number. See options at #enum_lin_bus_id in lin_db_tables.h.
* \param    frame_index [in] uint8_t    Frame index number or name from enum_lin_frame_id
* \param    reset       [in] uint8_t    TRUE (1) clear the flag to detect a new receive, FALSE (0) don't clear the flag to reset it manual
* \return   uint8_t                     TRUE (1) received, FALSE (0) nothing received
*
* Example Code
* \code{.c}
*   uint8_t ret;
*   
*   ret = lin_check_for_received_frame(LIN_BUS_0, LIN0_FRM_RX_INDEX_NAME_01, TRUE); //Check if LIN_BUS_0, received frame LIN0_FRM_RX_INDEX_NAME_01 with reset option = TRUE.
*   if( ret == TRUE )
*   {
*       // frame was received, do something
*   }
* \endcode 

*/
uint8_t lin_check_for_received_frame(uint8_t lin_module, uint8_t frame_index, uint8_t reset);

// WIP function for more convenient handling
bool lin_check_for_received_frame_reworked(enum_lin_dp_id frame_index, bool reset);

/*----------------------------------------------------------------------------*/
/**
* \brief    Clear the response frame flag to transmit the slave response data field.
* \details  On Slave mode to transmit the slave response data field.
*           On Master mode to tell the stack that we have read all received data.
*           It's not functional necessary for Master mode.
*
* \param    frame_index [in] uint8_t Frame index number or name from enum_lin_frame_id
* \return   void
*
* Example Code
* \code{.c}
*   lin_clear_response_frame_flag(LIN0_FRM_RESP_INDEX_NAME_01); 
* \endcode 

*/
void lin_clear_response_frame_flag(uint8_t frame_index);

// Macro "alias" for slave mode with clearer name
#define lin_enable_frame_transmission(x) 	lin_clear_response_frame_flag(x)


/*----------------------------------------------------------------------------*/
/**
* \brief    Clear the request frame flag to detect a new receive.
* \details  Only on Slave mode to detect a master request frame correctly.
*
*
* \param    frame_index [in] uint8_t Frame index number or name from enum_lin_frame_id
* \return   void
*
*   Example Code
*   \code{.c}
*       lin_clear_request_frame_flag(LIN0_FRM_REQ_INDEX_NAME_01); 
*   \endcode

*/
void lin_clear_request_frame_flag(uint8_t frame_index);


/*----------------------------------------------------------------------------*/
/**
* \brief    Check if response timeout was received, only on Master mode.
* \details  This function checks if response timeout on "frame_index" was received
*           and returns "1" if slave is not responding (timeout) or "0" if no 
*           timeout is reached.
*
*
* \param    frame_index [in] uint8_t    Frame index number or name from enum_lin_frame_id
* \return   uint8_t                     TRUE (1) = Slave not responding (timeout), FALSE (0) = not timeout reached
* 
* Example Code
* \code{.c}
*   uint8_t ret;
*   // Check if LIN0_FRM_INDEX_NAME_01 received a timeout
*   ret = lin_ma_check_frame_resp_timeout(LIN0_FRM_INDEX_NAME_01);
*   if( ret == TRUE )
*   {
*       // timeout reached - Slave not responding
*       // do something
*   }
* \endcode

*/
uint8_t lin_ma_check_frame_resp_timeout(uint8_t frame_index);

/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin
* \brief    Clear the response frame flag to transmit the slave response data field.
* \details  On Slave mode to transmit the slave response data field./n
*           On Master mode to tell the stack that we have read all received data./n
*           It's not functional necessary for Master mode.
* \pre
*
* \param    frame_index [in] uint8_t frame index number or name from enum_lin_frame_id
* \return   void
*/
void lin_header_received_ack(uint16_t frame_index);


/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin
* \brief    help function to get the database frame index by lin_module and LIN Frame ID
* \details
*
* \pre
*
* \param    lin_module [in] uint8_t corresponding SCI module / HW number
* \param    lin_id     [in] uint8_t LIN bus module handler number (lin_module)
* \return   uint16_t                LIN database frame index (needed for a correct database access)
*/
uint16_t lin_get_lin_frame_index(uint8_t lin_module, uint8_t lin_id);


/** \}*/
#endif

