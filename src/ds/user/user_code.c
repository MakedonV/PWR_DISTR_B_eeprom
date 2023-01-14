/*----------------------------------------------------------------------------*/
/**
 * \file         user_code.c
 * \brief        User code function implementation
 * \details
 * \date         20181001
 *
 */
/*----------------------------------------------------------------------------*/
#include "user_code.h"

uint32_t ENGINE_RPM, ENGINE_RPM_DSP, RPM_Total, RPM_Average, ENG_HRS_EEPROM, ENG_HRS_1sec;
uint32_t hour_flag, timer, display_timer_1000, display_timer_180;


uint8_t byte_H2_hour, byte_H1_hour,	byte_L2_hour, byte_L1_hour; 
uint8_t byte_L_rpm, byte_H_rpm;

uint8_t eeprom_val[3];
uint8_t counter_rpm;

	uint16_t counter = 0;
	


/*----------------------------------------------------------------------------*/
/**
 * \internal
 *
 * \endinternal
 *
 *
 * \test STATUS: *** UNTESTED ***
 */
void usercode_init(void)
{
	
	user_eeprom_read(0x10, sizeof(eeprom_val), &eeprom_val[0]);
	user_eeprom_read(0x20, sizeof(eeprom_val), &eeprom_val[1]);
	user_eeprom_read(0x30, sizeof(eeprom_val), &eeprom_val[2]);
	user_eeprom_read(0x40, sizeof(eeprom_val), &eeprom_val[3]);

	ENG_HRS_EEPROM = eeprom_val[0] | (eeprom_val[1] << 8) | (eeprom_val[2] << 16) |(eeprom_val[3] << 24);
		
	byte_H2_hour = ENG_HRS_EEPROM >> 24;
	byte_H1_hour = ENG_HRS_EEPROM >> 16;
	byte_L2_hour = ENG_HRS_EEPROM >> 8;
	byte_L1_hour = ENG_HRS_EEPROM & 0xFF;	
	user_can_send_msg(CAN_BUS_0, 0x18FEE500, EXTENDED_ID, 8, byte_L1_hour, byte_L2_hour, byte_H1_hour, byte_H2_hour, 0xFF, 0xFF, 0xFF, 0xFF);
	


	// user_eeprom_write(0x90, sizeof(8), 0);
	// user_eeprom_write(0x91, sizeof(8), 0);
	// user_eeprom_write(0x92, sizeof(8), 0);
	// user_eeprom_write(0x93, sizeof(8), 0);
	
	// user_can_send_msg(CAN_BUS_0, 0x001, STANDARD_ID, 8, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
}


/*----------------------------------------------------------------------------*/
/**
 * \internal
 *
 * \endinternal
 *
 *
 * \test STATUS: *** UNTESTED ***
 */
void usercode(void)
{	
	
	counter++;
	
	if (counter == 1000)
	{
		user_eeprom_read(0x90, sizeof(eeprom_val), &eeprom_val[0]);
		user_eeprom_read(0x91, sizeof(eeprom_val), &eeprom_val[1]);
		user_eeprom_read(0x92, sizeof(eeprom_val), &eeprom_val[2]);
		user_eeprom_read(0x93, sizeof(eeprom_val), &eeprom_val[3]);
		
		user_can_send_msg(CAN_BUS_0, 0x100, STANDARD_ID, 8, eeprom_val[0], eeprom_val[1], eeprom_val[2], eeprom_val[3], 0xFF, 0xFF, 0xFF, 0xFF);
		
		counter = 0;
	}	



/*
//##################################-----------------------------------------------------------------------------------
//#                                #-----------------------------------------------------------------------------------
//#   EEPROM                       #
//#                                #-----------------------------------------------------------------------------------
//##################################-----------------------------------------------------------------------------------


// The EEPROM user area spans from EE_USER_START (2048) to EE_USER_END (4095) bytes is reserved as user area.
// Only these 2048 bytes are available for the customer to use.
// The other 2048 bytes are reserved for factory data, calibration data and others.
// The user_eeprom_write and user_eeprom_read functions are working with relative addresses. When writing to addr = 50
// the functions will calculate the absolute address of the data. This means that the customer can access the user EEPROM
// by using the addresses between 0 and 2047.
// Wrong memory address or the combination of length (len) and address (addr) returns an error code.

//---------------------------------------------------------------------------------------------------------------------
//  Procedure       enum_HAL_NVM_RETURN_VALUE user_eeprom_write(uint32_t addr, uint32_t len, const uint8_t *ptr_data)
//
//  Overview        write eeprom
//
//  Input           addr        -   Relative address of the data [0 - 2047]
//                  len         -   Length in bytes
//                  *ptr_data   -   Pointer to uint8_t variable or array, to write data
//  Output          result      -   0 = success

    Example:        // Write one byte with the value 4 to address 0x04 and one with the value 2 to address 0x05 in the USER EEPROM
                    uint8_t write_val[2];
                    write_val[0] = 4;
                    write_val[1] = 2;
                    user_eeprom_write(0x04, sizeof(write_val), &write_val[0]);

//---------------------------------------------------------------------------------------------------------------------
//  Procedure       enum_HAL_NVM_RETURN_VALUE user_eeprom_read(uint32_t addr, uint32_t len, uint8_t *ptr_data)
//
//  Overview        read eeprom
//
//  Input           addr        -   Relative address of the data [0 - 2047]
//                  len         -   Length in bytes
//  Return          *ptr_data   -   Pointer to uint8_t variable or array to save read data in it
//  Output          result      -   0 = success

    Example:        // Read out two bytes starting at address 0x04 of the USER EEPROM area
                    uint8_t read_val[2];
                    user_eeprom_read(0x04, sizeof(read_val), &read_val[0]);

//---------------------------------------------------------------------------------------------------------------------
//  Procedure       uint16_t user_eeprom_read_module_eeprom_version();
//
//  Overview        read specific module data from the EEPROM. Used for integer type values.
//                  There exists one function for every factory data. See user_api_eeprom.h
//
//  Return          uint16_t   -   Value "eeprom version" from the factory data saved in the EEPROM.
 *
    Example:        // Save the return value of the function in a appropriate type.
                    uint16_t eeprom_version = 0;
                    eeprom_version = user_eeprom_read_module_eeprom_version();

//---------------------------------------------------------------------------------------------------------------------
//  Procedure       enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_drawing_nr(uint8_t buffer[], uint8_t size_buffer, bool filter);
//
//  Overview        read specific module data from the EEPROM. Used for ASCII coded values.
//                  There exists one function for every factory data. See user_api_eeprom.h
//
//  Input           size_buffer -   size of the given buffer. Must be appropriately sized. Refer to the documentation. (use for this value sizeof(buffer))
//                  filter      -   FALSE to read the raw data or TRUE to filter unnecessary whitespace.
//  Return          *buffer     -   Pointer to uint8_t variable or array to save read data in it.
//  Output          result      -   0 = success

    Example:        // Part number is a string with up to 21 characters. Example of a drawing number: "2.156.320.00"
                    uint8_t draw_nr[21] = {0};
                    user_eeprom_read_module_drawing_nr(draw_nr, sizeof draw_nr, 1);

*/
}


/*----------------------------------------------------------------------------*/
/**
	Regardless of what has been received, each CAN message can be evaluated here
*
    TO ENABLE:  set #define SET_CALLBACK_CAN_MESSAGE_RECEIVE in user_code.h
*
*/
void user_can_receive_msg_callback(uint8_t bus_id, bios_can_msg_typ* msg)
{
 

}


/*----------------------------------------------------------------------------*/
/**
*   Receive interrupt of serial communication interface (RS232)
*   instance is the UART interface number, e.g. 0 for UART0
*   data is 8 bit of data. this routine is called for every byte received.
*
*/
void user_int_rx_sci(uint8_t instance, uint8_t data)
{
    // don't put too much code in here. this is called in an interrupt!
}


/*----------------------------------------------------------------------------*/
/**
	Interrupt every 1ms
*
    TO ENABLE:  set #define SET_CALLBACK_1MS_TIMER in user_code.h
*/
void user_int_timer_1ms(void)
{
	ENGINE_RPM = user_freq_get_measured_freq(FREQ_IN1);
	RPM_Total += ENGINE_RPM;

user_can_db_set_signal_value(_ENGINE_RPM, ENGINE_RPM);
user_can_db_set_signal_value(_RPM_Total, RPM_Total);

	display_timer_1000++;

	if (display_timer_1000 == 1000)
	{
		RPM_Average = RPM_Total/1000;
		ENGINE_RPM_DSP = RPM_Average * 8; //* 41.6;
		byte_H_rpm = ENGINE_RPM_DSP >> 8;
		byte_L_rpm = ENGINE_RPM_DSP & 0xFF;	
		RPM_Total = 0;

		ENG_HRS_1sec += RPM_Average/16;
	

user_can_db_set_signal_value(_RPM_Average, RPM_Average);
user_can_db_set_signal_value(_ENG_HRS_1sec, ENG_HRS_1sec);



		if(ENG_HRS_1sec >= 18000) //1800-18 sec, 18000-180sec-3min_1bite_in_PGN:65253 
		{
			ENG_HRS_1sec = 0;

			ENG_HRS_EEPROM ++;

            byte_H2_hour = ENG_HRS_EEPROM >> 24;
	        byte_H1_hour = ENG_HRS_EEPROM >> 16;
	        byte_L2_hour = ENG_HRS_EEPROM >> 8;
	        byte_L1_hour = ENG_HRS_EEPROM & 0xFF;	

            user_eeprom_write(0x10, sizeof(byte_L1_hour), &byte_L1_hour);
			user_eeprom_write(0x20, sizeof(byte_L2_hour), &byte_L2_hour);
			user_eeprom_write(0x30, sizeof(byte_H1_hour), &byte_H1_hour);
			user_eeprom_write(0x40, sizeof(byte_H2_hour), &byte_H2_hour);
		}

user_can_db_set_signal_value(_ENG_HRS_EEPROM, ENG_HRS_EEPROM);

		// RPM  PGN:61444 
		user_can_send_msg(CAN_BUS_0, 0x18F00400, EXTENDED_ID, 8, 0xFF, 0xFF, 0xFF, byte_L_rpm, byte_H_rpm, 0xFF, 0xFF, 0xFF);	
		// engine hours PGN:65253 
		user_can_send_msg(CAN_BUS_0, 0x18FEE500, EXTENDED_ID, 8, byte_L1_hour, byte_L2_hour, byte_H1_hour, byte_H2_hour, 0xFF, 0xFF, 0xFF, 0xFF);
		
		display_timer_1000 = 0;
	}
	
	
	// dont put too much code in here. this is called in an interrupt!
}

