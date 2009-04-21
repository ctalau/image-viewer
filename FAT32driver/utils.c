#include "hw.h"
#include "utils.h"
#include "volid.h"
#include <ctype.h>
#include <stdint.h>

#ifdef TESTING
#include <sys/types.h>
#include <fcntl.h>
void print_hex( void * buf, size_t size, off_t off){
	int i,j;
	char * mybuf = ((char *) buf) + off;

	printf("        ");
	for( j=0; j < 16 ; j++){
		printf("%02d ", j);
	}
	printf("\n");

	for( i = 0; i < size ; i+=16){
		printf("%4d:   ", i);
		for( j=0; j < 16 && i+j < size ; j++){
			uint8_t ch = *(mybuf + i + j);
			printf("%02x ", ch);
		}
		
		printf("  ");
		for( j=0; j < 16 && i+j < size ; j++){
			uint8_t ch = *(mybuf + i + j);
			if(isprint(ch)){
				printf("%c", (char)ch);
			}else{
				printf(".");
			}
		}
		printf("\n");
	}

}
#endif



