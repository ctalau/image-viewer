TARGET=linux
BIN=draw
HEX=draw.hex
OBJS=draw_bmp.o
CFLAGS=-Wall -g -Os -std=gnu99  -I../LCD_driver/ -I../FAT32_driver/ -D__AVR_ATmega16__ -D_TEST_LCD_
CC=avr-gcc


all: pre-prog

build: $(BIN)

pre-prog: build
	avr-objcopy -j .text -j .data  -O ihex $(BIN) $(HEX)
	avr-size $(BIN)

prog: pre-prog
	avrboot $(HEX)

$(BIN): $(OBJS)



clean:
	rm -Rf $(BIN) $(HEX) create *.bmp *.map *.o *~


create: create_bmp.c
	gcc create_bmp.c -o create

