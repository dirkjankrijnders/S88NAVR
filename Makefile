MCU=atmega48

CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-g -Os -Wall -mmcu=$(MCU)

PROJ=S88NAtmega328
SRCS=S88NAtmega328.c
OBJS=S88NAtmega328.c.o
HEX=S88NAtmega328.hex

%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.s: .c
	$(CC) $(CFLAGS) -S $< -o $@

$(PROJ).out: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) 

$(PROJ).hex: $(PROJ).out
	$(OBJCOPY) -O ihex $< $@

all: $(PROJ).hex
