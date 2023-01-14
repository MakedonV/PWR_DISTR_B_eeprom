/*----------------------------------------------------------------------------*/
/**
 * \file         LIN_APP.c
 * \brief        Implementation of LIN Application
 * \details
 * \date         20190515
 * \author       DHakobyan
 *
 */
/*----------------------------------------------------------------------------*/

#include "lin_app.h"
#include "hal_lin.h"
#include "lin_stack.h"
#include "lin_db_tables.h"

#include "hal_io.h"
#include "io_tables.h"



void lin_init_all()
{
#ifndef LIN_STACK_EMPTY
    static bool once = true;

    if (once)
    {
    	once = false;
        lin_init();
    }
#endif
}


void lin_cyclic()
{
#ifndef LIN_STACK_EMPTY
    lin_queue_in_process();
#endif
}
