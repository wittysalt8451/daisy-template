# Project Name
TARGET = TwinningDelay

USE_DAISYSP_LGPL = 1

# Sources
CPP_SOURCES = TwinningDelay.cpp

# Library Locations
LIBDAISY_DIR = ../../libDaisy/
DAISYSP_DIR = ../../DaisySP/

LDFLAGS += -u _printf_float

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
