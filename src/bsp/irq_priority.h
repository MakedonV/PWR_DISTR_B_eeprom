/*----------------------------------------------------------------------------*/
/**
* \file         irq_priority.h
* \brief        Contains IRQ priority management function prototypes
*
* \date         20191001
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2019
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	irq_priority
* \brief 	    Contains IRQ priority management function prototypes
*
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_IRQ_PRIORITY_H_
#define SRC_IRQ_PRIORITY_H_

/*----------------------------------------------------------------------------*/
/**
* \ingroup      irq_priority
* \brief        Returns predefined priority of a given irq
*
* \details
*
* \pre
*
* \param irq_n [in]      uint8_t        irq number
* \return uint8_t                       Priority of a given IRQ
*/
uint8_t irq_priority_get(uint8_t irq_n);

/*----------------------------------------------------------------------------*/
/**
* \ingroup      irq_priority
* \brief        Sets predefined priorities to particular IRQs
*
* \details
*
* \pre
*
* \return None
*/
void irq_priority_set_all_predefined();

#endif // SRC_IRQ_PRIORITY_H_
