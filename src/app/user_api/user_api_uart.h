#ifndef SRC_USER_API_UART_H_
#define SRC_USER_API_UART_H_
/*----------------------------------------------------------------------------*/
/**
* \file         user_api_uart.h
* \brief        Declaration of UART functionality
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup     user_api_UART UART
* \{
* \brief        Provides function to send SCI data.
* \details
*/
/*----------------------------------------------------------------------------*/
#include "hal_sci.h"


/*----------------------------------------------------------------------------*/
/**
* \brief    Send data via sci bus
*
* \param    uart_interface            [in] uint8_t  UART interface
* \param    *send_buffer              [in] uint8_t  Data buffer to send
* \param    send_length               [in] uint8_t  amount of data to send
* \return   enum_HAL_SCI_RETURN_VALUE               Return code
*/
enum_HAL_SCI_RETURN_VALUE user_uart_send_buffer(uint8_t uart_interface, uint8_t *send_buffer, uint8_t send_length);


/*----------------------------------------------------------------------------*/
/**
* \brief    Set baudrate of SCI bus
*
* \param    uart_interface            [in] uint8_t            UART interface
* \param    baudrate                  [in] enum_HAL_SCI_BAUD  baudrate
* \return   enum_HAL_SCI_RETURN_VALUE                         Return code
*/
enum_HAL_SCI_RETURN_VALUE user_uart_set_baudrate(uint8_t uart_interface, enum_HAL_SCI_BAUD baudrate);


/*----------------------------------------------------------------------------*/
/**
* \brief    Set additional parameter of sci bus
*
* \param    uart_interface            [in] uint8_t                      UART interface
* \param    parity                    [in] lpuart_parity_mode_t         parity mode
* \param    stop_bit                  [in] lpuart_stop_bit_count_t      amount of bits per stop bit
* \param    bit_per_char              [in] lpuart_bit_count_per_char_t  amount of bits per char
* \return   enum_HAL_SCI_RETURN_VALUE                                   Return code
*/
enum_HAL_SCI_RETURN_VALUE user_uart_set_config_parameter(uint8_t uart_interface, lpuart_parity_mode_t parity, lpuart_stop_bit_count_t stop_bit, lpuart_bit_count_per_char_t bit_per_char );

#endif /* SRC_USER_API_UART_H_ */
/** \} */
