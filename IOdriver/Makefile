TARGET=linux
BIN=uart
HEX=uart.hex
OBJS=uart.o button.o
CFLAGS=-Wall -mmcu=atmega16  -Os  -D__AVR_ATmega16__ -D_USART_TEST_
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
	rm -Rf $(BIN) $(HEX) *.o *~
