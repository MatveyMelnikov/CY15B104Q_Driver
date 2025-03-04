MODULES_DIR = Modules

CY15B104Q_DRIVER_DIR = $(MODULES_DIR)/CY15B104Q_Driver

TESTS = Y

C_INCLUDES = \
-I$(CY15B104Q_DRIVER_DIR)/Inc

C_SOURCES = \
$(CY15B104Q_DRIVER_DIR)/Src/cy15b104q_driver.c

ifeq ($(TESTS), Y)
include MakefileTest.mk
else
C_SOURCES += Core/Src/main.c
include MakefileTarget.mk                                       
endif