#ifndef _LIN_DIAGNOSE_H_
#define _LIN_DIAGNOSE_H_
/*----------------------------------------------------------------------------*/
/**
* \file         lin_diagnose.h
* \brief        header for LIN diagnose functionality
* \date         20151019
* \author       Geissler
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup     lin_diag
* \brief        LIN diagnose functionality
* \details
*/
/*----------------------------------------------------------------------------*/
#include "lin_stack.h"

// --------------------------------------------------------------------------------
// LIN diagnose defines
// --------------------------------------------------------------------------------
#define NEGATIVE_RESPONSE_LEN       (uint8_t)0x03
#define NEGATIVE_RESPONSE_SID       (uint8_t)0x7F
#define DIAG_RESP_DATA_BUFFER_SIZE  (uint8_t)0x05

#define ASSIGN_NAD_SID                  (uint8_t)0xB0
#define ASSIGN_FRAME_IDENTIFIER_SID     (uint8_t)0xB1
#define READ_BY_DATA_IDENTIFIER_SID     (uint8_t)0xB2
#define WRITE_BY_DATA_IDENTIFIER_SID    (uint8_t)0x2E   // --> only for MRS write serial number

#define NRC_SUB_FUNCTION_NOT_SUPPORTED  (uint8_t)0x12

#define LIN_CONFIGURED_NAD (uint8_t)0x00
#define LIN_WILD_CARD_ID   (uint8_t)0x7F

/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin_diag
* \brief    LIN diagnose slave request frame received
* \details
*
* \pre
*
* \param    lin_module  [in] uint8_t    LIN index number --> enum_lin_bus_id
* \return   void
*/
void lin_diag_master_request(uint8_t lin_module);

/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin_diag
* \brief    LIN diagnose slave response frame header received, \n
*           to prepare the response data.
* \details
*
* \pre
*
* \param    lin_module  [in] uint8_t    LIN index number --> enum_lin_bus_id
* \return   uint8_t                     LIN slave response result \n
*                                       LIN_STATE_SUCCESS --> response data OK send response \n
*                                       LIN_STATE_ERROR --> response data wrong send no response
*/
uint8_t lin_diag_slave_response(uint8_t lin_module);

#endif
