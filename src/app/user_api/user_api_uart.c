/*
 * user_api_uart.c
 *
 *  Created on: 07.11.2019
 *      Author: VGoertz
 */

#include "user_api_uart.h"
#include "sci_app.h"

enum_HAL_SCI_RETURN_VALUE user_uart_send_buffer(uint8_t uart_interface, uint8_t *send_buffer, uint8_t send_length)
{
	return sci_tx_send(uart_interface, send_buffer, send_length);
}


enum_HAL_SCI_RETURN_VALUE user_uart_set_baudrate(uint8_t uart_interface, enum_HAL_SCI_BAUD baudrate)
{
    return sci_set_baudrate(uart_interface, baudrate);
}


enum_HAL_SCI_RETURN_VALUE user_uart_set_config_parameter(uint8_t uart_interface, lpuart_parity_mode_t parity, lpuart_stop_bit_count_t stop_bit, lpuart_bit_count_per_char_t bit_per_char )
{
    return sci_set_config_parameter( uart_interface, parity, stop_bit,  bit_per_char );

}
