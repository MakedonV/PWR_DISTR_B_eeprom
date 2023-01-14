###################################################################################################
# Makefile for sw18015_bsp_s32k_demoproject

###################################################################################################
# general options (use on demand)
# commandline options for make: --warn-undefined-variables, --just-print, --print-data-base
#

.SECONDARY: # don't delete intermediate files

# Gnu Make Debugger
# include gmd
# $(__BREAKPOINT)

MAKE_OPTIONS ?= -r
GCC_VERSION := $(shell arm-none-eabi-gcc -dumpversion)

export PROJECT_NAME ?= sw18015_bsp_s32k_demoproject
CI_COMMIT_SHA ?= 0


###################################################################################################
# Magic numbers to identify each MCU inside the BSW. Numbers for used MCU is described inside 
# MRS_BaugruppendatenV05_1.xlsx -> slide MCU_TYP -> column "MCU_Type (in EEPROM)"
# Needed for the graphlib. Depending on the value of the MCU_TYP define the graphlib selects 
# the right code inside the affected graphlib blocks
S32K144 = 6
S32K148 = 8
S32K142 = 11

###################################################################################################
# set the default configuration
# 
SYSTEM_TARGET        ?= standalone_app
SYSTEM_BUILD         ?= debug
SYSTEM_ARCH          ?= arm_cortex
SYSTEM_OS            ?= none

# defined based on IO_DEF when built with Applics Studio
SYSTEM_XTAL_MHZ      ?= 16
SYSTEM_HARDWARE      ?= S32K144
HW_TYP               ?= 


###################################################################################################
# determine operating system
#
ifeq ($(findstring Windows,$(OS)),Windows)
    INT_OPERATING_SYSTEM = Windows
else
    INT_OPERATING_SYSTEM = Linux
endif

###################################################################################################
# determine and compare GCC version
#
RECOMENDED_GCC_VERSION	= 8.3.1
ifeq ($(GCC_VERSION),$(RECOMENDED_GCC_VERSION))
    $(info arm_none_eabi_gcc version V$(GCC_VERSION))
else
    $(info arm_none_eabi_gcc version V$(GCC_VERSION))
    $(info you're using an untested version of GCC. The tested GCC version is $(RECOMENDED_GCC_VERSION))
endif


# handling for parallel jobs with option -j
# for OS Windows
ifeq ($(INT_OPERATING_SYSTEM),Windows)
# if cores of CPU is not handled by environment variable set -j1 which is like no parallel jobs
ifeq ($(NUMBER_OF_PROCESSORS),)
MAKE_OPTIONS         += -j1
else
MAKE_OPTIONS         += -j$(NUMBER_OF_PROCESSORS)
endif
endif
# for OS Linux
ifeq ($(INT_OPERATING_SYSTEM),Linux)
MAKE_OPTIONS         += -j$(shell grep -c ^processor /proc/cpuinfo)
endif


###################################################################################################
# replace src/ds folder with test_src/ds folder if requested
#
ifeq ($(HARDWARE_TESTPROJECT),yes)
    DS_DIR = src/ds_HW_TEST/$(HW_TYP)
else
    DS_DIR = src/ds/$(HW_TYP)
endif

###################################################################################################
# set mcu derivate dependent paths and variables
#
PATH_TO_LD			= "./build/$(SYSTEM_HARDWARE)/"
STARTUP_ASM			= startup_$(SYSTEM_HARDWARE)
SYSTEM_DERIVATE_SRC	= system_$(SYSTEM_HARDWARE).o
CPU_NAME_APPDX= 
ifeq ($(SYSTEM_HARDWARE),S32K144)
    
    ifeq ($(SYSTEM_TARGET),app)
        NAME_OF_LD = "$(SYSTEM_HARDWARE)_64_flash.ld"
    else
        NAME_OF_LD = "$(SYSTEM_HARDWARE)_64_flash_standalone.ld"
    endif

	# exception for S32K144 for its ugly name
    CPU_NAME_APPDX			= HFT0VLLT
endif

ifeq ($(SYSTEM_HARDWARE),S32K148)

    
    ifeq ($(SYSTEM_TARGET),app)
        NAME_OF_LD = "$(SYSTEM_HARDWARE)_256_flash.ld"
    else
        NAME_OF_LD = "$(SYSTEM_HARDWARE)_256_flash_standalone.ld"
    endif
endif

ifeq ($(SYSTEM_HARDWARE),S32K142)    
    ifeq ($(SYSTEM_TARGET),app)
        NAME_OF_LD = "$(SYSTEM_HARDWARE)_32_flash.ld"
    else
        NAME_OF_LD = "$(SYSTEM_HARDWARE)_32_flash_standalone.ld"
    endif
endif


CPU						= -DCPU_$(SYSTEM_HARDWARE)$(CPU_NAME_APPDX)

###################################################################################################
# set mcu crystal dependent variables
    XTAL_FREQ_HZ					= -DXTAL_FREQ_HZ=$$(( $(SYSTEM_XTAL_MHZ)*1000000 ))u

###################################################################################################
# get current time
#
#INTERNAL_CURRENT_TIME = $(shell date +\"%F\ %T\")

###################################################################################################
# help
#
help: 
	@echo
	@echo "Makefile for "$(PROJECT_NAME)
	@echo "============================="
	@echo
	@echo "productive targets (tasks):"
	@echo "image            : create image (compile+link)"
	@echo "The following build variables need to be set, otherwise the default setting will be used:"
	@echo ""
	@echo "Variable name        | Options"
	@echo "_________________________________________________"
	@echo ""
	@echo "SYSTEM_BUILD         | debug (default)|release"
	@echo "SYSTEM_TARGET        | standalone_app (default)|app|codecheck|swtest"
	@echo "SYSTEM_VARIANT       | select the hardware variant, see bsw.variants"
	@echo "HARDWARE_TESTPROJECT | no (default)|yes"
	@echo "_________________________________________________"
	@echo ""
	@echo "Example: make -r -j8 image SYSTEM_BUILD=release SYSTEM_TARGET=app SYSTEM_VARIANT=1 HW_TEST_VARIANT=4"
	@echo ""
	@echo "build_libs       : create precompiled libraries"
	@echo "doku             : document the source code (usually calling doxygen)"
	@echo "clean            : remove all created parts"
	@echo "dsl_clean        : remove all created parts except the libraries"
	@echo
	@echo "print configuration targets:"
	@echo "info             : dump out the configuration"
	@echo


###################################################################################################
# define the toolchain
#
ifneq (,$(filter $(SYSTEM_TARGET),app standalone_app))
    TOOLCHAIN_PATH		= 
    CC					= $(TOOLCHAIN_PATH)arm-none-eabi-gcc
    CCPLUS				= $(TOOLCHAIN_PATH)arm-none-eabi-gcc
    LD					= $(TOOLCHAIN_PATH)arm-none-eabi-g++
    BIN2HEX				= $(TOOLCHAIN_PATH)arm-none-eabi-objcopy
    AR					= $(TOOLCHAIN_PATH)arm-none-eabi-ar
    IMAGE_FILE_TYPE		= .s19
    
    SIGNING_BAT_PATH	?= app_utils\firmware_signing
    SIGN_EXE			= firmware_signing_proc.bat
endif

ifeq ($(SYSTEM_TARGET),swtest)
    TOOLCHAIN_PATH	= 
    CC				= $(TOOLCHAIN_PATH)arm-none-eabi-gcc
    CCPLUS			= $(TOOLCHAIN_PATH)g++
    LD				= $(TOOLCHAIN_PATH)gcc
    IMAGE_FILE_TYPE = 
endif

ifeq ($(SYSTEM_TARGET),codecheck)
    CC				= $(AXIVION_PATH)cafeCC
    CCPLUS			= $(AXIVION_PATH)cafeCC
    LD				= $(AXIVION_PATH)cafeCC
    AR				= #not required in case of codecheck, because it's only used for packing libraries
    IMAGE_FILE_TYPE	= .ir
endif


###################################################################################################
# build rules

INT_CONF_NAME_OF_IMAGE_STEM ?= $(SYSTEM_TARGET)_$(SYSTEM_BUILD)_$(HW_TYP)_$(SYSTEM_HARDWARE)_$(SYSTEM_XTAL_MHZ)MHz_$(SYSTEM_ARCH)_$(SYSTEM_OS)
INT_CONF_NAME_OF_IMAGE       = $(INT_CONF_NAME_OF_IMAGE_STEM)$(IMAGE_FILE_TYPE)
INT_CONF_PATH_TO_BIN         = ./bin
INT_CONF_PATH_TO_OBJ         = $(INT_CONF_PATH_TO_BIN)/obj
INT_CONF_PATH_TO_LIB         = $(INT_CONF_PATH_TO_BIN)/lib/$(SYSTEM_XTAL_MHZ)/$(SYSTEM_HARDWARE)$(LIB_HW_IDENTIFIER)_lib
INT_CONF_PATH_TO_USERCODE    = $(PATH_TO_SRC)/test

ALL_LIBRARIES =	$(INT_CONF_PATH_TO_LIB)/lib_$(SYSTEM_HARDWARE)_sdk.a \
				$(INT_CONF_PATH_TO_LIB)/lib_$(SYSTEM_HARDWARE)_bsp_cfg.a \
				$(INT_CONF_PATH_TO_LIB)/lib_$(SYSTEM_HARDWARE)_hal_impl_s32k.a \
				$(INT_CONF_PATH_TO_LIB)/lib_$(SYSTEM_HARDWARE)_bl_protocol.a

ifeq ($(INT_OPERATING_SYSTEM),Windows)
    # To sign the image use the following
    #SIGN_CMD = 	$(SIGN_EXE) $(INT_CONF_PATH_TO_BIN)/$(INT_CONF_NAME_OF_IMAGE) crc+sign
     
    # To only integrate the CRC32 use the following
    SIGN_CMD = 	cmd //C "$*$(SIGNING_BAT_PATH)\$(SIGN_EXE)" $(INT_CONF_PATH_TO_BIN)/$(INT_CONF_NAME_OF_IMAGE) crc 
else
    SIGN_CMD = #$(SIGNING_BAT_PATH)/firmware_signing_proc.sh ${APPLICATION_FILE} crc
endif

###################################################################################################
# definition of components, which files will be used to build a target
# Definig the include paths
CFLAGS_INCLUDE_PATH =

# -include does not create errors if the file doesn't exists
# contains the object files and include path definition for app/ dir, mainly defined recursive
-include src/src.mk

# If we want to take all sources in a path, use wildcards. Alternatively give the objects explicitly by name. It is suggested to place
# source files used to build different targets in different directories. E.g. The main file for softwaretests resides in src/<cppunitFramework>,
# the main file for the target application should reside in src/<application>, so that different targets are built of all files from different directories.
# We define variables for different groups of object files usually for every directory and add those groups according to the configuration.
PATH_TO_SRC           = ./
OBJS_OF_PATH_SRC      = $(patsubst $(PATH_TO_SRC)/%.cpp,$(INT_CONF_PATH_TO_OBJ)/%.o,$(wildcard $(PATH_TO_SRC)/*.cpp)) \
                        $(patsubst $(PATH_TO_SRC)/%.c,$(INT_CONF_PATH_TO_OBJ)/%.o,$(wildcard $(PATH_TO_SRC)/*.c))


PATH_TO_TEST_SRC       = test_src
OBJS_OF_PATH_TEST_SRC  = $(patsubst $(PATH_TO_TEST_SRC)/%.cpp,$(INT_CONF_PATH_TO_OBJ)/%.o,$(wildcard $(PATH_TO_TEST_SRC)/*.cpp)) \
                         $(patsubst $(PATH_TO_TEST_SRC)/%.c,$(INT_CONF_PATH_TO_OBJ)/%.o,$(wildcard $(PATH_TO_TEST_SRC)/*.c))


ifneq (,$(filter $(SYSTEM_TARGET),app standalone_app codecheck))
    INT_CONF_OBJFILES					+= $(INT_CONF_SDK_OBJFILES)
    INT_CONF_OBJFILES					+= $(INT_CONF_BSP_CFG_OBJFILES)
    INT_CONF_OBJFILES					+= $(INT_CONF_DS_OBJFILES)
    INT_CONF_OBJFILES					+= $(INT_CONF_HAL_IMPL_S32K_OBJFILES)
    INT_CONF_OBJFILES					+= $(INT_CONF_SFL_OBJFILES)
    INT_CONF_OBJFILES					+= $(INT_CONF_BL_PROTOCOL_OBJFILES)
    INT_CONF_OBJFILES					+= $(INT_CONF_APP_OBJFILES)
    INT_CONF_OBJFILES					+= $(INT_CONF_USER_API_OBJFILES)
endif


ifeq ($(SYSTEM_TARGET),swtest)
    INT_CONF_OBJFILES					+= $(OBJS_OF_PATH_SRC)
    INT_CONF_OBJFILES					+= $(INT_CONF_PATH_TO_OBJ)/CppUnitTestMain_Text.o
    INT_CONF_OBJFILES					+= $(OBJS_OF_PATH_TEST_SRC)
endif

###################################################################################################
# compiler- and linkerflags, defining INT_CONF_CFLAGS, INT_CONF_CPPFLAGS, INT_CONF_LFLAGS

# It is often useful to describe flags in macros
CFLAGS_C_C11        = -std=c11
CFLAGS_WARNINGS     = -Werror -Wall
CFLAGS_WARNINGS_LOW = -Wall
CFLAGS_WARNINGS_ALL = -pedantic -Werror -Wall -Wextra
CFLAGS_DEBUG        = -ggdb -g3 -O0
CFLAGS_DEPS         = -MMD -MP
CFLAGS_SOFTWARETEST = -DSOFTWARETEST
CFLAGS_PATCHES       = 
CPPFLAGS_TARGET      = 


# Define macros for the linker flags
LFLAGS_PTHREAD      = -pthread
LFLAGS_LIB_MATH     = -lm
LFLAGS_LIB_STD_CPP  = -lstdc++
LFLAGS_SOFTWARETEST = -lcppunit_64bit

###################################################################################################
# target specific flags
ifeq ($(SYSTEM_TARGET),swtest)
    # we need special handling for swtest because it's a different environment
    CFLAGS_TARGET        = $(CFLAGS_SOFTWARETEST) $(CFLAGS_DEBUG)
    CPPFLAGS_TARGET      = $(CFLAGS_SOFTWARETEST) $(CFLAGS_DEBUG)
    LFLAGS_TARGET        = $(LFLAGS_SOFTWARETEST)
    CFLAGS_INCLUDE_PATH += $(CFLAGS_INCLUDE_PATH_SWTEST)
else
	# target specific flags for all targets except swtest
    CFLAGS_TARGET			+= -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16                                                              		\
                              -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Xlinker                                           		\
                              --gc-sections $(CFLAGS_C_C11) -Wunused -Wall -Wno-unknown-pragmas -Wa,-adhln                                            		\
                              -DROLE_MINIMAL $(CPU) -D$(SYSTEM_HARDWARE)=$($(SYSTEM_HARDWARE)) -DMCU_TYP=$(SYSTEM_HARDWARE) $(XTAL_FREQ_HZ)

    LFLAGS_TARGET			+= -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16                         \
                              -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Xlinker      \
                              --gc-sections -specs=nano.specs -specs=nosys.specs -Wl,--print-memory-usage

    CFLAGS_INCLUDE_PATH += $(CFLAGS_INCLUDE_PATH_SRC) $(CFLAGS_INCLUDE_PATH_SRC_APP)
endif

# some flags are target dependent
ifeq ($(SYSTEM_TARGET),app)
    # app: no debug symbols, use bootloader
    CFLAGS_PATCHES       += -DPATCH_KSB_244
    CFLAGS_TARGET        += $(CFLAGS_PATCHES)
else ifeq ($(SYSTEM_TARGET),standalone_app)
    # we're not app, we're e.g. standalone_app, release or axivion codecheck
    CFLAGS_PATCHES       += 
    CFLAGS_TARGET        += $(CFLAGS_PATCHES) -DSTANDALONE_APP
endif


# some flags are architecture dependent
ifeq ($(SYSTEM_ARCH),arm_cortex)
    CFLAGS_ARCH     =  
    CPPFLAGS_ARCH   =  
    LFLAGS_ARCH     =  
else
    CFLAGS_ARCH     =  
    CPPFLAGS_ARCH   =  
    LFLAGS_ARCH     =  
endif

# some flags are os dependent
ifeq ($(SYSTEM_OS),none)
    CFLAGS_OS    =  
    CPPFLAGS_OS  =  
    LFLAGS_OS    =  
else
    CFLAGS_OS    =  
    CPPFLAGS_OS  =  
    LFLAGS_OS    =  
endif

# some flags are hardware dependent
ifeq ($(SYSTEM_HARDWARE),S32K144)
    CFLAGS_HARDWARE   =  
    CPPFLAGS_HARDWARE =  
    LFLAGS_HARDWARE   =  
else
    CFLAGS_HARDWARE   =  
    CPPFLAGS_HARDWARE =  
    LFLAGS_HARDWARE   =  
endif


# some flags are build dependent
ifeq ($(SYSTEM_BUILD),debug)
    CFLAGS_BUILD     = $(CFLAGS_DEBUG)
    CPPFLAGS_BUILD   = $(CFLAGS_DEBUG)
    LFLAGS_BUILD     = -g3 -O0 
else ifeq ($(SYSTEM_BUILD),release)
    CFLAGS_BUILD     = -O1
    CPPFLAGS_BUILD   = 
    LFLAGS_BUILD     = -O1
endif

# Combine the compiler- and linker flag macros (eventually differentiated by architecture, os ...)
INT_CONF_CFLAGS   = $(CFLAGS_BUILD) $(CFLAGS_TARGET) $(CFLAGS_ARCH) $(CFLAGS_OS) $(CFLAGS_HARDWARE) $(CFLAGS_INCLUDE_PATH) $(CFLAGS_DEPS)
INT_CONF_CPPFLAGS =                 $(CPPFLAGS_TARGET) $(CPPFLAGS_ARCH) $(CPPFLAGS_OS) $(CPPFLAGS_HARDWARE) $(CFLAGS_INCLUDE_PATH) $(CFLAGS_DEPS)
INT_CONF_LFLAGS   = $(LFLAGS_BUILD) $(LFLAGS_TARGET) $(LFLAGS_ARCH) $(LFLAGS_OS) $(LFLAGS_HARDWARE)


###################################################################################################
# dependencies
# must reside after initialization of all used variables.
# if you have trouble try omitting the preceding '-' on the include directive to see which are available
#
INT_CONF_DEPFILES = 

ifneq (,$(filter $(SYSTEM_TARGET),app standalone_app))
    INT_CONF_DEPFILES  = $(INT_CONF_OBJFILES:.o=.d)
    -include $(INT_CONF_DEPFILES)
# codecheck is always a full build and therefore excluded here
endif

###############################################################################################################
############################################# productive targets ##############################################
###############################################################################################################	

###################################################################################################
# pattern rule for c++ files
$(INT_CONF_PATH_TO_OBJ)/%.o : %.cpp $(INT_CONF_PATH_TO_OBJ)/.dirStampFile
	$(CCPLUS) -c $(INT_CONF_CPPFLAGS) $< -o $@

# pattern rule for c files
$(INT_CONF_PATH_TO_OBJ)/%.o : %.c $(INT_CONF_PATH_TO_OBJ)/.dirStampFile
	$(CC) -c $(INT_CONF_CFLAGS) $< -o $@ > $@.lst

# pattern rule for S files (assembler)
$(INT_CONF_PATH_TO_OBJ)/%.o : %.S $(INT_CONF_PATH_TO_OBJ)/.dirStampFile
	$(CC) -c $(CFLAGS_INCLUDE_PATH) -std=c11 -x assembler-with-cpp -O0 -g3 -mcpu=cortex-m4 -mthumb  $< -o $@


#####################################################################################################
# linker pattern (eventually several targets hex, elf, srec, s19... depending on each other)
# make image
#

ifeq ($(SYSTEM_TARGET),codecheck)
$(INT_CONF_PATH_TO_BIN)/$(INT_CONF_NAME_OF_IMAGE): $(INT_CONF_OBJFILES) $(INT_CONF_PATH_TO_BIN)/.dirStampFile
	$(LD) $(INT_CONF_OBJFILES) $(INT_CONF_LFLAGS) -o $(INT_CONF_PATH_TO_BIN)/$(INT_CONF_NAME_OF_IMAGE)
else
$(INT_CONF_PATH_TO_BIN)/$(INT_CONF_NAME_OF_IMAGE_STEM).elf: $(ALL_LIBRARIES)															\
	$(INT_CONF_SFL_OBJFILES)																											\
	$(INT_CONF_DS_OBJFILES)																												\
	$(INT_CONF_APP_OBJFILES)																											\
	$(INT_CONF_USER_API_OBJFILES)																										\
	$(INT_CONF_PATH_TO_BIN)/.dirStampFile
	$(LD) $(subst /,\,$(INT_CONF_LFLAGS)) -L $(PATH_TO_LD) -T $(NAME_OF_LD) -o $@														\
	$(INT_CONF_DS_OBJFILES) $(INT_CONF_SFL_OBJFILES) $(INT_CONF_APP_OBJFILES) $(INT_CONF_USER_API_OBJFILES)								\
	-Wl,--whole-archive																													\
	$(ALL_LIBRARIES)																													\
	-Wl,-Map=$@.map,--no-whole-archive

# convert to srec with S3 lines and sign it, if app target
$(INT_CONF_PATH_TO_BIN)/$(INT_CONF_NAME_OF_IMAGE): $(INT_CONF_PATH_TO_BIN)/$(INT_CONF_NAME_OF_IMAGE_STEM).hex
	$(BIN2HEX) -O srec $< $@ --srec-forceS3
ifeq ($(SYSTEM_TARGET),app)
	@echo
	@echo post processing, adding header to s19 file
	$(SIGN_CMD)
endif
endif

# convert to intel hex (intermediate and for debug/analysis purposes)
$(INT_CONF_PATH_TO_BIN)/$(INT_CONF_NAME_OF_IMAGE_STEM).hex: $(INT_CONF_PATH_TO_BIN)/$(INT_CONF_NAME_OF_IMAGE_STEM).elf
	$(BIN2HEX) -O ihex $< $@


#####################################################################################################
# target image
# create an image from the APP files and CFG, SDK and HAL libraries
#
image: $(INT_CONF_PATH_TO_BIN)/$(INT_CONF_NAME_OF_IMAGE) $(INT_CONF_PATH_TO_BIN)/.dirStampFile
	file $(INT_CONF_PATH_TO_BIN)/$(INT_CONF_NAME_OF_IMAGE)


#####################################################################################################
# create directories if necessary
#
$(INT_CONF_PATH_TO_BIN)/.dirStampFile:
	@mkdir -p $(INT_CONF_PATH_TO_BIN)
	@touch $@

$(INT_CONF_PATH_TO_OBJ)/.dirStampFile:
	@mkdir -p $(INT_CONF_PATH_TO_OBJ)
	@touch $@
	
$(INT_CONF_PATH_TO_LIB)/.dirStampFile:
	@mkdir -p $(INT_CONF_PATH_TO_LIB)
	@touch $@

#####################################################################################################
# target clean
#
.PHONY: dsl_clean
dsl_clean:
	echo "remove objects"
	echo "remove binaries"
	@rm -fv $(INT_CONF_PATH_TO_BIN)/*.elf
	@rm -fv $(INT_CONF_PATH_TO_BIN)/*.hex
	@rm -fv $(INT_CONF_PATH_TO_BIN)/*.s19
	@rm -fv $(INT_CONF_PATH_TO_BIN)/*.elf.map
	@rm -rfv $(INT_CONF_PATH_TO_OBJ)

.PHONY: clean
clean: dsl_clean
	echo "remove libraries"
	@rm -rfv $(INT_CONF_PATH_TO_BIN)

