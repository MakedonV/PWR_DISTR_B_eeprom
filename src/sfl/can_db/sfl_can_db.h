#ifndef SFL_CAN_DB_H
#define SFL_CAN_DB_H
/*----------------------------------------------------------------------------*/
/**
* \file         sfl_can_db.h
* \brief        this is the sfl_can_db header file which handles can db functionality
* \date         20160822
* \author       riegel
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup
* \brief This file contains functions to handle the CAN messages and put it in the respective FIFOs
* \details Depending on how the CAN message is received the appropriate function is called.
*
* A simple send function does as follows:  RX message buffer (rx interrupt triggered) -> CAN_Callback -> put
* message into RX-FIFO CAN.
*
* A simple send function does as follows: user_code.c -> user CAN message send -> put message into TX FIFO -> cyclic
* main -> send CAN message -> invoke TX message buffer -> remove from TX FIFO.
*
* A simple gateway function does as follows: RX message buffer (rx interrupt triggered) -> CAN_Callback -> put
* message into RX-FIFO CAN1 -> get from RX FIFO CAN1 -> put message into TX-FIFO CAN2 ->
* send CAN message -> invoke TX message buffer -> remove from TX FIFO CAN2.
*
*/
/*----------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "sfl_can_db_tables_data.h"

// ---------------------------------------------------------------------------------------------------
// defines
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------

/** this typedef describe function pointer used to execute given function on each can message received. */
typedef void (*callback_can_msg_receive_t)(uint8_t, bios_can_msg_typ*);

// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Initialise the can rx fifo.
* \details
*
* \pre
*
* \param    bus_id [in] const uint8_t  can bus id
* \return   void
*/
void sfl_can_db_rx_fifo_init(const uint8_t bus_id);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Initialise the can tx fifo.
* \details
*
* \pre
*
* \param    bus_id [in] const uint8_t  can bus id
* \return   void
*/
void sfl_can_db_tx_fifo_init(const uint8_t bus_id);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    cyclic trigger of tx fifo send for all can bus
* \details
*
* \pre
*
* \return   void
*/
void sfl_can_db_tx_fifo_cyclic();

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Check if tx fifo has content and send msg out.
* \details
*
* \pre
*
* \param    instance [in] const const uint8_t  can bus id
* \return   void
*/
void sfl_can_db_tx_callback( const uint8_t instance );

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Read CAN datapoint
* \details
*
* \pre
*
* \param    id       [in] const uint32_t  Datapoint
* \return   uint32_t                      value of the datapoint
*/
uint32_t sfl_can_db_get_value(const uint32_t id);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Set CAN datapoint
* \details
*
* \pre
*
* \param    id       [in] const uint32_t  CAN datapoint
* \param    wert_int [in] const uint32_t  Value
* \return   void
*/
void sfl_can_db_set_value(const uint32_t id, const uint32_t wert_int);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Read CAN datapoint on change and return the value of the datapoint
* \details
*
* \pre
*
* \param    id       [in] const uint32_t    CAN datapoint
* \param    changed  [in] uint8_t* const    Flag that will be set if the Datapoint changed
* \return   uint32_t                        value of the datapoint
*/
uint32_t sfl_can_db_get_value_on_change(const uint32_t id, uint8_t* const changed);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Set CAN datapoint transmit flag to manually send the datapoint
* \details
*
* \pre
*
* \param    id      [in] const uint16_t  CAN DB block
* \return   void
*/
void sfl_can_db_set_transmit_flag(const uint32_t id);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Get CAN block pointer
* \details
*
* \pre
*
* \param    id [in] const uint32_t    CAN block
* \return
*/
uint8_t* sfl_can_db_get_block_ptr(const uint32_t id);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    See if the given CAN block was received and if desired reset the flag
* \details
*
* \pre
*
* \param    id      [in] const uint32_t     CAN block
* \param    reset   [in] const uint8_t      Reset flag for block received
* \return   uint8_t                         "0" no message received,
*                                           "1" message received.
*/
uint8_t sfl_can_db_block_received(const uint32_t id, const uint8_t reset);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    See whether the CAN message with that datapoint is received
* \details
*
* \pre
*
* \param    id      [in] const uint32_t     CAN datapoint
* \return   uint8_t                         "0" no message received,
*                                           "1" message received, datapoint has same value,
*                                           "2" message received, datapoint has another value
*/
uint8_t sfl_can_db_test_dp_value(const uint32_t id);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*
* \pre
*
* \param    val     [in] uint32_t
* \return   uint8_t
*/
uint8_t sfl_bit_weight_32(uint32_t val);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*
* \pre
*
* \param    val     [in] uint32_t
* \return   uint8_t
*/
uint8_t sfl_bit_weight_8(uint32_t val);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*
* \pre
*
* \param    src  [in] const bios_can_msg_typ*
* \param    dst  [in] bios_can_msg_typ* const
* \return   void
*/
void sfl_os_can_copy_msg(const bios_can_msg_typ* src, bios_can_msg_typ* const dst);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    CAN Rx function
* \details
*
* \pre
*
* \param    p_bus_id    [in] const uint8_t                      CAN bus nr
* \param    ptr_can_msg [in] const struct_hal_can_frame* const  CAN frame
* \return   void
*/
//void sfl_can_db_rx_wrapper(uint8_t p_bus_id, flexcan_msgbuff_t* ptr_can_msg);
void sfl_can_db_rx_wrapper(const uint8_t p_bus_id, const struct_hal_can_frame* const ptr_can_msg);
/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Get CAN RX messages from RX fifo an put them to CAN DB
* \details
*
* \pre
*
* \return   void
*/
void sfl_can_queue_in_process(void);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*
* \pre
*
* \param    p_bus_id                  [in] const uint8_t
* \param    msg                       [in] const bios_can_msg_typ* const
* \return   enum_HAL_CAN_RETURN_VALUE
*/
enum_HAL_CAN_RETURN_VALUE sfl_can_db_tx_wrapper(const uint8_t p_bus_id, const bios_can_msg_typ* const msg);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    CAN gateway routine
* \details
*
* \pre
*
* \param    bus_id  [in] const uint8_t              CAN bus nr
* \param    msg     [in] bios_can_msg_typ* const    CAN message
* \return   uint8_t                                 Error
*/
uint8_t sfl_can_db_gateway(const uint8_t bus_id, bios_can_msg_typ* const msg);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Process received CAN messages
* \details
*
* \pre
*
* \param    bus_id [in] const uint8_t               CAN bus nr
* \param    msg    [in] bios_can_msg_typ* const     CAN message
* \return   void
*/
void sfl_can_input_block_to_db(const uint8_t bus_id, bios_can_msg_typ* const msg);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Cyclic CAN Tx check
* \details
*
* \return   void
*/
void sfl_can_db_output_to_bus( void );


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief   Controls sending for CAN messages according to given CAN bus in can db tables which are configured as transmission (Tx).
* \details Start and stop of sending for all CAN messages which are configured in can db tables.
*
* Following actions can be done:
*
* | input value 'can_bus'             | input value 'status'              | action                                                                             |
* | ----------------------------------| ----------------------------------| ---------------------------------------------------------------------------------- |
* | valid can bus from #can_block_id  | 0u (FALSE)                        | start sending of configured CAN messages (Tx) from can db tables for given CAN bus |
* | valid can bus from #can_block_id  | 1u (TRUE)<br>and all other values | stop sending of configured CAN messages (Tx) from can db tables for given CAN bus  |
* | #CAN_BUS_MAX                      | 0u (FALSE)                        | start sending of all configured CAN messages (Tx) from can db tables               |
* | #CAN_BUS_MAX                      | 1u (TRUE)<br>and all other values | stop sending of all configured CAN messages (Tx) from can db tables                |
*
* Following error feedback is provided:
*
* | return value | description                                             |
* | ------------ | ------------------------------------------------------- |
* | 1u (TRUE)    | given CAN bus is invalid<br>=> larger than #CAN_BUS_MAX |
*
* \param    can_bus [in] const uint8_t
* \param    status  [in] const uint8_t
* \return   uint8_t
*/
uint8_t sfl_can_db_stop_transmission(const uint8_t can_bus, const uint8_t status);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief   This is a macro to call #sfl_can_db_transmit_deactivate with old design for backward compatibility.
* \details Deactivates sending for all CAN messages in can db tables which are configured as transmission (Tx).
*/
#define sfl_can_db_transmit_deactivate( status ) sfl_can_db_stop_transmission( (CAN_BUS_MAX), status )

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief   Controls sending of CAN messages according to given CAN bus as a gateay on an other CAN bus.
* \details Only affects CAN messages from can db tables which are configured (in #can_block_db_const) to be send as a gateway on an other bus.
*
* Following actions can be done:
*
* | input value 'can_bus'             | input value 'status'              | action                                                       |
* | ----------------------------------| ----------------------------------| ------------------------------------------------------------ |
* | valid can bus from #can_block_id  | 0u (FALSE)                        | start sending of CAN messages as a gateway for according bus |
* | valid can bus from #can_block_id  | 1u (TRUE)<br>and all other values | stop sending of CAN messages as a gateway for according bus  |
* | #CAN_BUS_MAX                      | 0u (FALSE)                        | start sending of CAN messages as a gateway for all busses    |
* | #CAN_BUS_MAX                      | 1u (TRUE)<br>and all other values | stop sending of CAN messages as a gateway for all busses     |
*
* Following error feedback is provided:
*
* | return value | description                                             |
* | ------------ | ------------------------------------------------------- |
* | 1u (TRUE)    | given CAN bus is invalid<br>=> larger than #CAN_BUS_MAX |
*
* \param    can_bus [in] const uint8_t
* \param    status  [in] const uint8_t
* \return   uint8_t
*/
uint8_t sfl_can_db_stop_gateway_for_known_ids(const uint8_t can_bus, const uint8_t status);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief   Controls sending of configured CAN bus gateway.
* \details Only affects CAN messages on CAN busses which are configured (in #can_gateway_db_const) to be send as a gateway on an other bus.
*
* Following actions can be done:
*
* | input value 'can_bus'             | input value 'status'              | action                                                       |
* | ----------------------------------| ----------------------------------| ------------------------------------------------------------ |
* | valid can bus from #can_block_id  | 0u (FALSE)                        | start sending of CAN messages as a gateway for according bus |
* | valid can bus from #can_block_id  | 1u (TRUE)<br>and all other values | stop sending of CAN messages as a gateway for according bus  |
* | #CAN_BUS_MAX                      | 0u (FALSE)                        | start sending of CAN messages as a gateway for all busses    |
* | #CAN_BUS_MAX                      | 1u (TRUE)<br>and all other values | stop sending of CAN messages as a gateway for all busses     |
*
* Following error feedback is provided:
*
* | return value | description                                             |
* | ------------ | ------------------------------------------------------- |
* | 1u (TRUE)    | given CAN bus is invalid<br>=> larger than #CAN_BUS_MAX |
*
* \param    can_bus [in] const uint8_t
* \param    status  [in] const uint8_t
* \return   uint8_t
*/
uint8_t sfl_can_db_stop_gateway_for_unknown_ids(const uint8_t can_bus, const uint8_t status);

/*----------------------------------------------------------------------------*/
/**
* \ingroup  sfl_can
* \brief 	This function sends directly a can message
* \details
*
* \pre
*
* \param    ptr_can_handle 	[in] const struct_hal_can_handle* A pointer to th can handle.
* \param    can_id         	[in] const uint32_t Can ID
* \param    can_dlc        	[in] const uint8_t Data length ( max. 8)
* \param    can_fd          [in] const uint8_t set CAN FD byte, 0 = classic CAN, 1 = CAN FD
* \param    can_fd_brs      [in] const uint8_t
* \param    data_buffer   	[in] const uint8_t* Data of the CAN message to send
* \return   uint32_t
*/
enum_HAL_CAN_RETURN_VALUE sfl_can_send_message(const struct_hal_can_handle* ptr_can_handle, const uint32_t can_id, const uint8_t can_dlc, const uint8_t can_fd, const uint8_t can_fd_brs, const uint8_t* const data_buffer);
/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief   user callback for can message receive
* \details call this function in usercode_init()
*
* \param    callback [in] user_callback_can_msg_receive_t
* \return   void
*/
void set_callback_can_msg_receive(callback_can_msg_receive_t callback);


#endif

