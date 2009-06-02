#include <assert.h>
#include <stdio.h>
#include "file.h"
#include "utils.h"
#include "hw.h"
#include "volid.h"
#include "fat.h"



struct file_pointer the_file;




void open_f(struct dirent * file){
	get_dirent_cluster(file, &(the_file.cluster));
	the_file.offset = 0;
}
sector_t get_sector_file( struct file_pointer * dir){
	return
		cluster_begin_lba +
		(dir -> cluster-2) * sectors_per_cluster +
		dir -> offset / SECTOR_SIZE;
}

#define MIN(a,b)  ((a<b)? (a):(b))

// copiaza in bufferul userului
//toata citirea e din acelasi sector
void __read_f( void * buf, uint32_t size, void * read_buffer){
	sector_t sector = get_sector_file( &the_file);
	uint32_t sect_offset = the_file.offset % SECTOR_SIZE;
	uint32_t cnt;
	uint8_t * src = read_buffer;
	uint8_t * dst = buf;

	if(cached_sector != sector){
		//fprintf(stderr,"WARNING: Getting new sector: %d\n", sector);
		hw_read(read_buffer, sector);
		cached_sector = sector;
	}

	for(cnt= 0; cnt < size; cnt++){
		dst[cnt] = src[sect_offset + cnt];
	}
	advance_pointer( &the_file, size, read_buffer);
	//the_file.offset+=size;
}

void read_f( void * buf, uint32_t size, void * read_buffer){
	int32_t remain = SECTOR_SIZE - (the_file.offset % SECTOR_SIZE);
	uint8_t * dst = buf;

	if(remain < size){
		__read_f( dst, remain, read_buffer);
		size -=remain;
		dst += remain;
	}
	while(size > 0){
		uint32_t to_read = MIN( size, SECTOR_SIZE);
		__read_f( dst, to_read , read_buffer);
		size -= to_read;
		dst += to_read;
	}
}

void close_f(){
	the_file.offset = 0xffffffff;
}

//TODO mmap/munmap

// in principiu tree doar apelat advance pointer

#undef CHECK_LIBER
