
#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>


#define BYTE uint8_t

#ifdef TESTING
#include <unistd.h>
#include <stdio.h>

#define CHECK( ret_val, msg)\
	do{\
		if((ret_val) == -1){\
			printf(msg);\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

void print_hex( void * buf, size_t size, off_t off);
#else
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define CHECK(ret_val, msg)do{\
		if((ret_val) == -1){\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

#endif



#endif



