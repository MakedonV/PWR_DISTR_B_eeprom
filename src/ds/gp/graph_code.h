#ifndef _GRAPH_DEFINES_H_
#define _GRAPH_DEFINES_H_

/*--------------------------------------------------------------------------*/
/** \file     graph_defines.h
*   \brief    Generated graph_defines code by MRS Graphical Programming
*             Project name: HW_CC16WP.sch
*
*   \date 26.02.2020 16:09:26  \author SKienzler  dr/
*
*   \platform RH850F1L
* --------------------------------------------------------------------------
*   \defgroup Name      graph_func
* --------------------------------------------------------------------------*/

#define MEMBITARRAY_INDICES    1
#define MEM16ARRAY_INDICES     0
#define MEM32ARRAY_INDICES     0
#define CONST32ARRAY_INDICES   0

#define MEMBITARRAY_SIZE       1
#define MEM16ARRAY_SIZE        1
#define MEM32ARRAY_SIZE        1
#define CONST32ARRAY_SIZE      1

extern uint16_t ext_graph_cycle_time;

void graphcode_init(void);
void graphcode(void);
#endif