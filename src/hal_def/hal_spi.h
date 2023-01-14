#ifndef HAL_SPI_H
#define HAL_SPI_H
/*----------------------------------------------------------------------------*/
/**
 * \file         hal_spi.h
 * \brief        this is the hal_spi header, which includes definitions
 *               of SPI master handling & control.
 * \date         20160725
 * \author       Harter
 *
 */
/*----------------------------------------------------------------------------*/
/**
 * \defgroup hal_spi
 * \brief
 * \details
 */
/*----------------------------------------------------------------------------*/
// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------
#include "hal_data_types.h"

// ---------------------------------------------------------------------------------------------------
// defines
// ---------------------------------------------------------------------------------------------------

// TODO: check and if possible, remove this before merge with master
// The following define is needed to disable some HAL SPI functions for slave.
// This is related to the bug in RTM 2.0.0 verion of SDK. Once the bug is fixed
// one can, hopefully, enable the macro without drawbacks. For more information
// see: https://community.nxp.com/thread/496344
//#define HAL_SPI_SLAVE_EXTENDED_FUNCTIONS_ENABLE

// ---------------------------------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------------------------------
#define CS_ENABLE(ptr_spi_handle)               \
    if ( ptr_spi_handle->cs_control != NULL )   \
        ptr_spi_handle->cs_control(1);

#define CS_DISABLE(ptr_spi_handle)              \
    if ( ptr_spi_handle->cs_control != NULL )   \
        ptr_spi_handle->cs_control(0);

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------
typedef enum
{
    HAL_SPI_OK                      = 0u,     ///< no error
    HAL_SPI_ERROR_GENERAL               ,     ///< common error
    HAL_SPI_ERROR_BUSY                  ,     ///< instance is busy with transferring data
    HAL_SPI_ERROR_INVALID_PARAM	        ,     ///< invalid parameter
    HAL_SPI_ERROR_INIT_FAILED           ,     ///< initialization failed
    HAL_SPI_ERROR_CONFIG_FAILED         ,     ///< error while configuration of SPI module
    HAL_SPI_ERROR_DEINIT_FAILED         ,     ///< de-initialization failed
    HAL_SPI_ERROR_CHANNEL_INVALID       ,     ///< channel invalid e.g. already not opened
    HAL_SPI_ERROR_NO_CHANNEL_LEFT       ,     ///< no channel left on device
    HAL_SPI_ERROR_SET_CALLBACK          ,     ///< error until set of callback
    HAL_SPI_ERROR_SET_BAUDRATE          ,     ///< error until set of baudrate
    HAL_SPI_ERROR_TX_DATA               ,     ///< error no tx data available
    HAL_SPI_ERROR_RX_DATA               ,     ///< error no rx data available
    HAL_SPI_ERROR_WHILE_READING         ,     ///< error during read operation
    HAL_SPI_ERROR_WHILE_WRITING         ,     ///< error during write operation
    HAL_SPI_ERROR_WHILE_TRANSFER        ,     ///< error during transfer operation
    HAL_SPI_ERROR_NOT_IMPLEMENTED             ///< error function not implemented

} enum_HAL_SPI_RETURN_VALUE;

// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------

// Handler for one SPI module
typedef struct
{
        void* ptr_handle;
        uint8_t channel;
        void (*cs_control)(int active); //< this callback function can be used to implement SW chip select
} struct_hal_spi_handle;

// Handler for one SPI frame
typedef struct
{
        uint8_t* ptr_tx_data;
        uint8_t* ptr_rx_data;
        uint16_t size;
} struct_hal_spi_frame;

// Handler for one SPI master configuration
typedef struct
{
        uint32_t spi_baudrate;
        uint8_t  spi_which_pcs;
        uint8_t  spi_pcs_polarity;
        bool     spi_pcs_contiuous_mode;
        uint8_t  spi_clock_phase;
        uint8_t  spi_clock_polarity;
        bool     spi_lsb_first;
        void     (*spi_cs_control)(int active); //< this callback function can be used to implement SW chip select
} struct_hal_spi_master_config;

// Handler for one SPI slave configuration
typedef struct
{
        uint8_t  spi_which_pcs;
        uint8_t  spi_pcs_polarity;
        uint8_t  spi_clock_phase;
        uint8_t  spi_clock_polarity;
        bool     spi_lsb_first;
} struct_hal_spi_slave_config;

// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief definition of hal_spi_master_init to initialize spi functionality
 * \details
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to spi handle structure
 * \param    spi_channel               [in] uint8_t channel selection
 * \param    ptr_spi_config            [in] pointer to configuration structure. If null the default values will be used
 * \return   enum_HAL_SPI_RETURN_VALUE
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_master_init(struct_hal_spi_handle* ptr_spi_handle, uint8_t spi_channel, const struct_hal_spi_master_config* ptr_spi_config);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief definition of hal_spi_slave_init to initialize spi functionality
 * \details
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to spi handle structure
 * \param    spi_channel               [in] uint8_t channel selection
 * \param    ptr_spi_config            [in] pointer to configuration structure. If null the default values will be used
 * \return   enum_HAL_SPI_RETURN_VALUE
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_slave_init(struct_hal_spi_handle* ptr_spi_handle, uint8_t spi_channel, const struct_hal_spi_slave_config* ptr_spi_config);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief definition of hal_spi_master_deinit function to de-intialize spi functionality
 * \details
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to spi handle structure
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_master_deinit(struct_hal_spi_handle* ptr_spi_handle);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief definition of hal_spi_slave_deinit function to de-intialize spi functionality
 * \details
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to spi handle structure
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_slave_deinit(struct_hal_spi_handle* ptr_spi_handle);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief definition of hal_spi_master_send function to send commands via spi. No data can be received with this function.
 * \details
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to spi handle structure
 * \param    ptr_tx_data               [in] uint8_t*               pointer to tx data buffer
 * \param    uint8_t                   [in] uint8_t                number of data bytes to transmit
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_master_send(struct_hal_spi_handle* ptr_spi_handle, uint8_t *ptr_tx_data, uint8_t size);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief   definition of hal_spi_slave_send function to send commands via spi. No data can be received with this function.
 * \details Similar to hal_spi_master_send but with blocking until the data is sent or timeout occurs.
 *          This function can use the spi_cs_control() callback for manual control of CS pin.
 *
 * \pre 
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to spi handle structure
 * \param    ptr_tx_data               [in] uint8_t*               pointer to tx data buffer
 * \param    uint8_t                   [in] uint8_t                number of data bytes to transmit
 * \param    timeout                   [in] uint32_t               timeout value in ticks
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_master_send_blocking(struct_hal_spi_handle* ptr_spi_handle, uint8_t *ptr_tx_data, uint8_t size,
														uint32_t timeout);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief   definition of hal_spi_slave_send function to send commands via spi. No data can be received with this function.
 * \details
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to spi handle structure
 * \param    ptr_tx_data               [in] uint8_t*               pointer to tx data buffer
 * \param    uint8_t                   [in] uint8_t                number of data bytes to transmit
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
#ifdef HAL_SPI_SLAVE_EXTENDED_FUNCTIONS_ENABLE
enum_HAL_SPI_RETURN_VALUE hal_spi_slave_send(struct_hal_spi_handle* ptr_spi_handle, uint8_t *ptr_tx_data, uint8_t size);
#endif // HAL_SPI_SLAVE_EXTENDED_FUNCTIONS_ENABLE

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief   definition of hal_spi_slave_send function to send commands via spi. No data can be received with this function.
 * \details Similar to hal_spi_slave_send but with blocking until the data is sent or timeout occurs.
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to spi handle structure
 * \param    ptr_tx_data               [in] uint8_t*               pointer to tx data buffer
 * \param    uint8_t                   [in] uint8_t                number of data bytes to transmit
 * \param    timeout                   [in] uint32_t               timeout value in ticks
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
#ifdef HAL_SPI_SLAVE_EXTENDED_FUNCTIONS_ENABLE
enum_HAL_SPI_RETURN_VALUE hal_spi_slave_send_blocking(struct_hal_spi_handle* ptr_spi_handle, uint8_t *ptr_tx_data, uint8_t size,
														uint32_t timeout);
#endif // HAL_SPI_SLAVE_EXTENDED_FUNCTIONS_ENABLE

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief definition of hal_spi_master_receive function to get response via spi. No data can be sent with this function.
 * \details
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to spi handle structure
 * \param    ptr_rx_data               [in] uint8_t*               pointer to rx data buffer
 * \param    uint8_t                   [in] uint8_t                number of data bytes to receive
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_master_receive(struct_hal_spi_handle* ptr_spi_handle, uint8_t *ptr_rx_data, uint8_t size);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief   definition of hal_spi_master_receive_blocking function to get response via spi. No data can be sent with this function.
 * \details Similar to hal_spi_master_receive but with blocking until the data is received or timeout occurs.
 *          This function can use the spi_cs_control() callback for manual control of CS pin.
 *
 * \pre 
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to spi handle structure
 * \param    ptr_rx_data               [in] uint8_t*               pointer to rx data buffer
 * \param    uint8_t                   [in] uint8_t                number of data bytes to receive
 * \param    timeout                   [in] uint32_t               timeout value in ticks
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_master_receive_blocking(struct_hal_spi_handle* ptr_spi_handle, uint8_t *ptr_rx_data, uint8_t size,
															uint32_t timeout);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief   definition of hal_spi_slave_receive function to get response via spi. No data can be sent with this function.
 * \details
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to spi handle structure
 * \param    ptr_rx_data               [in] uint8_t*               pointer to rx data buffer
 * \param    uint8_t                   [in] uint8_t                number of data bytes to receive
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
#ifdef HAL_SPI_SLAVE_EXTENDED_FUNCTIONS_ENABLE
enum_HAL_SPI_RETURN_VALUE hal_spi_slave_receive(struct_hal_spi_handle* ptr_spi_handle, uint8_t *ptr_rx_data, uint8_t size);
#endif // HAL_SPI_SLAVE_EXTENDED_FUNCTIONS_ENABLE

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief   definition of hal_spi_slave_receive function to get response via spi. No data can be sent with this function.
 * \details Similar to hal_spi_slave_receive but with blocking until the data is received or timeout occurs.
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to spi handle structure
 * \param    ptr_rx_data               [in] uint8_t*               pointer to rx data buffer
 * \param    uint8_t                   [in] uint8_t                number of data bytes to receive
 * \param    timeout                   [in] uint32_t               timeout value in ticks
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
#ifdef HAL_SPI_SLAVE_EXTENDED_FUNCTIONS_ENABLE
enum_HAL_SPI_RETURN_VALUE hal_spi_slave_receive_blocking(struct_hal_spi_handle* ptr_spi_handle, uint8_t *ptr_rx_data, uint8_t size,
															uint32_t timeout);
#endif // HAL_SPI_SLAVE_EXTENDED_FUNCTIONS_ENABLE

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief definition of hal_spi_master_transfer function to transfer data via SPI. Use to both send and receive data
 * \details
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to SPI handle structure
 * \param    ptr_spi_frame             [in] struct_hal_spi_frame*  pointer to spi frame structure
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_master_transfer(struct_hal_spi_handle* ptr_spi_handle, struct_hal_spi_frame* ptr_spi_frame);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief   definition of hal_spi_master_transfer function to transfer data via SPI. Use to both send and receive data
 * \details Similar to hal_spi_master_transfer but with blocking until the data is transferred or timeout occurs.
 *          This function can use the spi_cs_control() callback for manual control of CS pin.
 *
 * \pre 
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to SPI handle structure
 * \param    ptr_spi_frame             [in] struct_hal_spi_frame*  pointer to spi frame structure
 * \param    timeout                   [in] uint32_t               timeout value in ticks
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_master_transfer_blocking(struct_hal_spi_handle* ptr_spi_handle, struct_hal_spi_frame* ptr_spi_frame,
															uint32_t timeout);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief   definition of hal_spi_slave_transfer function to transfer data via SPI. Use to both send and receive data
 * \details
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to SPI handle structure
 * \param    ptr_spi_frame             [in] struct_hal_spi_frame*  pointer to spi frame structure
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_slave_transfer(struct_hal_spi_handle* ptr_spi_handle, struct_hal_spi_frame* ptr_spi_frame);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief   definition of hal_spi_slave_transfer function to transfer data via SPI. Use to both send and receive data
 * \details Similar to hal_spi_slave_transfer but with blocking until the data is transferred or timeout occurs.
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to SPI handle structure
 * \param    ptr_spi_frame             [in] struct_hal_spi_frame*  pointer to spi frame structure
 * \param    timeout                   [in] uint32_t               timeout value in ticks
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_slave_transfer_blocking(struct_hal_spi_handle* ptr_spi_handle, struct_hal_spi_frame* ptr_spi_frame,
															uint32_t timeout);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief   definition of hal_spi_master_get_transfer_status function to check the status of the last transfer.
 * \details
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to SPI handle structure
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_master_get_transfer_status(struct_hal_spi_handle* ptr_spi_handle);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief definition of hal_spi_master_get_transfer_status function to check the status of the last transfer.
 * \details
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to SPI handle structure
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_slave_get_transfer_status(struct_hal_spi_handle* ptr_spi_handle);

/*----------------------------------------------------------------------------*/
/**
 * \ingroup
 * \brief   definition of hal_spi_master_set_delay function to setup delay times
 * \details 
 *
 * \pre
 *
 * \param    ptr_spi_handle            [in] struct_hal_spi_handle* pointer to SPI handle structure
 * \param    delay_CS_to_SCK           [in] uint32_t               CS to SCK delay (in microseconds)
 * \param    delay_SCK_to_CS           [in] uint32_t               SCK to CS delay (in microseconds)
 * \param    delay_betwen_transfers    [in] uint32_t               delay between SPI transfers (in microseconds)
 * \return   enum_HAL_SPI_RETURN_VALUE feedback of function
 */
enum_HAL_SPI_RETURN_VALUE hal_spi_master_set_delay(struct_hal_spi_handle* ptr_spi_handle, uint32_t delay_CS_to_SCK, uint32_t delay_SCK_to_CS, uint32_t delay_betwen_transfers);


#endif
