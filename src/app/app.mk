###################################################################################################
# app.mk - makefile for src/app directory
# PUT YOUR C and H FILES in SRC/APP and they will automatically be added to the build
#
# If you want to make changes to the build see instructions below.
###################################################################################################

###################################################################################################
# path is relative to the main makfile in project root, please change this to lead to your app/ folder
# some variables may not be defined in the release makefile (e.g. HW_TYP), this however will not cause any errors
INT_CONF_APP_BASE_PATH := src/app
INT_CONF_APP_HW_PATH := $(INT_CONF_APP_BASE_PATH)/$(HW_TYP)
INT_CONF_DS_BASE_PATH := src/ds

-include src/app/user_sources/app_user.mk

###################################################################################################
# MRS defintions
# If you are not familiar with it -> do not edit, this could break the build process.

###################################################################################################
# Get all subdirectories located in this directory
ALL_APP_SUBDIR := $(patsubst %/,%,$(sort $(dir $(wildcard $(shell find $(INT_CONF_APP_BASE_PATH)/ -depth)))))

# filter all hw types out, get all hw types from the ds folder
APP_HW_TYP_DIR := $(patsubst %/,%,$(subst $(INT_CONF_DS_BASE_PATH),$(INT_CONF_APP_BASE_PATH),$(sort $(dir $(wildcard $(INT_CONF_DS_BASE_PATH)/*/)))))
APP_CLEAN_DIR := $(filter-out $(APP_HW_TYP_DIR),$(ALL_APP_SUBDIR)) $(INT_CONF_APP_HW_PATH)

###################################################################################################
# all include paths for the app/ subdir.
# this variable will contains every subdir (including app/ itself) in which a header file is defined
CFLAGS_INCLUDE_PATH_SRC_APP_ALL := $(patsubst %/,%,$(sort $(dir $(foreach subdir, $(APP_CLEAN_DIR), $(wildcard $(subdir)/*h)))))
CFLAGS_INCLUDE_PATH_SRC_APP := $(addprefix -I,$(filter-out $(addprefix $(INT_CONF_APP_BASE_PATH)/, $(FILTER_C_HEADER_PATHS)), $(CFLAGS_INCLUDE_PATH_SRC_APP_ALL)))

###################################################################################################
# user_api objects, needed for lib
INT_CONF_USER_API_PATH = $(INT_CONF_APP_BASE_PATH)/user_api

INT_CONF_USER_API_C_SRCFILES_ALL := $(wildcard $(INT_CONF_USER_API_PATH)/*.c)
INT_CONF_USER_API_CPP_SRCFILES_ALL := $(wildcard $(INT_CONF_USER_API_PATH)/*.cpp)

INT_CONF_USER_API_C_SRCFILES := $(filter-out $(addprefix $(INT_CONF_APP_BASE_PATH)/, $(FILTER_SOURCE_FILES)),$(INT_CONF_USER_API_C_SRCFILES_ALL))
INT_CONF_USER_API_CPP_SRCFILES := $(filter-out $(addprefix $(INT_CONF_APP_BASE_PATH)/, $(FILTER_SOURCE_FILES)),$(INT_CONF_USER_API_CPP_SRCFILES_ALL))

INT_CONF_USER_API_OBJFILES := $(sort $(patsubst %.c, $(INT_CONF_PATH_TO_OBJ)/%.o, $(notdir $(INT_CONF_USER_API_C_SRCFILES)))		\
								$(patsubst %.cpp, $(INT_CONF_PATH_TO_OBJ)/%.o, $(notdir $(INT_CONF_USER_API_CPP_SRCFILES))))

###################################################################################################
# app objects
INT_CONF_APP_C_SRCFILES_ALL := $(foreach subdir, $(APP_CLEAN_DIR), $(wildcard $(subdir)/*.c))
INT_CONF_APP_CPP_SRCFILES_ALL := $(foreach subdir, $(APP_CLEAN_DIR), $(wildcard $(subdir)/*.cpp))

INT_CONF_APP_C_SRCFILES := $(filter-out $(INT_CONF_USER_API_C_SRCFILES) $(addprefix $(INT_CONF_APP_BASE_PATH)/, $(FILTER_SOURCE_FILES)),$(INT_CONF_APP_C_SRCFILES_ALL))
INT_CONF_APP_CPP_SRCFILES := $(filter-out $(INT_CONF_USER_API_CPP_SRCFILES) $(addprefix $(INT_CONF_APP_BASE_PATH)/, $(FILTER_SOURCE_FILES)),$(INT_CONF_APP_CPP_SRCFILES_ALL))

INT_CONF_APP_OBJFILES = $(sort $(patsubst %.c, $(INT_CONF_PATH_TO_OBJ)/%.o, $(notdir $(INT_CONF_APP_C_SRCFILES)))		\
						$(patsubst %.cpp, $(INT_CONF_PATH_TO_OBJ)/%.o, $(notdir $(INT_CONF_APP_CPP_SRCFILES))))
