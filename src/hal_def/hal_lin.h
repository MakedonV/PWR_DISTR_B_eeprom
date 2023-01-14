#ifndef HAL_LIN_H
#define HAL_LIN_H
/*----------------------------------------------------------------------------*/
/**
* \file         hal_lin.h
* \brief        this is the hal_lin header, which includes definitions
*               of LIN handling & control
* \date         20170321
* \author       Harter
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup hal_lin
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
#define HAL_LIN_DATA_MAX               8u
#define HAL_LIN_SET_WAKE_UP            1u
#define HAL_LINE_FRAME_SEPARATE_MODE   1u

// ---------------------------------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------
typedef enum
{
    HAL_LIN_OK                      = 0u,     ///< no error
    HAL_LIN_ERROR_NO_CHANNEL_LEFT       ,     ///< no lin channel left on device
    HAL_LIN_ERROR_DATA_LEN_INVALID      ,     ///< data length invalid
    HAL_LIN_ERROR_DIR_INVALID           ,     ///< direction selection invalid
    HAL_LIN_ERROR_CRC_INVALID           ,     ///< crc selection invalid
    HAL_LIN_ERROR_NODE_INVALID          ,     ///< lin node invalid
    HAL_LIN_ERROR_RESPONSE_INVALID      ,     ///< response selection invalid
	HAL_LIN_ERROR_OTHER					      ///< error other than the ones listed above

}enum_HAL_LIN_RETURN_VALUE;

typedef enum
{
	HAL_LIN_BREAK_DETECTION_DISABLED 	= 0u,	// Disables "LIN Break Detection" Circuit and ENABLES normal data receive (for S32k)
	HAL_LIN_BREAK_DETECTION_ENABLED 	= 1u	// Enables "LIN Break Detection" Circuit and DISABLES normal data receive (for S32k)
}HAL_LIN_BREAK_DETECTION;


// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------

// TODO how to unify Callbacks for different stacks???
// use generic type??  typedef void (*funcptr)(void);
typedef void (*hal_lin_tx_callback_t)(uint8_t);
typedef void (*hal_lin_rx_callback_t)(uint8_t, uint8_t, uint8_t);
typedef void (*hal_lin_error_callback_t)(uint8_t, uint8_t);

// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
* \ingroup hal_lin
* \brief
* 	Initialize a (logical) LIN module (hardware mapping is done in the layer below) with the desired baudrate.
* 	Note that this will map the  UART unit to the LIN bus
* \details
*
* \pre
*
* \param    hw_module 				  [in] uint8_t	Number of the (hardware) LIN module
* \param    baudrate                  [in] uint32_t		Desired baudrate
* \return   enum_HAL_LIN_RETURN_VALUE					Return status
*/
enum_HAL_LIN_RETURN_VALUE hal_lin_init(uint8_t hw_module, uint32_t baudrate);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* Called to de-initialize LIN
*
* \details
*	This will release the assigned UART for using it for other purposes (e.g. RS232)
* \pre
*
* \param    hw_module [in] uint8_t
* \return   void
*/
void hal_lin_de_init(uint8_t hw_module);

/*
 * TODO how to make this more generic???
 * Possible solution could be a pointer array and length as parameters
 * but which function pointer types should be used? this can't be really generic...
 *
 *	typedef void (*funcptr_t)(void);
 *
 *	funcptr_t ary_fptr[3] = { ((funcptr_t)lin_tx_frame_handling, (funcptr_t)lin_rx_frame_handling, NULL ) };
 *
 * e.g.
 *	LIN_MRS_DRV_SetCallbacks( uint8_t module, void *function_callbacks[], uint8_t number_of_callbacks );
 * respectively
 *	LIN_MRS_DRV_SetCallbacks( uint8_t module, void **function_callbacks, uint8_t number_of_callbacks );
 *
 *	but the calling module needs to know which pointer types are supported... is this an issue at all?
 *
 *	Usage of dynamic parameter list could also be useful
 *
 */
/*----------------------------------------------------------------------------*/
/**
* \ingroup hal_lin
* \brief
* \details
*
* \pre
*
* \param    hw_module                 [in] uint8_t							Number of the (hardware) LIN module
* \param    ptr_lin_tx_callback       [in] hal_lin_tx_callback_t    const	Transmit callback (handling single bytes)
* \param    ptr_lin_rx_callback       [in] hal_lin_rx_callback_t    const	Receive callback (handling single bytes)
* \param    ptr_lin_error_callback    [in] hal_lin_error_callback_t const	Error callback
* \return   enum_HAL_LIN_RETURN_VALUE
*/
enum_HAL_LIN_RETURN_VALUE hal_lin_set_callback( uint8_t hw_module,
												hal_lin_tx_callback_t    const ptr_lin_tx_callback,
												hal_lin_rx_callback_t    const ptr_lin_rx_callback,
												hal_lin_error_callback_t const ptr_lin_error_callback);


/*----------------------------------------------------------------------------*/
/**
* \ingroup hal_lin
* \brief
*  Transmit a single byte
* \details
*
* \pre
*
* \param    hw_module [in] uint8_t	Number of the (hardware) LIN module
* \param    data      [in] uint8_t	Data to transmit
* \param    blocking  [in] bool		If true, the driver only returns if the byte could be transmitted or timeout occurred
* \return   void
*/
void hal_lin_write_byte(uint8_t hw_module, uint8_t data, bool blocking);


/*----------------------------------------------------------------------------*/
/**
* \ingroup hal_lin
* \brief
* 	Enable correct LIN break detection for Master Mode.
* \details
*
* \pre
*
* \param    hw_module [in] uint8_t	Number of the (hardware) LIN module
* \return   void
*/
void hal_lin_set_master_mode(uint8_t hw_module);


/*----------------------------------------------------------------------------*/
/**
* \ingroup hal_lin
* \brief
* 	Enable/disable the LIN line break detection interrupt and set desired line break detection length
* \details
*
* \pre
*
* \param    hw_module 		 [in] uint8_t	Number of the (hardware) LIN module
* \param    enable_interrupt [in] bool		TRUE = Line break detection interrupt enabled / FALSE = disables
* \param    break_length     [in] uint8_t	Sets the line break detection length
* \return   void
*/
void hal_lin_break_detection_enable(uint8_t hw_module, bool enable_interrupt, uint8_t break_length);


/*----------------------------------------------------------------------------*/
/**
* \ingroup hal_lin
* \brief
* 	Transmit a LIN break signal
* \details
*
* \pre
*
* \param    hw_module [in] uint8_t	Number of the (hardware) LIN module
* \return   void
*/
void hal_lin_send_break(uint8_t hw_module);


/*----------------------------------------------------------------------------*/
/**
* \ingroup hal_lin
* \brief
* 	Enables/disables transmit interrupt
* \details
*
* \pre
*
* \param    hw_module [in] uint8_t	Number of the (hardware) LIN module
* \param    ena    	  [in] bool		TRUE = enable transmit interrupt / FALSE = disable
* \return   void
*/
void hal_lin_tx_complete_int_enable(uint8_t hw_module, bool ena);


/*----------------------------------------------------------------------------*/
/**
* \ingroup hal_lin
* \brief
* 	Enables/disables receive interrupt
* \details
*
* \pre
*
* \param    hw_module [in] uint8_t	Number of the (hardware) LIN module
* \param    ena    	  [in] bool		TRUE = enable receive interrupt / FALSE = disable
* \return   void
*/
void hal_lin_rx_int_enable(uint8_t hw_module, bool ena);


#endif // #ifndef HAL_LIN_H
