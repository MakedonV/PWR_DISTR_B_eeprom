#ifndef _CAN_APP_H_
#define _CAN_APP_H_
/*----------------------------------------------------------------------------*/
/**
* \file         can_app.h
* \brief
* \date         20190523
*
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup
* \brief
* \details
*/
/*----------------------------------------------------------------------------*/
#include "hal_can.h"
#include "flexcan_hw_access.h"
#include "sfl_can_db_tables_data.h"


// Handle which holds can error data
typedef struct
{
    uint32_t time_from_start_to_error;
    uint32_t last_can_error;
} struct_can_app_error;

typedef struct {
    uint16_t can_en_pin;      ///< can enable pin
    uint16_t can_stb_pin;     ///< can standby pin
} struct_can_pin_list;


/*----------------------------------------------------------------------------*/
/**
* \brief    create a local copy of the can handle
*
* \param    interface         [in]   uint8_t                  CAN bus nr.
* \param    ptr_handle        [out]  struct_hal_can_handle*   Pointer to handle
*
* \return   void
*
***/
void user_can_init(const struct_can_pin_list ptr_can_pin_list[]);


/*----------------------------------------------------------------------------*/
/**
* \brief    calculate can fd baudrate
*
* \param    arb_baud        [in]  uint32_t                  can-fd arbitration phase baudrate
* \param    arb_sample_pt   [in]  uint32_t                  can-fd arbitration phase sample point
* \param    data_baud       [in]  uint32_t                  can-fd data phase baudrate
* \param    data_sample_pt  [in]  uint32_t                  can-fd data phase sample point
* \param    arb_phase       [out] hal_can_time_segment*     can-fd arbitration phase sample point
* \param    data_phase      [out] hal_can_time_segment*     can-fd data phase baudrate
*
* \return   uint32_t                     Return code: 0= success, 1= failure
*
***/
uint32_t user_can_fd_calculate_baudrate(uint32_t arb_baud, uint32_t arb_sample_pt, uint32_t data_baud, uint32_t data_sample_pt, hal_can_time_segment* arb_phase, hal_can_time_segment* data_phase );

/*----------------------------------------------------------------------------*/
/**
* \brief    set can fd baudrate
*
* \param    interface       [in] uint8_t                 CAN bus nr.
* \param    can_fd_en       [in] uint8_t                 can-fd arbitration phase baudrate
* \param    baudrate        [in] enum_HAL_CAN_BAUDRATE   can-hs baudrate, if can_fd_en=0
* \param    arb_phase       [in] hal_can_time_segment    can-fd arbitration phase sample point
* \param    data_phase      [in] hal_can_time_segment    can-fd data phase baudrate
*
* \return   void
*
***/
void user_can_set_baudrate(uint8_t interface, uint8_t can_fd_en, enum_HAL_CAN_BAUDRATE baudrate, hal_can_time_segment arb_phase, hal_can_time_segment data_phase);

/*----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief    A getter function to retrieve the configured mask and filter macros
*           defined in can_db_tables.h into a given can_db_filter_typ array
* \details  This function is used as an interface between the library and app. The filter configurations
*           are defined as C Macros and are not able to be compiled into a library as its generated with
*           the can_db_tables from applics.
*
* \pre
*
* \param    filter      [in] can_db_filter_typ*     an array that holds the configuration structure for each can bus
* \return   void
*/
void ext_get_filter_and_mask(can_db_filter_typ filter[]);

#endif

