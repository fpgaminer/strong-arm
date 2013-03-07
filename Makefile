PROJ_NAME=libstrong-arm

SRCS = src/strong-arm.c \
       src/base58/base58.c \
       src/drbg/HMAC_DRBG.c \
       src/ecdsa/ecdsa.c \
       src/finite_field/ff_big.c \
       src/finite_field/finite_field.c \
       src/hmac/hmac.c \
       src/pbkdf2/pbkdf2.c \
       src/private/util.c \
       src/random/random.c \
       src/ripemd160/ripemd160.c \
       src/sha256/sha256.c \
       src/aes/aes256.c \
       src/utils.c \
       src/asn1/der.c


CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
AR=arm-none-eabi-ar

OBJDIR = build

CFLAGS  = -g -Wall -std=c99
CFLAGS += -mthumb -mcpu=cortex-m4
#CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
#CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -mfloat-abi=soft
# TODO: hard float was causing an exception; see what's up.

CFLAGS += -Istrong-arm -I. -Isrc/private -Ilibraries/CMSIS/ST/STM32F4xx/Include -Ilibraries/CMSIS/Include

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
