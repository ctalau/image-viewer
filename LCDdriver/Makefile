TARGET=linux
BIN=lcd
HEX=lcd.hex
OBJS=lcd.o
CFLAGS=-Wall -g -Os -std=gnu99  -I../IOdriver/ -D__AVR_ATmega16__ -D_TEST_LCD_
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
	rm -Rf $(BIN) $(HEX) *.map *.o *~
