#ifndef HAL_SCI_H
#define HAL_SCI_H
/*----------------------------------------------------------------------------*/
/**
* \file         hal_sci.h
* \brief        Definitions for hardware independent SCI functionality.
*/
/*----------------------------------------------------------------------------*/
/**
* \addtogroup   doc_hal_sci HAL SCI
* @{
* \brief        This documentation part describes the interface function definitions for a hardware-independent and abstracted SCI interface.
* \details      Main goal is to create a hardware-platform independent layer, that can be accessed from different HW-platforms.
*               A Serial Communication Interface (SCI) is a single master, single slave interface, concepted to exchange data bitwise after each other to a remote client.
*               In most cases a Universal (Synchron and) Asynchron Receiver Transmitter (UART or USART) provides a hardware access to Pins.
*               The implementation can differ widely from one MCU to another. So It is not clear if blocking write, DMA access,
*               interrupt driven or cyclic callers are needed.
*/
/*----------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "hal_data_types.h"
#include "lpuart_driver.h"

// ---------------------------------------------------------------------------------------------------
// defines
// ---------------------------------------------------------------------------------------------------
#define VAL_MAX_BUF                                 15u

// Common baudrates are predefined here:
typedef enum
{
    HAL_SCI_BAUD_600             = 600u,    ///< Baudrate is 600 Bit/s.
    HAL_SCI_BAUD_1200            = 1200u,   ///< Baudrate is 1200 Bit/s.
    HAL_SCI_BAUD_2400            = 2400u,   ///< Baudrate is 2400 Bit/s.
    HAL_SCI_BAUD_4800            = 4800u,   ///< Baudrate is 4800 Bit/s.
    HAL_SCI_BAUD_9600            = 9600u,   ///< Baudrate is 9600 Bit/s. This value is often used.
    HAL_SCI_BAUD_14400           = 14400u,  ///< Baudrate is 14400 Bit/s.
    HAL_SCI_BAUD_19200           = 19200u,  ///< Baudrate is 19200 Bit/s. This value is often used.
    HAL_SCI_BAUD_28800           = 28800u,  ///< Baudrate is 28800 Bit/s.
    HAL_SCI_BAUD_38400           = 38400u,  ///< Baudrate is 38400 Bit/s.
    HAL_SCI_BAUD_56000           = 56000u,  ///< Baudrate is 56000 Bit/s.
    HAL_SCI_BAUD_57600           = 57600u,  ///< Baudrate is 57600 Bit/s.
    HAL_SCI_BAUD_115200          = 115200u, ///< Baudrate is 115200 Bit/s.
    HAL_SCI_BAUD_128000          = 128000u, ///< Baudrate is 128000 Bit/s.
    HAL_SCI_BAUD_250000          = 250000u, ///< Baudrate is 250000 Bit/s.
    HAL_SCI_BAUD_256000          = 256000u  ///< Baudrate is 256000 Bit/s.
} enum_HAL_SCI_BAUD;


// ---------------------------------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------
/** This enum lists all available error codes that function of HAL SCI han return.*/
typedef enum
{
    HAL_SCI_OK                      = 0u,     ///< No error.
    HAL_SCI_ERROR_GENERAL               ,     ///< Common error.
    HAL_SCI_ERROR_INIT_FAILED           ,     ///< Initialization failed.
    HAL_SCI_ERROR_DEINIT_FAILED         ,     ///< De-initialization failed.
    HAL_SCI_ERROR_CHANNEL_INVALID       ,     ///< Channel invalid e.g. already not opened.
    HAL_SCI_ERROR_NO_CHANNEL_LEFT       ,     ///< No channel left on device.
    HAL_SCI_ERROR_SET_CALLBACK          ,     ///< Error until set of callback.
    HAL_SCI_ERROR_SET_BAUDRATE          ,     ///< Error until set of baudrate.
    HAL_SCI_ERROR_NOT_IMPLEMENTED             ///< Error function not implemented.

} enum_HAL_SCI_RETURN_VALUE;

// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------
/*----------------------------------------------------------------------------*/
/**
*   \brief      Function pointer type definition for new byte on SCI received callback.
*   \details    This function is called, when a new byte on SCI was received.
*               Use this just as a prototype for parameter and return value.
*               IMPORTANT: You should implement this function in your code.
*
* \param    *driverState    [in] *hal_sci_rx_callback_t)(lpuart_state_t
* \param    event           [in] uart_event_t
* \param    rxCallbackParam [in] void *
* \return   void
*/
typedef void (*hal_sci_rx_callback_t)(lpuart_state_t *driverState, uart_event_t event, void * rxCallbackParam);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Function pointer type definition for SCI byte transmission completed callback.
*   \details    This function is called, when a SCI byte was transmitted.
*               Use this just as a prototype for parameter and return value.
*               IMPORTANT: You should implement this function in your code.
*
*   \return     void
*/
typedef void (*hal_sci_tx_callback_t)(void);
/*----------------------------------------------------------------------------*/
/**
*   \brief      Function pointer type definition for SCI error callback.
*   \details    This function is called, when an error on SCI happened.
*               Use this just as a prototype for parameter and return value.
*               IMPORTANT: You should implement this function in your code.
*
*   \param      [in] uint8_t Error value from SCI.
*   \return     void
*/
typedef void (*hal_sci_error_callback_t)(uint8_t);

/** This struct contains all available callbacks that are supportet from the SCI.*/
typedef struct
{
    hal_sci_rx_callback_t rx_callback;          ///< You get this callback when a new byte on SCI was received. This contains the new received byte. Please see #hal_sci_rx_callback_t.
    hal_sci_tx_callback_t tx_callback;          ///< You get this callback when a SCI transmission has finnished. Please see #hal_sci_tx_callback_t.
    hal_sci_error_callback_t error_callback;    ///< You get this callback when an error happened on sci. Please see #hal_sci_error_callback_t.

}struct_hal_sci_callback_handle;

/** This struct contains all needed data for a handler to communicate with a single SCI.
*   Pass this handler to functions to identify your given SCI / UART.
*/
typedef struct
{
    void* ptr_handle;                                       ///< A pointer to the handle for extended configuration
    uint8_t channel;                                        ///< SCI Channel identifier. The channel that is used.
    uint32_t baudrate;                                      ///< Baudrate setup for SCI.
    struct_hal_sci_callback_handle hal_sci_callback_handle; ///< SCI Callback handler.

}struct_hal_sci_handle;

// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
*   \brief      Initialize SCI functionality.
*   \details    Call this function once before start SCI communication.
*               Initializes sci functionality and dma if selected.
*
*   \param      ptr_sci_handle            [out] #struct_hal_sci_handle* Pointer to new sci handle structure.
*   \param      sci_channel               [in] uint8_t SCI channel identification number.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_SCI_RETURN_VALUE as feedback.
*/
enum_HAL_SCI_RETURN_VALUE hal_sci_init(struct_hal_sci_handle* ptr_sci_handle, uint8_t sci_channel);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Release the SCI functionality.
*   \details    Use this function to clean up all used ressources.
*
*   \param      ptr_sci_handle            [in] #struct_hal_sci_handle* Pointer to SCI handle structure.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_SCI_RETURN_VALUE as feedback.
*/
enum_HAL_SCI_RETURN_VALUE hal_sci_deinit(struct_hal_sci_handle* ptr_sci_handle);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Transmit a single byte on SCI.
*   \details	Synchronously transmits a single byte.
*
*   \pre        You should have called #hal_sci_init once before.
*
*   \param      ptr_sci_handle            [in] #struct_hal_sci_handle* Pointer to sci handle structure.
*   \param      val                       [in] uint8_t Value that have to be transmitted.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_SCI_RETURN_VALUE as feedback.
*/
enum_HAL_SCI_RETURN_VALUE hal_sci_send_byte(struct_hal_sci_handle* ptr_sci_handle, uint8_t val);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Enables a receive operation for a single byte on SCI.
*   \details	The function is asynchronous and will keep the receive channel enabled until a byte arrives.
*
*   \pre        You should call #hal_sci_init prior to using this function.
*
*   \param      ptr_sci_handle            [in] #struct_hal_sci_handle* Pointer to sci handle structure.
*   \param      ptr_val                   [in] uint8_t* Pointer to single byte where the incoming data to be saved.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_SCI_RETURN_VALUE as feedback.
*/
enum_HAL_SCI_RETURN_VALUE hal_sci_receive_byte(struct_hal_sci_handle* ptr_sci_handle, uint8_t *ptr_val);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Asynchronously transmits a complete string on SCI.
*   \details    This string have to be zero terminated.
*
*   \pre        You should have called #hal_sci_init once before.
*
*   \param      ptr_sci_handle            [in] #struct_hal_sci_handle* Pointer to sci handle structure.
*   \param      ptr_str                   [in] uint8_t* Pointer to string that have to be transmitted.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_SCI_RETURN_VALUE as feedback.
*/
enum_HAL_SCI_RETURN_VALUE hal_sci_send_string(struct_hal_sci_handle* ptr_sci_handle, const uint8_t* ptr_str);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Synchronously transmits a complete string on SCI.
*   \details    This string have to be zero terminated.
*
*   \pre        You should have called #hal_sci_init once before.
*
*   \param      ptr_sci_handle            [in] #struct_hal_sci_handle* Pointer to sci handle structure.
*   \param      ptr_str                   [in] uint8_t* Pointer to string that have to be transmitted.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_SCI_RETURN_VALUE as feedback.
*/
enum_HAL_SCI_RETURN_VALUE hal_sci_send_string_block(struct_hal_sci_handle* ptr_sci_handle, const uint8_t* ptr_str, uint32_t timeout);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Asynchronously transmits a block of databytes on SCI.
*   \details
*
*   \pre        You should have called #hal_sci_init once before.
*
*   \param      ptr_sci_handle            [in] #struct_hal_sci_handle* pointer to sci handle structure
*   \param      ptr_data                  [in] uint8_t* Pointer to first data byte that should be transmitted.
*   \param      len                       [in] uint16_t Number of databytes that should be transmitted.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_SCI_RETURN_VALUE as feedback.
*/
enum_HAL_SCI_RETURN_VALUE hal_sci_send(struct_hal_sci_handle* ptr_sci_handle, const uint8_t* ptr_data, uint16_t len);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Synchronously transmits a complete block of databytes on SCI.
*   \details
*
*   \pre        You should have called #hal_sci_init once before.
*
*   \param      ptr_sci_handle            [in] #struct_hal_sci_handle* pointer to sci handle structure
*   \param      ptr_data                  [in] uint8_t* Pointer to first data byte that should be transmitted.
*   \param      len                       [in] uint16_t Number of databytes that should be transmitted.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_SCI_RETURN_VALUE as feedback.
*/
enum_HAL_SCI_RETURN_VALUE hal_sci_send_block(struct_hal_sci_handle* ptr_sci_handle, const uint8_t* ptr_data, uint16_t len,
												uint32_t timeout);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Asynchronously receives a block of databytes on SCI.
*   \details	The receive channel is turned off when the requested number of bytes is received.
*
*   \pre        You should have called #hal_sci_init once before.
*
*   \param      ptr_sci_handle            [in] #struct_hal_sci_handle* pointer to sci handle structure
*   \param      ptr_data                  [in] uint8_t* Pointer to a byte array where incoming bytes to be stored.
*   \param      len                       [in] uint16_t Number of databytes that should be received.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_SCI_RETURN_VALUE as feedback.
*/
enum_HAL_SCI_RETURN_VALUE hal_sci_receive(struct_hal_sci_handle* ptr_sci_handle, uint8_t* ptr_data, uint16_t len);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Synchronously receives a block of databytes on SCI.
*   \details	The function turns off the receive channel on exit.
*
*   \pre        You should have called #hal_sci_init once before.
*
*   \param      ptr_sci_handle            [in] #struct_hal_sci_handle* pointer to sci handle structure
*   \param      ptr_data                  [in] uint8_t* Pointer to first data byte that should be transmitted.
*   \param      len                       [in] uint16_t Number of databytes that should be transmitted.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_SCI_RETURN_VALUE as feedback.
*/
enum_HAL_SCI_RETURN_VALUE hal_sci_receive_block(struct_hal_sci_handle* ptr_sci_handle, uint8_t* ptr_data, uint16_t len, uint32_t timeout);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Returns remaining number of bytes to be still transmitted.
*   \details
*
*   \pre        You should have called #hal_sci_init once before.
*
*   \param      ptr_sci_handle            [in] #struct_hal_sci_handle* pointer to sci handle structure
*   \return     This function returns the remaining number of bytes to be transmitted or ((uint32_t)-1) on error.
*/
uint32_t hal_sci_send_status(struct_hal_sci_handle* ptr_sci_handle);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Returns remaining number of bytes to be still received.
*   \details
*
*   \pre        You should have called #hal_sci_init once before.
*
*   \param      ptr_sci_handle            [in] #struct_hal_sci_handle* pointer to sci handle structure
*   \return     This function returns the remaining number of bytes to be received or ((uint32_t)-1) on error.
*/
uint32_t hal_sci_receive_status(struct_hal_sci_handle* ptr_sci_handle);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Transmit a fix point value via sci interface.
*   \details    This is for legacy reasons.
*
*   \pre        You should have called #hal_sci_init once before.
*
*   \param      ptr_sci_handle            [in] #struct_hal_sci_handle* Pointer to sci handle structure.
*   \param      val                       [in] uint32_t Value to be send e.g. 32456.
*   \param      fix_point                 [in] uint8_t Fix point of value e.g. 32,456 = fix point 3.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_SCI_RETURN_VALUE as feedback.
*/
enum_HAL_SCI_RETURN_VALUE hal_sci_send_fix_point_val(struct_hal_sci_handle* ptr_sci_handle, uint32_t val, uint8_t fix_point);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Add or change the SCI callback setup.
*   \details    Use here your function name for the callbacks you want to use.
*               A callback can be disabled by setting it to value zero.
*
*   \pre        You should have called #hal_sci_init once before.
*
*   \param      ptr_sci_handle               [in] #struct_hal_sci_handle* Pointer to sci handle structure
*   \param      ptr_hal_sci_rx_callback      [in] #hal_sci_rx_callback_t Pointer to rx callback function
*   \param      ptr_hal_sci_tx_callback      [in] #hal_sci_tx_callback_t Pointer to tx callback function
*   \param      ptr_hal_sci_error_callback   [in] #hal_sci_tx_callback_t Pointer to error callback function
*   \return     This function returns zero for success or a positive error code from #enum_HAL_SCI_RETURN_VALUE as feedback.
*/
enum_HAL_SCI_RETURN_VALUE hal_sci_set_callback(struct_hal_sci_handle*    ptr_sci_handle,
                                               hal_sci_rx_callback_t     ptr_hal_sci_rx_callback,
                                               hal_sci_tx_callback_t     ptr_hal_sci_tx_callback,
                                               hal_sci_error_callback_t  ptr_hal_sci_error_callback);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Set baudrate of actual SCI channel.
*   \details    Not all values are possible, please check used oscillator and prescaler if it fits to your setup.
*
*   \pre        You should have called #hal_sci_init once before.
*
*   \param      ptr_sci_handle            [in] #struct_hal_sci_handle* Pointer to sci handle structure.
*   \param      baudrate                  [in] uint32_t New baudrate in bit/s.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_SCI_RETURN_VALUE as feedback.
*/
enum_HAL_SCI_RETURN_VALUE hal_sci_set_baudrate(struct_hal_sci_handle* ptr_sci_handle, uint32_t baudrate);
 /** @}*/
#endif
