TARGET=linux
BINS=fat
OBJS=utils.o hw.o partitions.o volid.o dir.o file.o
CFLAGS=-Wall -g

ifeq ($(TARGET),linux)
	CFLAGS+= -DTESTING
	CC=gcc
else
	CC=avr-gcc
endif


all: build

build: $(BINS)


fat: $(OBJS)



clean:
	rm -Rf $(BINS) *.o *~
