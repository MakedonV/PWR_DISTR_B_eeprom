#ifndef flexTimer1_H
#define flexTimer1_H
/* MODULE flexTimer1. */

/* Include inherited beans */
#include "clockMan1.h"
#include "ftm_ic_driver.h"
#include "ftm_mc_driver.h"
#include "ftm_qd_driver.h"
#include "ftm_oc_driver.h"
#include "ftm_pwm_driver.h"
#include "Cpu.h"

#include "io_tables.h"

#if defined(S32K148)
#define FTM_IC_MODULES_USED     8U //used in ftm_ic_driver
#else
#define FTM_IC_MODULES_USED     4U //used in ftm_ic_driver
#endif

// ####################################################################
// ADJUST MANUALLY -> size of table struct_ftm_init_config_tbl
#define FTM_FULL_CFG_SIZE   32   // size of struct_ftm_init_config_tbl
// ####################################################################


/*! @brief Device instance number */
#define MODULE_NONE     (uint8_t)(-1U)
#define MODULE_0        0U
#define INST_FLEXTIMER0 MODULE_0

#define MODULE_1 1U
#define INST_FLEXTIMER1 MODULE_1

#define MODULE_2 2U
#define INST_FLEXTIMER2 MODULE_2

#define MODULE_3 3U
#define INST_FLEXTIMER3 MODULE_3

#define MODULE_4 4U
#define INST_FLEXTIMER4 MODULE_4

#define MODULE_5 5U
#define INST_FLEXTIMER5 MODULE_5

#define MODULE_6 6U
#define INST_FLEXTIMER6 MODULE_6

#define MODULE_7 7U
#define INST_FLEXTIMER7 MODULE_7

typedef struct
{
    enum_pwm_pin_name pin_name;             // Name of the PWM Pin
    uint8_t pwm_instance;                   // PWM instance of the resembling pin
    uint8_t pwm_channel;                    // PWM channel of the resembling pin
}struct_FTM_CONFIG;

extern struct_FTM_CONFIG struct_ftm_config_tbl[];

typedef struct
{
    ftm_pwm_param_t *pwm_config_t;          // PWM user config structure
    ftm_input_param_t *ic_config_t;         // PWM user config structure
    enum_pin_name io_pin_name;              // The resembling pin name of the io_table (g_pin_mux_InitConfigArr)
    ftm_user_config_t *timer_mode_cfg;      // Timer mode configuration
    ftm_state_t * ftm_state;                // Timer module state
    uint32_t pwm_instance;                  // ftm module number
    uint32_t pwm_channel;                   // pwm channel number
    uint32_t pwm_pin_name;                  // pwm pin name
}struct_init_ftm_config_t;

extern struct_init_ftm_config_t struct_ftm_init_config_tbl[ FTM_FULL_CFG_SIZE ];

typedef struct
{
    enum_pin_name pin_name;
    uint32_t adc_alt_cfg;
    uint32_t ftm_alt_cfg;
    uint32_t active_alt;    // 0 = adc, 1 = ftm, -1 = unititalized
}struct_PIN_CFG_ALTS;

/** This enum lists all pins that can be changed from ftm to adc or reverse*/
typedef enum
{
    PIN_CFG_ALT_MAX
}enum_pin_cfg_alts_name;

/** List that contains all pins that can be changed from ftm to adc or reverse*/
extern struct_PIN_CFG_ALTS pin_cfg_alts[PIN_CFG_ALT_MAX + 1];

#endif

