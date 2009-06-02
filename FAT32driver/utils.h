
#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>
#include <stdio.h>

#define BYTE uint8_t


#ifdef TESTING

#include <unistd.h>
// macro folosit de AVR
#define PSTR(x)   (x)


#define CHECK( ret_val, msg)\
	do{\
		if((ret_val) == -1){\
			printf(msg);\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

#else


#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// typedef uint32_t off_t;
// typedef uint32_t size_t;
#include <avr/pgmspace.h>

#define CHECK(ret_val, msg)do{\
		if((ret_val) == -1){\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

#endif


void print_hex( void * buf, uint32_t size, uint32_t off);

#endif



