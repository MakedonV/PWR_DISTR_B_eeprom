/*
 * user_api_can.c
 *
 *  Created on: 12.09.2019
 *      Author: VGoertz
 */
#include "user_api_can.h"
#include "user_api_eeprom.h"
#include "sfl_can_db.h"
#include "modulhardwarecode.h"
#include "can_app.h"
#include "sfl_bl_protocol_s32k.h"

// 0 < No Error. Command Succeeded.
// 1 < Not further described error code.
// 2 < Error opening a can communication channel of the given can interface.
// 3 < All channels of this interface are already in use (Resource busy).
// 4 < No message available.
// 5 < Error setting the can filter.
// 6 < Given channel is not valid. This can happen when channel not opened yet.
// 7 < Reading failed. Happens only when a message is available.
// 8 < Writing failed.
// 9 < Given channel is actually busy.
// 10 < The given receive channel does not contain data (channel is empty).
// 11 < Receive channel was overwritten (Overflow).
// 12 < Error at setting the callback.
// 13 < Error at setting the baudrate.
// 14 < Function not implemented.
// 15 < Invalid DLC.


extern volatile struct_can_app_error can_error_handle[];

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
void user_can_module_rx( void ){
	sfl_can_queue_in_process();
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
void user_can_db_process_tx( void )
{
    sfl_can_db_output_to_bus();
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_CAN_RETURN_VALUE user_can_msg_send(const uint8_t can_bus, const uint32_t can_id, const enum_CAN_ID_TYPE id_type, const uint8_t can_dlc, const uint8_t data_byte_0, const uint8_t data_byte_1, const uint8_t data_byte_2, const uint8_t data_byte_3, const uint8_t data_byte_4, const uint8_t data_byte_5, const uint8_t data_byte_6, const uint8_t data_byte_7)
{
    enum_HAL_CAN_RETURN_VALUE ret_val = HAL_CAN_ERROR_GENERAL;
    struct_hal_can_handle hnd;
    sfl_can_get_handle(can_bus, &hnd);

    // Clear the the frame format bit and set it to the given value
    uint32_t can_id_complete = ( (can_id & ~(1<<31)) | (id_type<<31) );

    uint8_t data_buffer[8] = {0};
    data_buffer[0] = data_byte_0;
    data_buffer[1] = data_byte_1;
    data_buffer[2] = data_byte_2;
    data_buffer[3] = data_byte_3;
    data_buffer[4] = data_byte_4;
    data_buffer[5] = data_byte_5;
    data_buffer[6] = data_byte_6;
    data_buffer[7] = data_byte_7;

    if (NULL != hnd.can_handle_ptr)
    {
    	ret_val = sfl_can_send_message( &hnd, can_id_complete, can_dlc, 0, 0, data_buffer);
    }


    return ret_val;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_CAN_RETURN_VALUE user_can_msg_buffer_send(const uint8_t can_bus, const uint32_t can_id, const enum_CAN_ID_TYPE id_type, const uint8_t can_dlc, uint8_t *const data_buffer)
{
    enum_HAL_CAN_RETURN_VALUE ret_val = HAL_CAN_ERROR_GENERAL;
    struct_hal_can_handle hnd;
    sfl_can_get_handle(can_bus, &hnd);
    uint32_t temp_id;

    // Clear the the frame format bit and set it to the given value
    temp_id = ( (can_id & ~(1<<31)) | (id_type<<31) );

    if (NULL != hnd.can_handle_ptr)
    {
        ret_val = sfl_can_send_message( &hnd, temp_id, can_dlc, 0, 0, data_buffer);
    }

    return ret_val;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_CAN_RETURN_VALUE user_can_fd_msg_buffer_send(uint8_t can_bus, uint32_t can_id, enum_CAN_ID_TYPE id_type, uint8_t can_dlc, uint8_t can_fd_brs, uint8_t *const data_buffer)
{
    enum_HAL_CAN_RETURN_VALUE ret_val = HAL_CAN_ERROR_GENERAL;
    struct_hal_can_handle hnd;
    sfl_can_get_handle(can_bus, &hnd);

    // Clear the the frame format bit and set it to the given value
    can_id = ( (can_id & ~(1<<31)) | (id_type<<31) );

    if (NULL != hnd.can_handle_ptr)
    {
        // invoke sfl function with CAN FD flag
        ret_val = sfl_can_send_message( &hnd, can_id, can_dlc, 1, can_fd_brs, data_buffer);
    }

    return ret_val;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
void user_can_db_set_value(const uint32_t id, const uint32_t value_int)
{
    sfl_can_db_set_value(id, value_int);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_can_db_get_value(const uint32_t id)
{
    return sfl_can_db_get_value(id);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint8_t user_can_db_block_received(const uint32_t can_block_id, const uint8_t reset)
{
    return sfl_can_db_block_received( can_block_id, reset );
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint8_t user_can_db_test_dp_value(const uint32_t can_dp_id)
{
    return sfl_can_db_test_dp_value( can_dp_id );
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
void user_can_db_send_dp(const uint32_t can_dp_id)
{
    sfl_can_db_set_transmit_flag( can_dp_id );
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint8_t user_can_db_get_value_on_change(const uint32_t can_dp_id, uint32_t* const buffer_dp_value)
{
    uint8_t changed = 0;
    uint8_t ret_val;

    // readback the value, if the datapoint doesn't change, the current value will be returned
    *buffer_dp_value = sfl_can_db_get_value_on_change(can_dp_id, &changed);

    // check if given flag was changed
    if (!changed)
    {
        ret_val = 0;
    }
    else
    {
        ret_val = 1;
    }

    return ret_val;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint8_t user_can_db_transmit_deactivate( const uint8_t status )
{
    return sfl_can_db_transmit_deactivate( status );
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint8_t user_can_db_stop_transmission(const uint8_t can_bus, const uint8_t status)
{
    return sfl_can_db_stop_transmission( can_bus, status );
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint8_t user_can_db_stop_gateway_for_known_ids( const uint8_t can_bus, const uint8_t status )
{
    return sfl_can_db_stop_gateway_for_known_ids( can_bus, status );

}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint8_t user_can_db_stop_gateway_for_unknown_ids(const uint8_t can_bus, const uint8_t status)
{
    return sfl_can_db_stop_gateway_for_unknown_ids( can_bus, status );
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
void user_can_get_error(uint8_t can_bus, struct_error_watermark* const watermark)
{
    // return the latest error and timestamp, whe it occured
    // return state of can fifos

    watermark->esr1 = can_error_handle[can_bus].last_can_error;
    watermark->time_from_start_to_err_in_ms = can_error_handle[can_bus].time_from_start_to_error;

    if(watermark->esr1 & CAN_ESR1_BOFFINT_MASK )
    {
        watermark->last_state = CAN_BUS_OFF;
    }
    else if((watermark->esr1 & CAN_ESR1_ERRINT_MASK ) || (watermark->esr1 & CAN_ESR1_RXWRN_MASK ) || (watermark->esr1 & CAN_ESR1_TXWRN_MASK ) )
    {
        // at least one of the following errors occured: BIT1ERR, BIT0ERR, ACKERR, CRCERR, FRMERR, STFERR
        watermark->last_state = CAN_BUS_ERROR;
    }
    else
    {
        watermark->last_state = CAN_BUS_OK;
    }
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*   
* \endinternal
*
*/
uint8_t user_can_set_bl_baudrate(uint8_t const baudrate)
{
    static uint8_t ret;
    struct_hal_can_handle ptr_handle;
    uint16_t interface = user_eeprom_read_bl_can_bus(); // Get the current BL CAN interface from the EEPROM

    sfl_can_get_handle(interface, &ptr_handle);

    // we convert the baudrate to one of the predefined baudrates
    if ( (baudrate > HAL_CAN_BAUD_INVALID) && (baudrate < HAL_CAN_BAUD_MAX) )
    {
    	// Set the application baud rate
        hal_can_set_baudrate(&ptr_handle, baudrate);
        // use predefined baud rate
    	uint8_t tmp_baudrate = sfl_can_db_tables_convert_baud(baudrate,  1);
    	// Set the baud rate of the bootloader
        ret = sfl_bl_protocol_s32k_set_baudrate(&ptr_handle, tmp_baudrate);
    }
    else
    {
        ret = HAL_CAN_BAUD_INVALID;
    }
    return ret;
}


















