/*----------------------------------------------------------------------------*/
/**
* \file         irq_cfg.h
* \brief        Contains IRQ priority data types
*
* \date         20191001
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2019
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	irq_cfg
* \brief 	    Contains IRQ priority data types
*
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_BSP_CFG_IRQ_CFG_H_
#define SRC_BSP_CFG_IRQ_CFG_H_

#define IRQ_NAN		((uint8_t)(-1))

typedef struct
{
	uint8_t irqn_min;
	uint8_t irqn_max;
	uint8_t irqn_prio;
} struct_IRQ_PRIORITY;

#endif // SRC_BSP_CFG_IRQ_CFG_H_
