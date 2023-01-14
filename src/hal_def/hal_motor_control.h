#ifndef HAL_MOTOR_CONTROL_H
#define HAL_MOTOR_CONTROL_H
/*----------------------------------------------------------------------------*/
/**
* \file         hal_motor_control.h
* \brief        Definitions for hardware independent motor control unit.
*/
/*----------------------------------------------------------------------------*/
/** 
* \addtogroup   doc_hal_mot HAL MOT
* @{
* \brief        This documentation part describes the interface function definitions for a hardware-independent and abstracted motor control unit interface.
* \details      Main goal is to create a hardware-platform independent layer, that can be accessed from different HW-platforms.
*               
*
*/
/*----------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------------------------------
// includes
// ---------------------------------------------------------------------------------------------------

#include "hal_data_types.h"
#include "hal_motor_control_cfg.h"

// ---------------------------------------------------------------------------------------------------
// defines
// ---------------------------------------------------------------------------------------------------

#define HAL_MOTOR_CONTROL_DUTY_CYCLE_0_PERMILLE                    0    ///< Fixed value for minimum duty cycle in permill.
#define HAL_MOTOR_CONTROL_DUTY_CYCLE_1000_PERMILLE              1000    ///< Fixed value for maximum duty cycle in permill.

#define HAL_MOTOR_CONTROL_DUTY_CYCLE_0_PERCENT                     0    ///< Fixed value for minimum duty cycle in percent.
#define HAL_MOTOR_CONTROL_DUTY_CYCLE_100_PERCENT                 100    ///< Fixed value for maximum duty cycle in percent.

// ---------------------------------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------------------------------

/** This enum contains all possible error codes that can be returned from motor control functions.*/
typedef enum
{
    HAL_MOTOR_CONTROL_OK                    = 0u,               ///< No error.
    HAL_MOTOR_CONTROL_ERROR_MODE_INVALID                        ///< Invalid mode selected.

} enum_HAL_MOTOR_CONTROL_RETURN_VALUE;

/** This enum lists all available states a motor can become. Actual state of the motor, not user visible.*/
typedef enum
{
    HAL_MOTOR_CONTROL_STATE_NOT_SET        = 0u,                ///< Motor state not set.
    HAL_MOTOR_CONTROL_STATE_STOPPED            ,                ///< Motor state motor stopped.
    HAL_MOTOR_CONTROL_STATE_IN_BRAKE           ,                ///< Motor state motor in brake.
    HAL_MOTOR_CONTROL_STATE_MOVING_LEFT        ,                ///< Motor state motor moving left.
    HAL_MOTOR_CONTROL_STATE_MOVING_RIGHT       ,                ///< Motor state motor moving right.
    HAL_MOTOR_CONTROL_STATE_ERROR                               ///< Motor state error.

} enum_HAL_MOTOR_CONTROL_STATE;

/** This enum lists all available modes a motor can become. User function parameter for state setting.*/
typedef enum
{
    HAL_MOTOR_CONTROL_MODE_STOP            = 0u,                ///< Motor mode stop motor.
    HAL_MOTOR_CONTROL_MODE_BRAKE               ,                ///< Motor mode brake motor.
    HAL_MOTOR_CONTROL_MODE_MOVE_LEFT           ,                ///< Motor mode move left.
    HAL_MOTOR_CONTROL_MODE_MOVE_RIGHT          ,                ///< Motor mode move right.
    HAL_MOTOR_CONTROL_MODE_INVALID                              ///< Motor mode invalid. Do not use.

} enum_HAL_MOTOR_CONTROL_MODE;

// ---------------------------------------------------------------------------------------------------
// typedefs
// ---------------------------------------------------------------------------------------------------
/** This struct contains the complete motor control setup.*/
typedef struct
{
        enum_HAL_MOTOR_CONTROL_MODE  MotorDriveMode;            ///< User setup.
        uint16_t                     MotorDutyCycle;            ///< Motors PWM duty cycle (Percentage of supply voltage).
        enum_HAL_MOTOR_CONTROL_STATE internalMotorState;        ///< Motors actual state.

} struct_HAL_MOTOR_CONTROL;

// ---------------------------------------------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
*   \brief      Initialize motor controller
*   \details    This function overwrites the given control structure. 
*
*   \param      ptrMotorControlCfg              [in] struct_HAL_MOTOR_CONTROL_CFG* Pointer to motor configuration structure.
*   \param      ptrMotorControl                 [in] #struct_HAL_MOTOR_CONTROL*     Pointer to new motor control structure.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_MOTOR_CONTROL_RETURN_VALUE as feedback.
*/
enum_HAL_MOTOR_CONTROL_RETURN_VALUE hal_mc_init(struct_HAL_MOTOR_CONTROL_CFG* ptrMotorControlCfg,struct_HAL_MOTOR_CONTROL* ptrMotorControl);

/*----------------------------------------------------------------------------*/
/**
*   \brief      Motor move command.
*   \details    Use this function to set the new motor state.
*
*   \pre        The function #hal_mc_init must be called once before.
*
*   \param      ptrMotorControlCfg              [in] struct_HAL_MOTOR_CONTROL_CFG* Pointer to motor control structure.
*   \param      ptrMotorControl                 [in] #struct_HAL_MOTOR_CONTROL* Pointer to motor control structure.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_MOTOR_CONTROL_RETURN_VALUE as feedback.
*/
enum_HAL_MOTOR_CONTROL_RETURN_VALUE hal_mc_drive_motor(struct_HAL_MOTOR_CONTROL_CFG* ptrMotorControlCfg,struct_HAL_MOTOR_CONTROL* ptrMotorControl);


/*----------------------------------------------------------------------------*/
/**
*   \brief      Set the motors PWM frequency.
*   \details
*
*   \pre        The function #hal_mc_init must be called once before.
*
*   \param      ptrMotorControl                 [in] #struct_HAL_MOTOR_CONTROL* Pointer to motor control structure.
*   \return     This function returns zero for success or a positive error code from #enum_HAL_MOTOR_CONTROL_RETURN_VALUE as feedback.
*/
enum_HAL_MOTOR_CONTROL_RETURN_VALUE hal_mc_set_motor_frequency(struct_HAL_MOTOR_CONTROL* ptrMotorControl);

 /** @}*/
#endif


