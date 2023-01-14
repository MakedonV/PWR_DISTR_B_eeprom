#ifndef HAL_VERSION_S32K_H_
#define HAL_VERSION_S32K_H_
/*----------------------------------------------------------------------------*/
/** 
* \file         hal_version_s32k.h
* \brief        This file contains the version information for HAL implementation of platform 'S32K'.
*/
/*----------------------------------------------------------------------------*/
/** 
* \addtogroup   doc_hal_version  
*  @{   
* \details      Here is the implementation specific version information for hardware platform 'S32K'.
*               This file automatically checks if the given HAL header version matches the local implemented version.
*               A detailed description of the major, minor and revision version and when they are changed is described in the hal_version.h file.
*/
/*----------------------------------------------------------------------------*/
#include "hal_version.h"


#define HAL_VERSION_S32K_MAJOR        1       ///< Version identification for HAL implementation of platform 'S32K': Major number.
#define HAL_VERSION_S32K_MINOR        4       ///< Version identification for HAL implementation of platform 'S32K': Minor number.
#define HAL_VERSION_S32K_REVISION     0       ///< Version identification for HAL implementation of platform 'S32K': Revision number.

#if (HAL_VERSION_S32K_MAJOR != HAL_VERSION_MAJOR) || (HAL_VERSION_S32K_MINOR != HAL_VERSION_MINOR) || (HAL_VERSION_S32K_REVISION < HAL_VERSION_REVISION)
    #error "HAL definition version does not match HAL implementation version. Please update to compatible versions."
#endif

/** @}*/
#endif
