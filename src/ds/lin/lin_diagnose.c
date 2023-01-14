/*----------------------------------------------------------------------------*/
/**
* \file         lin_diagnose.c
* \brief        LIN diagnose functionality
* \details      user callback functions to be able to perform a LIN specific diagnose
* \date         20151019
* \author       Geissler
*
*/
/*----------------------------------------------------------------------------*/
#include "lin_diagnose.h"

// --------------------------------------------------------------------------------
// LIN diagnose
// --------------------------------------------------------------------------------
uint8_t mgl_lin_dig_request_sid = 0x00;
uint8_t mgl_pending_negativ_response_code = 0x00;
uint8_t mgl_lin_pos_resp_user_pci = 0x00;
uint8_t mgl_fci_internal_fault_table_counter = 0;
uint8_t mgl_fci_internal_fault_table_filled_number = 0;

uint8_t mgl_lin_resp_user_data[DIAG_RESP_DATA_BUFFER_SIZE] = { 0 };

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
* \bug
* \todo
*
* \test STATUS: *** UNTESTED ***
* 	Date       | Type    | Person
*	-----------|---------|-----------
*	20151019   | Author  | Geissler
*	201xxxxx   | Editor  | xxx
*	201xxxxx   | Tester  | xxx
*/
void lin_diag_master_request(uint8_t lin_module)
{
	// Here you can write your code

	// --------------------------------------------------------------------------------
	// For example a received go to sleep command:
	// --------------------------------------------------------------------------------
	// check for a go to sleep command from master
	// {0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	// --------------------------------------------------------------------------------
	/*
	if( (ext_ary_diag_master_request_data[0] == 0x00) && (ext_ary_diag_master_request_data[1] == 0xFF)
		&& (ext_ary_diag_master_request_data[2] == 0xFF) && (ext_ary_diag_master_request_data[3] == 0xFF)
		&& (ext_ary_diag_master_request_data[4] == 0xFF) && (ext_ary_diag_master_request_data[5] == 0xFF)
		&& (ext_ary_diag_master_request_data[6] == 0xFF) && (ext_ary_diag_master_request_data[7] == 0xFF)
		)
	{
		// --------------------------------------------------------------------------------
		// a go to sleep command from master was received
		// --------------------------------------------------------------------------------
		modulhardwarecode_sleep();
	}
	else
	{
		// do nothing
	}
	*/
	ext_current_slave_state[lin_module] = SLAVE_STATE_IDLE;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
* \bug
* \todo
*
* \test STATUS: *** UNTESTED ***
* 	Date       | Type    | Person
*	-----------|---------|-----------
*	20151019   | Author  | Geissler
*	201xxxxx   | Editor  | xxx
*	201xxxxx   | Tester  | xxx
*/
uint8_t lin_diag_slave_response(uint8_t lin_module)
{
	uint8_t ret_status = LIN_STATE_ERROR;

	// --------------------------------------------------------------------------------
	// in the case of no using to get no compiler INFO set all as (void)xy;
	// --------------------------------------------------------------------------------
	(void)lin_module;

	// Here you can write your code

	// example code:
	// ext_ary_diag_slave_response_data[0] = LIN_CONFIGURED_NAD;     // NAD
	// ext_ary_diag_slave_response_data[1] = NEGATIVE_RESPONSE_LEN;  // PCI
	// ext_ary_diag_slave_response_data[2] = NEGATIVE_RESPONSE_SID;

	// ext_ary_diag_slave_response_data[3] = mgl_lin_dig_request_sid;
	// ext_ary_diag_slave_response_data[4] = mgl_pending_negativ_response_code;
	// ext_ary_diag_slave_response_data[5] = 0xFF;
	// ext_ary_diag_slave_response_data[6] = 0xFF;
	// ext_ary_diag_slave_response_data[7] = 0xFF;

	// ret_status = LIN_STATE_SUCCESS;

	return ret_status;
}

// EOF