TARGET=linux
BIN=mmc
HEX=mmc.hex
OBJS=mmc.o ../IOdriver/uart.o
INCLUDE=../IOdriver/
MCU=atmega16
CFLAGS=-Wall -Os -std=gnu99 -mmcu=$(MCU)   -Wl,-defsym=__stack=0x80045F  -D__AVR_ATmega16__ -D_MMC_TEST_ -I$(INCLUDE)
LDFLAGS=-Wl,-Map,mmc.map
#-mtiny-stack
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
	rm -Rf $(BIN) $(HEX) *.S *.o *~ *.map
