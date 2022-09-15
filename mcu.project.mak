####################################################################
# Automatically-generated file. Do not edit!                       #
# Makefile Version 10                                              #
####################################################################

BASE_SDK_PATH = gecko_sdk
UNAME:=$(shell uname -s | sed -e 's/^\(CYGWIN\).*/\1/' | sed -e 's/^\(MINGW\).*/\1/')
ifeq ($(UNAME),MINGW)
# Translate "C:/super" into "/C/super" for MinGW make.
SDK_PATH := /$(shell echo $(BASE_SDK_PATH) | sed s/://)
endif
SDK_PATH ?= $(BASE_SDK_PATH)
COPIED_SDK_PATH ?= gecko_sdk_4.1.1

# This uses the explicit build rules below
PROJECT_SOURCE_FILES =

C_SOURCE_FILES   += $(filter %.c, $(PROJECT_SOURCE_FILES))
CXX_SOURCE_FILES += $(filter %.cpp, $(PROJECT_SOURCE_FILES))
CXX_SOURCE_FILES += $(filter %.cc, $(PROJECT_SOURCE_FILES))
ASM_SOURCE_FILES += $(filter %.s, $(PROJECT_SOURCE_FILES))
ASM_SOURCE_FILES += $(filter %.S, $(PROJECT_SOURCE_FILES))
LIB_FILES        += $(filter %.a, $(PROJECT_SOURCE_FILES))

C_DEFS += \
 '-DDEBUG_EFM=1' \
 '-DEFM32GG990F1024=1' \
 '-DSL_BOARD_NAME="BRD2200A"' \
 '-DSL_BOARD_REV="B05"' \
 '-DSL_COMPONENT_CATALOG_PRESENT=1'

ASM_DEFS += \
 '-DDEBUG_EFM=1' \
 '-DEFM32GG990F1024=1' \
 '-DSL_BOARD_NAME="BRD2200A"' \
 '-DSL_BOARD_REV="B05"' \
 '-DSL_COMPONENT_CATALOG_PRESENT=1'

INCLUDES += \
 -Iconfig \
 -Iautogen \
 -I. \
 -I$(SDK_PATH)/platform/Device/SiliconLabs/EFM32GG/Include \
 -I$(SDK_PATH)/platform/common/inc \
 -I$(SDK_PATH)/hardware/board/inc \
 -I$(SDK_PATH)/platform/CMSIS/Core/Include \
 -I$(SDK_PATH)/platform/service/device_init/inc \
 -I$(SDK_PATH)/platform/emlib/inc \
 -I$(SDK_PATH)/platform/driver/leddrv/inc \
 -I$(SDK_PATH)/platform/common/toolchain/inc \
 -I$(SDK_PATH)/platform/service/system/inc \
 -I$(SDK_PATH)/platform/service/sleeptimer/inc

GROUP_START =-Wl,--start-group
GROUP_END =-Wl,--end-group

PROJECT_LIBS = \
 -lgcc \
 -lc \
 -lm \
 -lnosys

LIBS += $(GROUP_START) $(PROJECT_LIBS) $(GROUP_END)

LIB_FILES += $(filter %.a, $(PROJECT_LIBS))

C_FLAGS += \
 -mcpu=cortex-m3 \
 -mthumb \
 -std=c99 \
 -Wall \
 -Wextra \
 -Os \
 -fdata-sections \
 -ffunction-sections \
 -fomit-frame-pointer \
 -imacros sl_gcc_preinclude.h \
 --specs=nano.specs \
 -g

CXX_FLAGS += \
 -mcpu=cortex-m3 \
 -mthumb \
 -std=c++11 \
 -fno-rtti \
 -fno-exceptions \
 -Wall \
 -Wextra \
 -Os \
 -fdata-sections \
 -ffunction-sections \
 -fomit-frame-pointer \
 -imacros sl_gcc_preinclude.h \
 --specs=nano.specs \
 -g

ASM_FLAGS += \
 -mcpu=cortex-m3 \
 -mthumb \
 -imacros sl_gcc_preinclude.h \
 -x assembler-with-cpp

LD_FLAGS += \
 -mcpu=cortex-m3 \
 -mthumb \
 -T"autogen/linkerfile.ld" \
 --specs=nano.specs \
 -Xlinker -Map=$(OUTPUT_DIR)/$(PROJECTNAME).map \
 -Wl,--gc-sections


####################################################################
# SDK Build Rules                                                  #
####################################################################
$(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_control_gpio.o: $(SDK_PATH)/hardware/board/src/sl_board_control_gpio.c
	@echo 'Building $(SDK_PATH)/hardware/board/src/sl_board_control_gpio.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/hardware/board/src/sl_board_control_gpio.c
CDEPS += $(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_control_gpio.d
OBJS += $(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_control_gpio.o

$(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_init.o: $(SDK_PATH)/hardware/board/src/sl_board_init.c
	@echo 'Building $(SDK_PATH)/hardware/board/src/sl_board_init.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/hardware/board/src/sl_board_init.c
CDEPS += $(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_init.d
OBJS += $(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_init.o

$(OUTPUT_DIR)/sdk/platform/common/src/sl_assert.o: $(SDK_PATH)/platform/common/src/sl_assert.c
	@echo 'Building $(SDK_PATH)/platform/common/src/sl_assert.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/common/src/sl_assert.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_assert.d
OBJS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_assert.o

$(OUTPUT_DIR)/sdk/platform/common/toolchain/src/sl_memory.o: $(SDK_PATH)/platform/common/toolchain/src/sl_memory.c
	@echo 'Building $(SDK_PATH)/platform/common/toolchain/src/sl_memory.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/common/toolchain/src/sl_memory.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/common/toolchain/src/sl_memory.d
OBJS += $(OUTPUT_DIR)/sdk/platform/common/toolchain/src/sl_memory.o

$(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFM32GG/Source/startup_efm32gg.o: $(SDK_PATH)/platform/Device/SiliconLabs/EFM32GG/Source/startup_efm32gg.c
	@echo 'Building $(SDK_PATH)/platform/Device/SiliconLabs/EFM32GG/Source/startup_efm32gg.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/Device/SiliconLabs/EFM32GG/Source/startup_efm32gg.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFM32GG/Source/startup_efm32gg.d
OBJS += $(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFM32GG/Source/startup_efm32gg.o

$(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFM32GG/Source/system_efm32gg.o: $(SDK_PATH)/platform/Device/SiliconLabs/EFM32GG/Source/system_efm32gg.c
	@echo 'Building $(SDK_PATH)/platform/Device/SiliconLabs/EFM32GG/Source/system_efm32gg.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/Device/SiliconLabs/EFM32GG/Source/system_efm32gg.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFM32GG/Source/system_efm32gg.d
OBJS += $(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFM32GG/Source/system_efm32gg.o

$(OUTPUT_DIR)/sdk/platform/driver/leddrv/src/sl_led.o: $(SDK_PATH)/platform/driver/leddrv/src/sl_led.c
	@echo 'Building $(SDK_PATH)/platform/driver/leddrv/src/sl_led.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/driver/leddrv/src/sl_led.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/driver/leddrv/src/sl_led.d
OBJS += $(OUTPUT_DIR)/sdk/platform/driver/leddrv/src/sl_led.o

$(OUTPUT_DIR)/sdk/platform/driver/leddrv/src/sl_simple_led.o: $(SDK_PATH)/platform/driver/leddrv/src/sl_simple_led.c
	@echo 'Building $(SDK_PATH)/platform/driver/leddrv/src/sl_simple_led.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/driver/leddrv/src/sl_simple_led.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/driver/leddrv/src/sl_simple_led.d
OBJS += $(OUTPUT_DIR)/sdk/platform/driver/leddrv/src/sl_simple_led.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_burtc.o: $(SDK_PATH)/platform/emlib/src/em_burtc.c
	@echo 'Building $(SDK_PATH)/platform/emlib/src/em_burtc.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/emlib/src/em_burtc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_burtc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_burtc.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_cmu.o: $(SDK_PATH)/platform/emlib/src/em_cmu.c
	@echo 'Building $(SDK_PATH)/platform/emlib/src/em_cmu.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/emlib/src/em_cmu.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_cmu.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_cmu.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_core.o: $(SDK_PATH)/platform/emlib/src/em_core.c
	@echo 'Building $(SDK_PATH)/platform/emlib/src/em_core.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/emlib/src/em_core.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_core.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_core.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_emu.o: $(SDK_PATH)/platform/emlib/src/em_emu.c
	@echo 'Building $(SDK_PATH)/platform/emlib/src/em_emu.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/emlib/src/em_emu.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_emu.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_emu.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_gpio.o: $(SDK_PATH)/platform/emlib/src/em_gpio.c
	@echo 'Building $(SDK_PATH)/platform/emlib/src/em_gpio.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/emlib/src/em_gpio.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_gpio.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_gpio.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_msc.o: $(SDK_PATH)/platform/emlib/src/em_msc.c
	@echo 'Building $(SDK_PATH)/platform/emlib/src/em_msc.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/emlib/src/em_msc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_msc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_msc.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_rtc.o: $(SDK_PATH)/platform/emlib/src/em_rtc.c
	@echo 'Building $(SDK_PATH)/platform/emlib/src/em_rtc.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/emlib/src/em_rtc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_rtc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_rtc.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_system.o: $(SDK_PATH)/platform/emlib/src/em_system.c
	@echo 'Building $(SDK_PATH)/platform/emlib/src/em_system.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/emlib/src/em_system.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_system.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_system.o

$(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_emu_s0.o: $(SDK_PATH)/platform/service/device_init/src/sl_device_init_emu_s0.c
	@echo 'Building $(SDK_PATH)/platform/service/device_init/src/sl_device_init_emu_s0.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/service/device_init/src/sl_device_init_emu_s0.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_emu_s0.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_emu_s0.o

$(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_hfxo_s0.o: $(SDK_PATH)/platform/service/device_init/src/sl_device_init_hfxo_s0.c
	@echo 'Building $(SDK_PATH)/platform/service/device_init/src/sl_device_init_hfxo_s0.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/service/device_init/src/sl_device_init_hfxo_s0.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_hfxo_s0.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_hfxo_s0.o

$(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_lfxo_s0.o: $(SDK_PATH)/platform/service/device_init/src/sl_device_init_lfxo_s0.c
	@echo 'Building $(SDK_PATH)/platform/service/device_init/src/sl_device_init_lfxo_s0.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/service/device_init/src/sl_device_init_lfxo_s0.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_lfxo_s0.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_lfxo_s0.o

$(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_nvic.o: $(SDK_PATH)/platform/service/device_init/src/sl_device_init_nvic.c
	@echo 'Building $(SDK_PATH)/platform/service/device_init/src/sl_device_init_nvic.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/service/device_init/src/sl_device_init_nvic.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_nvic.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_nvic.o

$(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer.o: $(SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer.c
	@echo 'Building $(SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer.o

$(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtc.o: $(SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtc.c
	@echo 'Building $(SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtc.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtc.o

$(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_init.o: $(SDK_PATH)/platform/service/system/src/sl_system_init.c
	@echo 'Building $(SDK_PATH)/platform/service/system/src/sl_system_init.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/service/system/src/sl_system_init.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_init.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_init.o

$(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_process_action.o: $(SDK_PATH)/platform/service/system/src/sl_system_process_action.c
	@echo 'Building $(SDK_PATH)/platform/service/system/src/sl_system_process_action.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(SDK_PATH)/platform/service/system/src/sl_system_process_action.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_process_action.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_process_action.o

$(OUTPUT_DIR)/project/app.o: app.c
	@echo 'Building app.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ app.c
CDEPS += $(OUTPUT_DIR)/project/app.d
OBJS += $(OUTPUT_DIR)/project/app.o

$(OUTPUT_DIR)/project/autogen/sl_board_default_init.o: autogen/sl_board_default_init.c
	@echo 'Building autogen/sl_board_default_init.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_board_default_init.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_board_default_init.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_board_default_init.o

$(OUTPUT_DIR)/project/autogen/sl_device_init_clocks.o: autogen/sl_device_init_clocks.c
	@echo 'Building autogen/sl_device_init_clocks.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_device_init_clocks.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_device_init_clocks.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_device_init_clocks.o

$(OUTPUT_DIR)/project/autogen/sl_event_handler.o: autogen/sl_event_handler.c
	@echo 'Building autogen/sl_event_handler.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_event_handler.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_event_handler.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_event_handler.o

$(OUTPUT_DIR)/project/autogen/sl_simple_led_instances.o: autogen/sl_simple_led_instances.c
	@echo 'Building autogen/sl_simple_led_instances.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_simple_led_instances.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_simple_led_instances.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_simple_led_instances.o

$(OUTPUT_DIR)/project/blink.o: blink.c
	@echo 'Building blink.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ blink.c
CDEPS += $(OUTPUT_DIR)/project/blink.d
OBJS += $(OUTPUT_DIR)/project/blink.o

$(OUTPUT_DIR)/project/main.o: main.c
	@echo 'Building main.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ main.c
CDEPS += $(OUTPUT_DIR)/project/main.d
OBJS += $(OUTPUT_DIR)/project/main.o

# Automatically-generated Simplicity Studio Metadata
# Please do not edit or delete these lines!
# SIMPLICITY_STUDIO_METADATA=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END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA