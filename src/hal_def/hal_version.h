#ifndef HAL_VERSION_H_
#define HAL_VERSION_H_
/*----------------------------------------------------------------------------*/
/** 
* \file         hal_version.h
*/
/*----------------------------------------------------------------------------*/
/** 
* @{
* \brief        This module describes the handing for HAL (Hardware Abstraction Layer) version information.
* \details      Idea is to have version information for the HAL definitions and the HAL HW-platform specific implementation.
*               A user of the HAL-library might update the implementation but not the header definitions - vice versa.
*               This could lead to strange or unexpected behavior. This header file is used to help keep both the definition
*               and the implementation on the same or a compatible version by employing version check macros. \n
* 
*               The version scheme used is in accordance with the MRS Design rules (git commit f030560): \n
*               -Major numbers are incremented on [eg: large HAL changes, concept changes] \n
*               -Minor numbers are incremented on [eg: each HAL-Def change, documentation updates and function changes eg. can mailbox to fifo] \n
*               -Revision numbers are incremented on [eg: HAL-Impl changes without functionality changes, bugfixes] \n
*               
*               To view the feature changes, visit the CHANGELOG.md file, for detailed information see commit messages in git.
*
* \internal     This group can be used to check if the HAL headers and HAL Implementation are on same version.\n
*               To avoid a type dependent implementation (would require hal_datatypes.h), just the raw define is used.\n
*               Please do not use the 'u' suffix for version values, or they will appear in version string, too.\n
*               The macros work for large version numbers and letters( eg. 1234.56789.ABCDEF ).\n
*               This implementation might trigger errors or warnings in some static analysis tools like axivion.
*                If so, it should be safe to turn them off.
*
*/
/*----------------------------------------------------------------------------*/

#define HAL_VERSION_MAJOR       1                       ///< Version identification for HAL definitions: Major number.
#define HAL_VERSION_MINOR       4                       ///< Version identification for HAL definitions: Minor number.
#define HAL_VERSION_REVISION    0                       ///< Version identification for HAL definitions: Revision number.

/** Dynamically build version string from defines ( #HAL_VERSION_MAJOR, #HAL_VERSION_MINOR, #HAL_VERSION_REVISION )*/
#define HAL_VERSION_NAME        BUILD_VERSION_STRING(HAL_VERSION_MAJOR,HAL_VERSION_MINOR,HAL_VERSION_REVISION) 

/* It would be nice to handle this macro in a separate File, to make it available for SFL, CFL and user, too.
*   \param x The literal that needs to be quoted.
*/
#define QUOTE(x) #x  
/** Concat three free definable parts to a version string. Between them a '.' is placed.
*   \param x First part of the newly generated version string.
*   \param y Second part of the newly generated version string.
*   \param z Third part of the newly generated version string.
*
*   Example: 
*   \code 
    BUILD_VERSION_STRING(1,150,32456A)
*   Generates the Output: \n
*   "1.150.32456A"
*/
#define     BUILD_VERSION_STRING(x,y,z) QUOTE(x) "." QUOTE(y) "." QUOTE(z)   
/** @}*/
#endif
