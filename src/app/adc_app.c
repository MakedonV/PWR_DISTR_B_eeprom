/*----------------------------------------------------------------------------*/
/**
 * \file         adc_app.c
 * \brief        Sourcecode for ADC Application
 * \details
 * \date         20180108
 * \author       MMueller
 *
 */
/*----------------------------------------------------------------------------*/
#include "adc_app.h"

#include "adConv1.h"
#include "sfl_math.h"
#include "io_tables.h"
#include "hal_io.h"
#include "adc_app.h"
#include "user_api_io.h"
#include "modulhardwarecode.h"
#include "ftm_app.h"

#include "defines_general.h"
// The following header should be included first
#include "role_types.h"
#include "ee_helper.h"
#include "role_protocol_bl.h"
// The following header should be included last
#include "role.h"

// ===================================================================================================
// Macros
// ===================================================================================================
#define NEW_PDB_CODE 1  // used to switch to new pdb_init() that iterates through a table rather than having individual lines of code for each pdb


#define CALIBR_MAX_ADC_VAL          (4095u)
#define CALIBR_MAX_VOLT_VAL         (5000u)

#define ADC_INA_OUT_N               (8u)
#define ADC_SENSE_N                 (4u)
#define SENSE_MULTIPLEXED_N         (2u)
#define ADC_CALIBR_INP_N            (ADC_INA_OUT_N + SENSE_MULTIPLEXED_N * ADC_SENSE_N)

#define CALIB_POINTS_SOURCE     3 // for the calibration values, there are per axis three support points (3 for X value 3 for Y values), which are used in the lookup table and are saved in the module eeprom for positive values
#define CALIB_POINTS_SINK       3 // for the calibration values, there are per axis three support points (3 for X value 3 for Y values), which are used in the lookup table and are saved in the module eeprom for negative values
#define CALIB_PAIR              2 // 2 values for each calibration point
#define CALIB_DATA_TYPE    int16_t

#define CALIB_VAL_SIZE     ( (CALIB_POINTS_SOURCE + CALIB_POINTS_SINK) * CALIB_PAIR * sizeof(CALIB_DATA_TYPE) )

FILENUM(89)   ///< This is to ease the tracking of assert failures. Each file should have its own unique file number.
// ===================================================================================================
// Global data definitions
// ===================================================================================================
floating_avg_values_data_t mgl_mean_current_value[ADC_INA_OUT_N] = {{0,0}};

struct_adc_results_t adc_results[ADC_MAX] = {0};
uint16_t adc_interrupt_values[ADC_MAX] = {0};

uint8_t mgl_multiplex_group_max = 0u;

uint32_t mgl_adc_counter[ADC_INSTANCE_COUNT] = {0}; //global ADC counter, which is incremented each time the ADC interrupt is processed.

// ===================================================================================================
// Internal function prototypes
// ===================================================================================================
void pdb_init(void);

uint32_t adc_get_counter(uint8_t adc_instance, IRQn_Type adc_interrupt);

// ===================================================================================================
// Functions
// ===================================================================================================
void pdb_init(void)
{
	// todo: this hasn't been tested on all hardware variants. can PDB channels be initialized even if they are unused?

    // init PDB 0
    PDB_DRV_Init(INST_PDB1, &pdb_InitConfig0);
    PDB_DRV_Enable(INST_PDB1);

    // init PDB 1
    PDB_DRV_Init(INST_PDB2, &pdb_InitConfig0);
    PDB_DRV_Enable(INST_PDB2);

    // init all channels and pretriggers
    for ( uint8_t channel = 0; channel < get_element_count_pdb_config_tbl(); channel++ )
    {
            PDB_DRV_ConfigAdcPreTrigger(pdb_config_tbl[channel].instance, pdb_config_tbl[channel].chn, &pdb_config_tbl[channel].adc_trigger_init_config);
    }


	// configure delay value (later used to calculate individiual delay values for PDB channels)
    uint32_t delayValue = 4000;

    // ============================================
    // PDB0
        PDB_DRV_SetTimerModulusValue(INST_PDB1, (uint32_t)delayValue);
        PDB_DRV_SetAdcPreTriggerDelayValue(INST_PDB1, 0UL, 0UL, ((uint32_t)delayValue/4)*0);
        PDB_DRV_SetAdcPreTriggerDelayValue(INST_PDB1, 1UL, 0UL, ((uint32_t)delayValue/4)*1);
	// ============================================
	// PDB1
        PDB_DRV_SetTimerModulusValue(INST_PDB2, (uint32_t)delayValue);
        PDB_DRV_SetAdcPreTriggerDelayValue(INST_PDB2, 0UL, 0UL, ((uint32_t)delayValue/4)*0);
        PDB_DRV_SetAdcPreTriggerDelayValue(INST_PDB2, 1UL, 0UL, ((uint32_t)delayValue/4)*1);

#if (defined(S32K148))
        PDB_DRV_SetAdcPreTriggerDelayValue(INST_PDB1, 2UL, 0UL, ((uint32_t)delayValue/4)*2 );
        PDB_DRV_SetAdcPreTriggerDelayValue(INST_PDB1, 3UL, 0UL, ((uint32_t)delayValue/4)*3 );

        PDB_DRV_SetAdcPreTriggerDelayValue(INST_PDB2, 2UL, 0UL, ((uint32_t)delayValue/4)*2);
        PDB_DRV_SetAdcPreTriggerDelayValue(INST_PDB2, 3UL, 0UL, ((uint32_t)delayValue/4)*3 );

#endif
		PDB_DRV_LoadValuesCmd(INST_PDB1);
		PDB_DRV_SoftTriggerCmd(INST_PDB1);

        PDB_DRV_LoadValuesCmd(INST_PDB2);
        PDB_DRV_SoftTriggerCmd(INST_PDB2);

}


void ADC_init(void)
{
    // ADC Module 0
    ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv0_ConvConfig0);
    // ADC Module 1
    ADC_DRV_ConfigConverter(INST_ADCONV2, &adConv1_ConvConfig0);

    // ---------------------------------------------------------------------------------------------------
    // get CALIBRATION identifier from EEPROM (2 Byte)
    // ---------------------------------------------------------------------------------------------------
	// special case: 400803, ANR 1076784  ID2441-2557 were the first (non-50mOhm) CC16WP with AutoCalibration on in production, but still had the old cal_data_identifier
    // the cal_data_identifier was manually updated to 28084 using MRS DS for these modules

	uint16_t *cal_data_identifier = (uint16_t*)ee_read(CALIBR_EE_TABLE_START_ADDR);
	bool run_autocalibration = true;

	switch(*cal_data_identifier)
	{
	case 0xFFFF:
		// no calibration data in EEPROM
		// we could merge this with default, however we also want to keep this status for other purposes, e.g. like not running certain code under this condition
		// mgl_cal_data_present = false; // could be used instead of HW_CALIBRATION_SUPPORT
		run_autocalibration = true;
		break;
	case 28083: // CC16WP v1 (8 INA, 8 HSD Sense) - NO AutoCalibration
		run_autocalibration = false;
		break;
	default:
		// 28084 // CC16WP v2 (8 INA, 8 HSD Sense) - with AutoCalibration
		// 28086 // CC16WP v3 draft (8 INA, 8 HSD Sense, reference and temperature voltage) - with AutoCalibration
		// 28085 // Powerdistributor - with AutoCalibration
		run_autocalibration = true;
		break;
	}

    // ---------------------------------------------------------------------------------------------------
    // run ADC self calibration (44.5.6 Calibration function in RM). Required for highest ADC accuracy.
    // ---------------------------------------------------------------------------------------------------
    // will take about 5 ADC cycles + 5 module clocks
    // NOTE: this could have negative effects on existing module calibration. see KSB-617 for more.
    // HW_CALIBRATION is activated for new images by default. Deactivating for older hardwares takes place through
    // the define inside modulehardwarecode.h which will be generated dependent on the info inside the IO_DEF.
    // ATTENTION: Hardwares which were produced without auto calibration activated should not use new applications with the
    // auto calibration enabled. Take care of NOTE from above and see KSB-617.
    if(run_autocalibration)
    {
		ADC_DRV_AutoCalibration(INST_ADCONV1);
		ADC_DRV_AutoCalibration(INST_ADCONV2);
    }

#if 0
    // ---------------------------------------------------------------------------------------------------
    // use custom userGain/userOffset for 5V vref correction from EEPROM. UNFINISHED and UNSUPPORTED.
    // ---------------------------------------------------------------------------------------------------
    adc_calibration_t user_calib_config;

    // Check if the EEPROM calibration data structure supports the 5V Vref, 28085 is the Cal Kenner which supports 5V Vref.
    if (*(uint16_t*) ee_read(CALIBR_EE_TABLE_MODULE_ADDR) == 28085)
    {
        user_calib_config.userGain = // read from EEPROM
        user_calib_config.userOffset = // read data from EEPROM
    }
    else // There is no data available inside the EEPROM
    {
        user_calib_config.userGain = 0;
        user_calib_config.userOffset = 0;
    }

    // Take over the user calibration data
    ADC_DRV_ConfigUserCalibration(INST_ADCONV1, &user_calib_config_adc);
    ADC_DRV_ConfigUserCalibration(INST_ADCONV2, &user_calib_config_adc);
#endif

    // Cycle through all ADC channels and initialize them
    for (uint8_t i = 0; i < ADC_MAX; i++)
    {
        if (adc_config_tbl[i].multiplex == 0)
        {
        ADC_DRV_ConfigChan(adc_config_tbl[i].adc_instance, adc_config_tbl[i].adc_channel, &adc_init_config_tbl[i].channel_cfg);
        }
        else
        {
            if (adc_config_tbl[i].multiplex_group > mgl_multiplex_group_max)
            {
                mgl_multiplex_group_max++;
            }
        }
    }

    // Install interrupt handler
    INT_SYS_InstallHandler(ADC0_IRQn, &ADC1_IRQHandler, (isr_t*) 0);
    INT_SYS_InstallHandler(ADC1_IRQn, &ADC2_IRQHandler, (isr_t*) 0);

#if NEW_PDB_CODE
    pdb_init();
#else
    pdb0_init();
    pdb1_init();
#endif

    /* Enable ADC interrupts */
    INT_SYS_EnableIRQ(ADC0_IRQn);
    INT_SYS_EnableIRQ(ADC1_IRQn);
}

void ADC_IRQHandler(uint8_t pdb_instance, uint8_t adc_instance)
{
    // Cycle through all ADC channels
    for (uint8_t channel = 0; channel < ADC_MAX; channel++)
    {
        // Only the pins of the ADC instance are relevant, which has triggered the interrupt
        if (adc_config_tbl[channel].adc_instance == adc_instance)
        {
                // Get channel result and put it into adc_interrupt_values variable, clear interrupt
                ADC_DRV_GetChanResult(adc_config_tbl[channel].adc_instance, adc_config_tbl[channel].adc_channel, &(adc_interrupt_values[channel]));
        }
    }

    PDB_DRV_SoftTriggerCmd(pdb_instance);
    //increment the global adc interrupt counter.
    mgl_adc_counter[adc_instance]++;
}

/* @brief: ADC Interrupt Service Routines */
void ADC1_IRQHandler(void)
{
    ADC_IRQHandler(INST_PDB1, INST_ADCONV1);
}

void ADC2_IRQHandler(void)
{
    ADC_IRQHandler(INST_PDB2, INST_ADCONV2);
}


/*----------------------------------------------------------------------------*/
/**
*  Get the global adc interrupt counter.
*
* \internal
* \endinternal
*
*/
uint32_t adc_get_counter(uint8_t adc_instance, IRQn_Type adc_interrupt)
{
    uint32_t counter;

    INT_SYS_DisableIRQ(adc_interrupt); //while getting the value of the counter, the adc interrupts are disabled.
    counter = mgl_adc_counter[adc_instance];
    INT_SYS_EnableIRQ(adc_interrupt);

    return counter;
}

// ===================================================================================================
// Public functions
// ===================================================================================================
uint32_t adc_get_measurement_range(enum_adc_pin_name const pin)
{
	uint32_t range = adc_config_tbl[pin].adc_vrefh;
#if ADC_SECOND_RANGE
	range = modulhardwarecode_adc_get_vrefh2(pin, range);
#endif

	return range;
}


/*----------------------------------------------------------------------------*/
/**
*  Returns the two-byte module identifier which is sitting at address 0
*  of calibration table
*
*/
uint16_t adc_get_module_type()
{
    // The first record (uint16_t) keeps the module identifier
    uint16_t *ptr = (uint16_t*)ee_read(CALIBR_EE_TABLE_MODULE_ADDR);
    uint16_t tmp = (NULL != ptr) ? (*ptr) : 0u;
    return tmp;
}

/*----------------------------------------------------------------------------*/
/**
* \internal
* This function is used to copy the ADC results from the ADC interrupt to a structure element which can be used for further processing
* without disabling the ADC interrupts again.
*
* \endinternal
*
* Date       | Type    | Person
* -----------|---------|-----------
* 20210218   | Author  | jallgeier
*/
void adc_copy_results_from_interrupt(uint8_t adc_instance, uint8_t* adc_done_flag, uint32_t* adc_counter, IRQn_Type adc_interrupt )
{
    uint32_t tmp_counter;

    tmp_counter = adc_get_counter(adc_instance, adc_interrupt);//get the global adc interrupt counter.


    //check if there are new ADC values available
    if (tmp_counter != *adc_counter)
    {
        //Set a flag to indicate that new adc values are available
        *adc_done_flag = 1;

        INT_SYS_DisableIRQ(adc_interrupt);
        //simply copy of the measured ADC register values.
        for ( uint8_t i = 0; i < ADC_MAX; ++i )
        {
            if(adc_config_tbl[i].adc_instance == adc_instance)
            {
                adc_results[i].result_raw = adc_interrupt_values[i];
            }

        }
        INT_SYS_EnableIRQ(adc_interrupt);
        *adc_counter = tmp_counter; // set the local adc_counter to the same value as the temporary adc counter
    }
}


/*----------------------------------------------------------------------------*/
/**
*  needs to be called periodically, ideally after each ADC cycle is completed to ensure calibrated values and raw adc samples are in sync
*
* \internal
* \endinternal
*
*/
void adc_processing(uint8_t *multiplex_group, bool hw_calibration_support)
{
    static uint32_t adc1_counter = 0, adc2_counter = 0;//local counter which contains the value of the global adc interrupt counter
    uint8_t adc_inst1_done = 0, adc_inst2_done = 0;//flag which indicates that new ADC values are available.
    static uint32_t multiplex_adc1_counter = 0, multiplex_adc2_counter = 0; //multiplex counter which are used for stability purpose
    bool right_multiplex_group = FALSE, adc1_stability_cnt_passed = FALSE, adc2_stability_cnt_passed = FALSE; //bool variables to make IF queries clearer
    uint8_t number_points = 0;

    adc_copy_results_from_interrupt(INST_ADCONV1, &adc_inst1_done, &adc1_counter, ADC0_IRQn);
    adc_copy_results_from_interrupt(INST_ADCONV2, &adc_inst2_done, &adc2_counter, ADC1_IRQn);

    //Depending on the size of the capacitors at the analog input, a small residual from
    //the previous selected input may be measured during multiplex switching. To give the
    //capacitor enough time to discharge, some ADC measurements to the corresponding channel
    //are "discarded" and only every MULTIPLEX_STABILITY_CYCLES measurement is stored.
    adc1_stability_cnt_passed = (((adc1_counter) - (multiplex_adc1_counter)) >= MULTIPLEX_STABILITY_CYCLES);
    adc2_stability_cnt_passed = (((adc2_counter) - (multiplex_adc2_counter)) >= MULTIPLEX_STABILITY_CYCLES);

    if (adc_inst1_done == 1 || adc_inst2_done == 1)
    {
        // ---------------------------------------------------------------------------------------------------
        // Copying of the raw ADC values into the DIGITS results vector with distinction which multiplex group is currently selected.
        // ---------------------------------------------------------------------------------------------------
        for ( uint8_t i = 0; i < ADC_MAX; ++i )
        {
            //check if the currently selected multiplex_group is the same as the multiplex group from the ADC input which should be determinate in the loop.
            right_multiplex_group = *multiplex_group == adc_config_tbl[i].multiplex_group;

            if ( adc_config_tbl[i].multiplex )
            {
                //WARNING if the multiplex should ever wanted to be decouple, this code wouldn't work anymore.
                if ( (right_multiplex_group) && (adc1_stability_cnt_passed && adc2_stability_cnt_passed) )
                {
                    adc_results[i].result_digit = adc_results[i].result_raw;
                }
            }
            else
            {
                adc_results[i].result_digit = adc_results[i].result_raw;
            }

            // ---------------------------------------------------------------------------------------------------
            // Die Zuweisung der Gewichteten Werte ist aktuell noch auskommentiert. Grund ist die Laufzeit von ca.
            // 85 us(+47% Gesamtlaufzeit) bei 38 ADC Channels bei der CC16. Aktuell befindet sich diese Zuweisung+Berechnung
            // in der get_mV Funktion. Ziel der Berechnung an dieser Stelle ist, zum einen eine bessere Uebersicht
            // zum Anderen, sollten im Falle vorhandener Kalibrierwerte, diese im Nachhinein in die result_unit
            // geschrieben werden.
            // ---------------------------------------------------------------------------------------------------
            //adc_results[i].result_unit = ((adc_results[i].result_digit * adc_get_measurement_range(i)) / adc_config_tbl[i].resolution);

            // ---------------------------------------------------------------------------------------------------
            // Calibration part. Not every module has calibrated values, so in this case some code parts must not be executed. (to reduce runtime overhead)
            // ---------------------------------------------------------------------------------------------------
            if(hw_calibration_support)
            {
                //check the calibration type of the input. If the type is NONE, nothing at all is executed - there is no else case.
                if ( adc_config_tbl[i].cal_type != CALIB_NONE )
                {
                    int16_t *ptr = (int16_t*) ee_read(CALIBR_EE_TABLE_DATA_ADDR + adc_config_tbl[i].cal_index);

                    //check if there are calibrated values saved on this eeprom area. If not return a '0'
                    if ( NULL == ptr || -1 == *ptr )
                    {
                        adc_results[i].result_cal = 0;
                        adc_results[i].result_filtered = (adc_results[i].result_filtered*(AD_FILTER-1) + adc_results[i].result_digit) / AD_FILTER;
                    }
                    else
                    {
                        //switch case, executes lookup function with parameters depending which calibrate type is selected.
                        switch ( adc_config_tbl[i].cal_type )
                        {
                            case CALIB_SOURCE_3P: //Only three source or sink points are available. So only the number of points is relevant.
                                number_points = 3;
                                adc_results[i].result_cal = os_util_lookup1D(ptr, ptr + number_points, number_points,adc_results[i].result_digit, LUT_MODE_EXTRAPOLATION_POS);
                                break;

                            case CALIB_SOURCE_SINK_3P: //There are three source points AND three sink points.
                                number_points = 3;
                                //todo: for now, this case looks like the "CALIB_SOURCE_3P" because we ignore the saved sink values and this will be reconstructed when adapting the measurement.
                                    adc_results[i].result_cal = os_util_lookup1D(ptr, ptr + number_points, number_points,adc_results[i].result_digit, LUT_MODE_EXTRAPOLATION_POS);
                                break;

                            default: //default state. We should never reach here, if we do return 0 as result to not leave users in the false hope of the values actually being calibrated.
                                adc_results[i].result_cal = 0;
                                break;

                        }
                        adc_results[i].result_filtered = (adc_results[i].result_filtered*(AD_FILTER-1) + adc_results[i].result_cal) / AD_FILTER;
                    }
                }
            }
            else
            {
                adc_results[i].result_filtered = (adc_results[i].result_filtered*(AD_FILTER-1) + adc_results[i].result_digit) / AD_FILTER;
            }
        }
#if FLOATING_AVG
        static floating_avg_data_t mean_filter_data[ADC_INA_OUT_N] = {0};
        for ( int i = 0; i < ADC_INA_OUT_N; ++i )
        {
            mean_filter_data[i].amount_of_values = mgl_mean_current_value[i].amount_values;
            add_to_float_avg(&mean_filter_data[i], adc_results[i+AI_INA_OUT0].result_cal);
            mgl_mean_current_value[i].current_value = adc_calc_float_avg(&mean_filter_data[i]);
        }
#endif
    }

    //check if there have been past enough ADC cycles, so that it is save to swap the multiplex channel.
    if ( (adc1_stability_cnt_passed) && (adc2_stability_cnt_passed) )
    {
        //"reset" of the multiplex counter
        multiplex_adc1_counter = adc1_counter;
        multiplex_adc2_counter = adc2_counter;
        //change the multiplex group
        *multiplex_group = (1 + *multiplex_group) % (mgl_multiplex_group_max + 1);
        //call of the adc multiplexing modulhardware code
        modulhardwarecode_adc_multiplex(*multiplex_group);
    }
}

/*----------------------------------------------------------------------------*/
/**
* \internal
*   adds a value to the floating average array
* \endinternal
*
*/
void add_to_float_avg(floating_avg_data_t *float_avg_data, uint16_t new_value)
{
	//safety check, to prevent divide-by-zero and other illegal operations. We just don't do anything then.
	if ( float_avg_data->amount_of_values > 0 )
	{
		if ( float_avg_data->amount_of_values > AD_FILTER )
		{
			float_avg_data->amount_of_values = AD_FILTER;
		}
		// write the new value to the intended position in the array.
		float_avg_data->a_data[float_avg_data->index_next_value] = new_value;
		// Der naechste Wert wird dann an die Position dahinter geschrieben.
		// set the floating average index counter to the next position
		float_avg_data->index_next_value++;
		// if the maximum amount of the average values is reached, the index counter is set to it's start position.
		float_avg_data->index_next_value %= float_avg_data->amount_of_values;

		float_avg_data->amount_of_values = 1;
	}
	else
	{

	}
}


/*----------------------------------------------------------------------------*/
/**
* \internal
* Function that returns the calculated output value of the floating average.
* \endinternal
*
*/
uint16_t adc_calc_float_avg(floating_avg_data_t *float_avg_data)
{
	uint32_t o_Result = 0u;

	//safety check, to prevent divide-by-zero and other illegal operations. We just don't do anything then.
	if ( float_avg_data->amount_of_values > 0 )
	{
		// limit amount of values to array size
		if ( float_avg_data->amount_of_values > AD_FILTER )
		{
			float_avg_data->amount_of_values = AD_FILTER;
		}
		uint32_t temp_sum = 0;
		// calculate the sum of all values of the array
		for ( uint16_t i = 0; i < (float_avg_data->amount_of_values); ++i )
		{
			temp_sum += (uint32_t) float_avg_data->a_data[i];
		}
		//divide the calculated sum through the selected amount of value
		o_Result = (temp_sum / float_avg_data->amount_of_values);
	}
	else
	{
		o_Result = 0u;
	}

	return (uint16_t) o_Result;
}


uint16_t get_mean_analog_value(enum_adc_pin_name analog_pin, uint8_t AmountValues)
{
    mgl_mean_current_value[analog_pin-(ADC_INA_OUT_N-1)].amount_values = AmountValues;
    return mgl_mean_current_value[analog_pin-(ADC_INA_OUT_N-1)].current_value;
}

uint16_t get_mean_analog_value_exact(enum_adc_pin_name analog_pin)
{
    return mgl_mean_current_value[analog_pin-(ADC_INA_OUT_N-1)].current_value;
}

