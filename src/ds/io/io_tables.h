#ifndef IO_TABLES_H
#define IO_TABLES_H

#include "pins_driver.h"
#include "hal_data_types.h"

#define NUM_OF_CONFIGURED_PINS 122
#define PD_NONE 3
#define PD_OUT GPIO_OUTPUT_DIRECTION
#define PD_IN GPIO_INPUT_DIRECTION
#define PORT_INTERNAL_PULL_DOW_ENABLED PORT_INTERNAL_PULL_DOWN_ENABLED
#define DOX_SHIFT_IN_DS DO_SHIFT_IN_DS

/* custom pin names defined in applics pin editor */
 

extern pin_settings_config_t g_pin_mux_InitConfigArr[NUM_OF_CONFIGURED_PINS];

typedef enum
{
    CALIB_NONE                      = 0u,     ///< No Calibration at all
    CALIB_SOURCE_1P                     ,     ///< 1 sink or source points
    CALIB_SOURCE_2P                     ,     ///< 2 sink or source points
    CALIB_SOURCE_3P                     ,     ///< 3 sink or source points
    CALIB_SOURCE_4P                     ,     ///< 4 sink or source points
    CALIB_SOURCE_SINK_1P                ,     ///< 1 sink and source points
    CALIB_SOURCE_SINK_2P                ,     ///< 2 sink and source points
    CALIB_SOURCE_SINK_3P                ,     ///< 3 sink and source points
    CALIB_SOURCE_SINK_4P                      ///< 4 sink and source points
} enum_adc_CALIBRATION_TYPE;

/* enum which contains the names of all available pins of the MCU */
typedef enum
{
// PORTA                                             ,
DI_AI_MOTOR_1_VM1                       = 0          ,
DOX_SHIFT_OE_N                                       ,
MC_I2C_SDA                                           ,
MC_I2C_SCL                                           ,
DI_AIX_MUX_1                                         ,
DI_AIX_MUX_2                                         ,
DO_LIN_EN                                            ,
DO_MOTOR_2_INH                                       ,
CAN_EN1                                              ,
MC_FREQ_IN4                                          ,
MC_FREQ_IN3                                          ,
CAN_WAKE1                                            ,
DI_AIX_MUX_3                                         ,
DI_AIX_MUX_4                                         ,
DO_MOTOR_1_INH                                       ,
DO_PWM_HSD_OUT13                                     ,
DO_PWM_HSD_OUT14                                     ,
DO_PWM_HSD_OUT15                                     ,
DO_PWM_HSD_OUT16                                     ,
DO_MOTOR_1_INH_2                                     ,
DOX_SHIFT_DS_INTERFACE                               ,
DO_MOTOR_2_INH_2                                     ,
// PORTB                                             ,
MC_SCI_RXD                                           ,
MC_SCI_TXD                                           ,
MC_FREQ_IN2                                          ,
MC_FREQ_IN1                                          ,
DO_PWM_HSD_OUT_10                                    ,
DO_PWM_HSD_OUT_11                                    ,
XTAL                                                 ,
EXTAL                                                ,
DOX_SHIFT_ST_CP                                      ,
DO_PWM_DEBUG_LED                                     ,
DO_PWM_HSD_OUT_2                                     ,
DO_PWM_HSD_OUT_7                                     ,
MC_CAN_RXD3                                          ,
MC_CAN_TXD3                                          ,
DI_AI_VREF                                           ,
DI_AI_X1000_19_RS485_RS232_AE                        ,
DI_AI_PIGGY_1                                        ,
DO_CS_DIS                                            ,
DO_PWM_HSD_OUT_9                                     ,
DI_AI_HSD_CURRENT_OUT_13                             ,
DI_AI_HSD_CURRENT_OUT_14                             ,
DI_AI_HSD_CURRENT_OUT_15                             ,
DO_PWM_SOLAR_LOAD                                    ,
DOX_SHIFT_SH_CP                                      ,
DOX_SHIFT_IN_DS                                      ,
DI_AI_KL30                                           ,
DOX_SHIFT_MR_N                                       ,
// PORTC                                             ,
DO_PWM_HSD_OUT_8                                     ,
DI_AI_PIGGY_2                                        ,
DI_AI_PIGGY_3                                        ,
DI_AI_PIGGY_4                                        ,
MC_CAN_RXD2                                          ,
MC_CAN_TXD2                                          ,
DO_NMEA_2000_MESSUNG                                 ,
DO_PWM_HSD_OUT_6                                     ,
DO_PWM_HSD_OUT_4                                     ,
DO_LIN_WAKE                                          ,
DO_PWM_HSD_OUT_5                                     ,
DI_AI_HSD_SENSE_1                                    ,
DI_AI_HSD_SENSE_2                                    ,
DI_AI_HSD_SENSE_3                                    ,
DI_AI_HSD_SENSE_4                                    ,
DI_AI_HSD_SENSE_5                                    ,
DI_AI_HSD_SENSE_6                                    ,
DI_AI_HSD_SENSE_7                                    ,
DO_PWM_HSD_OUT_3                                     ,
DI_AI_HSD_SENSE_8                                    ,
DI_AI_HSD_CURRENT_OUT_16                             ,
DI_AI_SOLAR_I                                        ,
// PORTD                                             ,
MC_SPI_CLK_PIGGY                                     ,
MC_SPI_MISO_PIGGY                                    ,
MC_SPI_MOSI_PIGGY                                    ,
DO_SPI_CS_PIGGY                                      ,
DI_AI_TASTER_2                                       ,
DOX_SHIFT_SH_CP_INTERFACE                            ,
DO_MUX_A_ANA                                         ,
DO_MUX_B_ANA                                         ,
DO_MUX_C_ANA                                         ,
MC_FREQ_KL15                                         ,
MC_FREQ_IN6                                          ,
MC_FREQ_IN5                                          ,
DOX_SHIFT_OE_INTERFACE_N                             ,
DO_RS_DCDC_10V                                       ,
DO_KL30_EN                                           ,
DO_POWER_EN                                          ,
DOX_SHIFT_MR_INTERFACE_N                             ,
DO_VREF_EN                                           ,
DI_AI_TASTER                                         ,
DI_AI_HSD_SENSE_9                                    ,
DI_AI_HSD_SENSE_10                                   ,
DI_AI_HSD_SENSE_11                                   ,
DI_AI_HSD_SENSE_12                                   ,
DO_PWM_MOTOR_1_POS                                   ,
DO_PWM_MOTOR_1_NEG                                   ,
DO_PWM_MOTOR_2_POS                                   ,
DO_PWM_MOTOR_2_NEG                                   ,
// PORTE                                             ,
CAN_ERR1                                             ,
CAN_STB1                                             ,
DI_AI_MOTOR_1_VM                                     ,
CAN_STB2                                             ,
MC_CAN_RXD1                                          ,
MC_CAN_TXD1                                          ,
DI_AI_MOTOR_2_VM1                                    ,
CAN_EN3                                              ,
DO_PWM_HSD_OUT_12                                    ,
CAN_STB3                                             ,
CAN_ERR3                                             ,
CAN_WAKE3                                            ,
CAN_EN2                                              ,
CAN_ERR2                                             ,
CAN_WAKE2                                            ,
MC_FREQ_IN7                                          ,
DOX_SHIFT_ST_CP_INTERFACE                            ,
DI_AI_SOLAR_V                                        ,
DO_PWM_HSD_OUT_1                                     ,
DI_AI_MOTOR_2_VM                                     ,
DI_AI_MOTOR_2_IS_NEG                                 ,
DI_AI_MOTOR_1_IS_NEG                                 ,
DI_AI_MOTOR_2_IS_POSITIVE                            ,
DI_AI_MOTOR_1_IS_POSITIVE                            ,
PIN_MAX                                              ,
 
}enum_pin_name;


/* enum which contains the names of the virtual pins*/
typedef enum
{
DOM_BEREICH_30V_7                  = PIN_MAX          ,
DOM_BEREICH_30V_6                                     ,
DOM_BEREICH_30V_5                                     ,
DOM_BEREICH_30V_4                                     ,
DOM_BEREICH_30V_3                                     ,
DOM_BEREICH_30V_2                                     ,
DOM_BEREICH_30V_1                                     ,
DOM_PD7                                               ,
DOM_PD6                                               ,
DOM_PD5                                               ,
DOM_PD4                                               ,
DOM_PD3                                               ,
DOM_PD2                                               ,
DOM_PD1                                               ,
DOM_PU7                                               ,
DOM_PU6                                               ,
DOM_PU5                                               ,
DOM_PU4                                               ,
DOM_PU3                                               ,
DOM_PU2                                               ,
DOM_PU1                                               ,
DIM_IN1                                               ,
DIM_IN2                                               ,
DIM_IN3                                               ,
DIM_IN4                                               ,
DIM_IN5                                               ,
DIM_IN6                                               ,
DIM_IN7                                               ,
DIM_KL_15                                             ,
DIM_VDD_NMEA                                          ,
DIM_PIGGY_VARIANTE_LEFT                               ,
DIM_PIGGY_VARIANTE_RIGHT                              ,
DIM_HSD_IO_1                                          ,
DIM_HSD_IO_2                                          ,
DIM_HSD_IO_3                                          ,
DIM_HSD_IO_4                                          ,
DIM_HSD_IO_5                                          ,
DIM_HSD_IO_6                                          ,
DIM_HSD_IO_7                                          ,
DIM_HSD_IO_8                                          ,
DIM_HSD_IO_9                                          ,
DIM_HSD_IO_10                                         ,
DIM_HSD_IO_11                                         ,
DIM_HSD_IO_12                                         ,
DIM_HSD_IO_13                                         ,
DIM_HSD_IO_14                                         ,
DIM_HSD_IO_15                                         ,
DIM_HSD_IO_16                                         ,
VIRTUAL_PIN_MAX                                       ,
 
}enum_virtual_pin_name;


/* enum which contains the names of the PWM channels */
typedef enum
{
PWM_DEBUG_LED             = 0          ,
PWM_HSD_OUT_1                          ,
PWM_HSD_OUT_2                          ,
PWM_HSD_OUT_3                          ,
PWM_HSD_OUT_4                          ,
PWM_HSD_OUT_5                          ,
PWM_HSD_OUT_6                          ,
PWM_HSD_OUT_7                          ,
PWM_HSD_OUT_8                          ,
PWM_HSD_OUT_9                          ,
PWM_HSD_OUT_10                         ,
PWM_HSD_OUT_11                         ,
PWM_HSD_OUT_12                         ,
PWM_HSD_OUT_13                         ,
PWM_HSD_OUT_14                         ,
PWM_HSD_OUT_15                         ,
PWM_HSD_OUT_16                         ,
PWM_MOTOR_1_NEG                        ,
PWM_MOTOR_1_POS                        ,
PWM_MOTOR_2_NEG                        ,
PWM_MOTOR_2_POS                        ,
PWM_SOLAR_LOAD                         ,
PWM_MAX                                ,
FREQ_IN1                               ,
FREQ_IN2                               ,
FREQ_IN3                               ,
FREQ_IN4                               ,
FREQ_IN5                               ,
FREQ_IN6                               ,
FREQ_IN7                               ,
FREQ_KL15                              ,
FREQ_MAX                               ,
 
}enum_pwm_pin_name;


/* enum which contains the names of the ADC channels */
typedef enum
{
AI_MOTOR_1_VM1                       = 0          ,
AIX_MUX_1                                         ,
AIX_MUX_2                                         ,
AI_PIGGY_2                                        ,
AI_PIGGY_3                                        ,
AI_PIGGY_4                                        ,
AI_HSD_SENSE_1                                    ,
AI_HSD_SENSE_2                                    ,
AI_HSD_SENSE_3                                    ,
AI_HSD_SENSE_4                                    ,
AI_HSD_CURRENT_OUT_13                             ,
AI_HSD_CURRENT_OUT_14                             ,
AI_HSD_CURRENT_OUT_15                             ,
AI_KL30                                           ,
AI_HSD_SENSE_5                                    ,
AI_HSD_SENSE_6                                    ,
AI_HSD_SENSE_7                                    ,
AI_HSD_SENSE_8                                    ,
AI_HSD_CURRENT_OUT_16                             ,
AI_SOLAR_I                                        ,
AI_TASTER_2                                       ,
AI_VREF                                           ,
AI_MOTOR_1_VM                                     ,
AIX_MUX_3                                         ,
AIX_MUX_4                                         ,
AI_X1000_19_RS485_RS232_AE                        ,
AI_MOTOR_2_VM1                                    ,
AI_PIGGY_1                                        ,
AI_TASTER                                         ,
AI_HSD_SENSE_9                                    ,
AI_HSD_SENSE_10                                   ,
AI_HSD_SENSE_11                                   ,
AI_HSD_SENSE_12                                   ,
AI_SOLAR_V                                        ,
AI_MOTOR_2_VM                                     ,
AI_MOTOR_2_IS_NEG                                 ,
AI_MOTOR_1_IS_NEG                                 ,
AI_MOTOR_2_IS_POSITIVE                            ,
AI_MOTOR_1_IS_POSITIVE                            ,
AIM_IN_1                                          ,
AIM_IN_2                                          ,
AIM_IN_3                                          ,
AIM_IN_4                                          ,
AIM_IN_5                                          ,
AIM_IN_6                                          ,
AIM_IN_7                                          ,
AIM_POWER_TEMP                                    ,
AIM_TEMP_1                                        ,
AIM_TEMP_3                                        ,
AIM_TEMP_2                                        ,
AIM_TEMP_OUT_13_16                                ,
AIM_KL_15                                         ,
AIM_VDD_NMEA                                      ,
AIM_PIGGY_VARIANTE_LEFT                           ,
AIM_PIGGY_VARIANTE_RIGHT                          ,
AIM_HSD_IO_1                                      ,
AIM_HSD_IO_2                                      ,
AIM_HSD_IO_3                                      ,
AIM_HSD_IO_4                                      ,
AIM_HSD_IO_5                                      ,
AIM_HSD_IO_6                                      ,
AIM_HSD_IO_7                                      ,
AIM_HSD_IO_8                                      ,
AIM_HSD_IO_9                                      ,
AIM_HSD_IO_10                                     ,
AIM_HSD_IO_11                                     ,
AIM_HSD_IO_12                                     ,
AIM_HSD_IO_13                                     ,
AIM_HSD_IO_14                                     ,
AIM_HSD_IO_15                                     ,
AIM_HSD_IO_16                                     ,
ADC_MAX                                           ,
 
}enum_adc_pin_name;


/*! @brief ADC structure io_tables
 * Implements : adc_config_tbl
 */
typedef struct
{
        enum_adc_pin_name name;                 // Define the name of the channel
        uint8_t adc_instance;                   // Define on which instance the channel is
        uint8_t adc_channel;                    // Define the number of the channel
        uint32_t adc_vrefh;
        uint32_t resolution;
        _Bool multiplex;                        //Define if this Pin is a multiplex input and thus not connected physically to the processor
        uint8_t multiplex_group;                //Define which to which multiplex group this pin belongs (further information in modulhardwarecode_adc_multiplex)
        uint16_t cal_index;                     //Contains the Index on which address the calibration values has been saved in the eeprom. 0 if there are no values available
        enum_adc_CALIBRATION_TYPE cal_type;     //what kind of calibration is used.
}struct_adc_config_t;

typedef struct
{
        enum_virtual_pin_name name; // Virtual pin name
        uint8_t current_val;        // current_value of the virtual pin
}struct_VIRTUAL_PINS_CFG;/*! @brief Declaration of the ADC structure */


extern const struct_adc_config_t adc_config_tbl[ADC_MAX + 1];
extern struct_VIRTUAL_PINS_CFG virtual_pin[VIRTUAL_PIN_MAX - PIN_MAX];


#endif 
