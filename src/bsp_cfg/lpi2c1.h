#ifndef lpi2c1_H
#define lpi2c1_H
/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : lpi2c1.h
**     Project     : S32K_BSP
**     Processor   : S32K144_100
**     Component   : lpi2c
**     Version     : Component SDK_S32K14x_07, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K14x_07
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-01-11, 13:43, # CodeGen: 25
**     Contents    :
**         LPI2C_DRV_MasterInit                - status_t LPI2C_DRV_MasterInit(uint32_t instance,const...
**         LPI2C_DRV_MasterDeinit              - status_t LPI2C_DRV_MasterDeinit(uint32_t instance);
**         LPI2C_DRV_MasterGetBaudRate         - void LPI2C_DRV_MasterGetBaudRate(uint32_t instance, lpi2c_baud_rate_params_t...
**         LPI2C_DRV_MasterSetBaudRate         - void LPI2C_DRV_MasterSetBaudRate(uint32_t instance,const lpi2c_mode_t...
**         LPI2C_DRV_MasterSetSlaveAddr        - void LPI2C_DRV_MasterSetSlaveAddr(uint32_t instance,const uint16_t...
**         LPI2C_DRV_MasterSendData            - status_t LPI2C_DRV_MasterSendData(uint32_t instance,const uint8_t *...
**         LPI2C_DRV_MasterSendDataBlocking    - status_t LPI2C_DRV_MasterSendDataBlocking(uint32_t instance,const uint8_t *...
**         LPI2C_DRV_MasterAbortTransferData   - status_t LPI2C_DRV_MasterAbortTransferData(uint32_t instance);
**         LPI2C_DRV_MasterReceiveData         - status_t LPI2C_DRV_MasterReceiveData(uint32_t instance,uint8_t *...
**         LPI2C_DRV_MasterReceiveDataBlocking - status_t LPI2C_DRV_MasterReceiveDataBlocking(uint32_t instance,uint8_t *...
**         LPI2C_DRV_MasterGetTransferStatus   - status_t LPI2C_DRV_MasterGetTransferStatus(uint32_t instance,uint32_t *...
**         LPI2C_DRV_MasterIRQHandler          - void LPI2C_DRV_MasterIRQHandler(uint32_t instance);
**         LPI2C_DRV_SlaveInit                 - status_t LPI2C_DRV_SlaveInit(uint32_t instance,const...
**         LPI2C_DRV_SlaveDeinit               - status_t LPI2C_DRV_SlaveDeinit(uint32_t instance);
**         LPI2C_DRV_SlaveSetTxBuffer          - status_t LPI2C_DRV_SlaveSetTxBuffer(uint32_t instance,const uint8_t *...
**         LPI2C_DRV_SlaveSetRxBuffer          - status_t LPI2C_DRV_SlaveSetRxBuffer(uint32_t instance,uint8_t *...
**         LPI2C_DRV_SlaveSendData             - status_t LPI2C_DRV_SlaveSendData(uint32_t instance,const uint8_t *...
**         LPI2C_DRV_SlaveSendDataBlocking     - status_t LPI2C_DRV_SlaveSendDataBlocking(uint32_t instance,const uint8_t *...
**         LPI2C_DRV_SlaveReceiveData          - status_t LPI2C_DRV_SlaveReceiveData(uint32_t instance,uint8_t *...
**         LPI2C_DRV_SlaveReceiveDataBlocking  - status_t LPI2C_DRV_SlaveReceiveDataBlocking(uint32_t instance,uint8_t *...
**         LPI2C_DRV_SlaveGetTransferStatus    - status_t LPI2C_DRV_SlaveGetTransferStatus(uint32_t instance,uint32_t *...
**         LPI2C_DRV_SlaveAbortTransferData    - status_t LPI2C_DRV_SlaveAbortTransferData(uint32_t instance);
**         LPI2C_DRV_SlaveIRQHandler           - void LPI2C_DRV_SlaveIRQHandler(uint32_t instance);
**
**     Copyright 1997 - 2015 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     All Rights Reserved.
**     
**     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
**     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
**     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
**     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
**     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
**     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**     THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file lpi2c1.h
** @version 01.00
*/
/*!
**  @addtogroup lpi2c1_module lpi2c1 module documentation
**  @{
*/

/* MODULE lpi2c1. */
/*!
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macros were defined for consistency reasons, all the instances have a corresponding number.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.1, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.2, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.4, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.5, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 */

/* Include inherited beans */
#include "lpi2c_driver.h"
#include "clockMan1.h"
#include "Cpu.h"
#include "dmaController1.h"
#include "osif1.h"

/*! @brief Number of I2C instances */
#define NUMBER_OF_I2C_INSTANCES     (1U)

/*! @brief Device instance number */
#define INST_LPI2C1 0

/*! @brief Master configuration declaration */
extern const lpi2c_master_user_config_t lpi2c1_MasterConfig0;

/*! @brief Driver state structure which holds driver runtime data */
extern lpi2c_master_state_t lpi2c1MasterState;

/*! @brief Master callback external declaration */
extern void lpi2c1_MasterCallback0(uint8_t instance,i2c_master_event_t masterEvent,void *userData);

/*! @brief Driver and configuration structure
 * Implements : struct_i2c_config
 */
typedef struct
{
    uint8_t i2c_instance_number;                            // Instance number
    lpi2c_master_state_t * i2c_state_struct;                     // Driver state structure which holds driver runtime data
    const lpi2c_master_user_config_t * i2c_init_config_struct;   // Structure for initialization parameters
}struct_i2c_config_t;

/*! @brief Declaration of the driver and configuration table structure */
extern struct_i2c_config_t struct_i2c_config_tbl[NUMBER_OF_I2C_INSTANCES];

#endif
/* ifndef __lpi2c1_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
