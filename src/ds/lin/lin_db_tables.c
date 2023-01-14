/*----------------------------------------------------------------------------*/
/**
* \file         lin_db_tables.c
* \brief        LIN database dictionary
* \details      Is generated from the ldf.
* \date         20160609
* \author       LDF Interpreter
*
*/
/*----------------------------------------------------------------------------*/


#include "lin_db_tables.h"

// --------------------------------------------------------------------------------
// add timeout flags to LIN DB Tables
// --------------------------------------------------------------------------------
#ifndef CONVERT_BIT_TO_BYTE_SIZE
#define CONVERT_BIT_TO_BYTE_SIZE(bit_size)     (1)
#endif

uint8_t ext_dyn_lin_bux_max = LIN_BUS_MAX;
uint16_t ext_dyn_frame_counter_max = LIN_ENUM_FRM_MAX;
uint16_t ext_dyn_dp_counter_max = LIN_DP_MAX;

uint8_t ext_dyn_lin_msg_queue_size = LIN_MSG_QUEUE_SIZE;
uint8_t ext_dyn_lin_header_queue_size = LIN_HEADER_QUEUE_SIZE;

// ================================================================================
// extern variables
// usable for customer no dyn in the namespace
// ================================================================================
uint8_t ext_lin_response_error_flag[LIN_BUS_MAX] = { FALSE };    // reporting to the network, slave state
uint8_t ext_lin_tx_frame_index[LIN_BUS_MAX];
uint8_t ext_ary_lin_test_frame_flag[LIN_ENUM_FRM_MAX];
uint8_t ext_ary_lin_timeout_flags[CONVERT_BIT_TO_BYTE_SIZE(LIN_ENUM_FRM_MAX)] = { 0 };

// --------------------------------------------------------------------------------
// LIN diagnose
// --------------------------------------------------------------------------------
uint8_t ext_current_slave_state[LIN_BUS_MAX] = { SLAVE_STATE_IDLE };
uint8_t ext_pending_slave_response[LIN_BUS_MAX] = { SLAVE_RESP_NO_RESPONSE };

// ================================================================================
// extern variables
// ================================================================================
uint8_t ext_dyn_lin_route_gw_max = LIN_GW_DB_MAX;

uint8_t ext_dyn_lin_rx_len[LIN_BUS_MAX];     // LIN message expected length
uint8_t ext_dyn_rx_lin_cnt[LIN_BUS_MAX];     // LIN message received byte counter

uint8_t ext_dyn_lin_tx_state[LIN_BUS_MAX] = { LIN_TX_IDLE };
uint8_t ext_dyn_lin_rx_state[LIN_BUS_MAX] = { LIN_RX_IDLE };

uint8_t ext_dyn_lin_rx_timeout[LIN_BUS_MAX] = { 1 };
uint8_t ext_dyn_lin_tx_frame_data_counter[LIN_BUS_MAX] = { 0 };
uint8_t ext_dyn_rx_lin_frame_index[LIN_BUS_MAX];
uint8_t ext_dyn_rx_lin_diag_response[LIN_BUS_MAX] = { FALSE };
uint8_t ext_dyn_tx_data_counter[LIN_BUS_MAX] = { 0 };

struct_lin_rx_msg_type ext_dyn_lin_rx_message[LIN_BUS_MAX];

uint8_t ext_dyn_ary_lin_frame_req_rx_flag[LIN_ENUM_FRM_MAX] = { 0 };

uint8_t* ext_dyn_mgl_lin_tx_data_ptr[LIN_BUS_MAX];

uint16_t ext_dyn_crc_sum[LIN_BUS_MAX];

uint32_t ext_dyn_lin_timer_rx_timeout[LIN_BUS_MAX];

// ================================================================================
// LIN receive message queue's
// ================================================================================
struct_lin_rx_msg_type  ext_dyn_lin_rx_msg_queue[LIN_BUS_MAX][LIN_MSG_QUEUE_SIZE];

uint8_t  ext_dyn_lin_rx_msg_queue_read[LIN_BUS_MAX] = { 0 };
uint8_t  ext_dyn_lin_rx_msg_queue_write[LIN_BUS_MAX] = { 0 };

// Header Rx queue
uint8_t ext_dyn_lin_rx_header_queue[LIN_BUS_MAX][LIN_MSG_QUEUE_SIZE];

uint8_t ext_dyn_lin_rx_header_queue_read[LIN_BUS_MAX] = { 0 };
uint8_t ext_dyn_lin_rx_header_queue_write[LIN_BUS_MAX] = { 0 };

// ================================================================================
// LIN transmit message queue's
// ================================================================================
struct_lin_tx_msg_type ext_dyn_lin_tx_message[LIN_BUS_MAX];

// ================================================================================
// Schedule table
// ================================================================================
uint8_t ext_dyn_lin_bus_sleep_flag[LIN_BUS_MAX] = { TRUE };
uint8_t ext_dyn_lin_schedule_line_index[LIN_BUS_MAX] = { 0 };

uint32_t ext_dyn_lin_schedule_timer[LIN_BUS_MAX];
uint32_t ext_dyn_lin_auto_sleep_timer[LIN_BUS_MAX];

// --------------------------------------------------------------------------------
// LIN frame data field
// --------------------------------------------------------------------------------
// nothing

volatile const struct_lin_datapoint_db_const_type lin_datapoint_db_const[LIN_DP_MAX];

// --------------------------------------------------------------------------------
// LIN bus database description
// --------------------------------------------------------------------------------
// LIN-Node-Mode --> LIN_MASTER, LIN_SLAVE, LIN_SNIFFER --> enum_lin_mode_types
// --------------------------------------------------------------------------------
struct_lin_bus_db_type ext_lin_bus_db[LIN_BUS_MAX];

// --------------------------------------------------------------------------------
// LIN route gateway database
// --------------------------------------------------------------------------------
struct_lin_route_gateway_db_type ext_lin_route_gateway_db[1];

// --------------------------------------------------------------------------------
// LIN frame table declaration
// --------------------------------------------------------------------------------
// note: - the order in the frame array (ext_lin_frame_db)
//         must be the same as in the enum (enum_lin_frames)
//       - the column "Enhanced-CRC" is the type of the protected LIN identifier,
//         LIN 1.x it is classic (FALSE) and LIN 2.x enhanced (TRUE)
//       - max frames buffer size (maximum with all diagnostic frames) "user_code.h"
// --------------------------------------------------------------------------------
struct_lin_frame_db_type ext_lin_frame_db[] =
{
	//------------------------------------------------------------------------------------------------------------------------------------------------
	//  LIN-HW-Module  id     protected-id  rx_tx type  Enhanced-CRC  DLC  Data_ptr (to LIN frame data field)          LIN frame
	//------------------------------------------------------------------------------------------------------------------------------------------------
	//  LIN_BUS_0
	//------------------------------------------------------------------------------------------------------------------------------------------------
	{	LIN_BUS_0,     0x00,  0x00,         LIN_RX,     FALSE,        0,   (uint8_t*)0 },                                // LIN0_FRM_MAX
};

// --------------------------------------------------------------------------------
// LIN frame sniffer database declaration
// --------------------------------------------------------------------------------
// needed for receive of known and unknown frames
// --------------------------------------------------------------------------------
volatile struct_lin_frame_sniffer_type ext_lin_sniffed_rx_frame_db[LIN_BUS_MAX] = { 0 };

// --------------------------------------------------------------------------------
// LIN Product Identification
// --------------------------------------------------------------------------------
const uint8_t lin_diag_product_id_aag[5] =
{
	0x00,  // SupplierID LSB
	0x00,  // SupplierID MSB
	0x00,  // FunctionID LSB
	0x00,  // FunctionID MSB
	0x00   // VariantID
};

// --------------------------------------------------------------------------------
// LIN diagnose product id database declaration as pointer array
// --------------------------------------------------------------------------------
// one product id for each LIN bus
// --------------------------------------------------------------------------------
volatile const uint8_t* ext_ptr_ary_lin_diag_product_id[LIN_BUS_MAX] =
{
	//-----------------------------------
	//  pointer to LIN product ID table
	//-----------------------------------
		&lin_diag_product_id_aag[0],
};

// --------------------------------------------------------------------------------
// LIN schedule table database
// --------------------------------------------------------------------------------
volatile const struct_lin_schedule_table_db_const_type lin_schedule_table_db_const[LIN_ST_MAX];

