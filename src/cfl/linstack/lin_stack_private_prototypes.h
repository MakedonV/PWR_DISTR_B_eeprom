#ifndef LIN_STACK_PRIVATE_PROTOTYPES_H
#define LIN_STACK_PRIVATE_PROTOTYPES_H
/*----------------------------------------------------------------------------*/
/**
* \file         lin_stack_private_prototypes.h
* \brief        LIN stack private prototypes header file\n
*               additional file to LIN stack source code file (lin_stack.c)
* \date         20151015
* \author       Geissler
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup     lin
* \brief        LIN functionality
* \details
*/
/*----------------------------------------------------------------------------*/
#include "hal_data_types.h"
#include "lin_db_tables.h"
#include "lin_diagnose.h"
#include <string.h>

/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin
* \brief    Calculate the protected LIN ID (protected identifier field, parity)
* \details
*
* \pre
*
* \param    lin_id [in] uint8_t LIN ID
* \return   uint8_t protected LIN ID
*/
uint8_t private_lin_stack_calc_protected_lin_id( uint8_t lin_id ); // TODO function required?

/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin
* \brief    Calculate the LIN ID CRC(protected identifier field, parity)\n
*           and save it to a global array.
* \details
*
* \pre
*
* \return   void
*/
void private_lin_id_crc_calculate_and_save(void);


/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin
* \brief    help function to start a frame send, only with a frame index (call from idle)
* \details  get the needed bus information from the LIN frame database
*
* \pre
*
* \param    lin_frame_index [in] uint8_t    index of the LIN frame --> enum_lin_frame_id
* \return   void
*/
void private_lin_tx_frame_send_start(uint8_t lin_frame_index);


/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin
* \brief    help function to start a frame send, only with a frame index (call from interrupt)
* \details  get the needed bus information from the LIN frame database
*
* \pre
*
* \param    lin_frame_index [in] uint8_t    index of the LIN frame --> enum_lin_frame_id
* \return   void
*/
void private_lin_int_tx_frame_send_start(uint8_t lin_frame_index);


/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin
* \brief    help function to start a frame send only with a frame index
* \details  get the needed bus information from the LIN frame database
*
* \pre
*
* \param    lin_frame_index [in] uint8_t    index of the LIN frame --> enum_lin_frame_id
* \param    call_src        [in] uint8_t    call source (caller) for different calling from interrupt --> enum_lin_tx_frame_start_types
* \return   void
*/
void private_lin_transmit_frame_initiation(uint8_t lin_frame_index, uint8_t call_src);


/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin
* \brief    help function to save a received LIN message to receive fifo queue
* \details
*
* \pre
*
* \param    lin_module  [in] uint8_t            LIN index number --> enum_lin_bus_id
* \param    rx_msg [in] struct_lin_rx_msg_type
* \return   void
*/
void private_lin_module_save_rx_msg(uint8_t lin_module, struct_lin_rx_msg_type rx_msg);


/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin
* \brief    help function to get a saved message from the receive fifo queue
* \details
*
* \pre
*
* \param    lin_module  [in] uint8_t                LIN index number --> enum_lin_bus_id
* \param    msg     [in] struct_lin_rx_msg_type*    massage from the queue output
* \return   uint8_t                                 LIN_STATE_ERROR (queue is empty or wrong HW module), LIN_STATE_SUCCESS (message is in msg output)
*/
uint8_t private_lin_module_get_saved_rx_msg(uint8_t lin_module, struct_lin_rx_msg_type* msg);


/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin
* \brief    help function to set the LIN receiver of the stack into idle and enable the SCI break detection.
* \details
*
* \pre
*
* \param    lin_module  [in] uint8_t    LIN index number --> enum_lin_bus_id
* \param    lin_hw_module [in] uint8_t  LIN SCI HW number --> BIOS_SCI1, ...
* \return   void
*/
void private_lin_module_rx_idle_and_enable_header_reception(uint8_t lin_module, uint8_t lin_hw_module);

/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin
* \brief    help function to get the LIN bus module handler number from the corresponding SCI module / HW number
* \details
*
* \pre
*
* \param    module  [in] uint8_t    corresponding SCI module / HW number
* \return   uint8_t                 LIN bus module handler number (lin_module)
*/
uint8_t private_lin_get_lin_module(uint8_t module);


/*----------------------------------------------------------------------------*/
/**
* \ingroup  lin
* \brief    help function to get the database frame index
* \details
*
* \pre
*
* \param    lin_module [in] uint8_t corresponding SCI module / HW number
* \param    lin_id     [in] uint8_t LIN bus module handler number (lin_module)
* \return   uint16_t                LIN database frame index (needed for a correct database access)
*/
uint16_t private_lin_get_lin_frame_index(uint8_t lin_module, uint8_t lin_id);


#endif




