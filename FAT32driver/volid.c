#include "partitions.h"
#include "volid.h"
#include "fat.h"
#include "hw.h"
#include <string.h>
#include <stdlib.h>


uint32_t fat_begin_lba;
uint32_t cluster_begin_lba;
uint8_t  sectors_per_cluster;
uint32_t root_dir_first_cluster;


#ifndef _LOW_MEM_

#define ALIIGNED_SIZE 32
static void print_alligned(const char * str1, int nmbr){
	uint8_t i;
	printf("\t%s",str1);
	for(i=0; i < ALIIGNED_SIZE - strlen(str1); i++){
		printf(".");
	}
	printf("%4d\n", nmbr);
}

void print_volid( void * read_buffer){
	struct volid * vol;
	hw_read(read_buffer, partition_lba_start);
	vol = read_buffer;


	print_alligned("Bytes per sector:", vol->bytes_per_sect);
	print_alligned("Sectors per cluster:",vol->sectors_per_cluster);
	print_alligned("Reserved Sectors:", vol->reserved_sectors);
	print_alligned("No. of Fats", vol->no_of_fats);
	print_alligned("Sectors per fat", vol->sectors_per_fat);
	print_alligned("Root's first cluster", vol->root_first_cluster);

	printf("Signature: %2x%2x ...", vol->signature[0], vol->signature[1]);
	if( vol->signature[0] == 0x55 && vol->signature[1] == 0xaa){
		printf("  OK.\n");
	}else{
		printf(" FAILED.\n");
		exit(EXIT_FAILURE);
	}

	return;
}
#else

void print_volid( void * read_buffer){
}
#endif


// Afla parametrii sistemului de fisiere
// @pre: trebuie apelata functia init_partitions
void initFAT32(void * read_buffer ){
	struct volid * vol;
	hw_read(read_buffer, partition_lba_start);
	vol = read_buffer;

	fat_begin_lba = partition_lba_start + vol->reserved_sectors;
	cluster_begin_lba =
		partition_lba_start +
		vol -> reserved_sectors +
		( vol -> no_of_fats * vol -> sectors_per_fat);

	sectors_per_cluster = vol -> sectors_per_cluster;
	root_dir_first_cluster = vol -> root_first_cluster;
	return;
}


