####################################################################
# User Makefile                                                    #
# This will not be overwritten. Edit as desired.                   #
####################################################################
.SUFFIXES:				# ignore builtin rules
.PHONY: all debug release clean

# Default goal
all: debug

####################################################################
# Definitions                                                      #
####################################################################

# Values set by the initial generation
PROJECTNAME = mcu

# Command output is hidden by default, it can be enabled by
# setting VERBOSE=true on the commandline.
ifeq ($(VERBOSE),)
  ECHO = @
endif

# Build Directories
BUILD_DIR = build
LST_DIR = lst

ifneq ($(filter $(MAKECMDGOALS),release),)
  OUTPUT_DIR = $(BUILD_DIR)/release
else
  OUTPUT_DIR = $(BUILD_DIR)/debug
endif

SDK_PATH = gecko_sdk

LIBS = \
 -Wl,--start-group \
 -lgcc \
 -lc \
 -lm \
 -lnosys \
 -Wl,--end-group

C_DEFS   = \
 '-DDEBUG_EFM=1' \
 '-DEFM32GG990F1024=1' \
 '-DSL_BOARD_NAME="BRD2200A"' \
 '-DSL_BOARD_REV="B05"'
ASM_DEFS = \
 '-DDEBUG_EFM=1' \
 '-DEFM32GG990F1024=1' \
 '-DSL_BOARD_NAME="BRD2200A"' \
 '-DSL_BOARD_REV="B05"'

INCLUDES = \
 -Iconfig \
 -Iautogen \
 -Iinc \
 -I$(SDK_PATH)/platform/Device/SiliconLabs/EFM32GG/Include \
 -I$(SDK_PATH)/platform/common/inc \
 -I$(SDK_PATH)/hardware/board/inc \
 -I$(SDK_PATH)/platform/CMSIS/Core/Include \
 -I$(SDK_PATH)/platform/service/device_init/inc \
 -I$(SDK_PATH)/platform/emdrv/common/inc \
 -I$(SDK_PATH)/platform/emdrv/dmadrv/inc \
 -I$(SDK_PATH)/platform/emdrv/spidrv/inc \
 -I$(SDK_PATH)/platform/emlib/inc \
 -I$(SDK_PATH)/platform/driver/leddrv/inc \
 -I$(SDK_PATH)/platform/common/toolchain/inc \
 -I$(SDK_PATH)/platform/service/system/inc \
 -I$(SDK_PATH)/platform/service/sleeptimer/inc

C_FLAGS           = \
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
C_FLAGS_DEBUG     = 
C_FLAGS_RELEASE   = 
ASM_FLAGS         = \
 -mcpu=cortex-m3 \
 -mthumb \
 -imacros sl_gcc_preinclude.h \
 -x assembler-with-cpp
ASM_FLAGS_DEBUG   = 
ASM_FLAGS_RELEASE = 
LD_FLAGS          = \
 -mcpu=cortex-m3 \
 -mthumb \
 -T"autogen/linkerfile.ld" \
 --specs=nano.specs \
 -Xlinker -Map=$(OUTPUT_DIR)/$(PROJECTNAME).map \
 -Wl,--gc-sections

OBJS = 

####################################################################
# Definitions of toolchain.                                        #
# You might need to do changes to match your system setup          #
####################################################################

AR      = "arm-none-eabi-gcc-ar"
CC      = "arm-none-eabi-gcc"
OBJCOPY = "arm-none-eabi-objcopy"
LD      = "arm-none-eabi-gcc"

####################################################################
# Rules                                                            #
####################################################################

# -MMD : Don't generate dependencies on system header files.
# -MP  : Add phony targets, useful when a h-file is removed from a project.
# -MF  : Specify a file to write the dependencies to.
DEPFLAGS = -MMD -MP -MF $(@:.o=.d)

C_SRC = \
 $(wildcard src/*.c) \
 $(wildcard autogen/*.c) \
 $(wildcard config/*.c)
GECKO_C_SRC = \
 $(SDK_PATH)/hardware/board/src/sl_board_control_gpio.c \
 $(SDK_PATH)/hardware/board/src/sl_board_init.c \
 $(wildcard $(SDK_PATH)/platform/emdrv/dmadrv/src/*.c) \
 $(SDK_PATH)/platform/emdrv/spidrv/src/spidrv.c \
 $(wildcard $(SDK_PATH)/platform/emlib/src/*.c) \
 $(SDK_PATH)/platform/common/src/sl_assert.c \
 $(SDK_PATH)/platform/common/toolchain/src/sl_memory.c \
 $(SDK_PATH)/platform/driver/leddrv/src/sl_simple_led.c \
 $(SDK_PATH)/platform/driver/leddrv/src/sl_led.c \
 $(SDK_PATH)/platform/service/device_init/src/sl_device_init_emu_s0.c \
 $(SDK_PATH)/platform/service/device_init/src/sl_device_init_hfxo_s0.c \
 $(SDK_PATH)/platform/service/device_init/src/sl_device_init_lfxo_s0.c \
 $(SDK_PATH)/platform/service/device_init/src/sl_device_init_nvic.c \
 $(SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer.c \
 $(SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtc.c \
 $(SDK_PATH)/platform/service/system/src/sl_system_init.c \
 $(SDK_PATH)/platform/service/system/src/sl_system_process_action.c \
 $(wildcard $(SDK_PATH)/platform/Device/SiliconLabs/EFM32GG/Source/*.c)
CSOURCES = $(C_SRC) $(GECKO_C_SRC)

#GECKO_A_SRC = $(SDK_PATH)/platform/Device/SiliconLabs/EFM32GG/Source/GCC/startup_efm32gg.S
ASMSOURCES  = #$(GECKO_A_SRC)

COBJS       = $(addprefix $(OUTPUT_DIR)/,$(CSOURCES:.c=.o))
ASMOBJS     = $(addprefix $(OUTPUT_DIR)/,$(ASMSOURCES:.S=.o))
OBJS        += $(COBJS) $(ASMOBJS)

CDEPS       += $(addprefix $(OUTPUT_DIR)/,$(CSOURCES:.c=.d))
ASMDEPS     += $(addprefix $(OUTPUT_DIR)/,$(ASMSOURCES:.S=.d))

#C_PATHS   = $(subst \,/,$(sort $(dir $(C_SOURCE_FILES))))
#ASM_PATHS = $(subst \,/,$(sort $(dir $(ASM_SOURCE_FILES))))

#vpath %.c $(C_PATHS)
#vpath %.s $(ASM_PATHS)
#vpath %.S $(ASM_PATHS)

override CFLAGS = $(C_FLAGS) $(C_DEFS) $(INCLUDES) $(DEPFLAGS)
override ASMFLAGS = $(ASM_FLAGS) $(ASM_DEFS) $(INCLUDES) $(DEPFLAGS)

# Rule Definitions
debug: C_FLAGS += $(C_FLAGS_DEBUG) 
debug: ASM_FLAGS += $(ASM_FLAGS_DEBUG)
debug: $(OUTPUT_DIR)/$(PROJECTNAME).out

release: C_FLAGS += $(C_FLAGS_RELEASE) 
release: ASM_FLAGS += $(ASM_FLAGS_RELEASE)
release: $(OUTPUT_DIR)/$(PROJECTNAME).out

# include auto-generated dependency files (explicit rules)
ifneq (clean,$(findstring clean, $(MAKECMDGOALS)))
-include $(CDEPS)
-include $(ASMDEPS)
endif

$(OUTPUT_DIR)/$(PROJECTNAME).out: $(OBJS) $(LIB_FILES)
	@echo 'Linking $(OUTPUT_DIR)/$(PROJECTNAME).out'
	@echo $(OBJS) > $(OUTPUT_DIR)/linker_objs
	$(ECHO)$(LD) $(LD_FLAGS) @$(OUTPUT_DIR)/linker_objs $(LIBS) -o $(OUTPUT_DIR)/$(PROJECTNAME).out
	$(ECHO)$(OBJCOPY) $(OUTPUT_DIR)/$(PROJECTNAME).out -O binary $(OUTPUT_DIR)/$(PROJECTNAME).bin
	$(ECHO)$(OBJCOPY) $(OUTPUT_DIR)/$(PROJECTNAME).out -O ihex $(OUTPUT_DIR)/$(PROJECTNAME).hex
	$(ECHO)$(OBJCOPY) $(OUTPUT_DIR)/$(PROJECTNAME).out -O srec $(OUTPUT_DIR)/$(PROJECTNAME).s37
	@echo 'Done.'

$(OBJS):

$(OUTPUT_DIR)/%.o: %.c
	@echo 'Building $<'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.o: %.S
	@echo 'Building $<'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(ASMFLAGS) -c -o $@ $<

clean:
	$(RM) -rf $(BUILD_DIR)
