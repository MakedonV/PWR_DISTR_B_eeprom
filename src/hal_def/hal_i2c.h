#ifndef _HAL_I2C_H_
#define _HAL_I2C_H_
/*----------------------------------------------------------------------------*/
/**
 * \file         hal_i2c.h
 * \brief        This header contains the definition of the I2C module. The driver
*                is only for master operation.
 * \date         20180803
 *
 */
/*----------------------------------------------------------------------------*/
/**
 * \defgroup
 * \brief
 * \details
 */
/*----------------------------------------------------------------------------*/
#include "hal_data_types.h"


// Enumeration with error codes
typedef enum
{
    HAL_I2C_OK                      = 0u,       ///< No error
    HAL_I2C_ERROR_GENERAL               ,       ///< General error
    HAL_I2C_ERROR_INIT_FAILED           ,       ///< Error while initializing the module
    HAL_I2C_ERROR_DEINIT_FAILED         ,       ///< Error while de-initializing the module
    HAL_I2C_ERROR_NO_CHANNEL_LEFT       ,       ///< Error there is no channel left
    HAL_I2C_ERROR_WHILE_SENDING         ,       ///< Error while sending data
    HAL_I2C_ERROR_WHILE_RECEIVING       ,       ///< Error while receiving data

} enum_HAL_I2C_RETURN_VALUE;


// Handle which holds I2C info
typedef struct
{
        void* i2c_handle_ptr;       ///< Structure for identification of I2C module
        uint8_t i2c_instance;       ///< Number of the I2C module
} struct_hal_i2c_handle;

// Structure which holds the TX and RX data
typedef struct
{
        uint8_t* i2c_rx_buffer;     ///< Receive buffer of the I2C module
        uint32_t i2c_rx_size;       ///< Size of the RX buffer
        uint8_t* i2c_tx_buffer;     ///< Transmit buffer of the I2C module
        uint32_t i2c_tx_size;       ///< Size of the TX buffer
        uint8_t  i2c_stop_bit;      ///< Stop bit
        uint32_t timeout_size;      ///< Timeout size
} struct_hal_i2c_frame;

// Handler for one I2C master configuration
typedef struct
{
        uint32_t baudrate;
        uint16_t slaveAddress;                      /*!< Slave address, 7-bit or 10-bit */
        bool is10bitAddr;                           /*!< Selects 7-bit or 10-bit slave address */
} struct_hal_i2c_master_config;

/*----------------------------------------------------------------------------*/
/**
 * \ingroup  hal_i2c
 * \brief    This function initializes the I2C master
 * \details
 *
 * \param    ptr_i2c_handle            [in] struct_hal_i2c_handle* Handle which contains the information of the the I2C module
 * \param    instance                  [in] uint8_t Define the I2C module
 * \param    baudrate                  [in] uint32_t Baudrate of the I2C module, absolute Number e.g. 4000000U
 * \return   enum_HAL_I2C_RETURN_VALUE Result of the function as error code
 */
enum_HAL_I2C_RETURN_VALUE hal_i2c_master_init(struct_hal_i2c_handle* ptr_i2c_handle, uint8_t instance, const struct_hal_i2c_master_config *i2c_config);


/*----------------------------------------------------------------------------*/
/**
 * \ingroup  hal_i2c
 * \brief    This function de-initializes the I2C master
 * \details
 *
 * \param    ptr_i2c_handle            [in] struct_hal_i2c_handle* Handle which contains the information of the the I2C module
 * \return   enum_HAL_I2C_RETURN_VALUE Result of the function as error code
 */
enum_HAL_I2C_RETURN_VALUE hal_i2c_master_deinit(struct_hal_i2c_handle* ptr_i2c_handle);


/*----------------------------------------------------------------------------*/
/**
 * \ingroup  hal_i2c
 * \brief    This function sends the content of a buffer via I2C
 * \details  The "i2c_tx_buffer", contained inside the struct_hal_i2c_handle pointer is sent blocking via the I2C module
 *
 * \param    ptr_i2c_handle            [in] struct_hal_i2c_handle* Handle which contains the information of the the I2C module
 * \param    size                      [in] uint8_t Number of bytes to be send
 * \return   enum_HAL_I2C_RETURN_VALUE Result of the function as error code
 */
enum_HAL_I2C_RETURN_VALUE hal_i2c_master_send_blocking(struct_hal_i2c_handle* ptr_i2c_handle, struct_hal_i2c_frame* ptr_i2c_frame);


/*----------------------------------------------------------------------------*/
/**
 * \ingroup  hal_i2c
 * \brief    This function receives a buffer via I2C
 * \details  The "i2c_rx_buffer", contained inside the struct_hal_i2c_handle pointer, is filled with the received data. This function reads blocking.
 *
 * \param    ptr_i2c_handle            [in] struct_hal_i2c_handle* Handle which contains the information of the the I2C module
 * \param    size                      [in] uint8_t Number of bytes to be send
 * \return   enum_HAL_I2C_RETURN_VALUE Result of the function as error code
 */
enum_HAL_I2C_RETURN_VALUE hal_i2c_master_receive_blocking(struct_hal_i2c_handle* ptr_i2c_handle, struct_hal_i2c_frame* ptr_i2c_frame);


/*----------------------------------------------------------------------------*/
/**
* \internal
* Function for I2C master set address of slave.
* \endinternal
*/
/*----------------------------------------------------------------------------*/
/**
 * \ingroup  hal_i2c
 * \brief    This function sets an address of slave device
 * \details
 *
 * \param    ptr_i2c_handle            [in] struct_hal_i2c_handle* Handle which contains the information of the the I2C module
 * \param    address                   [in] uint16_t Slave device address
 * \param    is10bitAddr               [in] bool Slave device address length
 * \return   none
 */
enum_HAL_I2C_RETURN_VALUE hal_i2c_master_set_slave_address(struct_hal_i2c_handle* ptr_i2c_handle, uint16_t address, bool is10bitAddr);


#endif
