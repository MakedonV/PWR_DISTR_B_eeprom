#ifndef SRC_USER_API_CAN_H_
#define SRC_USER_API_CAN_H_
/*----------------------------------------------------------------------------*/
/**
* \file         user_api_can.h
* \brief        Declaration of CAN functionality
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup     user_api_can CAN
* \{
* \brief        This handles the CAN functionality
* \details      Functions are provided here to work with the CAN.
*               Some functions are processed immediately, others cyclically.
*
*/
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include "hal_can.h"
#include "can_db_tables.h"

// ---------------------------------------------------------------------------------------------------
// typedefs / enums
// ---------------------------------------------------------------------------------------------------
/** This enum lists both values for CAN id types.*/
typedef enum
{
    STANDARD_ID  = 0,   ///< 11 Bit Identifier
    EXTENDED_ID         ///< 29 Bit Identifier
}enum_CAN_ID_TYPE;

/** This enum lists both values for CAN id types.*/
typedef enum
{
    CAN_BUS_OK  = 0,
    CAN_BUS_ERROR,
    CAN_BUS_OFF
}enum_CAN_ERROR_TYPE;

/** This struct defines the can error watermark with timestamp */
typedef struct
{
    enum_CAN_ERROR_TYPE last_state;         //< last state of the can transceiver
    uint32_t esr1;                          //< can error register ESR1 for advanced error diagnosis (S32K-RM.pdf p.1672)
    uint32_t time_from_start_to_err_in_ms;  //< timestamp from the start of the device in ms until the last error happened
}struct_error_watermark;

/*----------------------------------------------------------------------------*/
/**
* \brief    Get messages from CAN RX fifo manually
*
* \details  This function gets the messages in the CAN RX fifo and handles them. This is useful for usercode/graphcode which blocks the system, longer than the fifo is able to buffer incoming messages.
*
*/
void user_can_module_rx();
#define user_can_db_clear_rx_buff() user_can_module_rx()

/*----------------------------------------------------------------------------*/
/**
* \brief    Output the messages in the CAN DB and CAN TX fifo manually
*
* \details  This function processes all TX Telegrams which are ready to be sent by the determining the cycle time.
*           Next it will get all messages in the CAN TX fifo and puts them on the bus.
*           This is useful for usercode/graphcode which blocks the system, longer than the fifo is able to handle outgoing messages.
*           The runtime of the function is dependent on the quantity of CAN DB entries and manually sent CAN TX messages via user_code.
*
*/
void user_can_db_process_tx( void );

/*----------------------------------------------------------------------------*/
/**
* \brief    Sends the CAN message immediately
*
* \details  Send function. Returns HAL_CAN_ERROR_FIFO_FULL if the fifo is full and sending is not possible.
*
* \param    can_bus     [in] const uint8_t             CAN bus ID
* \param    can_id      [in] const uint32_t            CAN ID
* \param    id_type     [in] const enum_CAN_ID_TYPE    CAN ID type, STANDARD or EXTENDED
* \param    can_dlc     [in] const uint8_t             CAN Data Length Code, Number of bytes in the data field
* \param    data_byte_0 [in] const uint8_t             Data_0 to be transmitted
* \param    data_byte_1 [in] const uint8_t             Data_1 to be transmitted
* \param    data_byte_2 [in] const uint8_t             Data_2 to be transmitted
* \param    data_byte_3 [in] const uint8_t             Data_3 to be transmitted
* \param    data_byte_4 [in] const uint8_t             Data_4 to be transmitted
* \param    data_byte_5 [in] const uint8_t             Data_5 to be transmitted
* \param    data_byte_6 [in] const uint8_t             Data_6 to be transmitted
* \param    data_byte_7 [in] const uint8_t             Data_7 to be transmitted
*
* \return   enum_HAL_CAN_RETURN_VALUE            Return code
*/
enum_HAL_CAN_RETURN_VALUE user_can_msg_send(const uint8_t can_bus, const uint32_t can_id, const enum_CAN_ID_TYPE id_type, const uint8_t can_dlc, const uint8_t data_byte_0, const uint8_t data_byte_1, const uint8_t data_byte_2, const uint8_t data_byte_3, const uint8_t data_byte_4, const uint8_t data_byte_5, const uint8_t data_byte_6, const uint8_t data_byte_7);
#define user_can_send_msg( can_bus,can_id,id_type,can_dlc,data_byte_0,data_byte_1,data_byte_2,data_byte_3,data_byte_4,data_byte_5,data_byte_6,data_byte_7 ) user_can_msg_send( can_bus,can_id,id_type,can_dlc,data_byte_0,data_byte_1,data_byte_2,data_byte_3,data_byte_4,data_byte_5,data_byte_6,data_byte_7 )


/*----------------------------------------------------------------------------*/
/**
* \brief    Sends the CAN/CAN-FD message buffer immediately
*
* \details  Send function for CAN/CAN-FD. If can is not configured as FD, only the first 8 byte will be transfered.
*
* \param    can_bus     [in] uint8_t                CAN bus ID
* \param    can_id      [in] uint32_t               CAN ID
* \param    id_type     [in] enum_CAN_ID_TYPE       ID type, normal or extended
* \param    can_dlc     [in] uint8_t                CAN-FD DLC (0-15), not data length in Byte
* \param    data_buffer [in] uint8_t *const         Data buffer to be transmitted
*
* \return   uint8_t                                 Return code
*/
enum_HAL_CAN_RETURN_VALUE user_can_msg_buffer_send(const uint8_t can_bus, const uint32_t can_id, const enum_CAN_ID_TYPE id_type, const uint8_t can_dlc, uint8_t *const data_buffer);
#define user_can_send_msg_buffer( can_bus,can_id,id_type,can_dlc,data_buffer ) user_can_msg_buffer_send( can_bus,can_id,id_type,can_dlc,data_buffer )

/*----------------------------------------------------------------------------*/
/**
* \brief    Sends the CAN-FD message buffer immediately
*
* \details  Send function
*
* \param    can_bus     [in] uint8_t                CAN bus ID
* \param    can_id      [in] uint32_t               CAN ID
* \param    id_type     [in] enum_CAN_ID_TYPE       ID type, normal or extended
* \param    can_dlc     [in] uint8_t                CAN DLC, not data length in Byte
* \param    can_fd_brs  [in] uint8_t                set the BRS flag in CAN FD frame
* \param    data_buffer [in] uint8_t *const         Data buffer to be transmitted
*
* \return   uint8_t                                 Return code
*/
enum_HAL_CAN_RETURN_VALUE user_can_fd_msg_buffer_send(uint8_t can_bus, uint32_t can_id, enum_CAN_ID_TYPE id_type, uint8_t can_dlc, uint8_t can_fd_brs, uint8_t *const data_buffer);

/*----------------------------------------------------------------------------*/
/**
* \brief    Sends CAN data point value out cyclically
*
* \param    id        [in] const uint32_t     CAN Datapoint ID from can_db_tables.h
* \param    value_int [in] const uint32_t     Data value to be transmitted
*
*/
void user_can_db_set_value(const uint32_t id, const uint32_t value_int);
#define user_can_db_set_signal_value( id,value_int ) user_can_db_set_value( id,value_int )


/*----------------------------------------------------------------------------*/
/**
* \brief    Get CAN data point value, cyclic execution
*
* \param    id       [in] const uint32_t      CAN Datapoint ID from can_db_tables.h
*
* \return   uint32_t                          Return code
*/
uint32_t user_can_db_get_value(const uint32_t id);
#define user_can_db_get_signal_value( id ) user_can_db_get_value( id )


/*----------------------------------------------------------------------------*/
/**
* \brief    See if a CAN DB block was received and reset the associated flag
*
* \param    can_block_id        [in] const uint32_t         name of the defined CAN DB block
*                                                           see can_block_db_const in can_db_tables.c
* \param    reset               [in] const uint8_t          reset = 1, do not reset = 0
*
* \return   uint8_t                                         "0" no message received,
*                                                           "1" message received.
*/
uint8_t user_can_db_block_received(const uint32_t can_block_id, const uint8_t reset);


/*----------------------------------------------------------------------------*/
/**
* \brief    See whether the CAN message with that datapoint is received
*
* \param    can_dp_id          [in] const uint32_t      name of the defined CAN DB datapoint,
*                                                       see can_datenpunkt_db_const in can_db_tables.c
*
* \return   uint8_t                                     "0" no message received,
*                                                       "1" message received, datapoint has same value,
*                                                       "2" message received, datapoint has another value
*/
uint8_t user_can_db_test_dp_value(const uint32_t can_dp_id);


/*----------------------------------------------------------------------------*/
/**
* \brief    Set the transmission flag for the given CAN DB datapoint, to manually send a configured datapoint. Associated CAN block must be configured to transmit.
*
* \param    can_dp_id          [in] const uint32_t      name of the defined CAN DB datapoint,
*                                                       see can_datenpunkt_db_const in can_db_tables.c
*
*/
void user_can_db_send_dp(const uint32_t can_dp_id);
#define user_can_db_set_transmit_flag(...) user_can_db_send_dp(__VA_ARGS__)

/*----------------------------------------------------------------------------*/
/**
* \brief    Return the value of the given datapoint only when its value is changed. If the datapoint doesn't change, the current value will be returned
*
* \param    can_dp_id           [in] const uint32_t     name of the defined CAN DB datapoint,
*                                                       see can_datenpunkt_db_const in can_db_tables.c
* \param    buffer_dp_value     [in] const uint32_t*    buffer in which the datapoint value will be saved in.
*
* \return   uint8_t                                     0 value didn't change,
*                                                       1 value changed
*/
uint8_t user_can_db_get_value_on_change(const uint32_t can_dp_id, uint32_t* const buffer_dp_value);

/*----------------------------------------------------------------------------*/
/**
* \brief    Pause or restart the CAN telegram transmit function of the can db.
*
* \param    can_bus   [in] const uint8_t  CAN bus nr.
* \param    status    [in] const uint8_t  stop = 1, start = 0
*
* \return   uint8_t                       Return code
*/
uint8_t user_can_db_stop_transmission(const uint8_t can_bus, const uint8_t status);


/*----------------------------------------------------------------------------*/
/**
* \brief    Pause or restart the CAN telegram transmit function of the can db for all can bus.
*
* \param    status    [in] const uin8_t stop = 1, start = 0
*
* \return   uint8_t                     Return code
*/
uint8_t user_can_db_transmit_deactivate( const uint8_t status );


/*----------------------------------------------------------------------------*/
/**
* \brief    Pause or restart the CAN bus to CAN bus direct gateway function.
*
* \param    can_bus   [in] const uint8_t  CAN bus nr.
* \param    status    [in] const uint8_t  stop = 1, start = 0
*
* \return   uint8_t                     Return code
*/
uint8_t user_can_db_stop_gateway_for_known_ids( const uint8_t can_bus, const uint8_t status );


/*----------------------------------------------------------------------------*/
/**
* \brief    Pause or restart the id based gateway function of the can db.
*
* \param    can_bus   [in] const uint8_t  CAN bus nr.
* \param    status    [in] const uint8_t  stop = 1, start = 0
*
* \return   uint8_t                     Return code
*
**/
uint8_t user_can_db_stop_gateway_for_unknown_ids(const uint8_t can_bus, const uint8_t status);


/*----------------------------------------------------------------------------*/
/**
* \brief    return can error watermark with timestamp
*
* \param    can_bus   [in] const uint8_t                    CAN bus nr.
* \param    watermark [in] struct_error_watermark* const    pointer to where watermark is saved
**/
void user_can_get_error(const uint8_t can_bus, struct_error_watermark* const watermark);

/*----------------------------------------------------------------------------*/
/**
* \brief    Set the bootloader and application baud rate. This function will set the bootloader and application baud rate
* 			to the baud rate given with the "baudrate" parameter. This baud rate change will only be temporary until the
* 			next restart of the module. Set "CAN_BL_BAUDRATE_MODE" define in user_code.h to "1" for the baud rate change
* 			to be permanently. Because this function overwrites the EEPROM, it should be called only once.
*
* \param    baudrate   [in] const uint8_t		baud rate which should be used and stored to EEPROM, select from the following enums:
* 												HAL_CAN_BAUD_1000
* 												HAL_CAN_BAUD_500
* 												HAL_CAN_BAUD_250
* 												HAL_CAN_BAUD_125
* 												HAL_CAN_BAUD_100
* 												HAL_CAN_BAUD_800
* 												HAL_CAN_BAUD_83
* 												HAL_CAN_BAUD_50
* 												HAL_CAN_BAUD_33
* 												HAL_CAN_BAUD_20
* 												HAL_CAN_BAUD_10
*
* \return 	uint8_t								Return code
*
**/
uint8_t user_can_set_bl_baudrate(uint8_t const baudrate);


#endif /* SRC_USER_API_CAN_H_ */
/** \} */

