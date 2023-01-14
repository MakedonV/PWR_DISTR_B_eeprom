/* ###################################################################
 **     Filename    : main.c
 **     Processor   : S32K14x
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.00
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */


// Including needed modules to compile this module/procedure
#include "Cpu.h"
#include "dmaController1.h"
#include "flexTimer1.h"
#include "core_cm4.h"

// Include HAL modules
#include "hal_lin.h"
#include "hal_sci.h"
#include "hal_can.h"
#include "hal_io.h"
#include "hal_pwm.h"
#include "hal_nvm.h"
#include "hal_tick.h"
#include "hal_watchdog.h"

// Include APP modules
#include "user_code.h"
#include "graph_code.h"
#include "adc_app.h"
#include "lin_app.h"
#include "sci_app.h"
#include "can_app.h"

// Include SFL modules
#include "sfl_can_db_tables_data.h"
#include "sfl_can_db.h"
#include "sfl_bl_protocol.h"

// Include STD libs
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "watchdog1.h"
#include "modulhardwarecode.h"
#include "irq_priority.h"


// Private helper function
void supporting_driver_init(void);

int main(void)
{
	uint8_t multiplex_group;
	// Clock initialization
	CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

	// Initialize the pin configuration
	hal_io_init(NUM_OF_CONFIGURED_PINS);

	// Initialize bootloader protocol
	// Important: the following BL protocol initialization must precede the
	// modulhardwarecode_init function call. Otherwise an ASSERT is going to be issued
	(void)sfl_bl_protocol_s32k_init();

	// Initialize supporting driver modules (FTM, LPTMR, DMA, etc.)
	supporting_driver_init();

	// Initialize the SYSTICK module with 1ms clock
	hal_tick_init();
	uint32_t current_sysTick_1ms = 0; // Declare variable for SYSTICK handling
	uint32_t cycle_timestamp = 0;       // Timestamp variable for CycleTime

	// Initialize the NVM (EEPROM) module
	hal_nvm_init();

	// Initialize the ADC module
	ADC_init();

	// Initialize the PWM module
	hal_pwm_init(PWM_MAX);

	// Initialize the frequency capturing module
	hal_freq_init(FREQ_MAX);

	// Set predefined interrupt priorities
	irq_priority_set_all_predefined();

	lin_init_all();

	// Initialize modulehardwarecode
	modulhardwarecode_init();

#ifdef SET_CALLBACK_1MS_TIMER
	// Initialize 1ms_timer callback
	set_callback_timer_1ms(user_int_timer_1ms);
#endif

#ifdef SET_CALLBACK_CAN_MESSAGE_RECEIVE
	// Initialize can_msg_receive callback
	set_callback_can_msg_receive(user_can_receive_msg_callback);
#endif

	// execute adc_processing to supply the adc values to the customer (if needed inside the usercode_init() function)
	// multiplexed ADC inputs won't be available inside usercode_init
	adc_processing(&multiplex_group, HW_CALIBRATION_SUPPORT);

	// Initialize usercode
	usercode_init();

	// Initialize graphcode
	graphcode_init();

	// Start infinite loop
	while(1)
	{
		// Signal event bit and request a kick
		hal_watchdog_signal(WDT_SIGNAL_MAIN_LOOP_BIT | WATCHDOG_KICK_REQUEST);

		// Get the current value of the 1ms timer
		hal_get_timestamp(&current_sysTick_1ms, HAL_PRECISION_1MS);

		// Maintain BL protocol state
		(void)sfl_bl_protocol_s32k_cyclic();

		// update adc channels (e.g. those with calibration) in background
		adc_processing(&multiplex_group, HW_CALIBRATION_SUPPORT);


		/***********************************************************************************
		 * Input
		 ************************************************************************************/
		sfl_can_queue_in_process();

		lin_cyclic();

		// Execute modulehardwarecode cyclic
		modulhardwarecode_cyclic();

		/***********************************************************************************
		 * Processing
		 ************************************************************************************/
		if ( (current_sysTick_1ms - cycle_timestamp) >= ext_graph_cycle_time)
		{
			// Take over current timestamp
			cycle_timestamp = current_sysTick_1ms;

			// Execute usercode cyclic
			usercode();

			// Execute graphcode cyclic
			graphcode();
		}


		/***********************************************************************************
		 * Output
		 ************************************************************************************/
		// Cyclic CAN output
		sfl_can_db_output_to_bus();
	}
}

// must be called prior to hal_pwm_init() and hal_freq_init()
void supporting_driver_init(void)
{
	// Initialize the DMA (for SCI)
	EDMA_DRV_Init(&dmaController1_State, &dmaController1_InitConfig0, edmaChnStateArray, edmaChnConfigArray, EDMA_CONFIGURED_CHANNELS_COUNT);
}

