###################################################################################################
# This file includes the rule for the src/ subdirectory. All needed object files are 
# defined in here. Some object files are defined recursively, meaning for every .c or .cpp file in the
# directory there will be a .o file. Other object files are defined manualy depending on the needs of the
# developer. The same applies for the C header include paths.
# This file is later included into the main makefile located in the root of the project. Be aware of
# the paths as they are defined relative to the root of the demoproject.
###################################################################################################

###################################################################################################
# -include does not create errors if the file doesn't exists
# contains the object files and include path definition for app/ directory, mainly defined recursively
# path is relative to the main makfile in project root, please change this to lead to your app/ folder
-include src/app/app.mk

###################################################################################################
# all include paths for the src/ subdir, app/ not included
CFLAGS_INCLUDE_PATH_SRC			=	-I src															\
									-I src/bsp														\
									-I src/bsp/sdk/middleware/lin/coreapi							\
									-I src/bsp/sdk/middleware/lin/diagnostic						\
									-I src/bsp/sdk/middleware/lin/include							\
									-I src/bsp/sdk/middleware/lin/lowlevel							\
									-I src/bsp/sdk/middleware/lin/transport							\
									-I src/bsp/sdk/platform/devices									\
									-I src/bsp/sdk/platform/devices/common							\
									-I src/bsp/sdk/platform/devices/$(SYSTEM_HARDWARE)/include		\
									-I src/bsp/sdk/platform/devices/$(SYSTEM_HARDWARE)/include/cmsis\
									-I src/bsp/sdk/platform/devices/$(SYSTEM_HARDWARE)/startup		\
									-I src/bsp/sdk/platform/drivers/inc								\
									-I src/bsp/sdk/platform/drivers/src/adc							\
									-I src/bsp/sdk/platform/drivers/src/clock						\
									-I src/bsp/sdk/platform/drivers/src/clock/S32K1xx				\
									-I src/bsp/sdk/platform/drivers/src/crc							\
									-I src/bsp/sdk/platform/drivers/src/edma						\
									-I src/bsp/sdk/platform/drivers/src/flash						\
									-I src/bsp/sdk/platform/drivers/src/flexcan 					\
									-I src/bsp/sdk/platform/drivers/src/ftm 						\
									-I src/bsp/sdk/platform/drivers/src/interrupt 					\
									-I src/bsp/sdk/platform/drivers/src/lin 						\
									-I src/bsp/sdk/platform/drivers/src/lpi2c 						\
									-I src/bsp/sdk/platform/drivers/src/lpit 						\
									-I src/bsp/sdk/platform/drivers/src/lptmr 						\
									-I src/bsp/sdk/platform/drivers/src/lpspi 						\
									-I src/bsp/sdk/platform/drivers/src/lpuart 						\
									-I src/bsp/sdk/platform/drivers/src/pdb 						\
									-I src/bsp/sdk/platform/drivers/src/pins 						\
									-I src/bsp/sdk/platform/drivers/src/rtc 						\
									-I src/bsp/sdk/platform/drivers/src/wdog						\
									-I src/bsp/sdk/rtos/osif										\
									-I src/bsp/project_cfg/startup_code								\
									-I src/bsp_cfg/													\
									-I src/bsp_cfg/$(HW_TYP)										\
									-I src/bl														\
									-I src/bl/protocol/common										\
									-I src/bl/protocol/role											\
									-I src/bl/protocol/role/board									\
									-I src/bl/protocol/role/phy										\
									-I src/bl/protocol/role/protocol								\
									-I src/sfl/bl_protocol											\
									-I src/cfl/linstack												\
									-I src/cfl/linstack/lin/include									\
									-I src/cfl/linstack/lin/lowlevel								\
									-I src/sfl/can_db												\
									-I src/sfl/db													\
									-I src/sfl/fifo													\
									-I src/sfl/math													\
									-I src/sfl/timer												\
									-I src/hal_impl_s32k											\
									-I src/hal_def													\
									-I $(DS_DIR)/can												\
									-I $(DS_DIR)/dsl												\
									-I $(DS_DIR)/gp													\
									-I $(DS_DIR)/io													\
									-I $(DS_DIR)/lin												\
									-I $(DS_DIR)/module												\
									-I $(DS_DIR)/user

###################################################################################################
# Rules have to be given individually for every path containing source files or via VPATH for several directories
VPATH =		src																\
			src/bsp															\
			src/bsp/sdk/platform/devices									\
			src/bsp/sdk/platform/devices/common								\
			src/bsp/sdk/platform/devices/$(SYSTEM_HARDWARE)/include			\
			src/bsp/sdk/platform/devices/$(SYSTEM_HARDWARE)/include/cmsis	\
			src/bsp/sdk/platform/devices/$(SYSTEM_HARDWARE)/startup			\
			src/bsp/sdk/platform/drivers/inc								\
			src/bsp/sdk/platform/drivers/src/adc							\
			src/bsp/sdk/platform/drivers/src/clock							\
			src/bsp/sdk/platform/drivers/src/clock/S32K1xx					\
			src/bsp/sdk/platform/drivers/src/crc							\
			src/bsp/sdk/platform/drivers/src/edma							\
			src/bsp/sdk/platform/drivers/src/flash							\
			src/bsp/sdk/platform/drivers/src/flexcan						\
			src/bsp/sdk/platform/drivers/src/ftm							\
			src/bsp/sdk/platform/drivers/src/interrupt 						\
			src/bsp/sdk/platform/drivers/src/lin 							\
			src/bsp/sdk/platform/drivers/src/lpi2c 							\
			src/bsp/sdk/platform/drivers/src/lpit 							\
			src/bsp/sdk/platform/drivers/src/lptmr 							\
			src/bsp/sdk/platform/drivers/src/lpspi 							\
			src/bsp/sdk/platform/drivers/src/lpuart 						\
			src/bsp/sdk/platform/drivers/src/pdb 							\
			src/bsp/sdk/platform/drivers/src/pins 							\
			src/bsp/sdk/platform/drivers/src/rtc 							\
			src/bsp/sdk/platform/drivers/src/wdog							\
			src/bsp/sdk/rtos/osif											\
			src/bsp/project_cfg/startup_code								\
			src/bsp_cfg/													\
			src/bsp_cfg/$(HW_TYP)											\
			src/bl															\
			src/sfl/bl_protocol												\
			src/bl/protocol/common											\
			src/bl/protocol/role											\
			src/bl/protocol/role/board										\
			src/bl/protocol/role/phy										\
			src/bl/protocol/role/protocol									\
			src/cfl/linstack												\
			src/cfl/linstack/lin/include									\
			src/cfl/linstack/lin/lowlevel									\
			src/sfl/can_db													\
			src/sfl/db														\
			src/sfl/fifo													\
			src/sfl/math													\
			src/sfl/timer													\
			src/hal_impl_s32k												\
			src/hal_def														\
			$(APP_CLEAN_DIR)												\
			$(DS_DIR)/can													\
			$(DS_DIR)/dsl													\
			$(DS_DIR)/gp													\
			$(DS_DIR)/io													\
			$(DS_DIR)/lin													\
			$(DS_DIR)/module												\
			$(DS_DIR)/user


INT_CONF_SFL_OBJFILES =			$(INT_CONF_PATH_TO_OBJ)/sfl_db.o							\
								$(INT_CONF_PATH_TO_OBJ)/sfl_can_db.o 						\
								$(INT_CONF_PATH_TO_OBJ)/sfl_can_db_tables_data.o 			\
								$(INT_CONF_PATH_TO_OBJ)/sfl_timer.o 						\
								$(INT_CONF_PATH_TO_OBJ)/sfl_math.o 							\
								$(INT_CONF_PATH_TO_OBJ)/sfl_bl_protocol_s32k.o				\
								$(INT_CONF_PATH_TO_OBJ)/sfl_fifo.o

INT_CONF_DS_OBJFILES =			$(INT_CONF_PATH_TO_OBJ)/can_db_tables.o						\
								$(INT_CONF_PATH_TO_OBJ)/io_tables.o 						\
								$(INT_CONF_PATH_TO_OBJ)/io_variables.o 						\
								$(INT_CONF_PATH_TO_OBJ)/graph_code.o 						\
								$(INT_CONF_PATH_TO_OBJ)/graph_support.o 					\
								$(INT_CONF_PATH_TO_OBJ)/module_code.o 						\
								$(INT_CONF_PATH_TO_OBJ)/lin_db_tables.o						\
								$(INT_CONF_PATH_TO_OBJ)/lin_diagnose.o						\
								$(INT_CONF_PATH_TO_OBJ)/user_code.o							\
								$(INT_CONF_PATH_TO_OBJ)/modulhardwarecode.o