/*----------------------------------------------------------------------------*/
/**
 * \file         sfl_can_db.c
 * \brief        this is the sfl_can_db source file which handles can db functionality
 * \details
 * \date         20160822
 * \author       riegel
 *
 */
/*----------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "sfl_db.h"
#include "sfl_can_db.h"
#include "sfl_bl_protocol.h"

// ---------------------------------------------------------------------------------------------------
// tables
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// module globals
// ---------------------------------------------------------------------------------------------------
uint32_t Save_can_db_test_dp_value = 0;
extern volatile const can_datenpunkt_db_const_typ can_datenpunkt_db_const[];
extern can_block_db_ram_typ can_block_db_ram[];
extern volatile const can_block_db_const_typ can_block_db_const[];
extern can_db_typ can_db;
extern volatile const can_gateway_db_const_typ can_gateway_db_const[];
extern volatile const can_bus_db_const_typ can_bus_db_const[];

static callback_can_msg_receive_t callback_can_msg_receive = NULL_PTR;



/*----------------------------------------------------------------------------*/
/**
* \internal
*   Init rx fifo for given can instance
* \endinternal
*
*
* \test STATUS: *** UNTESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20200624   | Author  | fzeller
*/
void sfl_can_db_rx_fifo_init(const uint8_t bus_id)
{
    // get copies of the sfl_fifo_frames
    struct_sfl_can_fifo_frame *temp_fifo_frame = can_fifo_config_actual[bus_id]->ptr_rx_fifo_buffer;

    // iterate through the fifo sfl_fifo_frame and point to the correct payload data array
    for(uint8_t i=0; i<sfl_can_db_get_fifo_size(bus_id, 0); i++)
    {
        temp_fifo_frame->header.ptr_data = temp_fifo_frame->data;
        ++temp_fifo_frame;
    }

    // NOTE: sizeof could be unsafe in pre-compiled libs
    sfl_fifo_init( can_fifo_config_actual[bus_id]->rx_fifo_config, can_fifo_config_actual[bus_id]->rx_fifo_size, sizeof(struct_sfl_can_fifo_frame));

}

/*----------------------------------------------------------------------------*/
/**
* \internal
*   Init tx fifo for given can instance
* \endinternal
*
*
* \test STATUS: *** UNTESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20200624   | Author  | fzeller
*/
void sfl_can_db_tx_fifo_init(const uint8_t bus_id)
{
    // get copies of the sfl_fifo_frames
    struct_sfl_can_fifo_frame *temp_fifo_frame = can_fifo_config_actual[bus_id]->ptr_tx_fifo_buffer;

    // iterate through the fifo sfl_fifo_frame and point to the correct payload data array
    for(uint8_t i=0; i<sfl_can_db_get_fifo_size(bus_id, 1); i++)
    {
        temp_fifo_frame->header.ptr_data = temp_fifo_frame->data;
        ++temp_fifo_frame;
    }

    // NOTE: sizeof could be unsafe in pre-compiled libs
    sfl_fifo_init( can_fifo_config_actual[bus_id]->tx_fifo_config, can_fifo_config_actual[bus_id]->tx_fifo_size, sizeof(struct_sfl_can_fifo_frame));

}

/*----------------------------------------------------------------------------*/
/**
* \internal
*   cyclic trigger of tx fifo send for all can bus
* \endinternal
*
*
* \test STATUS: *** UNTESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20200624   | Author  | fzeller
*/
void sfl_can_db_tx_fifo_cyclic()
{
    volatile uint8_t interface = 0;

    // iterate through all available can buses
    for ( interface = 0; interface < CAN_BUS_MAX; interface++ )
    {
        sfl_can_db_tx_callback( interface );
    }
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*   Check if tx fifo has content and send msg out.
* \endinternal
*
*
* \test STATUS: *** UNTESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20200624   | Author  | fzeller
*/
void sfl_can_db_tx_callback( const uint8_t instance )
{
    // initial situation: tx callback -> tx msg from mb was send successfully -> a msg box should be free
    enum_SFL_FIFO_ERROR_CODES sfl_fifo_err = SFL_FIFO_ERROR_UNKNOWN;
    struct_hal_can_handle temp_can_handle;
    temp_can_handle.can_handle_number = instance;
    struct_sfl_can_fifo_frame temp_can_msg;

    // make sure the process of checking, sending and removing an element is not interrupted. Could else lead to messages put out multiple times or even skipped
    hal_sys_disable_all_interrupts();
    //get messages from fifo
    sfl_fifo_err = sfl_fifo_get_preview( can_fifo_config_actual[instance]->tx_fifo_config, (uint8_t*) &temp_can_msg, (uint8_t*) can_fifo_config_actual[instance]->ptr_tx_fifo_buffer );
    if ( sfl_fifo_err == SFL_FIFO_ERROR_NONE )
    { // fifo contains data
        temp_can_msg.header.ptr_data = temp_can_msg.data; //let the hal_can_frame pointer know where its data payload is.
        if( hal_can_send( &temp_can_handle, &temp_can_msg.header ) == HAL_CAN_OK ){
            // sending was successful, remove msg from fifo
            sfl_fifo_get( can_fifo_config_actual[instance]->tx_fifo_config, (uint8_t*) &temp_can_msg, (uint8_t*) can_fifo_config_actual[instance]->ptr_tx_fifo_buffer );
        }
    }
    else
    {   // fifo empty or busy
        // @TODO CAN ERROR WATERMARK
    }
    hal_sys_enable_all_interrupts();
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* Read CAN datapoint
* \endinternal
*
*
* \test STATUS: *** TESTED ***
*   Date       | Type    | Person
*   -----------|---------|-----------
*   20160823   | Author  | riegel
*/
uint32_t sfl_can_db_get_value(const uint32_t id)
{
    can_datenpunkt_db_const_typ dp = can_datenpunkt_db_const[id];

    return sfl_db_get_signal_value_from_data_block(can_block_db_ram[dp.nr_can_block].msg.data, dp.pos_bit_0, dp.bit_laenge, dp.data_format);
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 * Set CAN datapoint
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 */
void sfl_can_db_set_value(const uint32_t id, const uint32_t wert_int)
{
    can_datenpunkt_db_const_typ dp = can_datenpunkt_db_const[id];

    sfl_db_put_signal_value_to_data_block(wert_int, can_block_db_ram[dp.nr_can_block].msg.data, dp.pos_bit_0, dp.bit_laenge, dp.data_format);
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 * Read CAN datapoint on change
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 */
uint32_t sfl_can_db_get_value_on_change(const uint32_t id, uint8_t* const changed)
{
    uint32_t u_long, last;
    uint8_t i = 0;

    can_datenpunkt_db_const_typ dp = can_datenpunkt_db_const[id];
    u_long = sfl_db_get_signal_value_from_data_block(can_block_db_ram[dp.nr_can_block].msg.data, dp.pos_bit_0, dp.bit_laenge, dp.data_format);
    last = sfl_db_get_signal_value_from_data_block(can_block_db_ram[dp.nr_can_block].last_data, dp.pos_bit_0, dp.bit_laenge, dp.data_format);
    *changed = (u_long != last);

    if (*changed)
	{
        // @TODO: does not work with can-fd
        while (i < 8)
        {
            can_block_db_ram[dp.nr_can_block].last_data[i] = can_block_db_ram[dp.nr_can_block].msg.data[i];
            ++i;
        }
    }
    return u_long;
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 * Set CAN datapoint transmit flag
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 *   20210715   | Editor  | daniel.leer
 */
void sfl_can_db_set_transmit_flag(const uint32_t id)
{
    can_datenpunkt_db_const_typ dp = can_datenpunkt_db_const[id];

    if (can_block_db_const[dp.nr_can_block].tx == 1)
    {
        can_block_db_ram[dp.nr_can_block].transmit = 1;
    }
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 * Get CAN block pointer
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 */
uint8_t* sfl_can_db_get_block_ptr(const uint32_t id)
{
    return can_block_db_ram[id].msg.data;
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 * CAN block received
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 */
uint8_t sfl_can_db_block_received(const uint32_t id, const uint8_t reset)
{
    uint8_t retval = can_block_db_ram[id].received;

    if (reset)
    {
        can_block_db_ram[id].received = 0;
    }
    return retval;
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 * See whether the CAN message with that datapoint is received
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 */
uint8_t sfl_can_db_test_dp_value(const uint32_t id)
{
    uint8_t ret = 0;    // kein neues Telegramm
    uint32_t u_long;

    // --------------------------------------------------------------------------------
    // Wert aus CAN-Block lesen
    // --------------------------------------------------------------------------------
    can_datenpunkt_db_const_typ dp = can_datenpunkt_db_const[id];

    if (sfl_can_db_block_received(dp.nr_can_block, 1) == 1)
    {
        u_long = sfl_db_get_signal_value_from_data_block(can_block_db_ram[dp.nr_can_block].msg.data, dp.pos_bit_0, dp.bit_laenge, dp.data_format);

        if (u_long != Save_can_db_test_dp_value)
        {
            ret = 2;
        }
        else
        {
            ret = 1;
        }
        Save_can_db_test_dp_value = u_long;
    }
    return ret;
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 *
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 */
uint8_t sfl_bit_weight_32(uint32_t val)
{
    uint8_t i = 32, retval = 0;

    while (i-- > 0)
    {
        if (val & 1)
        {
            ++retval;
        }
        val = val >> 1;
    }
    return retval;
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 *
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 */
uint8_t sfl_bit_weight_8(uint32_t val)
{
    uint8_t i = 8, retval = 0;

    while (i-- > 0)
    {
        if (val & 1)
        {
            ++retval;
        }

        val = val >> 1;
    }
    return retval;
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 *
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 */
void sfl_os_can_copy_msg(const bios_can_msg_typ* const src, bios_can_msg_typ* const dst)
    {

    dst->id = src->id;
    dst->id_ext = src->id_ext;
    dst->remote_tx = src->remote_tx;
    dst->len = src->len;
    dst->prty = src->prty;
    dst->can_fd = src->can_fd;
    dst->can_fd_brs = src->can_fd_brs;

    memcpy(&dst->data, &src->data, src->len);
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 * CAN Rx function
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 */
//void sfl_can_db_rx_wrapper(uint8_t p_bus_id, flexcan_msgbuff_t* ptr_can_msg)
void sfl_can_db_rx_wrapper(const uint8_t p_bus_id, const struct_hal_can_frame* const ptr_can_msg)
{
    enum_SFL_FIFO_ERROR_CODES retval = SFL_FIFO_ERROR_MAX;

    struct_sfl_can_fifo_frame l_can_msg;

    // Setup the hal_can_frame structure
    memcpy(&l_can_msg.header, ptr_can_msg, sizeof(struct_hal_can_frame));
    l_can_msg.header.ptr_data  = NULL;  // FIFO does a shallow copy and we don't want to copy the address, so set to NULL.
    memcpy(l_can_msg.data, ptr_can_msg->ptr_data, hal_can_dlc_to_len(ptr_can_msg->can_dlc));

    // put message into fifo
    retval = sfl_fifo_put( can_fifo_config_actual[p_bus_id]->rx_fifo_config, (uint8_t*) &l_can_msg, (uint8_t*) can_fifo_config_actual[p_bus_id]->ptr_rx_fifo_buffer);
    if(retval != SFL_FIFO_ERROR_NONE)
	{
        // fifo full
        //  @TODO CAN ERROR WATERMARK

    }

    (void)sfl_bl_protocol_s32k_process_rx_msg(ptr_can_msg);

}


/*----------------------------------------------------------------------------*/
/**
* \internal
*   Get CAN RX messages from RX fifo an put them to CAN DB
* \endinternal
*
*
* \test STATUS: *** UNTESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20200624   | Author  | fzeller
*/
void sfl_can_queue_in_process(void)
{
    uint8_t instance = 0;
    enum_SFL_FIFO_ERROR_CODES retval = SFL_FIFO_ERROR_UNKNOWN;
    bios_can_msg_typ temp_can_msg_typ;
    struct_sfl_can_fifo_frame temp_can_msg;

    // walk through all can busses
    for( instance = 0; instance < can_db.BUS_MAX; instance++ )
    {
        retval = sfl_fifo_get(can_fifo_config_actual[instance]->rx_fifo_config, (uint8_t*) &temp_can_msg, (uint8_t*) can_fifo_config_actual[instance]->ptr_rx_fifo_buffer);
        if(retval == SFL_FIFO_ERROR_NONE)
        {
            temp_can_msg_typ.id         = temp_can_msg.header.can_id & 0x7FFFFFFF;
            temp_can_msg_typ.id_ext     = ((temp_can_msg.header.can_id>>31) & 0x1 );
            temp_can_msg_typ.len        = hal_can_dlc_to_len(temp_can_msg.header.can_dlc);
            temp_can_msg_typ.can_fd     = temp_can_msg.header.can_fd;
            temp_can_msg_typ.can_fd_brs = temp_can_msg.header.can_fd_brs;
            memcpy(temp_can_msg_typ.data, temp_can_msg.data, temp_can_msg_typ.len);
            sfl_can_input_block_to_db(instance, &temp_can_msg_typ);
        }
    }
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 * CAN Tx function
 * with fifo
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 */
enum_HAL_CAN_RETURN_VALUE sfl_can_db_tx_wrapper(const uint8_t p_bus_id, const bios_can_msg_typ* const msg)
{
    struct_sfl_can_fifo_frame l_can_msg;

    enum_HAL_CAN_RETURN_VALUE error = HAL_CAN_ERROR_GENERAL;
    enum_SFL_FIFO_ERROR_CODES retval;

    // Setup the hal_can_frame structure
    l_can_msg.header.can_id = msg->id | ((uint32_t) msg->id_ext << 31);
    l_can_msg.header.can_dlc = hal_can_len_to_dlc(msg->len);
    l_can_msg.header.can_fd = msg->can_fd;
    l_can_msg.header.can_fd_brs  = msg->can_fd_brs;
    l_can_msg.header.ptr_data  = NULL;  // FIFO does a shallow copy and we don't want to copy the address, so set to NULL.
    memcpy(l_can_msg.data, msg->data, msg->len);

    // put message into fifo
    retval = sfl_fifo_put( can_fifo_config_actual[p_bus_id]->tx_fifo_config, (uint8_t*) &l_can_msg, (uint8_t*) can_fifo_config_actual[p_bus_id]->ptr_tx_fifo_buffer);
    if ( retval != SFL_FIFO_ERROR_NONE )
    {   //fifo full or busy
        error = HAL_CAN_ERROR_WHILE_WRITING;
        //  @TODO CAN ERROR WATERMARK
    }
    else
    {
        error = HAL_CAN_OK;
    }
    return error;
}


/*----------------------------------------------------------------------------*/
/**
 * \internal
 * CAN gateway routine
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 */
uint8_t sfl_can_db_gateway(const uint8_t bus_id, bios_can_msg_typ* const msg)
{
    uint8_t i; // dst_hw_module;
    enum_HAL_CAN_RETURN_VALUE error = HAL_CAN_OK;

    // --------------------------------------------------------------------------------
    // DR20100908 Achtung! CAN_GATEWAY_DB_MAX wurde bisher als #define in der vorkompilierten Lib verwendet!!!
    // --------------------------------------------------------------------------------
    for (i=0; i<CAN_GATEWAY_DB_MAX; ++i)
    //for ( i = 0; i < can_db.GATEWAY_DB_MAX; i++ )
	{
        // --------------------------------------------------------------------------------
        // Bus-ID gefunden, auf die weitergeleitet werden soll
        // --------------------------------------------------------------------------------
        if (bus_id == can_gateway_db_const[i].bus_id_in)
        {
//            dst_hw_module = can_bus_db_const[can_gateway_db_const[i].bus_id_out].hw_module_id;

            // --------------------------------------------------------------------------------
            // send msg
            // --------------------------------------------------------------------------------
            error = sfl_can_db_tx_wrapper(can_bus_db_const[can_gateway_db_const[i].bus_id_out].hw_module_id, msg);

            if(error != HAL_CAN_OK)
			{
                //  @TODO CAN ERROR WATERMARK
            }
        }
    }

    return (error != HAL_CAN_OK);
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 * Process received CAN messages
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 */
void sfl_can_input_block_to_db(const uint8_t bus_id, bios_can_msg_typ* const msg)
{
    uint8_t i;
    uint32_t tmp_mask;
    enum_HAL_CAN_RETURN_VALUE send_error = HAL_CAN_ERROR_GENERAL;

    // --------------------------------------------------------------------------------
    // Alle CAN-Bloecke durchgehen
    // --------------------------------------------------------------------------------
    //@TODO: io-def dependent?
    for (i = 0; i < dyn_CAN_BLOCK_MAX; i++)
    {
        // --------------------------------------------------------------------------------
        // Gehoert CAN-Block zu dieser Bus-ID und ist es ein Rx-Block und stimmt das Format 11bit/29bit?
        // --------------------------------------------------------------------------------
        if (bus_id == can_block_db_const[i].bus_id && can_block_db_const[i].tx == 0 && msg->id_ext == can_block_db_const[i].can_id_ext)
        {
            tmp_mask = ~can_block_db_const[i].can_id_mask;

            // --------------------------------------------------------------------------------
            // Stimmt die CAN-ID?
            // --------------------------------------------------------------------------------
            if ((msg->id == can_block_db_const[i].can_id)
            // Oder mit Maske, findet eine Uebereinstimmung der "Kernnibbles" statt?
                    || (can_block_db_const[i].can_id_mask && ((msg->id & tmp_mask) == (can_block_db_const[i].can_id & tmp_mask))))
            {

                // --------------------------------------------------------------------------------
                // Ohne Mux_length Angabe direkt weitermachen, ansonsten pruefen, ob der Mux-Wert uebereinstimmt und nur dann weitermachen
                // --------------------------------------------------------------------------------
                if (!can_block_db_const[i].mux_length
                        || (sfl_db_get_signal_value_from_data_block(&msg->data[0], can_block_db_const[i].mux_start, can_block_db_const[i].mux_length, DF_INTEL) == can_block_db_const[i].mux_val))
                {
                    // Wenn die SA-Adresse bei dieser 29bit-ID geprueft werden soll, die Referenz-SA in der ID an der richtigen Stelle enthalten ist und die aktuelle SA
                    // in der empfangenen ID NICHT vorhanden ist, dann ignoriere diese ID.
                    // Hinweis: Bei Rx-Botschaften befindet sich die SA an Bit 8-15 und bei Tx-Botschaften an Bit 0-7.
                    if ((can_db.sa_active && msg->id_ext && (can_block_db_const[i].can_id & 0xFF00) == can_db.sa_db_rx && (msg->id & 0xFF00) != can_db.sa_val_rx) == FALSE)
                    {
                        // if gateway is configured send message here on according bus
                        if ((NONE) != can_block_db_const[i].can_bus_gw)
                        {
                            // not locked with #sfl_can_db_stop_gateway_for_known_ids
                            if( (FALSE) == can_block_db_ram[i].stop_gw_known_ids )
                            {
                                send_error = sfl_can_db_tx_wrapper(can_block_db_const[i].can_bus_gw, msg);
                                if(send_error != HAL_CAN_OK){
                                //  @TODO CAN ERROR WATERMARK
                            }
                            else
                            {
                                // do nothing
                            }

//                            (void) sfl_can_db_tx_wrapper(can_block_db_const[i].can_bus_gw, msg);

                            }
                        }
                        else
                        {
                            sfl_timer_set_timestamp(&can_block_db_ram[i].time_stamp_read, HAL_PRECISION_1MS);
                            can_block_db_ram[i].received = 1;
                        }
                        // copy data of message to local buffer
                        sfl_os_can_copy_msg(msg, &(can_block_db_ram[i].msg));
                        // break for-loop -> don't set i to #dyn_CAN_BLOCK_MAX due to usage later on!
                        break;
                    }
                }
            }
        }

    }

    // --------------------------------------------------------------------------------
    // Falls Gateway-Input: ID der Nachricht kommt nicht in der Datenbank vor, wurde aber vom Filter akzeptiert
    // --------------------------------------------------------------------------------
    if( ((IS_GW_INPUT)       == can_bus_db_const[bus_id].gw_input) &&
        ((dyn_CAN_BLOCK_MAX) == i                                )   )
    {
        // not locked with #sfl_can_db_stop_gateway_for_unknown_ids
        if( (FALSE) == ext_sfl_can_stop_gw_unknown_ids[bus_id] )
        {
            (void)sfl_can_db_gateway(bus_id, msg);
        }
        else
        {
            // do nothing
        }
    }


    // --------------------------------------------------------------------------------
    // Usercode callback
    // --------------------------------------------------------------------------------
    if(callback_can_msg_receive != NULL_PTR)
    {
        // regardless of what has been sent this function is called
        callback_can_msg_receive(bus_id, msg);
    }
}

/*----------------------------------------------------------------------------*/
/**
 * \internal
 * Cyclic CAN Tx check
 * \endinternal
 *
 *
 * \test STATUS: *** TESTED ***
 *   Date       | Type    | Person
 *   -----------|---------|-----------
 *   20160823   | Author  | riegel
 */

void sfl_can_db_output_to_bus( void )
{

    uint16_t d, flag_senden;
    uint8_t dlc, t_elapsed;

    // Werte in Bloecke eintragen
    for (uint32_t counter = 0; counter < dyn_CAN_BLOCK_MAX; (counter)++)
    {
        // Wenn der Block gesendet werden soll / DR151009 Senden aktiv oder nicht? Wird mit Funktion can_db_transmit_deactivate gesteuert
        if (can_block_db_const[counter].tx && !can_block_db_ram[counter].transmit_stop)
        {
            flag_senden = 0;
            // convert can-fd dlc to len in byte
            dlc = hal_can_dlc_to_len(can_block_db_const[counter].msg_len_dlc);

// check if transmit flag is set
            if (can_block_db_ram[counter].transmit == 1)
			{
                // Sende anforderungen unter 10ms werden blockiert
                if (sfl_timer_time_elapsed(&t_elapsed, can_block_db_ram[counter].time_stamp_transmit, 10, HAL_PRECISION_1MS) == SFL_TIMER_ERR_OK && t_elapsed == TRUE)
                {
                    sfl_timer_set_timestamp(&can_block_db_ram[counter].time_stamp_transmit, HAL_PRECISION_1MS);
                    flag_senden = 1; // Programm/Userbefehl die Nachricht zusenden.
                }
            }
// Wenn Zykluszeit ueberschritten sende sofort
            if (can_block_db_const[counter].zykluszeit_ms_max || flag_senden == 1) // Wenn die zykluszeit_max = 0 ist, dann nie senden.
            {
                if (sfl_timer_time_elapsed(&t_elapsed, can_block_db_ram[counter].time_stamp_write, can_block_db_const[counter].zykluszeit_ms_max, HAL_PRECISION_1MS)
                        == SFL_TIMER_ERR_OK&& t_elapsed == TRUE)
                {
                    flag_senden = 1; // Wenn die zykluszeit_max ueberschritten ist, dann senden.
                }
                else if (sfl_timer_time_elapsed(&t_elapsed, can_block_db_ram[counter].time_stamp_write, can_block_db_const[counter].zykluszeit_ms_min, HAL_PRECISION_1MS)
                        == SFL_TIMER_ERR_OK&& t_elapsed == TRUE)
                {
                    // do not send if data is the same as last data
                    // @TODO: does not work eventually with can-fd
                    for (d = 0; d < dlc; d++)
                        if (can_block_db_ram[counter].msg.data[d] != can_block_db_ram[counter].last_data[d])
                            break;
// Case nicht beruecksichtigt
                    if (d < dlc)
                        flag_senden = 1;  // Differenz gefunden
                }

// einen Block senden, wenn Flag gesetzt
                if (flag_senden)
                {
                    can_block_db_ram[counter].msg.id = can_block_db_const[counter].can_id;

                    // Wenn eine J1939 Source-Adresse definiert ist, es sich um eine 29bit handelt und die gesuchte SA in der Datenbank-ID enthalten ist, dann veraendert die Tx-ID
                    if (can_db.sa_active && can_block_db_const[counter].can_id_ext && (can_block_db_const[counter].can_id & 0xFF) == can_db.sa_db)
                    {
                        can_block_db_ram[counter].msg.id = (can_block_db_ram[counter].msg.id & 0xFFFFFF00) | can_db.sa_val;
                    }

                    can_block_db_ram[counter].msg.id_ext = can_block_db_const[counter].can_id_ext;
                    can_block_db_ram[counter].msg.len = dlc;

                    enum_HAL_CAN_RETURN_VALUE debug_err = sfl_can_db_tx_wrapper(can_block_db_const[counter].bus_id, &can_block_db_ram[counter].msg);
                    if (debug_err == HAL_CAN_OK)
                    {
                        can_block_db_ram[counter].transmit = 0; // Sende_Anforderungsflag loeschen
                        for (d = 0; d < dlc; d++)
                        {
                            can_block_db_ram[counter].last_data[d] = can_block_db_ram[counter].msg.data[d];
                        }

                        // we only update the timestamp if the send was successful.
                        sfl_timer_set_timestamp(&can_block_db_ram[counter].time_stamp_write, HAL_PRECISION_1MS);
                    }
                    else
                    {
                        // CAN MB overflow / no CAN available
                    	// this message will be tried again on the next call to sfl_can_db_output_to_bus (usually at the end of the next super-loop cycle)
                    }
                }
            }
        }
    }

    sfl_can_db_tx_fifo_cyclic();
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* Controls sending for CAN messages according to given CAN bus in can db tables which are configured as transmission (Tx).\n
* For backward compability and old function see #sfl_can_db_transmit_deactivate.
* \endinternal
*
*
* \test STATUS: *** UNTESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20200709   | Author  | DPaul
* 20200709   | Editor  | DPaul
* 20200709   | Tester  | DPaul
*/
uint8_t sfl_can_db_stop_transmission(const uint8_t can_bus, const uint8_t status)
{
    // local variables
    uint8_t idx;
    uint8_t local_tx_status;
    uint8_t ret_err;

    // preparation
    local_tx_status = 0xFFu;
    ret_err         = (FALSE);

    // CAN bus is invalid
    if( can_bus > (dyn_CAN_BUS_MAX) )
    {
        ret_err = (TRUE);
    }
    // handle messages according to CAN bus
    else
    {
        // loop through all messages in #can_block_db_const
        for( idx = 0u;  idx < (dyn_CAN_BLOCK_MAX); idx++ )
        {
            // message is configured as Tx?
            if( 0u != can_block_db_const[idx].tx )
            {
                // special case if given bus is #CAN_BLOCK_MAX -> don't compare CAN bus of message
                if( can_bus == (dyn_CAN_BUS_MAX) )
                {
                    local_tx_status = (uint8_t) status;
                }
                else
                {
                    if( can_bus == can_block_db_const[idx].bus_id )
                    {
                        local_tx_status = status;
                    }
                    else
                    {
                        // do nothing
                    }
                }
            }
            else
            {
                // do nothing
            }

            // take over value only if conditions before are valid
            if( 0xFFu != local_tx_status )
            {
                can_block_db_ram[idx].transmit_stop = local_tx_status;
            }
            else
            {
                // do nothing
            }

            // preparation for next cycle
            local_tx_status = 0xFFu;
        }
    }

    // return error feedback
    return ret_err;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* Controls sending of configured CAN messages according to given CAN bus as a gateay on an other CAN bus.
* \endinternal
*
*
* \test STATUS: *** UNTESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20200709   | Author  | DPaul
* 20200709   | Editor  | DPaul
* 20200709   | Tester  | DPaul
*/
uint8_t sfl_can_db_stop_gateway_for_known_ids(const uint8_t can_bus, const uint8_t status)
{
    // local variables
    uint8_t idx;
    uint8_t local_gw_status;
    uint8_t ret_err;

    // preparation
    local_gw_status = 0xFFu;
    ret_err         = (FALSE);

    // CAN bus is invalid
    if( can_bus > (dyn_CAN_BUS_MAX) )
    {
        ret_err = (TRUE);
    }
    // handle messages according to CAN bus
    else
    {
        // loop through all messages in #can_block_db_const
        for( idx = 0u;  idx < (dyn_CAN_BLOCK_MAX); idx++ )
        {
            // message is configured for gateway?
            if( (NONE) != can_block_db_const[idx].can_bus_gw )
            {
                // special case if given bus is #CAN_BLOCK_MAX -> don't compare CAN bus of message
                if( can_bus == (dyn_CAN_BUS_MAX) )
                {
                    local_gw_status = status;
                }
                else
                {
                    if( can_bus == can_block_db_const[idx].bus_id )
                    {
                        local_gw_status = status;
                    }
                    else
                    {
                        // do nothing
                    }
                }
            }
            else
            {
                // do nothing
            }

            // take over value only if conditions before are valid
            if( 0xFFu != local_gw_status )
            {
                can_block_db_ram[idx].stop_gw_known_ids = local_gw_status;
            }
            else
            {
                // do nothing
            }

            // preparation for next cycle
            local_gw_status = 0xFFu;
        }
    }

    // return error feedback
    return ret_err;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* Controls sending of configured CAN bus gateway.
* \endinternal
*
*
* \test STATUS: *** UNTESTED ***
* Date       | Type    | Person
* -----------|---------|-----------
* 20200709   | Author  | DPaul
* 20200709   | Editor  | DPaul
* 20200709   | Tester  | DPaul
*/
uint8_t sfl_can_db_stop_gateway_for_unknown_ids(const uint8_t can_bus, const uint8_t status)
{
    // local variables
    uint8_t idx;
    uint8_t local_gw_status;
    uint8_t ret_err;

    // preparation
    local_gw_status = 0xFFu;
    ret_err         = (FALSE);

    // CAN bus is invalid
    if( can_bus > (dyn_CAN_BUS_MAX) )
    {
        ret_err = (TRUE);
    }
    // handle messages according to CAN bus
    else
    {
        // loop through all busses in #can_bus_db_const
        for( idx = 0u;  idx < (dyn_CAN_BUS_MAX); idx++ )
        {
            // special case if given bus is #CAN_BLOCK_MAX -> don't compare CAN bus of message
            if( can_bus == (dyn_CAN_BUS_MAX) )
            {
                local_gw_status = status;
            }
            else
            {
                if( can_bus == idx )
                {
                    local_gw_status = status;
                }
                else
                {
                    // do nothing
                }
            }

            // take over value only if conditions before are valid
            if( 0xFFu != local_gw_status )
            {
                ext_sfl_can_stop_gw_unknown_ids[idx] = local_gw_status;
            }
            else
            {
                // do nothing
            }

            // preparation for next cycle
            local_gw_status = 0xFFu;
        }
    }

    // return error feedback
    return ret_err;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* send an user-defined CAN message
* \endinternal
*
*
* \test STATUS: *** TESTED ***
*   Date       | Type    | Person
*   -----------|---------|-----------
*   20160914   | Author  | dpaul
*   20160914   | Editor  | dpaul
*   20160914   | Tester  | dpaul
*   20210630   | Editor  | daniel.leer
*/
enum_HAL_CAN_RETURN_VALUE sfl_can_send_message(const struct_hal_can_handle* ptr_can_handle, const uint32_t can_id, const uint8_t can_dlc, const uint8_t can_fd, const uint8_t can_fd_brs, const uint8_t* const data_buffer)
{
    // local variables
    bios_can_msg_typ can_tx_msg;
    enum_HAL_CAN_RETURN_VALUE ret_val = HAL_CAN_ERROR_GENERAL;

    // create CAN message
    can_tx_msg.id = can_id;
    can_tx_msg.id_ext = ( ( can_id >> 31 ) & 0x1 ); // extract ext id
    can_tx_msg.len = hal_can_dlc_to_len(can_dlc);
    can_tx_msg.can_fd = can_fd;
    can_tx_msg.can_fd_brs = can_fd_brs;

    memset(can_tx_msg.data, 0, sizeof(can_tx_msg.data)/sizeof(uint8_t));
    memcpy(can_tx_msg.data, data_buffer, can_tx_msg.len);

    // get the correct range for this
    uint8_t sfl_max_payload_size = 0;
    if (!can_fd)
    {
        sfl_max_payload_size = SFL_CAN_CLASSIC_PAYLOAD_SIZE;
    }
    else
    {
        sfl_max_payload_size = SFL_CAN_FD_PAYLOAD_SIZE;
    }

    if (can_tx_msg.len <= sfl_max_payload_size)
    {
        ret_val = sfl_can_db_tx_wrapper(ptr_can_handle->can_handle_number, &can_tx_msg);
    }
    else
    {
        ret_val = HAL_CAN_ERROR_DLC_INVALID;
    }

    return ret_val;
}


/*----------------------------------------------------------------------------*/
/**
* \internal
* can_msg_receive user callback
* \endinternal
*
*/
void set_callback_can_msg_receive(callback_can_msg_receive_t callback)
{
    callback_can_msg_receive = callback;
}


