#ifndef _VOLID_H_
#define _VOLID_H_


// Parametrii file-sistemului exportati de acest modul
extern uint32_t fat_begin_lba;
extern uint32_t cluster_begin_lba;
extern uint8_t  sectors_per_cluster;
extern uint32_t root_dir_first_cluster;

// volume id sector
struct volid{
	uint8_t blah1 [11];
	uint16_t bytes_per_sect;
	uint8_t sectors_per_cluster;
	uint16_t reserved_sectors;
	uint8_t no_of_fats;
	uint8_t blah2 [19];
	uint32_t sectors_per_fat;
	uint8_t blah3 [4];
	uint32_t  root_first_cluster;
	uint8_t blah4 [462];
	// FIXME: de verificat semnatura 0x55aa
	uint8_t signature[2];
}__attribute__((packed));

#ifdef TESTING
void print_volid( void * read_buffer);
#endif
void initFAT32( void  * read_buffer);


#endif
