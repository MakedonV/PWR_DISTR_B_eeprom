#ifndef SFL_CAN_DB_VERSION_H
#define SFL_CAN_DB_VERSION_H
/*----------------------------------------------------------------------------*/
/**
* \file         sfl_can_db_version.h
* \brief        Version information for MRS CAN DB functionality.
*/
/*----------------------------------------------------------------------------*/
/**
* \addtogroup   sfl_can_db 
* \{
*   History:
*   --------
*   Increment SFL_CAN_DB_VERSION on each change.
*
*   Version Number | Description
*   ---------------|--------------------------------------------------------------------
*                1 | Initial version.
*                2 | - converted function sfl_can_db_transnmit_deactive ro sfl_can_db_stop_transmission (refer commentary of function)
*                  |   -> it is backwards compatible due to wrapper
*                  | - added function sfl_can_db_stop_gateway_for_known_ids (refer commentary of function)
*                  | - added function sfl_can_db_stop_gateway_for_unknown_ids (refer commentary of function)
*/
#define SFL_CAN_DB_VERSION   2u   ///< Version Number (integer) for MRS can db functionality

/** \} */
#endif // SFL_CAN_DB_VERSION_H

