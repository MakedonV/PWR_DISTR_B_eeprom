/*
 * user_api_eeprom.c
 *
 *  Created on: 14.10.2019
 *      Author: VGoertz
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include "defines_general.h"

// The following header should be included first
#include "role_types.h"
// The following header can be placed anywhere after inclusion of role_types.h
#include "role_base.h"
#include "role_board.h"
#include "ee_helper.h"
// The following header should be included last
#include "role.h"

#include "user_api_eeprom.h"
#include "hal_nvm.h"

FILENUM(23)   ///< This is to ease the tracking of assert failures. Each file should have its own unique file number.



/*----------------------------------------------------------------------------*/
/**
*  see header for documentation
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_raw(uint32_t const ee_addr, uint32_t const len, uint8_t *const ptr_data)
{
    if(ptr_data)
    {
        // Check if the given eeprom_address is inside the the EEPROM boarders
        if( (EE_FACTORY_DATA_START <= ee_addr) && (ee_addr <= EE_USER_END) )
        {
            // Check if the size of the given read operation is inside the user data EEPROM space
            if( (ee_addr + len - 1) <= EE_USER_END )
            {
                return hal_nvm_eeprom_read_by_address(ee_addr, len, ptr_data);
            }
            else
            {
                return HAL_NVM_ERROR_DATA_LEN_INVALID;
            }
        }
        else
        {
            return HAL_NVM_ERROR_DATA_ADDR_INVALID;
        }
    }
    else
    {
        return HAL_NVM_ERROR_DATA_ADDR_INVALID;
    }
}


/*----------------------------------------------------------------------------*/
/**
*  see header for documentation
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_write_raw(uint32_t const ee_addr, uint32_t const len, uint8_t const *const ptr_data)
{
    // Check if the given eeprom_address is inside the user space of the EEPROM
    if( (EE_USER_START <= ee_addr) && (ee_addr <= EE_USER_END) )
    {
        // Check if the size of the given write operation fits into the user data EEPROM space
        if( (ee_addr + len - 1) <= EE_USER_END )
        {
            return hal_nvm_eeprom_write_by_address(ee_addr, len, ptr_data);
        }
        else
        {
            return HAL_NVM_ERROR_DATA_LEN_INVALID;
        }
    }
    else
    {
        return HAL_NVM_ERROR_DATA_ADDR_INVALID;
    }
}


/*----------------------------------------------------------------------------*/
/**
*  see header for documentation
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read(uint32_t const ee_addr, uint32_t const len, uint8_t *const ptr_data)
{
    uint32_t eeprom_address = EE_USER_START + ee_addr;

    // Check if the given eeprom_address is inside the user space of the EEPROM
    if( (EE_USER_START <= eeprom_address) && (eeprom_address <= EE_USER_END) )
    {
        return user_eeprom_read_raw(eeprom_address, len, ptr_data);
    }
    else
    {
        return HAL_NVM_ERROR_DATA_ADDR_INVALID;
    }
}


/*----------------------------------------------------------------------------*/
/**
*  see header for documentation
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_write(uint32_t const ee_addr, uint32_t const len, uint8_t const *const ptr_data)
{
    uint32_t eeprom_address = EE_USER_START + ee_addr;

    // Check if the given eeprom_address is inside the user space of the EEPROM
    if( (EE_USER_START <= eeprom_address) && (eeprom_address <= EE_USER_END) )
    {
        return user_eeprom_write_raw(eeprom_address, len, ptr_data);
    }
    else
    {
        return HAL_NVM_ERROR_DATA_ADDR_INVALID;
    }
}


/*----------------------------------------------------------------------------*/
/**
*  see header for documentation
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_32bit(uint32_t const ee_addr, uint32_t const len, uint32_t *const ptr_data)
{
    return user_eeprom_read(ee_addr, len, (uint8_t*) ptr_data);
}


/*----------------------------------------------------------------------------*/
/**
*  see header for documentation
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_write_32bit(uint32_t const ee_addr, uint32_t const len, uint32_t const *const ptr_data)
{
    return user_eeprom_write(ee_addr, len, (uint8_t*) ptr_data);
}


/*----------------------------------------------------------------------------*/
/**
*  see header for documentation
*/
uint32_t user_eeprom_read_value_32bit(uint32_t const ee_addr)
{
    uint8_t arr[4];
    uint32_t return_val = 0x00;

    user_eeprom_read(ee_addr, 4, arr);

    memcpy(&return_val, &arr, sizeof(arr));

    // todo: how could we do this with more less code?
    return return_val;
}


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_write_value_32bit(uint32_t const ee_addr, uint32_t const value)
{
    return user_eeprom_write(ee_addr, 4, (uint8_t*) &value);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t user_eeprom_read_module_eeprom_version(void)
{
    // get pointer to EEPROM_VERSION in EEPROM
    return *user_eeprom_ptr_to_uint16_field_member(EEPROM_VERSION);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t user_eeprom_read_module_id(void)
{
    // get pointer to id in EEPROM and correct endianness
    return SWAP16(*user_eeprom_ptr_to_uint16_field_member(ID));
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_eeprom_read_module_serial_nr(void)
{
    uint32_t retval = 0;

    // get pointer to serial number in EEPROM, correct endianness and mask out Geraetetypnummer / device type number
    retval = SWAP32(*(uint32_t *)EE_READ_PTR(serial_number)) & 0x00FFFFFF;
    return retval;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint32_t user_eeprom_read_module_device_type(void)
{

    uint32_t retval = 0;

    // get pointer to serial number in EEPROM, correct endianness and mask out serial number
    retval = SWAP32(*(uint32_t *)EE_READ_PTR(serial_number)) & 0xFF000000;
    return retval;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_part_nr(uint8_t buffer[], uint8_t const size_buffer, bool const filter)
{
    return user_eeprom_read_module_ascii_field(PART_NR, buffer, size_buffer, filter);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_drawing_nr(uint8_t buffer[], uint8_t const size_buffer, bool const filter)
{
    return user_eeprom_read_module_ascii_field(DRAWING_NR, buffer, size_buffer, filter);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_name(uint8_t buffer[], uint8_t const size_buffer, bool const filter)
{
    return user_eeprom_read_module_ascii_field(HW_TYPE, buffer, size_buffer, filter);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_order_nr(uint8_t buffer[], uint8_t const size_buffer, bool const filter)
{
    return user_eeprom_read_module_ascii_field(ORDER_NR, buffer, size_buffer, filter);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_test_date(uint8_t buffer[], uint8_t const size_buffer, bool const filter)
{
    return user_eeprom_read_module_ascii_field(TEST_DATE, buffer, size_buffer, filter);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t user_eeprom_read_module_mcu_type(void)
{
    // get pointer to MCU in EEPROM
    return *user_eeprom_ptr_to_uint16_field_member(MCU);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_hw_version(uint8_t buffer[], uint8_t const size_buffer, bool const filter)
{
    return user_eeprom_read_module_ascii_field(HW_VERSION, buffer, size_buffer, filter);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t user_eeprom_read_module_hw_can_active(void)
{
    // get pointer to HW_ACTIVE in EEPROM
    return *user_eeprom_ptr_to_uint16_field_member(HW_ACTIVE);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t user_eeprom_read_module_bootloader_version(void)
{
    //	 get pointer to BL_VERSION in EEPROM and correct endianness
    return SWAP16(*user_eeprom_ptr_to_uint16_field_member(BL_VERSION));
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t user_eeprom_read_module_reset_counter(void)
{
    // get pointer to RESET_COUNTER in EEPROM
    return *user_eeprom_ptr_to_uint16_field_member(RESET_COUNTER);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint8_t user_eeprom_read_module_reset_reason(void)
{
    uint8_t retval = 0, lvd = 0, loc = 0, ilad = 0, ilop = 0, cop = 0;

    //get the value from the reset_reason
    lvd = *(uint8_t *)EE_READ_PTR(reset_reason_lvd);
    loc = *(uint8_t *)EE_READ_PTR(reset_reason_loc);
    ilad = *(uint8_t *)EE_READ_PTR(reset_reason_ilad);
    ilop = *(uint8_t *)EE_READ_PTR(reset_reason_ilop);
    cop = *(uint8_t *)EE_READ_PTR(reset_reason_cop);

    // shift the bits in proper order
    retval = retval | (cop << 4) | (ilop << 3) | (ilad << 2) | (loc << 1) | lvd;
    return retval;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t user_eeprom_read_module_prog_status(void)
{
    //	 get pointer to id in EEPROM and correct endianness
    return SWAP16(*user_eeprom_ptr_to_uint16_field_member(PROG_STATUS)) & 0x000F;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_app_name(uint8_t *const buffer, uint8_t const size_buffer, bool const filter)
{
    return user_eeprom_read_module_ascii_field(APP_NAME, buffer, size_buffer, filter);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_app_version(uint8_t *const buffer, uint8_t const size_buffer, bool const filter)
{
    return user_eeprom_read_module_ascii_field(APP_VERSION, buffer, size_buffer, filter);
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_write_app_info(uint8_t const *const app_name, uint8_t const *const app_version)
{
    EE_WRITE(module_name, app_name);
    EE_WRITE(sw_version, app_version);

    return HAL_NVM_OK;
}


/*----------------------------------------------------------------------------*/
/**
* \internal
* ideally this would be handled with bootloader protocol, but it doesn't currently provide an API
* \endinternal
*
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_reset_reset_counter(void)
{
    EE_WRITE_MAX32(reset_counter, 0u);
//	bl_protocol_bl_clear_reset_counter();    // static function in role_protocol (currently unused and probably optimized out)
    return HAL_NVM_OK;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t user_eeprom_read_module_cop_wd_timeout(void)
{
    //get pointer to id in EEPROM
    return !SWAP16(*user_eeprom_ptr_to_uint16_field_member(COP_WD_TIMEOUT));
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t user_eeprom_read_bl_can_bus(void)
{
    //get pointer to id in EEPROM
    return SWAP16(*user_eeprom_ptr_to_uint16_field_member(BL_CAN_BUS));
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint16_t* user_eeprom_ptr_to_uint16_field_member(enum_USER_UINT16_EEPROM_FIELD_NAME field_name)
{
    // return the address of the member in struct_EEPROM14 and cast it to an uint16*
    switch(field_name)
    {
        case EEPROM_VERSION:
            return (uint16_t *)EE_READ_PTR(eeprom_version);
            break;
        case ID:
            return (uint16_t *)EE_READ_PTR(id);
            break;
        case MCU:
            return (uint16_t *)EE_READ_PTR(mcu_type);
            break;
        case HW_ACTIVE:
            return (uint16_t *)EE_READ_PTR(hw_can_active);
            break;
        case BL_VERSION:
            return (uint16_t *)EE_READ_PTR(bl_version);
            break;
        case RESET_COUNTER:
            return (uint16_t *)EE_READ_PTR(reset_counter);
            break;
        case PROG_STATUS:
            return (uint16_t *)EE_READ_PTR(prog_state);
            break;
        case COP_WD_TIMEOUT:
            return (uint16_t *)EE_READ_PTR(wd_timeout);
            break;
        case BL_CAN_BUS:
            return (uint16_t *)EE_READ_PTR(bl_canbus);
            break;
        default:
            return 0;
            break;
    }
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint8_t* user_eeprom_ptr_to_char_field_member(enum_USER_ASCII_EEPROM_FIELD_NAME field_name)
{
    // return the address of the member in struct_EEPROM14 and cast it to an uint8*
    switch(field_name)
    {
        case PART_NR:
            return (uint8_t *)EE_READ_PTR(part_number);
            break;
        case DRAWING_NR:
            return (uint8_t *)EE_READ_PTR(drawing_number);
            break;
        case HW_TYPE:
            return (uint8_t *)EE_READ_PTR(name);
            break;
        case ORDER_NR:
            return (uint8_t *)EE_READ_PTR(order_number);
            break;
        case TEST_DATE:
            return (uint8_t *)EE_READ_PTR(test_date);
            break;
        case HW_VERSION:
            return (uint8_t *)EE_READ_PTR(hw_version);
            break;
        case APP_NAME:
            return (uint8_t *)EE_READ_PTR(module_name);
            break;
        case APP_VERSION:
            return (uint8_t *)EE_READ_PTR(sw_version);
            break;
        default:
            return NULL;
            break;
    }
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
uint8_t user_eeprom_length_of_field_member(enum_USER_ASCII_EEPROM_FIELD_NAME field_name)
{
    // return the length of the specific array member in struct_EEPROM14
    switch(field_name)
    {
        case PART_NR:
            return EE_MEMB_SIZE(part_number);
            break;
        case DRAWING_NR:
            return EE_MEMB_SIZE(drawing_number);
            break;
        case HW_TYPE:
            return EE_MEMB_SIZE(name);
            break;
        case ORDER_NR:
            return EE_MEMB_SIZE(order_number);
            break;
        case TEST_DATE:
            return EE_MEMB_SIZE(test_date);
            break;
        case HW_VERSION:
            return EE_MEMB_SIZE(hw_version);
            break;
        case APP_NAME:
            return EE_MEMB_SIZE(module_name);
            break;
        case APP_VERSION:
            return EE_MEMB_SIZE(sw_version);
            break;
        default:
            return 0;
            break;
    }
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
enum_HAL_NVM_RETURN_VALUE user_eeprom_read_module_ascii_field(enum_USER_ASCII_EEPROM_FIELD_NAME field_name, uint8_t *buffer, uint8_t size_buffer, bool filter)
{

    uint8_t length = user_eeprom_length_of_field_member(field_name);
    uint8_t char_buffer[length+1];

    // make sure the buffer is clean, in order to delete obsolete data
    memset(buffer, '\0', size_buffer);
    if(size_buffer >= length+1)
    {
        uint8_t *read_ptr;

        read_ptr=user_eeprom_ptr_to_char_field_member(field_name);

        if(read_ptr != NULL)
        {
            memcpy(char_buffer, read_ptr, length);
            if (filter)
            {
                uint8_t sp=0;
                // go through the buffer an check if there are consecutive whitespace
                for(uint8_t i=0; i<length; ++i){
                    if(char_buffer[i]==' ')
                    {
                        ++sp;
                    }else{
                        sp=0;
                    }

                    if(sp == 2)
                    {
                        // if found, go back and set the remaining characters to \0
                        --i;
                        memset(&char_buffer[i], '\0', length-i+1);
                        break;
                    }
                    else
                    {

                    }
                }
            }

            char_buffer[length] = '\0';
            memcpy(buffer, char_buffer, length+1);
            return HAL_NVM_OK;
        }else
        {
            return HAL_NVM_ERROR_DATA_LEN_INVALID;
        }
    }
    else
    {

    }
    return HAL_NVM_ERROR_WHILE_READING;
}
