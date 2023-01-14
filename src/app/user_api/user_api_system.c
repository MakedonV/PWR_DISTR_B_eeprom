/*
 * user_api_system.c
 *
 *  Created on: 03.03.2020
 *      Author: VGoertz
 */

#include "user_api_system.h"
#include "device_registers.h"


/*----------------------------------------------------------------------------*/
/**
* \internal
*
* \endinternal
*
*/
void user_system_soft_reset()
{
    // this function triggers a system reset
    SystemSoftwareReset();
}
