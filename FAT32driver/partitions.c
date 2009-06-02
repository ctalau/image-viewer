
#include "partitions.h"
#include "utils.h"
#include "hw.h"

uint32_t partition_lba_start;


// Macro pentru verificarea tipului unei partitii
#define  CHECK_PARTITION(part) ( (part->type) == TYPE_1 || (part->type) == TYPE_2)


void init_partition(void * read_buffer){
	struct partition_t * part;
	struct MBR * mbr_buf = read_buffer;
	uint32_t i;

	// Citim MBR-ul de pe disc
	hw_read( read_buffer, MBR_LBA);
	part =  (struct partition_t * ) &(mbr_buf -> part1);


	for(i=0; i< 4; i++){
		if( CHECK_PARTITION(part)){
			partition_lba_start = part -> lba_begin;
			return;
		}
		part++;
	}
}

