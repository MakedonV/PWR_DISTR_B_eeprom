/*----------------------------------------------------------------------------*/
/**
* \file         sfl_bl_protocol_s32k.h
* \brief        Definitions for MRS Bootloader Protocol S32K wrapper.
*               Due to differences between the HCS BL protocol and S32K BL protocol implementations
*               the S32K relevant parts are kept here, separately from the main SFL BL Protocol.
*               The S32K functionality should be accessible by inclusion of the general SFL BL Protocol header file.
*
*               This file and its *.c companion represents macros, constants and wrapper functions which are used
*               either in SFL library or in the main application parts to do certain BL Protocol operations.
*               Only the necessary wrapper functions are presented, which can be supplemented by adding new
*               functions in future as needed.
*/
/*----------------------------------------------------------------------------*/

#ifndef SFL_BL_PROTOCOL_S32K_H__
#define SFL_BL_PROTOCOL_S32K_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "flexcan_driver.h"

// ---------------------------------------------------------------------------------------------------
// defines, typedefs
// ---------------------------------------------------------------------------------------------------
#define BL_CAN_RX_IDX   (2u) // This is the RX message box index which is reserved for the bootloader.
#define BL_CAN_TX_IDX   (6u) // This is the TX message box index which is reserved for the bootloader.

typedef enum
{
    SFL_BLP_ERROR_NONE = 0u,                ///< No error happened.
    SFL_BLP_ERROR_OPERATION,                ///< General failure of desired operation.
    SFL_BLP_ERROR_PORT,                     ///< CAN port is invalid or unrelated.
	SFL_BLP_NON_BLP_IDX,					///< Provided IDX is not equal to BLP dedicated RX IDX
} enum_SFL_BLP_ERROR_CODES;


/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Initialize BL protocol. This function should be called first before
*           calling any other BL related function.
* \details
*
* \pre
*
* \return   enum_SFL_BLP_ERROR_CODES 		                SFL_BLP_ERROR_NONE - success, otherwise failure code
*/
enum_SFL_BLP_ERROR_CODES sfl_bl_protocol_s32k_init();

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Check if the provided message box id is equal to the one dedicated for BL protocol
*           If the message box belongs to BL protocol the message will be processed further by BL protocol
* \details
*
* \pre
*
* \param    idx     [in] uint32_t							CAN message box id to  check against the BL-dedicated ID
* \return   enum_SFL_BLP_ERROR_CODES 		                SFL_BLP_ERROR_NONE - if equal, SFL_BLP_NON_BLP_IDX otherwise
*/
enum_SFL_BLP_ERROR_CODES sfl_bl_protocol_s32k_is_msg_for_bl(uint32_t idx, flexcan_state_t *flexcanState);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Maintains BL protocol state
* \details
*
* \pre
*
* \return   enum_SFL_BLP_ERROR_CODES 		                Always returns SFL_BLP_ERROR_NONE
*/
enum_SFL_BLP_ERROR_CODES sfl_bl_protocol_s32k_cyclic();

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Passes RX CAN message to BL protocol
* \details
*
* \pre
*
* \param    ptr_can_msg     [in] struct_hal_can_frame*		The RX data is passed to
*                                                      		BL protocol for further processing
* \return   enum_SFL_BLP_ERROR_CODES 		                SFL_BLP_ERROR_NONE - success, otherwise failure code
*/
enum_SFL_BLP_ERROR_CODES sfl_bl_protocol_s32k_process_rx_msg(const struct_hal_can_frame* ptr_can_msg);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Initialize BL protocol related CAN Rx and TX message processing
* \details
*
* \pre
*
* \return   enum_SFL_BLP_ERROR_CODES 		               SFL_BLP_ERROR_NONE - success, otherwise failure code
*/
enum_SFL_BLP_ERROR_CODES sfl_bl_protocol_s32k_rx_tx_init(uint8_t rx_msg_box_num, uint8_t tx_msg_box_num);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Sets EEPROM baudrate field with a given value
* \details
*
* \pre
*
* \param    ptr_can_handle  [in] struct_hal_can_handle*    	Active CAN handle
* \param    baudrate        [in] uint8_t    				Baudrate enumerated value
* \return   enum_SFL_BLP_ERROR_CODES 		                SFL_BLP_ERROR_NONE - success, otherwise failure code
*/
enum_SFL_BLP_ERROR_CODES sfl_bl_protocol_s32k_set_baudrate(const struct_hal_can_handle* ptr_can_handle, uint8_t baudrate);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Get Baudrate from EEPROM
* \details
*
* \pre
*
* \return   uint8_t 		                                bootloader baudrate wich is stored in EEPROM
*/
enum_SFL_BLP_ERROR_CODES sfl_bl_protocol_s32k_get_baudrate(void);
                                                                    
/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    Handful function to calculate the baudrate parameters based on arbitration & data phase info
* \details
*
* \pre
*
* \param    can_clock_hz    [in] uint32_t    				CAN clock in Hz
* \param    arbitr_baud_hz  [in] uint32_t    				Arbitration phase baudrate in Hz
* \param    data_baud_hz    [in] uint32_t    				Data phase baudrate in Hz
* \param    arbitr_sample_pnt_percent  [in] uint8_t    		Arbitration phase sample point in %
* \param    data_sample_pnt_percent  [in] uint8_t    		Data phase sample point in %
* \param    ptr_arbitr_tseg  [out] flexcan_time_segment_t*	Arbitration phase parameters for FlexCAN driver functions
* \param    ptr_data_tseg  [out] flexcan_time_segment_t*	Data phase parameters for FlexCAN driver functions
* \return   enum_SFL_BLP_ERROR_CODES 		                SFL_BLP_ERROR_NONE - success, otherwise failure code
*/
enum_SFL_BLP_ERROR_CODES sfl_bl_protocol_s32k_calc_baudrate_params( uint32_t can_clock_hz, uint32_t arbitr_baud_hz, uint32_t data_baud_hz,
																	uint8_t  arbitr_sample_pnt_percent, uint8_t  data_sample_pnt_percent,
																	flexcan_time_segment_t * ptr_arbitr_tseg, flexcan_time_segment_t * ptr_data_tseg);

#ifdef __cplusplus
}
#endif

#endif // SFL_BL_PROTOCOL_S32K_H__
