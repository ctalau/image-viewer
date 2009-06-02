#TARGET=linux
TARGET=avr
BIN=fat
HEX=fat.hex
OBJS=main.o fat.o utils.o hw.o partitions.o volid.o dir.o file.o
CFLAGS=-Wall -Os -std=gnu99

ifeq ($(TARGET),linux)
	CFLAGS+= -DTESTING
	CC=gcc
else
	CFLAGS+= -mmcu=atmega16 -D__AVR_ATmega16__  -D_LOW_MEM_ -I. -I../BMPlib/ -I../SDdriver/ -I../IOdriver/ -I../LCDdriver -Wl,-defsym=__stack=0x80045F
	CC=avr-gcc
	OBJS+=../BMPlib/draw_bmp.o ../IOdriver/uart.o ../SDdriver/mmc.o ../IOdriver/button.o ../LCDdriver/lcd.o
	LDFLAGS= -Wl,-M,fat.map
endif


all: pre-prog

build: $(BIN)

pre-prog: build
	avr-objcopy -j .text -j .data  -O ihex $(BIN) $(HEX)
	avr-size $(BIN)

prog: pre-prog
	avrboot fat.hex

fat: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@



clean:
	rm -Rf $(BIN) $(HEX) *.map *.o *~
