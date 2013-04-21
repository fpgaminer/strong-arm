PROJ_NAME=main

SRCS = src/main.c \
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
       src/key_chain.c


CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

OBJDIR = build

CFLAGS  = -g -Wall -Wno-missing-braces -std=c99
CFLAGS += -I../include -Isrc
LDFLAGS = -g -L../build -lstrong-arm

ifdef CYGWIN_MINGW
	CC=i686-pc-mingw32-gcc
	OBJCOPY=i686-pc-mingw32-objcopy
	AR=i686-pc-mingw32-ar
else
	CFLAGS += -mthumb -mcpu=cortex-m4
	#CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
	#CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
	CFLAGS += -mfloat-abi=soft
	# TODO: hard float was causing an exception; see what's up.
	
	SRCS += src/system_stm32f4xx.c \
	        src/startup_stm32f4xx.s

	CFLAGS += -I../libraries/STM32F4xx_StdPeriph_Driver/inc -I../libraries/CMSIS/ST/STM32F4xx/Include -I../libraries/CMSIS/Include
	LDFLAGS += -Wl,-Map,$(OBJDIR)/$(PROJ_NAME).map -Tstm32f4_flash.ld
	LDFLAGS += --specs=nano.specs --specs=rdimon.specs -lc -lc -lrdimon
endif


OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)
OBJS := $(addprefix $(OBJDIR)/,$(OBJS))


all: $(OBJDIR)/$(PROJ_NAME).elf $(OBJDIR)/$(PROJ_NAME).hex $(OBJDIR)/$(PROJ_NAME).bin

$(OBJDIR)/%.elf: $(OBJS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $^ $@

%.bin: %.elf
	$(OBJCOPY) -O binary $^ $@

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.s
	$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR):
	mkdir -p $@

clean:
	find $(OBJDIR) -type f -name '*.o' -print0 | xargs -0 -r rm
	rm -f $(OBJDIR)/$(PROJ_NAME).elf
	rm -f $(OBJDIR)/$(PROJ_NAME).hex
	rm -f $(OBJDIR)/$(PROJ_NAME).bin
	rm -f $(OBJDIR)/$(PROJ_NAME).map


program: $(OBJDIR)/$(PROJ_NAME).elf
	openocd-0.6.1 -f program.cfg


# Dependdencies
$(OBJDIR)/$(PROJ_NAME).elf: $(OBJS) ../build/libstrong-arm.a | $(OBJDIR)
