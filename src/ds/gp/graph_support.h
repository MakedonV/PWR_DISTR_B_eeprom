#ifndef GRAPH_SUPPORT_H
#define GRAPH_SUPPORT_H

#include "hal_data_types.h"
#include "dsl_cfg.h"
#include "sfl_timer.h"
#include "sfl_can_db.h"
#include "sfl_fifo.h"
#include "sfl_math.h"
#include "hal_can.h"
#include "hal_io.h"
#include "hal_lin.h"
#include "hal_pwm.h"
#include "flexTimer1.h"
#include "adConv1.h"
#include "io_tables.h"
#include "user_api_can.h"
#include "user_api_eeprom.h"
#include "user_api_io.h"
#include "user_api_pwm.h"
#include "user_api_system.h"
#include "user_api_timer.h"
#include "user_api_util.h"
#include "user_api_uart.h"
#include "user_api_ai.h"
#include "can_db_tables.h"
#include "lin_db_tables.h"
#include "lin_stack.h"
#include "modulhardwarecode.h"

#define DATEN_IN 0
#define DATEN_OUT 1

#define PD_NONE 3
#define PD_OUT GPIO_OUTPUT_DIRECTION
#define PD_IN GPIO_INPUT_DIRECTION
#define PORT_INTERNAL_PULL_DOW_ENABLED PORT_INTERNAL_PULL_DOWN_ENABLED

#ifdef GRAPH_USE_DOUBLE
    #define ROUND(x)        \
		if (x < 0)          \
		(int32)(x - 0.5)    \
		else                \
		(int32)(x + 0.5)
#else                   
    #define ROUND(x) x
#endif 

#define volt *1000L  
#define Volt *1000L

#define sec  *1000L
#define Sec  *1000L

typedef unsigned int index_type;


/** used for type conversion (big/little endian) */
typedef union
{
	uint32_t word;
	struct
	{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}b;  ///< big endian behavior. here for legacy (HCS) support.
	struct
	{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}b_big_endian;
	struct
	{
		uint8_t byte0;
		uint8_t byte1;
		uint8_t byte2;
		uint8_t byte3;
	}b_little_endian;
}long_byte_typ;


typedef union
{
	uint8_t byte;
	struct
	{
		uint8_t bit0 :1;
		uint8_t bit1 :1;
		uint8_t bit2 :1;
		uint8_t bit3 :1;
		uint8_t bit4 :1;
		uint8_t bit5 :1;
		uint8_t bit6 :1;
		uint8_t bit7 :1;
	}b;
}int_bit_typ;

extern const uint8_t bios_bitmask[];
extern uint8_t  timer_diff;
extern uint32_t user_variable[];


uint32_t get(index_type index);

void set(index_type index, uint32_t value);

uint32_t get_max(index_type index);

uint32_t get_min(index_type index);


#endif /* GRAPH_SUPPORT_H */
