/*----------------------------------------------------------------------------*/
/**
* \file         role_base.h
* \brief        Contains the base role structure definition
*
* \date         20200330
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2020
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	role_base
* \brief        Contains struct_ROLE definition.
*               Any implemented role should be derived from this structure.
*               This header file has not special inclusion order except that it
*               should come after the inclusion of role_types.h.
*               This header file should not be include in files which do not
*               define a role.
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_ROLE_BASE_H_
#define SRC_ROLE_BASE_H_

#if defined(__cplusplus)
extern "C" {
#endif

// ===================================================================================================
// The struct_ROLE structure represents the abstract role base structure.
// Any file which implements a role should include this file and
// must define a structure whose first member must be an instance of struct_ROLE structure.
// ===================================================================================================
typedef struct struct_ROLE_VTBL struct_ROLE_VTBL;

struct struct_ROLE_VTBL
{
    enum_CODE (*dothis)(role_id_t, role_do_id_t, va_list *ptr_arg_list);
    void (*play)(role_id_t);
};

struct struct_ROLE
{
    struct_ROLE_VTBL    *vptr;
    const role_id_t      id;
    enum_ROLE_STATE      state;
};


#ifdef __cplusplus
}
#endif

#endif /* SRC_ROLE_BASE_H_ */
