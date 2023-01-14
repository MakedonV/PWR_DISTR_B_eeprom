/*----------------------------------------------------------------------------*/
/**
* \file         mcu_dep.h
* \brief        Includes MCU specific and system header files
*
* \date         20200206
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2020
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_S32_DEVICE_MCU_DEP_H_
#define SRC_S32_DEVICE_MCU_DEP_H_

#include "device_registers.h"

#if defined(S32K14x_SERIES)
    #include "core_cm4.h"
#endif // S32K14x_SERIES

#if defined(S32K148_SERIES)
#include "system_S32K148.h"
#elif defined(S32K146_SERIES)
#include "system_S32K146.h"
#elif defined(S32K144_SERIES)
#include "system_S32K144.h"
#elif defined(S32K142_SERIES)
#include "system_S32K142.h"
#endif

#endif /* SRC_S32_DEVICE_MCU_DEP_H_ */
