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

unsigned char  buffer[SECTOR_SIZE];
lba_t cached_sector = 0xffffffff;



/**
  * Avanseaza in fisier/director cu o anumita distanta
  */
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


