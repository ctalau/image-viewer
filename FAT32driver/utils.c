#include "hw.h"
#include "utils.h"
#include "volid.h"
#include <ctype.h>
#include <stdint.h>


#ifndef _LOW_MEM_
void print_hex( void * buf, uint32_t size, uint32_t off){
	uint32_t i,j;
	char * mybuf = ((char *) buf) + off;

	printf("        ");
	for( j=0; j < 16 ; j++){
		printf(PSTR("%2d "),(int) j);
	}
	printf(PSTR("\n"));

	for( i = 0; i < size ; i+=16){
		printf("%4x:   ", (int)i);
		for( j=0; j < 16 && i+j < size ; j++){
			uint8_t ch = *(mybuf + i + j);
			if(ch >0x0f)
				printf("%x ", ch);
			else
				printf("%x  ",ch);
		}

		printf("  ");
		for( j=0; j < 16 && i+j < size ; j++){
			uint8_t ch = *(mybuf + i + j);
			if(isprint(ch)){
				printf("%c", (char)ch);
			}else{
				printf(PSTR("."));
			}
		}
		printf(PSTR("\n"));
	}

}
#else
void print_hex( void * buf, uint32_t size, uint32_t off){}
#endif


