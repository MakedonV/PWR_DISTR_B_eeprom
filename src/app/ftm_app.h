/*
 * ftm_app.h
 *
 *  Created on: 4 Jan 2021
 *      Author: fzeller
 */

#ifndef APP_FTM_APP_H_
#define APP_FTM_APP_H_

#include "flexTimer1.h"


/* Structure which contains all needed variables to handle the software dither features */
typedef struct {
  uint16_t 	dither_promille;		// Duty cycle of the dither in promille
  uint16_t 	dither_counts;			// Counter to monitor how often the dither value was active
  uint16_t	dither_counter;			// Number at which the dither needs to be inverted
  uint16_t 	pwm_promille_backup;	// Backup of the PWM duty cycle
  uint16_t 	pwm_frequency_old;		// Backup of the PWM frequency
  int8_t  	dither_stat;			// Contains the info if dither is positive or negative
  uint16_t 	duty_cycle;				// Contains the currently valid duty cycle
  uint16_t 	pwm_period;				// Contains the period time in ms
}struct_pwm_vals_def;

/* Global array which contains all values needed to handle the dither feature */
extern struct_pwm_vals_def pwm_vals_t[PWM_MAX + 1];


/*----------------------------------------------------------------------------*/
/** TODO document function
* \brief
* \details
*
* \pre
*
* \param    pin      [in] enum_pin_name
* \return   uint32_t
*/
uint32_t change_pin_to_ftm(enum_pin_name pin);


/*----------------------------------------------------------------------------*/
/** TODO document function
* \brief
* \details
*
* \pre
*
* \param    pin      [in] enum_pin_name
* \return   uint32_t
*/
uint32_t change_pin_to_adc(enum_pin_name pin);


/*----------------------------------------------------------------------------*/
/**
* \brief    Function which handles the dither adaptions at runtime
* \details  This function will do the calculations to split the dither frequency as
* 			well as the dither duty into a symmetrical dither around the the duty
* 			cycle.
*
* 			It is called by each FTM module when a timer overflow interrupt
* 			is triggered.
*
* \pre
*
* \param    module [in] uint8_t	The FTM module which called the handle
* \return   void
*/
void FTM_IRQHandler(uint8_t module);


/*----------------------------------------------------------------------------*/
/**
* \brief	Interrupt handler for FTM0 overflow interrupt
* \details
*
* \pre
*
* \return   void
*/
void FTM0_IRQHandler(void);


/*----------------------------------------------------------------------------*/
/**
* \brief	Interrupt handler for FTM1 overflow interrupt
* \details
*
* \pre
*
* \return   void
*/
void FTM1_IRQHandler(void);


/*----------------------------------------------------------------------------*/
/**
* \brief	Interrupt handler for FTM2 overflow interrupt
* \details
*
* \pre
*
* \return   void
*/
void FTM2_IRQHandler(void);


/*----------------------------------------------------------------------------*/
/**
* \brief	Interrupt handler for FTM3 overflow interrupt
* \details
*
* \pre
*
* \return   void
*/
void FTM3_IRQHandler(void);


/*----------------------------------------------------------------------------*/
/**
* \brief	Interrupt handler for FTM4 overflow interrupt
* \details
*
* \pre
*
* \return   void
*/
void FTM4_IRQHandler(void);


/*----------------------------------------------------------------------------*/
/**
* \brief	Interrupt handler for FTM5 overflow interrupt
* \details
*
* \pre
*
* \return   void
*/
void FTM5_IRQHandler(void);


/*----------------------------------------------------------------------------*/
/**
* \brief	Interrupt handler for FTM6 overflow interrupt
* \details
*
* \pre
*
* \return   void
*/
void FTM6_IRQHandler(void);


/*----------------------------------------------------------------------------*/
/**
* \brief	Interrupt handler for FTM7 overflow interrupt
* \details
*
* \pre
*
* \return   void
*/
void FTM7_IRQHandler(void);



#endif /* APP_FTM_APP_H_ */
