###################################################################################################
# app_user.mk - makefile for src/app/user_sources directory
# PUT YOUR C and H FILES in SRC/APP/USER_SOURCES and they will automatically be added to the build
#
# If you want to make changes to the build see instructions below.
###################################################################################################

###################################################################################################
# User definitions

# A list of include paths and source files that *shouldn't* be included, only add the subdirectory relative to app/
FILTER_C_HEADER_PATHS := # do_not_include_subdir 	\
						 # user_sources/do_not_include_subdir
						 
FILTER_SOURCE_FILES := # do_not_include.c			\
					   # user_sources/do_not_include.c