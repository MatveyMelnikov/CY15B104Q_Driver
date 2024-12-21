CFLAGS = -DTEST -DUNITY_INCLUDE_CONFIG_H
TESTS_DIR = Tests
MOCKS_DIR = $(TESTS_DIR)/Mocks
FAKES_DIR = $(TESTS_DIR)/Fakes
UNITY_DIR = $(MODULES_DIR)/Unity-2.6.0

FOR_TARGET = N

TARGET_CY15B104Q_DRIVER_DIR = $(TESTS_DIR)/Target_Tests/CY15B104Q_Driver

HOST_CY15B104Q_DRIVER_DIR = $(TESTS_DIR)/Host_Tests/CY15B104Q_Driver

C_INCLUDES += \
-I$(UNITY_DIR)/src \
-I$(UNITY_DIR)/extras/fixture/src \
-I$(UNITY_DIR)/extras/memory/src \
-I$(TESTS_DIR) \
-I$(MOCKS_DIR)/Inc \
-I$(FAKES_DIR)/Inc

C_SOURCES += \
$(UNITY_DIR)/src/unity.c \
$(UNITY_DIR)/extras/fixture/src/unity_fixture.c \
$(UNITY_DIR)/extras/memory/src/unity_memory.c

ifeq ($(FOR_TARGET), Y)
C_SOURCES += \
$(TESTS_DIR)/target_tests.c \
$(TARGET_CY15B104Q_DRIVER_DIR)/cy15b104q_driver_test_runner.c \
$(TARGET_CY15B104Q_DRIVER_DIR)/cy15b104q_driver_test.c

include MakefileTarget.mk
else
C_SOURCES += \
$(TESTS_DIR)/host_tests.c \
$(HOST_CY15B104Q_DRIVER_DIR)/cy15b104q_driver_test_runner.c \
$(HOST_CY15B104Q_DRIVER_DIR)/cy15b104q_driver_test.c \
$(MOCKS_DIR)/Src/mock.c \
$(MOCKS_DIR)/Src/mock_spi.c

include MakefileTestHost.mk
endif
