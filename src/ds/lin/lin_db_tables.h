#ifndef _LIN_DB_TABLES_H_
#define _LIN_DB_TABLES_H_
/*----------------------------------------------------------------------------*/
/**
* \file         lin_db_tables.h
* \brief        LIN database description
* \date         20160609
* \author       LDF Interpreter
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup     lin_db
* \brief        LIN database
* \details
*/
/*----------------------------------------------------------------------------*/
#include "hal_data_types.h"

// --------------------------------------------------------------------------------
// mark the empty LIN stack to save memory if it don't needed
// --------------------------------------------------------------------------------
#define LIN_STACK_EMPTY

// --------------------------------------------------------------------------------
// LIN receiver message queue size
// --------------------------------------------------------------------------------
#define LIN_MSG_QUEUE_SIZE    1
#define LIN_HEADER_QUEUE_SIZE 1

// --------------------------------------------------------------------------------
// Maximum LIN receive and transmit buffer length
// --------------------------------------------------------------------------------
#define MAX_LIN_TX_BUFFER_LEN (uint8_t)1
#define MAX_LIN_RX_BUFFER_LEN (uint8_t)15

/// Define all the possible LIN buses.
typedef enum
{
	LIN_BUS_0 = 0,
	LIN_BUS_MAX
}enum_lin_bus_id;

/// Define all the route gateway id's
typedef enum
{
	LIN_GW_DB_MAX = 0,
}enum_lin_route_gw_id;

/// LIN mode types, Node modus
typedef enum
{
	LIN_MASTER = 0,
	LIN_SLAVE,
	LIN_SNIFFER
}enum_lin_mode_types;

/// LIN communication types
typedef enum
{
	LIN_RX = 0,
	LIN_TX,
}enum_lin_communication_type;

/// LIN frame flag types
typedef enum
{
	LIN_FRM_FLG_CLEAR = 0,
	LIN_FRM_FLG_REQ_RX,
	LIN_FRM_FLG_RESP_RX,
	LIN_FRM_FLG_ANY_RX,
	LIN_FRM_FLG_TIMEOUT_SLAVE_NOT_RESPONDING
}enum_lin_frame_flag_types;

/// LIN RX state machine receive state types
typedef enum
{
	LIN_RX_IDLE = 0,
	LIN_RX_BREAK,
	LIN_RX_HEADER_ID,
	LIN_RX_RESPONSE_WAIT,
	LIN_RX_RESPONSE,
	LIN_RX_RESPONSE_TX,
	LIN_RX_MA_RESPONSE,
	LIN_RX_TX_ACTIVE
}enum_lin_receive_state_types;

/// LIN TX state machine transmit state types
typedef enum
{
	LIN_TX_IDLE = 0,
	LIN_TX_ACTIVE
}enum_lin_transmit_state_types;

/// LIN transmit types, used for different calling of lin_tx_frame_send()
typedef enum
{
	LIN_TRANSMIT_DONT_CARE = 0,
	LIN_TRANSMIT_START
} enum_lin_transmit_type_types;

/// slave diagnose slave state (transmit)
typedef enum
{
	SLAVE_STATE_IDLE = 0,
	SLAVE_STATE_RECEIVE_REQUEST,
	SLAVE_STATE_TRANSMIT_RESPONSE
}enum_lin_diag_slave_state_types;

/// slave diagnose response types
typedef enum
{
	SLAVE_RESP_NO_RESPONSE = 0,
	SLAVE_RESP_NEGATIVE_RESPONSE,
	SLAVE_RESP_POSITIVE_RESPONSE,
	SLAVE_RESP_OTHER_MESSAGE,
	SLAVE_RESP_LIN_PRODUCT_ID,
	SLAVE_RESP_POSITIVE_ASSIGN_FID,
	SLAVE_RESP_POSITIVE_SAVE_CONFIG,
}enum_lin_diag_slave_response_types;

/// Define LIN datapoints
typedef enum
{
	LIN_DP_MAX = 1,
}enum_lin_dp_id;

/// Define LIN Frames
typedef enum
{
	LIN0_FRM_MAX = 1,
	LIN_ENUM_FRM_MAX,
}enum_lin_frame_id;

/// Define LIN Schedule table list
typedef enum
{
	LIN_ST_MAX = 1,
}enum_lin_schedules;

// ********************************************************************************
// ********************************** Structures **********************************
// ********************************************************************************

/// structure for LIN bus database type
typedef struct
{
	uint8_t  lin_hw_module;
	uint8_t  hw_active;
	uint16_t lin_baudrate;
	uint8_t	 lin_node_modus;
	uint8_t	 lin_frame_start_index;
	uint8_t	 lin_frame_max;
	uint8_t  lin_gw_input;
	uint8_t  route_enable;
}struct_lin_bus_db_type;

/// structure for LIN frame database type
typedef struct
{
	uint8_t  lin_hw_module;
	uint8_t  id;
	uint8_t  prot_id;
	uint8_t	 rx_tx;
	uint8_t  enhanced_crc;
	uint8_t	 length;
	uint8_t* ptr;
}struct_lin_frame_db_type;

/// structure for LIN sniffer frame type
typedef struct
{
	uint8_t id;
	uint8_t	length;
	uint8_t	data[8];
	uint8_t	crc;
	uint8_t	received_flag;
}struct_lin_frame_sniffer_type;

/// structure for LIN datapoint type
typedef struct
{
	uint8_t		lin_frame_index;
	uint8_t		pos_bit_0;
	uint8_t		bit_laenge;
	uint8_t* ptr_signal_init_value;
}struct_lin_datapoint_db_const_type;

/// structure for a LIN schedule table type
typedef struct
{
	uint8_t  lin_frame_index;
	uint16_t lin_frame_delay_time_ms;
}struct_lin_schedule_line;

/// structure for a LIN schedule table database type
typedef struct
{
	uint8_t                     lin_bus_index;
	uint8_t                     lin_schedule_table_length;
	struct_lin_schedule_line* ptr_lin_schedule_table;
}struct_lin_schedule_table_db_const_type;

/// structure for LIN receive message type
typedef struct
{
	uint8_t id;
	uint8_t	length;
	uint8_t	data[MAX_LIN_RX_BUFFER_LEN];
}struct_lin_rx_msg_type;

/// structure for LIN transmit message type
typedef struct
{
	uint8_t	tx_length;
	uint8_t	tx_data[MAX_LIN_TX_BUFFER_LEN];
}struct_lin_tx_msg_type;

/// structure for LIN route gateway
typedef struct
{
	uint8_t bus_id_in;
	uint8_t bus_id_out;
}struct_lin_route_gateway_db_type;

// ********************************************************************************
// ******************************* extern variables *******************************
// ********************************************************************************
// --------------------------------------------------------------------------------
extern uint8_t ext_lin_tx_byte_check_disable;
// --------------------------------------------------------------------------------

extern uint8_t ext_dyn_lin_bux_max;
extern uint16_t ext_dyn_frame_counter_max;
extern uint16_t ext_dyn_dp_counter_max;

// ================================================================================
// extern variables
// usable for customer no dyn in the namespace
// ================================================================================
extern uint8_t ext_lin_response_error_flag[];   // reporting to the network, slave state
extern uint8_t ext_lin_tx_frame_index[];
extern uint8_t ext_ary_lin_test_frame_flag[];
extern uint8_t ext_ary_lin_timeout_flags[];

// --------------------------------------------------------------------------------
// LIN diagnose
// --------------------------------------------------------------------------------
extern uint8_t ext_current_slave_state[];
extern uint8_t ext_pending_slave_response[];

// ================================================================================
// extern variables
// ================================================================================
extern uint8_t ext_dyn_lin_route_gw_max;

extern uint8_t ext_dyn_lin_rx_len[];    // LIN message expected length
extern uint8_t ext_dyn_rx_lin_cnt[];    // LIN message received byte counter

extern uint8_t ext_dyn_lin_tx_state[];
extern uint8_t ext_dyn_lin_rx_state[];

extern uint8_t ext_dyn_lin_rx_timeout[];
extern uint8_t ext_dyn_lin_tx_frame_data_counter[];
extern uint8_t ext_dyn_rx_lin_frame_index[];
extern uint8_t ext_dyn_rx_lin_diag_response[];

extern struct_lin_rx_msg_type ext_dyn_lin_rx_message[];

extern uint8_t ext_dyn_ary_lin_frame_req_rx_flag[];

extern uint8_t* ext_dyn_mgl_lin_tx_data_ptr[];

extern uint16_t ext_dyn_crc_sum[];

extern uint32_t ext_dyn_lin_timer_rx_timeout[];

// ================================================================================
// Schedule table
// ================================================================================
extern uint8_t ext_dyn_lin_bus_sleep_flag[];
extern uint8_t ext_dyn_lin_schedule_line_index[];

extern uint32_t ext_dyn_lin_schedule_timer[];
extern uint32_t ext_dyn_lin_auto_sleep_timer[];

// ================================================================================
// Frame data byte arrays
// ================================================================================
// nothing

// ================================================================================
// Database structures
// ================================================================================
extern struct_lin_bus_db_type                   ext_lin_bus_db[];
extern struct_lin_route_gateway_db_type			ext_lin_route_gateway_db[];
extern struct_lin_frame_db_type                 ext_lin_frame_db[];
extern volatile struct_lin_frame_sniffer_type   ext_lin_sniffed_rx_frame_db[];

extern volatile const uint8_t* ext_ptr_ary_lin_diag_product_id[];

extern volatile const struct_lin_datapoint_db_const_type        lin_datapoint_db_const[];
extern volatile const struct_lin_schedule_table_db_const_type   lin_schedule_table_db_const[];

// ================================================================================
// LIN receive message queue's
// ================================================================================
extern struct_lin_rx_msg_type  ext_dyn_lin_rx_msg_queue[LIN_BUS_MAX][LIN_MSG_QUEUE_SIZE];

extern uint8_t  ext_dyn_lin_rx_msg_queue_read[];
extern uint8_t  ext_dyn_lin_rx_msg_queue_write[];

// Header Rx queue
extern uint8_t ext_dyn_lin_rx_header_queue[LIN_BUS_MAX][LIN_MSG_QUEUE_SIZE];

extern uint8_t ext_dyn_lin_rx_header_queue_read[];
extern uint8_t ext_dyn_lin_rx_header_queue_write[];

// ================================================================================
// LIN transmit message queue's
// ================================================================================
extern struct_lin_tx_msg_type ext_dyn_lin_tx_message[];

// ================================================================================
// LIN dynamic defines
// ================================================================================
extern uint8_t ext_dyn_lin_msg_queue_size;
extern uint8_t ext_dyn_lin_header_queue_size;

#endif
