#ifndef _USER_CODE_H_
#define _USER_CODE_H_
/*----------------------------------------------------------------------------*/
/**
* \file         user_code.h
* \brief        User code function header
* \date         20181001
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup
* \brief
* \details
*/
/*----------------------------------------------------------------------------*/


#include "graph_support.h"

#define SET_CALLBACK_1MS_TIMER
#define SET_CALLBACK_CAN_MESSAGE_RECEIVE

// Define the user data EEPROM start address and size
#define USERDATENSATZ_START 2048
#define USERDATENSATZ_SIZE  2048

// Define the threshold of the digital input emulation of the analog inputs (set to 6V by default). This value is referencing
// to the maximum digits of the analog input. Default value is 2/3 of analog input resolution which is
// 4096. => 2/3 x 4096 = 2730 digits.
// Conversion result >= 2730 resembles digital "1"
// Conversion result <  2730 resembles digital "0"
//#define DI_EMULATION_THRESHOLD  2730    // unit: digits


/*----------------------------------------------------------------------------*/
/**
* \ingroup  usercode
* \brief    This function is called prior to the main-loop. The user can place his
*           own initializations here.
* \details
*
* \pre
*
* \return   void
*/
void usercode_init(void);


/*----------------------------------------------------------------------------*/
/**
* \ingroup  usercode
* \brief    This function is called periodically in the main function. The user
*           can place his application here.
* \details
*
* \pre      none
*
* \return   void
*/
void usercode(void);


/*----------------------------------------------------------------------------*/
/**
* \ingroup  usercode
* \brief    Receive a message from CAN bus
*
* \details
*
* \pre      none
*
* \return   void
*/
void user_can_receive_msg_callback(uint8_t bus_id, bios_can_msg_typ* msg);


/*----------------------------------------------------------------------------*/
/**
* \ingroup  usercode
* \brief    Receive a message from SCI bus
*
* \details
*
* \pre      none
*
* \return   void
*/
void user_int_rx_sci(uint8_t instance, uint8_t data);


/*----------------------------------------------------------------------------*/
/**
* \ingroup  usercode
* \brief    Interrupt each 1ms
*
* \details
*
* \pre      none
*
* \return   void
*/
void user_int_timer_1ms(void);

#endif
