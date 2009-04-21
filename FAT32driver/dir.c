#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dir.h"
#include "fat.h"
#include "hw.h"
#include "volid.h"

uint8_t check( struct dirent * file){
	if(file->attr == LFN_ENTRY ||  // intrare LFN
			file->name[0] == DELETED_ENTRY // sau fisier sters
			){
		return DIR_INVALID;
	}
	if( file -> name[0] == 0x00 )
		return DIR_END;
	return DIR_VALID;

}

void get_dirent_name(struct dirent * file , char * name_buf ){
	uint8_t *p = (uint8_t *) &(file -> name) ;
	int i;
	// NU suportam caractere Japoneze cu 0xE5

	for(i=0; i < 8; i++){
		if( *(p+i) == 0x20){
			break;
		}
		*(name_buf++) = *(p+i);
	}
	if( (file -> attr & ATTR_DIR)){
		*name_buf++ = 0;
	}else{
		*name_buf++ = '.';
	}
	// extensia
	for(i=8; i < 11; i++){
		*name_buf++ = *(p+i);
	}
	return;
}

void is_dir( struct dirent * file, uint8_t * is){
	*is = ((file -> attr & ATTR_DIR)); 
	return;
}

void is_dot( struct dirent * file, uint8_t * is){
	*is = ((file -> name[0] == DOT_ENTRY)); 
	return;
}
void get_dirent_cluster( struct dirent * file, uint32_t * cluster){
	uint32_t high = file -> cluster_high;
	// FIXME nush daca AVR poate face << pe 16 bytes
	*cluster = (high << 16) + file -> cluester_low;
	return;
}



void get_dirent_size( struct dirent * file, uint32_t * size){
	*size = file->size;
	return;
}

/*********************************************
  * Structura de director intoarsa
  * nu se poate deschide mai mult de un fisier pana nu este inchis precedentul
  ***********************************************/
struct DIR global;
#define CHECK_LIBER()  assert(global.offset == 0xffffffff)

struct DIR * openroot(){
	global.cluster =2;
	global.offset = 0;
	return &global;
}

// deschide directorul reprezentat de intrarea 'entry' 
struct DIR    * opendir ( struct dirent * entry ){
	get_dirent_cluster( entry, &(global.cluster));
	global.offset = 0;
	return &global; 
}

// inchide ultimul director deschis
void closedir( ){
	global.offset = 0xffffffff;
}

sector_t get_sector( struct DIR * dir){
	return 
		cluster_begin_lba +
		(dir -> cluster-2) * sectors_per_cluster +
		dir -> offset / SECTOR_SIZE;
}

// intoarce urmatoarea intrare in director
struct dirent * readdir ( struct DIR * dir, void * read_buffer){
	sector_t sector = get_sector( dir);
	uint32_t  sect_offset = (dir->offset % SECTOR_SIZE);
	uint8_t * buffer = read_buffer;

	if(cached_sector != sector){
		//printf("Getting new sector: %d\n", sector);
		hw_read(read_buffer, sector);
		cached_sector = sector;
	}
	dir -> offset += sizeof(struct dirent);
	assert(dir -> offset < sectors_per_cluster * SECTOR_SIZE); 
	//advance_pointer( dir, sizeof(struct dirent), read_buffer);

	return (struct dirent *)( buffer + sect_offset);
}

void rewinddir( struct DIR * dir){
	dir -> offset = 0;
}

void seekdir(struct DIR * dir, uint32_t offset){
	dir-> offset = sizeof(struct dirent) * offset;
}



#undef CHECK_LIBER
