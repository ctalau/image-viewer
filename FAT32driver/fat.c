#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


#include "utils.h"
#include "partitions.h"
#include "volid.h"
#include "fat.h"
#include "dir.h"
#include "file.h"

void * buffer;
lba_t cached_sector = 0xffffffff;


#ifdef TESTING
#include <unistd.h>
#include <fcntl.h>
void printdirs(struct DIR * pre_dir, int count){
	char namebuf [12];
	char data [100000];
	uint32_t cluster, size;
	int i;
	uint8_t ret_code, is=0;
	struct dirent * crt_dir;
	struct DIR * dir;

	do{
		crt_dir = readdir( pre_dir, buffer);
		ret_code = check (crt_dir);
		get_dirent_name(crt_dir, namebuf);
		get_dirent_cluster( crt_dir, &cluster);
		get_dirent_size(crt_dir, &size);

		if(ret_code == DIR_INVALID || ret_code == DIR_END)
			continue;

		for(i=0; i < count; i++)
			printf(" ");
		printf("File: %s \tcl:%d \tsz:%d\n", namebuf, cluster, size);

		is_dir(crt_dir, &is);
		if( is){
			if(count < 2){
				dir = opendir(crt_dir);
				printf("DIRECTOR\n");
				printdirs(dir, count+2);
			}
		}else{
			open_f(crt_dir);
			read_f( data, size, buffer);
			close_f();
			data[size] = 0;
			for(i=0; i < size; i++)
				putc(data[i],stdout);
			printf("\n");
		}
	}while( ret_code != DIR_END);
	exit(EXIT_SUCCESS);
}
#endif


int main(){
	buffer = malloc(SECTOR_SIZE);

	init_partition(buffer);
	//printf("LBA start: %u\n", partition_lba_start);

	initFAT32(buffer);
	//print_volid(buffer);

	//printdirs(openroot(),0); 
	return EXIT_SUCCESS;
}

// Avanseaza in fisier/director cu o anumita distanta
#define ENTRIES_PER_FAT_SECTOR  (SECTOR_SIZE/sizeof(uint32_t))
void advance_pointer( struct file_pointer * dir, uint32_t amount, void * read_buffer){
	uint32_t cluster = dir -> cluster;
	uint32_t * buffer = read_buffer;
	dir -> offset += amount;

	while( (dir -> offset) >= (sectors_per_cluster * SECTOR_SIZE)){
		uint32_t fat_sector = cluster / ENTRIES_PER_FAT_SECTOR;
		dir -> offset -= sectors_per_cluster * SECTOR_SIZE;

		// suprascriem sectorul care era inainte in cache
		cached_sector = 0xffffffff;
		hw_read( read_buffer, fat_begin_lba + fat_sector);

		dir -> cluster = buffer[ cluster % ENTRIES_PER_FAT_SECTOR];
		assert( dir->cluster < 0xfffffff8);
	}
}

