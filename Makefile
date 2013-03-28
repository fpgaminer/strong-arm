PROJ_NAME=libstrong-arm
OBJDIR = build

SRCS = src/strong-arm.c \
       src/base58/base58.c \
       src/drbg/HMAC_DRBG.c \
       src/ecdsa/ecdsa.c \
       src/finite_field/ff_big.c \
       src/finite_field/finite_field.c \
       src/hmac/hmac.c \
       src/pbkdf2/pbkdf2.c \
       src/private/util.c \
       src/ripemd160/ripemd160.c \
       src/sha256/sha256.c \
       src/aes/aes256.c \
       src/utils.c \
       src/asn1/der.c


CFLAGS = -g -Wall -std=c99

ifdef CYGWIN_MINGW
	CC=i686-pc-mingw32-gcc
	OBJCOPY=i686-pc-mingw32-objcopy
	AR=i686-pc-mingw32-ar
else
	# ARM Cortex M4 (STM32F4)
	CC=arm-none-eabi-gcc
	OBJCOPY=arm-none-eabi-objcopy
	AR=arm-none-eabi-ar

	CFLAGS += -mthumb -mcpu=cortex-m4
	#CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
	#CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
	CFLAGS += -mfloat-abi=soft
	# TODO: hard float was causing an exception; see what's up.
	CFLAGS += -O3

	TARGET_STM32F4 = 1
endif


CFLAGS += -Istrong-arm -I. -Isrc/private

ifdef TARGET_STM32F4
	SRCS += src/random/random_stm32f4.c
	CFLAGS += -Ilibraries/CMSIS/ST/STM32F4xx/Include -Ilibraries/CMSIS/Include
else
	SRCS += src/random/random_win32.c
endif


OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)
OBJS := $(patsubst src%.o,build%.o, $(OBJS))


all: $(OBJDIR)/$(PROJ_NAME).a

$(OBJDIR)/%.a: $(OBJS)
	$(AR) rcs $@ $^

$(OBJDIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: src/%.s
	$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR):
	mkdir -p $@

clean:
	rm -f $(OBJDIR)/$(PROJ_NAME).a
	find $(OBJDIR) -type f -name '*.o' -print0 | xargs -0 -r rm


# Dependdencies
$(OBJDIR)/$(PROJ_NAME).elf: $(OBJS) | $(OBJDIR)
