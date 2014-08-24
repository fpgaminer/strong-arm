# Inspired by (https://github.com/mbcrawfo/GenericMakefile)
BIN_NAME := main

C_SOURCES = \
       src/main.c \
       src/aes.c \
       src/base58.c \
       src/drbg.c \
       src/ecdsa.c \
       src/finite_field.c \
       src/hmac.c \
       src/pbkdf2.c \
       src/random.c \
       src/ripemd160.c \
       src/sha256.c \
       src/keychain.c \
       src/threefish.c

SRC_EXT = c
SRC_PATH = src
COMPILE_FLAGS = -std=c99 -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual
COMPILE_FLAGS += -Wno-missing-braces
#COMPILE_FLAGS = -Wconversion -Wsign-conversion
RCOMPILE_FLAGS = -O3
DCOMPILE_FLAGS = -g
INCLUDES = -I../include -Isrc
LINK_FLAGS = -lstrong-arm
RLINK_FLAGS = -O3
DLINK_FLAGS = -g


# Target
TARGET ?= linux

# Build and output paths
RBUILD_PATH = build/$(TARGET)/release
DBUILD_PATH = build/$(TARGET)/debug

ifeq ($(TARGET),linux)
	CC = gcc
	OBJCOPY = objcopy
	AR = ar

	DLINK_FLAGS += -L../build/linux/debug/
	RLINK_FLAGS += -L../build/linux/release/
else ifeq ($(TARGET),cygwin_mingw)
	CC=i686-pc-mingw32-gcc
	OBJCOPY=i686-pc-mingw32-objcopy
	AR=i686-pc-mingw32-ar

	DLINK_FLAGS += -L../build/cygwin_mingw/debug/
	RLINK_FLAGS += -L../build/cygwin_mingw/release/
else ifeq ($(TARGET),stm32f4)
	CC = arm-none-eabi-gcc
	OBJCOPY = arm-none-eabi-objcopy
	AR = arm-none-eabi-ar

	COMPILE_FLAGS += -mthumb -mcpu=cortex-m4
	# TODO: hard float was causing an exception; see what's up.
	#COMPILE_FLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
	#COMPILE_FLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
	COMPILE_FLAGS += -mfloat-abi=soft

	C_SOURCES += src/system_stm32f4xx.c
	C_SOURCES += src/startup_stm32f4xx.s

	INCLUDES += -I../libraries/STM32F4xx_StdPeriph_Driver/inc -I../libraries/CMSIS/ST/STM32F4xx/Include -I../libraries/CMSIS/Include

	LINK_FLAGS += -Tstm32f4_flash.ld
	LINK_FLAGS += -mthumb -mcpu=cortex-m0
	LINK_FLAGS += -mfloat-abi=soft
	# Semihosting while debugging
	DLINK_FLAGS += --specs=rdimon.specs -u _printf_float
	RLINK_FLAGS += --specs=nosys.specs

	DLINK_FLAGS += -Wl,-Map,$(DBUILD_PATH)/$(BIN_NAME).map
	RLINK_FLAGS += -Wl,-Map,$(RBUILD_PATH)/$(BIN_NAME).map

	DLINK_FLAGS += -L../build/stm32f4/debug/
	RLINK_FLAGS += -L../build/stm32f4/release/
else
$(error "TARGET must be set, e.g. make TARGET=linux")
endif


# Verbose option, to output compile and link commands
export V = false
export CMD_PREFIX = @
ifeq ($(V),true)
	CMD_PREFIX =
endif

# Combine compiler and linker flags
RCCFLAGS = $(CCFLAGS) $(COMPILE_FLAGS) $(RCOMPILE_FLAGS)
RLDFLAGS = $(LDFLAGS) $(LINK_FLAGS) $(RLINK_FLAGS)
DCCFLAGS = $(CCFLAGS) $(COMPILE_FLAGS) $(DCOMPILE_FLAGS)
DLDFLAGS = $(LDFLAGS) $(LINK_FLAGS) $(DLINK_FLAGS)

# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
DOBJECTS := $(C_SOURCES:%.c=$(DBUILD_PATH)/%.o)
DOBJECTS := $(DOBJECTS:%.s=$(DBUILD_PATH)/%.o)
ROBJECTS := $(C_SOURCES:%.c=$(RBUILD_PATH)/%.o)
ROBJECTS := $(ROBJECTS:%.s=$(RBUILD_PATH)/%.o)

# Set the dependency files that will be used to add header dependencies
DDEPS = $(DOBJECTS:.o=.d)
RDEPS = $(ROBJECTS:.o=.d)

# Main rule
all: dirs $(DBUILD_PATH)/$(BIN_NAME) $(RBUILD_PATH)/$(BIN_NAME)

# Create the directories used in the build
.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(DOBJECTS))
	@mkdir -p $(dir $(ROBJECTS))

# Link the executable
$(DBUILD_PATH)/$(BIN_NAME): $(DOBJECTS)
	@echo "Linking: $@"
	$(CMD_PREFIX)$(CC) $(DOBJECTS) $(DLDFLAGS) -o $@

$(RBUILD_PATH)/$(BIN_NAME): $(ROBJECTS)
	@echo "Linking: $@"
	$(CMD_PREFIX)$(CC) $(ROBJECTS) $(RLDFLAGS) -o $@

# Add dependency files, if they exist
-include $(DDEPS)
-include $(RDEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(DBUILD_PATH)/%.o: %.c
	@echo "Compiling: $< -> $@"
	$(eval BUILD_PATH := $(DBUILD_PATH))
	$(CMD_PREFIX)$(CC) $(DCCFLAGS) $(INCLUDES) -I$(DBUILD_PATH) -MP -MMD -c $< -o $@

$(DBUILD_PATH)/%.o: %.s
	@echo "Compiling: $< -> $@"
	$(eval BUILD_PATH := $(DBUILD_PATH))
	$(CMD_PREFIX)$(CC) $(DCCFLAGS) $(INCLUDES) -I$(DBUILD_PATH) -MP -MMD -c $< -o $@

$(RBUILD_PATH)/%.o: %.c
	@echo "Compiling: $< -> $@"
	$(eval BUILD_PATH := $(RBUILD_PATH))
	$(CMD_PREFIX)$(CC) $(RCCFLAGS) $(INCLUDES) -I$(RBUILD_PATH) -MP -MMD -c $< -o $@

$(RBUILD_PATH)/%.o: %.s
	@echo "Compiling: $< -> $@"
	$(eval BUILD_PATH := $(RBUILD_PATH))
	$(CMD_PREFIX)$(CC) $(RCCFLAGS) $(INCLUDES) -I$(RBUILD_PATH) -MP -MMD -c $< -o $@



.PHONE: program
program: $(OBJDIR)/$(PROJ_NAME).elf
	openocd-0.6.1 -f program.cfg

.PHONE: flash-and-debug
flash-and-debug: $(OBJDIR)/$(PROJ_NAME).elf
	$(DB) --command=gdb/stm32f4.script $^

.PHONE: clean
clean:
	@echo "Deleting directories"
	@$(RM) -r build
