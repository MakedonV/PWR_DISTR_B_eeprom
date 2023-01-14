/*----------------------------------------------------------------------------*/
/**
 * \file         can_app.c
 * \brief
 * \details
 * \date         20190523
 *
 */
/*----------------------------------------------------------------------------*/
#include "can_app.h"
#include "canCom1.h"
#include "sfl_can_db.h"
#include "sfl_bl_protocol.h"
#include "flexcan_driver.h"
#include <stdbool.h>
#include "io_tables.h"
#include "hal_io.h"
#include "user_code.h"
#include "can_db_tables.h"

#define MAX_CAN_BUS_SUPPORTED 3

volatile struct_can_app_error can_error_handle[MAX_CAN_BUS_SUPPORTED];


/**
 * External function prototypes
 */
void CAN_Callback(uint8_t instance, flexcan_event_type_t eventType, uint32_t buffIdx, flexcan_state_t *flexcanState);
void CAN2_Callback(uint8_t instance, flexcan_event_type_t eventType, uint32_t buffIdx, flexcan_state_t *flexcanState);
void CAN3_Callback(uint8_t instance, flexcan_event_type_t eventType, uint32_t buffIdx, flexcan_state_t *flexcanState);

void CAN_Callback_Error(uint8_t instance, flexcan_event_type_t eventType, flexcan_state_t *flexcanState);

extern volatile const can_bus_db_const_typ can_bus_db_const[];


/*----------------------------------------------------------------------------*/
/**
* \internal
* device independent for s32k14x devices
* \endinternal
*
*/
void user_can_init(const struct_can_pin_list ptr_can_pin_list[])
{
    static bool once = true;
    struct_hal_can_handle handle[CAN_BUS_MAX];

    uint8_t i;
    if (once)
    {
        once = false;

        sfl_can_db_tables_data_init();          // Init the can_db_tables
        sfl_can_db_fifo_init();

        // get the can filter/mask settings defined in can_db_tables
        can_db_filter_typ can_db_filter_config[CAN_BUS_MAX];
        ext_get_filter_and_mask(can_db_filter_config);
        sfl_can_db_init(can_db_filter_config);  // Init the CAN module

        for (uint8_t i = 0; i < CAN_BUS_MAX; i++)       // Init CAN baudrates for all CAN modules
        {
            sfl_can_get_handle(i , &handle[i]);
            sfl_can_db_tables_data_init_baudrate(handle[i], can_bus_db_const[i]);
        }
    }


    // Cycle through all CAN_BUS modules to enable CAN transceivers,
    // assign receiver callbacks and to save the CAN handle information for general use.
    // See user_can_copy_handle function and user_can_cyclic function which contains a sample code.
    for (i = 0; i < CAN_BUS_MAX; i++)
    {
        // Setup the hal_can_handle structure
        handle[i].can_handle_number = can_bus_db_const[i].hw_module_id;

        // If CAN module is active
        if (can_bus_db_const[i].hw_module_active == TRUE)
        {
            if( ( ptr_can_pin_list[can_bus_db_const[i].hw_module_id].can_en_pin != PIN_MAX ) &&
                ( ptr_can_pin_list[can_bus_db_const[i].hw_module_id].can_stb_pin != PIN_MAX ) )
            {
                switch (can_bus_db_const[i].hw_module_id)
                {
                case 0:     // CAN_BUS_0
                    hal_io_do_set(ptr_can_pin_list[can_bus_db_const[i].hw_module_id].can_en_pin, 1);      // Enable module
                    hal_io_do_set(ptr_can_pin_list[can_bus_db_const[i].hw_module_id].can_stb_pin, 1);     // Enable standby pin

                    handle[i].rx_cb_function = (hal_can_rx_callback_function_type) CAN_Callback;
                    break;

                case 1:     // CAN_BUS_1
                    hal_io_do_set(ptr_can_pin_list[can_bus_db_const[i].hw_module_id].can_en_pin, 1);      // Enable module
                    hal_io_do_set(ptr_can_pin_list[can_bus_db_const[i].hw_module_id].can_stb_pin, 1);     // Enable standby pin

                    handle[i].rx_cb_function = (hal_can_rx_callback_function_type) CAN2_Callback;
                    break;
                case 2:     // CAN_BUS_2
                    hal_io_do_set(ptr_can_pin_list[can_bus_db_const[i].hw_module_id].can_en_pin, 1);      // Enable module
                    hal_io_do_set(ptr_can_pin_list[can_bus_db_const[i].hw_module_id].can_stb_pin, 1);     // Enable standby pin

                    handle[i].rx_cb_function = (hal_can_rx_callback_function_type) CAN3_Callback;
                    break;
                default:
                    // do nothing
                    break;
                }
            }
            else
            {
                // PIN not valid
            }

            hal_can_set_receive_callback(&handle[i], NULL);        // Install callback handler

            handle[i].err_cb_function = (hal_can_err_callback_function_type) CAN_Callback_Error;
            hal_can_set_error_callback(&handle[i], NULL);          // install error callback handler
        }
    }
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
void user_can_set_baudrate(uint8_t interface, uint8_t can_fd_en, enum_HAL_CAN_BAUDRATE baudrate, hal_can_time_segment arb_phase, hal_can_time_segment data_phase)
{
    struct_hal_can_handle ptr_handle;

    sfl_can_get_handle(interface, &ptr_handle);

    ptr_handle.can_ext_baudrate.jumpwidth   = arb_phase.jumpwidth;
    ptr_handle.can_ext_baudrate.phase_seg1  = arb_phase.phase_seg1;
    ptr_handle.can_ext_baudrate.phase_seg2  = arb_phase.phase_seg2;
    ptr_handle.can_ext_baudrate.pre_divider = arb_phase.pre_divider;
    ptr_handle.can_ext_baudrate.prop_seg    = arb_phase.prop_seg;

    ptr_handle.can_fd_ext_baudrate.jumpwidth   = data_phase.jumpwidth;
    ptr_handle.can_fd_ext_baudrate.phase_seg1  = data_phase.phase_seg1;
    ptr_handle.can_fd_ext_baudrate.phase_seg2  = data_phase.phase_seg2;
    ptr_handle.can_fd_ext_baudrate.pre_divider = data_phase.pre_divider;
    ptr_handle.can_fd_ext_baudrate.prop_seg    = data_phase.prop_seg;

    if( can_fd_en != 0)
    { // can-fd
        ptr_handle.can_baudrate = HAL_CAN_FD_BAUD_EXT;
        ptr_handle.can_fd_data_baudrate = HAL_CAN_FD_BAUD_EXT;
    }
    else
    { // can-hs
        if ( ( baudrate > HAL_CAN_BAUD_INVALID) && ( baudrate < HAL_CAN_BAUD_EXT) )
        {
            // use predefined baudrate
            ptr_handle.can_baudrate = baudrate;
            ptr_handle.can_fd_data_baudrate = HAL_CAN_BAUD_INVALID;
        }
        else
        {
            // use ext baud
            ptr_handle.can_baudrate = HAL_CAN_BAUD_EXT;
            ptr_handle.can_fd_data_baudrate = HAL_CAN_BAUD_INVALID;
        }

    }

    hal_can_set_baudrate(&ptr_handle, ptr_handle.can_baudrate );
    hal_can_set_fd_data_baudrate(&ptr_handle, ptr_handle.can_fd_data_baudrate);
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_can_fd_calculate_baudrate(uint32_t arb_baud, uint32_t arb_sample_pt, uint32_t data_baud, uint32_t data_sample_pt, hal_can_time_segment* arb_phase, hal_can_time_segment* data_phase )
{
    struct_ROLE_CAN_EXT_BAUD_DEMO io;
    uint32_t error = 0;

    struct_hal_can_handle local_can_handle;
    local_can_handle.is_can_fd = true;

    io.inp.arbitr_baud    = arb_baud;        // arbitration baud in kbits
    io.inp.data_baud      = data_baud;       // data baud in kbits
    io.inp.can_clock      = 80000000u;       // 80MHz pll -> CAN clock is the core clock
    io.inp.arbitr_smp     = arb_sample_pt;   // % sample point
    io.inp.data_smp       = data_sample_pt;  // % sample point

    error = calculate_can_baudrate(&io);

    error = set_can_baudrate_timing(&local_can_handle.can_ext_baudrate, &io.out.phase[0], CAN_FD_FRAME_PHASE_ARBITRATION);

    error = set_can_baudrate_timing(&local_can_handle.can_fd_ext_baudrate, &io.out.phase[1], CAN_FD_FRAME_PHASE_DATA);

    if (error == 0 )
    {
        arb_phase->jumpwidth    = local_can_handle.can_ext_baudrate.jumpwidth;
        arb_phase->phase_seg1   = local_can_handle.can_ext_baudrate.phase_seg1;
        arb_phase->phase_seg2   = local_can_handle.can_ext_baudrate.phase_seg2;
        arb_phase->pre_divider  = local_can_handle.can_ext_baudrate.pre_divider;
        arb_phase->prop_seg     = local_can_handle.can_ext_baudrate.prop_seg;

        data_phase->jumpwidth   = local_can_handle.can_fd_ext_baudrate.jumpwidth;
        data_phase->phase_seg1  = local_can_handle.can_fd_ext_baudrate.phase_seg1;
        data_phase->phase_seg2  = local_can_handle.can_fd_ext_baudrate.phase_seg2;
        data_phase->pre_divider = local_can_handle.can_fd_ext_baudrate.pre_divider;
        data_phase->prop_seg    = local_can_handle.can_fd_ext_baudrate.prop_seg;

        // @TODO: some baudrates, especially over 2000kbaud can be wrong: check for alternative sample points

        if( ( ( arb_phase->jumpwidth == 0 ) &&
            ( arb_phase->phase_seg1 == 0 ) &&
            ( arb_phase->phase_seg2 == 0 ) &&
            ( arb_phase->pre_divider == 0 ) &&
            ( arb_phase->prop_seg == 0 ) &&
            ( data_phase->jumpwidth == 0 ) &&
            ( data_phase->phase_seg1 == 0 ) &&
            ( data_phase->phase_seg2 == 0 ) &&
            ( data_phase->pre_divider == 0 ) &&
            ( data_phase->prop_seg == 0 ) ) )
        {
            // baudrate not valid
            error = 1;
        }
    }

    return error;

}

void CAN_Callback(uint8_t instance, flexcan_event_type_t eventType, uint32_t buffIdx, flexcan_state_t *flexcanState)
{
    struct_sfl_can_fifo_frame can_msg_receive;
    can_msg_receive.header.ptr_data = can_msg_receive.data;

    struct_hal_can_handle can_handle;
    can_handle.can_handle_number = instance;
    can_handle.can_handle_mb_idx = buffIdx;

    // State machine
    switch (eventType)
    {
        case FLEXCAN_EVENT_TX_COMPLETE:
            sfl_can_db_tx_callback(instance);
            break;
        case FLEXCAN_EVENT_RXFIFO_COMPLETE:
            break;
        case FLEXCAN_EVENT_RX_COMPLETE:
        	if ( SFL_BLP_NON_BLP_IDX == sfl_bl_protocol_s32k_is_msg_for_bl(buffIdx, flexcanState) )
        	{
                // get received can msg, re-enable the FIFO receive interrupt
                hal_can_receive(&can_handle, &can_msg_receive.header);
                // Take over the data to db
                sfl_can_db_rx_wrapper(instance, &can_msg_receive.header);
        	}
        	else
        	{
        		// do nothing
        	}
            break;
        default:
            break;
    }
}

void CAN2_Callback(uint8_t instance, flexcan_event_type_t eventType, uint32_t buffIdx, flexcan_state_t *flexcanState)
{
    struct_sfl_can_fifo_frame can_msg_receive;
    can_msg_receive.header.ptr_data = can_msg_receive.data;

    struct_hal_can_handle can_handle;
    can_handle.can_handle_number = instance;
    can_handle.can_handle_mb_idx = buffIdx;

    // State machine
    switch (eventType)
    {
        case FLEXCAN_EVENT_TX_COMPLETE:
            sfl_can_db_tx_callback(instance);
            break;
        case FLEXCAN_EVENT_RXFIFO_COMPLETE:

            break;
        case FLEXCAN_EVENT_RX_COMPLETE:
        	if ( SFL_BLP_NON_BLP_IDX == sfl_bl_protocol_s32k_is_msg_for_bl(buffIdx, flexcanState) )
        	{
				// get received can msg, re-enable the FIFO receive interrupt
				hal_can_receive(&can_handle, &can_msg_receive.header);
				// Take over the data to db
				sfl_can_db_rx_wrapper(instance, &can_msg_receive.header);
        	}
        	else
        	{
        		// do nothing
        	}
            break;
        default:
            break;
    }
}

void CAN3_Callback(uint8_t instance, flexcan_event_type_t eventType, uint32_t buffIdx, flexcan_state_t *flexcanState)
{
    struct_sfl_can_fifo_frame can_msg_receive;
    can_msg_receive.header.ptr_data = can_msg_receive.data;

    struct_hal_can_handle can_handle;
    can_handle.can_handle_number = instance;
    can_handle.can_handle_mb_idx = buffIdx;

    // State machine
    switch (eventType)
    {
        case FLEXCAN_EVENT_TX_COMPLETE:
            sfl_can_db_tx_callback(instance);
            break;
        case FLEXCAN_EVENT_RXFIFO_COMPLETE:

            break;
        case FLEXCAN_EVENT_RX_COMPLETE:
        	if ( SFL_BLP_NON_BLP_IDX == sfl_bl_protocol_s32k_is_msg_for_bl(buffIdx, flexcanState) )
			{
        		// get received can msg, re-enable the FIFO receive interrupt
        		hal_can_receive(&can_handle, &can_msg_receive.header);
        		// Take over the data to db
        		sfl_can_db_rx_wrapper(instance, &can_msg_receive.header);
			}
			else
			{
				// do nothing
			}
            break;
        default:
            break;
    }
}

void CAN_Callback_Error(uint8_t instance, flexcan_event_type_t eventType, flexcan_state_t *flexcanState)
{
    if (FLEXCAN_EVENT_ERROR == eventType)
    {
        const uint32_t err_masks = (CAN_ESR1_ACKERR_MASK
                                  | CAN_ESR1_STFERR_MASK
                                  | CAN_ESR1_ERROVR_MASK
								  | CAN_ESR1_FRMERR_MASK
								  | CAN_ESR1_CRCERR_MASK
								  | CAN_ESR1_BIT0ERR_MASK | CAN_ESR1_BIT1ERR_MASK
								  );

        uint32_t err1 = FLEXCAN_DRV_GetErrorStatus(instance);

    	if ( (err1 & err_masks) )
    	{
            // save current error
    	    can_error_handle[instance].last_can_error = err1;
    	    // get error timestamp
    	    hal_get_timestamp((uint32_t*)&can_error_handle[instance].time_from_start_to_error, HAL_PRECISION_1MS);

    		if (err1 & CAN_ESR1_TX_MASK)
    		{
        	    // hal_can_tx_callback function reserves for transfer a mailbox fixed to 8
        		// If this mailbox number gets changed in time, the following mb_idx should
        		// also be updated
                for(uint32_t mb_idx = 4u; mb_idx < struct_can_config_tbl[instance].can_init_config_struct->max_num_mb; ++mb_idx )
        	    {
        		    if (STATUS_SUCCESS != FLEXCAN_DRV_GetTransferStatus(instance, mb_idx))
        	        {
						FLEXCAN_DRV_AbortTransfer(instance, mb_idx);
        	        }
        		    else
        		    {
        		    	// do nothing
        		    }
        	    }
    		}

    		if (err1 & CAN_ESR1_RX_MASK)
    		{
        	    // hal_can_tx_callback function reserves for transfer a mailbox fixed to 8
        		// If this mailbox number gets changed in time, the following mb_idx should
        		// also be updated
        		for ( uint32_t mb_idx = 0u; mb_idx < 3u; ++mb_idx )
        	    {
        		    if (STATUS_SUCCESS != FLEXCAN_DRV_GetTransferStatus(instance, mb_idx))
        	        {
						FLEXCAN_DRV_AbortTransfer(instance, mb_idx);
						sfl_can_rx_int_enable(instance, mb_idx);
        	        }
        		    else
        		    {
        		    	// do nothing
        		    }
        	    }
    		}

            CAN_Type * const canBase[] = CAN_BASE_PTRS;
            FLEXCAN_ClearErrIntStatusFlag(canBase[instance]);

            if (CAN_ESR1_ERROVR_MASK == (err1 & CAN_ESR1_ERROVR_MASK))
			{
        		canBase[instance]->ESR1 |= CAN_ESR1_ERROVR_MASK;
			}
        	else
        	{
        		// do nothing
        	}
    	}
    	else
    	{
    		// do nothing
    	}
    }
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* The implementation of the function contains a fall through switch case.
* Unfortunately there is not really an elegant way of getting the defined Macros dynamically.
* Therefore it had to be hard coded, lets hope this will be changed to an enum or array so its easier
* to add more CAN busses and CAN filter/mask configurations
* \endinternal
*/
void ext_get_filter_and_mask(can_db_filter_typ filter[])
{
    uint8_t bus_id = 0;

    // ATTENTION: switch case contains fall through

    // check how many CAN busses are defined
    switch(CAN_BUS_MAX){
#ifdef CAN2_FILTER_A
        // 3 CAN busses configured
        case(3):
            // CAN_BUS_2
            bus_id = CAN_BUS_2;
            filter[bus_id].filter[0] = CAN2_FILTER_A;
            filter[bus_id].filter[1] = CAN2_FILTER_B;
            filter[bus_id].mask[0] = CAN2_MASK_A;
            filter[bus_id].mask[1] = CAN2_MASK_B;
            filter[bus_id].mask_division = CAN2_MASK_DIVISION;
            // ATTENTION: no break, will fall through
#endif
        // 2 CAN busses configured
#ifdef CAN1_FILTER_A
        case(2):
            // CAN_BUS_1
            bus_id = CAN_BUS_1;
            filter[bus_id].filter[0] = CAN1_FILTER_A;
            filter[bus_id].filter[1] = CAN1_FILTER_B;
            filter[bus_id].mask[0] = CAN1_MASK_A;
            filter[bus_id].mask[1] = CAN1_MASK_B;
            filter[bus_id].mask_division = CAN1_MASK_DIVISION;
            // ATTENTION: no break, will fall through as intended
#endif
        // 1 CAN bus configured
        case(1):
            // CAN_BUS_0
            bus_id = CAN_BUS_0;
            filter[bus_id].filter[0] = CAN0_FILTER_A;
            filter[bus_id].filter[1] = CAN0_FILTER_B;
            filter[bus_id].mask[0] = CAN0_MASK_A;
            filter[bus_id].mask[1] = CAN0_MASK_B;
            filter[bus_id].mask_division = CAN0_MASK_DIVISION;
            break;

        // 0 or more than CAN_BUS_MAX configured
        default:
            filter = NULL;
            break;
    }

}
