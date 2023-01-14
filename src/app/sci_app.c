/*----------------------------------------------------------------------------*/
/**
 * \file         sci_APP.c
 * \brief        Implementation of sci Application
 * \details
 * \date         20190515
 * \author       DHakobyan
 *
 */
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

#include "lpuart1.h"
#include "sci_app.h"

#include "hal_io.h"
#include "hal_sci.h"
#include "io_tables.h"

#include "sfl_timer.h"

#include "user_code.h"


void UART_Callback0(lpuart_state_t *driverState, uart_event_t event, void * rxCallbackParam);
void UART_Callback1(lpuart_state_t *driverState, uart_event_t event, void * rxCallbackParam);
void UART_Callback2(lpuart_state_t *driverState, uart_event_t event, void * rxCallbackParam);

uint8_t buffer_8bit0 = 0;
uint8_t buffer_8bit1 = 0;
uint8_t buffer_8bit2 = 0;


// Define the SCI structure for UART
static struct_hal_sci_handle hal_sci_config[MAX_LPUART_INSTANCES] __attribute__((unused))= {
    { .channel = INST_LPUART0, .baudrate = HAL_SCI_BAUD_115200, .hal_sci_callback_handle = { .rx_callback = UART_Callback0, .tx_callback = NULL, .error_callback = NULL, } },
    { .channel = INST_LPUART1, .baudrate = HAL_SCI_BAUD_115200, .hal_sci_callback_handle = { .rx_callback = UART_Callback1, .tx_callback = NULL, .error_callback = NULL, } },
    { .channel = INST_LPUART2, .baudrate = HAL_SCI_BAUD_115200, .hal_sci_callback_handle = { .rx_callback = UART_Callback2, .tx_callback = NULL, .error_callback = NULL, } },
};



void UART_Callback0(lpuart_state_t *driverState, uart_event_t event, void * rxCallbackParam)
{
    switch (event)
    {
        case UART_EVENT_RX_FULL:
            user_int_rx_sci(hal_sci_config[0].channel, buffer_8bit0);
            LPUART_DRV_SetRxBuffer(hal_sci_config[0].channel, &buffer_8bit0, 1);
            break;

        case UART_EVENT_END_TRANSFER:
            break;

        default:
            break;
    }
}

void UART_Callback1(lpuart_state_t *driverState, uart_event_t event, void * rxCallbackParam)
{
    switch (event)
    {
        case UART_EVENT_RX_FULL:
            user_int_rx_sci(hal_sci_config[1].channel, buffer_8bit1);
            LPUART_DRV_SetRxBuffer(hal_sci_config[1].channel, &buffer_8bit1, 1);
            break;

        case UART_EVENT_END_TRANSFER:
            break;

        default:
            break;
    }
}

void UART_Callback2(lpuart_state_t *driverState, uart_event_t event, void * rxCallbackParam)
{
	switch (event)
    {
        case UART_EVENT_RX_FULL:
    		user_int_rx_sci(hal_sci_config[2].channel, buffer_8bit2);
    		LPUART_DRV_SetRxBuffer(hal_sci_config[2].channel, &buffer_8bit2, 1);
			break;

    	case UART_EVENT_END_TRANSFER:
            break;

        default:
            break;
    }
}


// NOTE: init, sleep, deinit should be part of modulhardwarecode
enum_HAL_SCI_RETURN_VALUE sci_init(uint8_t interface)
{
    if ( interface >= NUMBER_OF_LPUART_INSTANCES)
    {
        return HAL_SCI_ERROR_CHANNEL_INVALID;
    }
    else
    {
        hal_sci_init(&hal_sci_config[interface], hal_sci_config[interface].channel);
        hal_sci_set_callback(&hal_sci_config[interface], hal_sci_config[interface].hal_sci_callback_handle.rx_callback, hal_sci_config[interface].hal_sci_callback_handle.tx_callback, hal_sci_config[interface].hal_sci_callback_handle.error_callback);
        hal_sci_receive(&hal_sci_config[interface], &buffer_8bit0, 1); // Listen for one Byte
        return HAL_SCI_OK;
    }
}

enum_HAL_SCI_RETURN_VALUE sci_set_baudrate(uint8_t interface, enum_HAL_SCI_BAUD baudrate)
{
    if ( interface >= NUMBER_OF_LPUART_INSTANCES)
    {
        return HAL_SCI_ERROR_CHANNEL_INVALID;
    }
    else
    {
        hal_sci_config[interface].baudrate = baudrate;
        return hal_sci_set_baudrate(&hal_sci_config[interface], baudrate);
    }
}

enum_HAL_SCI_RETURN_VALUE sci_set_config_parameter(uint8_t interface, lpuart_parity_mode_t parity, lpuart_stop_bit_count_t stop_bit, lpuart_bit_count_per_char_t bit_per_char )
{
    if ( interface >= NUMBER_OF_LPUART_INSTANCES)
    {
        return HAL_SCI_ERROR_CHANNEL_INVALID;
    }
    else
    {
        // change configuration structure parameter
        struct_lpuart_config[interface].lpuart_init_config_struct->parityMode = parity;
        struct_lpuart_config[interface].lpuart_init_config_struct->stopBitCount = stop_bit;
        struct_lpuart_config[interface].lpuart_init_config_struct->bitCountPerChar = bit_per_char;

#if 0
        // set pointer to additional parameters.
        hal_sci_config->ptr_handle = &struct_lpuart_config[interface];
#endif

        sci_deinit(interface);

        sci_init(interface);
        return HAL_SCI_OK;
    }
}

enum_HAL_SCI_RETURN_VALUE sci_sleep(uint8_t interface)
{
    return sci_deinit(interface);
}

enum_HAL_SCI_RETURN_VALUE sci_deinit(uint8_t interface)
{
    if ( interface >= NUMBER_OF_LPUART_INSTANCES)
    {
        return HAL_SCI_ERROR_CHANNEL_INVALID;
    }
    else
    {
        return hal_sci_deinit(&hal_sci_config[interface]);
    }
}


enum_HAL_SCI_RETURN_VALUE sci_tx_send(uint8_t interface, uint8_t* send_buffer, uint8_t send_length)
{
    if ( interface >= NUMBER_OF_LPUART_INSTANCES)
    {
        return HAL_SCI_ERROR_CHANNEL_INVALID;
    }
    else
    {
        return hal_sci_send_block(&hal_sci_config[interface], (const uint8_t*)send_buffer, send_length, 30);
    }

}
