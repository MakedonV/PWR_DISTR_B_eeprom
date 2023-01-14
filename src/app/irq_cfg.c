/*----------------------------------------------------------------------------*/
/**
* \file         irq_cfg.c
* \brief
*
* \platform     CPU: S32K144 | Compiler: GCC
* \copyright    MRS Electronic GmbH & Co. KG 2019
*/
/*----------------------------------------------------------------------------*/

#include <stdint.h>
#include "irq_cfg.h"
#include "device_registers.h"

/**
 * All non-listed interrupts have by default a priority of 0 (highest priority),
 * meaning they can preempt any of the following interrupts with priority above 0.
 */
const struct_IRQ_PRIORITY mgl_irq_prio[] =
{
		{.irqn_min = DMA0_IRQn,				.irqn_max = DMA15_IRQn,					.irqn_prio = 4},	// ADC-related
		{.irqn_min = ADC0_IRQn,				.irqn_max = ADC1_IRQn,					.irqn_prio = 4},	// ADC-related
		{.irqn_min = PDB0_IRQn,				.irqn_max = PDB0_IRQn,					.irqn_prio = 4},	// ADC-related
		{.irqn_min = PDB1_IRQn,				.irqn_max = PDB1_IRQn,					.irqn_prio = 4},	// ADC-related
		{.irqn_min = LPTMR0_IRQn,			.irqn_max = LPTMR0_IRQn,				.irqn_prio = 3},	// LIN-related
		{.irqn_min = PORTA_IRQn,			.irqn_max = PORTA_IRQn,					.irqn_prio = 2},	// LIN-related
		{.irqn_min = LPUART2_RxTx_IRQn,		.irqn_max = LPUART2_RxTx_IRQn,			.irqn_prio = 8},	// LPUART-related
		{.irqn_min = LPUART0_RxTx_IRQn,		.irqn_max = LPUART1_RxTx_IRQn,			.irqn_prio = 2},	// LIN-related
		{.irqn_min = PORTB_IRQn,			.irqn_max = PORTE_IRQn,					.irqn_prio = 8},	// Port-related
		{.irqn_min = CAN0_ORed_IRQn,		.irqn_max = CAN0_ORed_16_31_MB_IRQn,	.irqn_prio = 7},	// CAN0-related
		{.irqn_min = CAN1_ORed_IRQn,		.irqn_max = CAN1_ORed_0_15_MB_IRQn,		.irqn_prio = 7},	// CAN1-related
		{.irqn_min = CAN2_ORed_IRQn,		.irqn_max = CAN2_ORed_0_15_MB_IRQn,		.irqn_prio = 7},	// CAN2-related
		{.irqn_min = LPI2C0_Slave_IRQn,		.irqn_max = LPI2C0_Slave_IRQn,			.irqn_prio = 5},	// I2C0-related
		{.irqn_min = LPI2C0_Master_IRQn,	.irqn_max = LPI2C0_Master_IRQn,			.irqn_prio = 5},	// I2C0-related
		{.irqn_min = LPSPI0_IRQn,			.irqn_max = LPSPI2_IRQn,				.irqn_prio = 6},	// LPSPI-related
		{.irqn_min = FTM0_Ch0_Ch1_IRQn,		.irqn_max = FTM0_Ovf_Reload_IRQn,		.irqn_prio = 1},	// PWM-related
		{.irqn_min = FTM1_Ch0_Ch1_IRQn,		.irqn_max = FTM1_Ovf_Reload_IRQn,		.irqn_prio = 1},	// PWM-related
		{.irqn_min = FTM2_Ch0_Ch1_IRQn,		.irqn_max = FTM2_Ovf_Reload_IRQn,		.irqn_prio = 1},	// PWM-related
		{.irqn_min = FTM3_Ch0_Ch1_IRQn,		.irqn_max = FTM3_Ovf_Reload_IRQn,		.irqn_prio = 1},	// PWM-related

		{.irqn_min = IRQ_NAN, .irqn_max = IRQ_NAN, .irqn_prio = IRQ_NAN}								// end of records
};
