
/*
 * Minimal UART implementation for MRS LIN Stack
 *
 * lpuart_driver_mrs_lin.h
 *
 */

#ifndef LPUART_DRIVER_MRS_LIN_H__
#define LPUART_DRIVER_MRS_LIN_H__

// TODO cleanup includes
#include <stddef.h>
#include <stdbool.h>
#include "device_registers.h"
#include "status.h"
#include "interrupt_manager.h"
#include "lpuart_hw_access.h"
#include "hal_data_types.h" // TODO is it ok to include a HAL header in this layer?

// **********************************
// * Enumerations
// **********************************
typedef enum
{
    LIN_MRS_DRV_OK                      = 0u,     ///< no error
    LIN_MRS_DRV_ERROR_NO_CHANNEL_LEFT       ,     ///< no lin channel left on device
    LIN_MRS_DRV_ERROR_DATA_LEN_INVALID      ,     ///< data length invalid
    LIN_MRS_DRV_ERROR_DIR_INVALID           ,     ///< direction selection invalid
    LIN_MRS_DRV_ERROR_CRC_INVALID           ,     ///< crc selection invalid
    LIN_MRS_DRV_ERROR_NODE_INVALID          ,     ///< lin node invalid
    LIN_MRS_DRV_ERROR_RESPONSE_INVALID      ,     ///< response selection invalid
	LIN_MRS_DRV_ERROR_OTHER					      ///< error other than the ones listed above

}enum_LIN_MRS_DRV_RETVAL;


// **********************************
// * Typedefs
// **********************************
// Callback function pointer type definitions
typedef void (*lin_tx_callback_t)(uint8_t);
typedef void (*lin_rx_callback_t)(uint8_t, uint8_t, uint8_t);
typedef void (*lin_error_callback_t)(uint8_t, uint8_t);


// **********************************
// * Function Prototypes
// **********************************
/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*	Initialize a LIN module with the specified baudrate.
*
*	Note that this will automatically de-activate the standard UART functionality.
*	Call LIN_MRS_DRV_De_Init() using standard UART again.
* \pre
*
* \param    module   [in] uint8_t	Logical number of LIN module
* \param    baudrate [in] uint32_t	Baudrate [baud]
* \return   status_t ! Will always return success - kept for compatibility reasons !
*/
status_t LIN_MRS_DRV_Init(uint8_t module, uint32_t baudrate);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*
* \pre
*
* \param    hw_module [in] uint8_t
* \return   void
*/
void LIN_MRS_DRV_De_Init(uint8_t hw_module);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*	Set the baudrate of the specified LIN (logical) module.
* \pre
*
* \param    module          [in] uint8_t	Logical number of LIN module
* \param    desiredBaudRate [in] uint32_t	Baudrate [baud]
* \return   void
*/
void LIN_MRS_DRV_SetBaudRate(uint8_t module, uint32_t desiredBaudRate);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*	Main IRQ handler that is called from the UART ISRs
* \pre
*
* \param    module [in] uint32_t		Logical LIN module to access
* \return   void
*/
void LIN_MRS_DRV_IRQHandler(uint32_t module); // TODO leave uint32_t because of speed optimization or change to uint8_t?


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*	Set callbacks of the LIN stack functions that are called by the UART ISR(s)
* \pre
*
* \param    module                 [in] uint8_t
* \param    ptr_lin_tx_callback    [in] lin_tx_callback_t    const 		Function pointer for the LIN stack's TX function
* \param    ptr_lin_rx_callback    [in] lin_rx_callback_t    const		Function pointer for the LIN stack's RX function
* \param    ptr_lin_error_callback [in] lin_error_callback_t const		Function pointer for the LIN stack's error handler
* \return   uint8_t
*/

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
 */

uint8_t LIN_MRS_DRV_SetCallbacks( uint8_t module,
								  lin_tx_callback_t 	const ptr_lin_tx_callback,
								  lin_rx_callback_t 	const ptr_lin_rx_callback,
								  lin_error_callback_t 	const ptr_lin_error_callback);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*	Get the base address of a LIN HW unit via logical LIN module number
* \pre
*
* \param    module [in] uint8_t		Logical number of LIN module
* \return   LPUART_Type *const		Base address pointer to the specified LIN HW unit
*/
LPUART_Type *const LIN_MRS_DRV_GetUartBase(uint8_t module);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*	Set LIN break detection length
* \pre
*
* \param    base   [in] LPUART_Type *const			Base address pointer to the LIN HW unit to access
* \param    length [in] lpuart_break_char_length_t	Length: LPUART_BREAK_CHAR_10_BIT_MINIMUM or LPUART_BREAK_CHAR_13_BIT_MINIMUM
* \return   void
*/
void LIN_MRS_DRV_SetBreakCharDetectLength(LPUART_Type *const base, lpuart_break_char_length_t length); // TODO make inline again??


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*	This function is used to generate a LIN break (in Master Mode)
* \pre
*
* \param    base [in] LPUART_Type *const	Base address pointer to the LIN HW unit to access
* \return   void
*/
void LIN_MRS_DRV_QueueBreakField(LPUART_Type *const base);  // TODO make inline again?


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*	Enable correct LIN break detection for Master Mode: TODO check if correct!
* \pre
*
* \param    module [in] uint8_t		Logical LIN module to access
* \return   void
*/
void LIN_MRS_DRV_SetMasterMode(uint8_t hw_module);


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief
* \details
*	This is used in the UART interrupt handlers to check if LIN is active for the selected UART module or not.
*	If LIN is active, the ISR jumps to LIN_MRS_DRV_IRQHandler(), otherwise to the standard UART handler.
* \pre
*
* \param    module [in] uint8_t		Logical LIN module to access
* \return   bool
*/
bool LIN_MRS_DRV_LinIsActive(uint8_t hw_module); // TODO change to uint32_t for speed improvement? // TODO improve naming


/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/


#endif /* LPUART_DRIVER_MRS_LIN_H__ */
/******************************************************************************/
/* EOF */
/******************************************************************************/
