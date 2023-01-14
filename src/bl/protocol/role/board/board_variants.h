/*----------------------------------------------------------------------------*/
/**
* \file         board_cc16wp_s32k144.h
* \brief        Contains board specific macros
*
* \date         20200306
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2020
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	role
* \brief        A board specific role file contains macros used by the PHY and, possibly,
*               other roles. If a board role is going to be defined it should be implemented
*               in #BOARD_FILENAME.c file.
*
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_ROLE_BOARD_BOARD_H_
#define SRC_ROLE_BOARD_BOARD_H_

#if defined(__cplusplus)
extern "C" {
#endif

// ===================================================================================================
// Board configuration macros and typedefs
// ===================================================================================================

/**
 * Ports
 */
typedef enum
{
    BOARD_PORTA,
    BOARD_PORTB,
    BOARD_PORTC,
    BOARD_PORTD,
    BOARD_PORTE
} enum_BOARD_PORT_OFFSET;


#if (BOARD_CAN_BAUDRATE_DEFAULT == 1000)
#define BOARD_CAN_DEFAULT_BAUDRATE  E_BAUD_1MBIT

#elif (BOARD_CAN_BAUDRATE_DEFAULT == 800)
#define BOARD_CAN_DEFAULT_BAUDRATE  E_BAUD_800KBIT

#elif (BOARD_CAN_BAUDRATE_DEFAULT == 500)
#define BOARD_CAN_DEFAULT_BAUDRATE  E_BAUD_500KBIT

#elif (BOARD_CAN_BAUDRATE_DEFAULT == 250)
#define BOARD_CAN_DEFAULT_BAUDRATE  E_BAUD_250KBIT

#elif (BOARD_CAN_BAUDRATE_DEFAULT == 125)
#define BOARD_CAN_DEFAULT_BAUDRATE  E_BAUD_125KBIT

#elif (BOARD_CAN_BAUDRATE_DEFAULT == 100)
#define BOARD_CAN_DEFAULT_BAUDRATE  E_BAUD_100KBIT

#elif (BOARD_CAN_BAUDRATE_DEFAULT == 83)
#define BOARD_CAN_DEFAULT_BAUDRATE  E_BAUD_83KBIT

#elif (BOARD_CAN_BAUDRATE_DEFAULT == 50)
#define BOARD_CAN_DEFAULT_BAUDRATE  E_BAUD_50KBIT

#elif (BOARD_CAN_BAUDRATE_DEFAULT == 33)
#define BOARD_CAN_DEFAULT_BAUDRATE  E_BAUD_33KBIT

#elif (BOARD_CAN_BAUDRATE_DEFAULT == 20)
#define BOARD_CAN_DEFAULT_BAUDRATE  E_BAUD_20KBIT

#elif (BOARD_CAN_BAUDRATE_DEFAULT == 10)
#define BOARD_CAN_DEFAULT_BAUDRATE  E_BAUD_10KBIT

#else
#error "BOARD_CAN_BAUDRATE_DEFAULT is undefined. Please correct the CAN_BAUDRATE_KBITS_DEFAULT parameter in the respective variant configuration in bt.variants. file."
#endif // BOARD_CAN_BAUDRATE_DEFAULT


/**
 * CAN
 */
#define BOARD_CAN_INVALID_REC   ((uint8_t)0xFF)

#define IS_CAN_INSTANCE_VALID(instance) ((uint8_t)(CAN_INSTANCES & (0x1 << instance)) ? ((uint8_t)instance) : BOARD_CAN_INVALID_REC)

_Static_assert( (BOARD_CAN_INVALID_REC != IS_CAN_INSTANCE_VALID(BOARD_CAN_BUS_DEFAULT)), "The default CAN instance CAN_INTERFACE_DEFAULT is invalid for the given variant (check bt.variants file).");


/**
 * CAN common initialization lists for CAN0, CAN1 and CAN2
 */
#if defined(CPU_S32K148)
#define BOARD_CAN_FD_ENS            {true, true, true}
#define BOARD_CAN_CAN_CLK           {PCC_FlexCAN0_INDEX, PCC_FlexCAN1_INDEX, PCC_FlexCAN2_INDEX}

#elif defined(CPU_S32K146)
#define BOARD_CAN_FD_ENS            {true, true, false}
#define BOARD_CAN_CAN_CLK           {PCC_FlexCAN0_INDEX, PCC_FlexCAN1_INDEX, PCC_FlexCAN2_INDEX}

#elif (defined(CPU_S32K144HFT0VLLT) || defined(CPU_S32K144LFT0MLLT))
#define BOARD_CAN_FD_ENS            {true, false, false}
#define BOARD_CAN_CAN_CLK           {PCC_FlexCAN0_INDEX, PCC_FlexCAN1_INDEX, PCC_FlexCAN2_INDEX}

#elif defined(CPU_S32K142)
#define BOARD_CAN_FD_ENS            {true, false, false}
#define BOARD_CAN_CAN_CLK           {PCC_FlexCAN0_INDEX, PCC_FlexCAN1_INDEX, BOARD_CAN_INVALID_REC}

#elif defined(CPU_S32K118)
#define BOARD_CAN_FD_ENS            {true, false, false}
#define BOARD_CAN_CAN_CLK           {PCC_FlexCAN0_INDEX, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#elif defined(CPU_S32K116)
#define BOARD_CAN_FD_ENS            {true, false, false}
#define BOARD_CAN_CAN_CLK           {PCC_FlexCAN0_INDEX, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#elif defined(CPU_MK10DN512VLL10)
#define BOARD_CAN_FD_ENS            {false}
#define BOARD_CAN_CAN_CLK           {kCLOCK_Flexcan0}

#else
#error "MCU type unknown"
#endif

#if (defined(CPU_S32K148) || defined(CPU_S32K146) || defined(CPU_S32K144HFT0VLLT) || defined(CPU_S32K144LFT0MLLT) || defined(CPU_S32K142))

#define BOARD_CAN_CLK_SRCS          {FLEXCAN_CLK_SOURCE_PERIPH, FLEXCAN_CLK_SOURCE_PERIPH, FLEXCAN_CLK_SOURCE_PERIPH}

#elif defined(CPU_MK10DN512VLL10)

#define BOARD_CAN_CLK_SRCS          {kFLEXCAN_ClkSrcPeri, kFLEXCAN_ClkSrcPeri}

#else
#error "MCU type unknown. Please add support of new MCU."
#endif // MCU_SERIES

#define BOARD_USE_INTERNAL_WDOG


#if (BOARD_PINOUT_CONFIG == 1)
// e.g. RemcoII ConnectedLogger S32K144, Rev. D, E
#define BOARD_CAN_GPIO_CLK          {PCC_PORTE_INDEX, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_GPIO_MUX          {5u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_RX_PORTS          {PORTE, NULL, NULL}
#define BOARD_CAN_TX_PORTS          {PORTE, NULL, NULL}

#define BOARD_CAN_RX_PINS           {4u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_TX_PINS           {5u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_EN_PORTS          {BOARD_PORTA}
#define BOARD_CAN_EN_PINS           {11u}
#define BOARD_CAN_STB_PORTS         {BOARD_PORTE}
#define BOARD_CAN_STB_PINS          {13u}

#ifdef BOARD_USE_EXTERNAL_WDOG
#undef BOARD_USE_EXTERNAL_WDOG
#endif // BOARD_USE_EXTERNAL_WDOG

#define BOARD_DO_POWER_PORT         BOARD_PORTE
#define BOARD_DO_POWER_PIN          16u

#elif (BOARD_PINOUT_CONFIG == 2)
// e.g. CC16WP S32K144, S32K148, Rev. A, B, C
#define BOARD_CAN_GPIO_CLK          {PCC_PORTE_INDEX, PCC_PORTA_INDEX, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_GPIO_MUX          {5u, 3u, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_RX_PORTS          {PORTE, PORTA, NULL}
#define BOARD_CAN_TX_PORTS          {PORTE, PORTA, NULL}

#define BOARD_CAN_RX_PINS           {4u, 12u, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_TX_PINS           {5u, 13u, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_EN_PORTS          {BOARD_PORTA, BOARD_PORTE}
#define BOARD_CAN_EN_PINS           {11u, 12u}
#define BOARD_CAN_STB_PORTS         {BOARD_PORTE, BOARD_PORTE}
#define BOARD_CAN_STB_PINS          {1u, 3u}

//#define BOARD_USE_EXTERNAL_WDOG
#define BOARD_EXT_WDOG_PORT         BOARD_PORTE
#define BOARD_EXT_WDOG_PIN          11u

#define BOARD_DO_POWER_PORT         BOARD_PORTD
#define BOARD_DO_POWER_PIN          15u

#elif (BOARD_PINOUT_CONFIG == 3)
// e.g. HMI Panel OnRoad Rev. A
#define BOARD_CAN_GPIO_CLK          {PCC_PORTE_INDEX, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_GPIO_MUX          {5u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_RX_PORTS          {PORTE, NULL, NULL}
#define BOARD_CAN_TX_PORTS          {PORTE, NULL, NULL}

#define BOARD_CAN_RX_PINS           {4u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_TX_PINS           {5u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_EN_PORTS          {BOARD_PORTE}
#define BOARD_CAN_EN_PINS           {11u}

#ifdef BOARD_USE_EXTERNAL_WDOG
#undef BOARD_USE_EXTERNAL_WDOG
#endif // BOARD_USE_EXTERNAL_WDOG

#define BOARD_DO_POWER_PORT         BOARD_PORTE
#define BOARD_DO_POWER_PIN          16u

#elif (BOARD_PINOUT_CONFIG == 4)
// e.g. HMI Panel OnRoad Rev. B
#define BOARD_CAN_GPIO_CLK          {PCC_PORTE_INDEX, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_GPIO_MUX          {5u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_RX_PORTS          {PORTE, NULL, NULL}
#define BOARD_CAN_TX_PORTS          {PORTE, NULL, NULL}

#define BOARD_CAN_RX_PINS           {4u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_TX_PINS           {5u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_EN_PORTS          {BOARD_PORTA}
#define BOARD_CAN_EN_PINS           {11u}
#define BOARD_CAN_STB_PORTS         {BOARD_PORTE}
#define BOARD_CAN_STB_PINS          {1u}

#ifdef BOARD_USE_EXTERNAL_WDOG
#undef BOARD_USE_EXTERNAL_WDOG
#endif // BOARD_USE_EXTERNAL_WDOG

#define BOARD_DO_POWER_PORT         BOARD_PORTE
#define BOARD_DO_POWER_PIN          16u

#elif (BOARD_PINOUT_CONFIG == 5)
// e.g. MiniDisplay Rev. A
#define BOARD_CAN_GPIO_CLK          {PCC_PORTB_INDEX, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_GPIO_MUX          {5u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_RX_PORTS          {PORTB, NULL, NULL}
#define BOARD_CAN_TX_PORTS          {PORTB, NULL, NULL}

#define BOARD_CAN_RX_PINS           {0u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_TX_PINS           {1u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_EN_PORTS          {BOARD_PORTD}
#define BOARD_CAN_EN_PINS           {0u}
#define BOARD_CAN_STB_PORTS         {BOARD_PORTD}
#define BOARD_CAN_STB_PINS          {14u}

#ifdef BOARD_USE_EXTERNAL_WDOG
#undef BOARD_USE_EXTERNAL_WDOG
#endif // BOARD_USE_EXTERNAL_WDOG

#define BOARD_DO_POWER_PORT         BOARD_PORTA
#define BOARD_DO_POWER_PIN          15u

#elif (BOARD_PINOUT_CONFIG == 6)
// e.g. MiniDisplay Rev. B
#define BOARD_CAN_GPIO_CLK          {PCC_PORTB_INDEX, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_GPIO_MUX          {5u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_RX_PORTS          {PORTB, NULL, NULL}
#define BOARD_CAN_TX_PORTS          {PORTB, NULL, NULL}

#define BOARD_CAN_RX_PINS           {0u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_TX_PINS           {1u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_EN_PORTS          {BOARD_PORTC}
#define BOARD_CAN_EN_PINS           {10u}
#define BOARD_CAN_STB_PORTS         {BOARD_PORTC}
#define BOARD_CAN_STB_PINS          {11u}

#ifdef BOARD_USE_EXTERNAL_WDOG
#undef BOARD_USE_EXTERNAL_WDOG
#endif // BOARD_USE_EXTERNAL_WDOG

#define BOARD_DO_POWER_PORT         BOARD_PORTA
#define BOARD_DO_POWER_PIN          13u

#elif (BOARD_PINOUT_CONFIG == 7)
// e.g. Bode TuerSteuerung TB12-24, S32K148, Rev. A
#define BOARD_CAN_GPIO_CLK          {PCC_PORTE_INDEX, PCC_PORTC_INDEX, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_GPIO_MUX          {5u, 3u, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_RX_PORTS          {PORTE, PORTC, NULL}
#define BOARD_CAN_TX_PORTS          {PORTE, PORTC, NULL}

#define BOARD_CAN_RX_PINS           {4u, 6u, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_TX_PINS           {5u, 7u, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_EN_PORTS          {BOARD_PORTA, BOARD_PORTE}
#define BOARD_CAN_EN_PINS           {11u, 12u}
#define BOARD_CAN_STB_PORTS         {BOARD_PORTE, BOARD_PORTE}
#define BOARD_CAN_STB_PINS          {1u, 3u}

#define BOARD_EXT_WDOG_PORT         BOARD_PORTE
#define BOARD_EXT_WDOG_PIN          11u

#define BOARD_DO_POWER_PORT         BOARD_PORTD
#define BOARD_DO_POWER_PIN          15u

#elif (BOARD_PINOUT_CONFIG == 8)
// e.g. MGW S32K148 Rev. B
#define BOARD_CAN_GPIO_CLK          {PCC_PORTE_INDEX, PCC_PORTA_INDEX, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_GPIO_MUX          {5u, 3u, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_RX_PORTS          {PORTE, PORTA, NULL}
#define BOARD_CAN_TX_PORTS          {PORTE, PORTA, NULL}

#define BOARD_CAN_RX_PINS           {4u, 12u, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_TX_PINS           {5u, 13u, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_EN_PORTS          {BOARD_PORTA, BOARD_PORTD}
#define BOARD_CAN_EN_PINS           {11u, 4u}
#define BOARD_CAN_STB_PORTS         {BOARD_PORTE, BOARD_PORTB}
#define BOARD_CAN_STB_PINS          {1u, 10u}

#ifdef BOARD_USE_EXTERNAL_WDOG
#undef BOARD_USE_EXTERNAL_WDOG
#endif // BOARD_USE_EXTERNAL_WDOG

#define BOARD_DO_POWER_PORT         BOARD_PORTD
#define BOARD_DO_POWER_PIN          15u

#elif (BOARD_PINOUT_CONFIG == 9)
// e.g. Wallbox Rev. B
#define BOARD_CAN_GPIO_CLK          {PCC_PORTE_INDEX, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_GPIO_MUX          {5u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_RX_PORTS          {PORTE, NULL, NULL}
#define BOARD_CAN_TX_PORTS          {PORTE, NULL, NULL}

#define BOARD_CAN_RX_PINS           {4u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_TX_PINS           {5u, BOARD_CAN_INVALID_REC, BOARD_CAN_INVALID_REC}

#ifdef BOARD_USE_EXTERNAL_WDOG
#undef BOARD_USE_EXTERNAL_WDOG
#endif // BOARD_USE_EXTERNAL_WDOG

#elif (BOARD_PINOUT_CONFIG == 10)
// e.g. Powerdistributor, S32K148, 3 CAN, Rev. B
#define BOARD_CAN_GPIO_CLK          {PCC_PORTE_INDEX, PCC_PORTC_INDEX, PCC_PORTB_INDEX}
#define BOARD_CAN_GPIO_MUX          {5u, 3u, 4u}

#define BOARD_CAN_RX_PORTS          {PORTE, PORTC, PORTB}
#define BOARD_CAN_TX_PORTS          {PORTE, PORTC, PORTB}

#define BOARD_CAN_RX_PINS           {4u, 6u, 12u}
#define BOARD_CAN_TX_PINS           {5u, 7u, 13u}

#define BOARD_CAN_EN_PORTS          {BOARD_PORTA, BOARD_PORTE, BOARD_PORTE}
#define BOARD_CAN_EN_PINS           {11u, 12u, 7u}
#define BOARD_CAN_STB_PORTS         {BOARD_PORTE, BOARD_PORTE, BOARD_PORTE}
#define BOARD_CAN_STB_PINS          {1u, 3u, 9u}

#ifdef BOARD_USE_EXTERNAL_WDOG
#undef BOARD_USE_EXTERNAL_WDOG
#endif // BOARD_USE_EXTERNAL_WDOG

#define BOARD_DO_POWER_PORT         BOARD_PORTD
#define BOARD_DO_POWER_PIN          15u

#elif (BOARD_PINOUT_CONFIG == 11)
// e.g. Wallbox Rev. A, MK10
#define BOARD_CAN_GPIO_CLK          {kCLOCK_PortA, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_GPIO_MUX          {kPORT_MuxAlt2, BOARD_CAN_INVALID_REC}

#define BOARD_CAN_RX_PORTS          {PORTA, NULL}
#define BOARD_CAN_TX_PORTS          {PORTA, NULL}

#define BOARD_CAN_RX_PINS           {13u, BOARD_CAN_INVALID_REC}
#define BOARD_CAN_TX_PINS           {12u, BOARD_CAN_INVALID_REC}

#ifdef BOARD_USE_EXTERNAL_WDOG
#undef BOARD_USE_EXTERNAL_WDOG
#endif // BOARD_USE_EXTERNAL_WDOG

#endif // BOARD_PINOUT_CONFIG


#ifdef __cplusplus
}
#endif

#endif /* SRC_ROLE_BOARD_BOARD_H_ */
