#include "hw.h"
#include "utils.h"


#ifdef TESTING
#include <fcntl.h>
#include <unistd.h>
void hw_read(void * buf, lba_t lba){
	int disk = open("/dev/hdb", O_RDONLY);
	int bytes_read = 0;
	int bytes_read_lt = 0;

	lseek( disk, lba * SECTOR_SIZE, SEEK_SET);
	while( bytes_read < SECTOR_SIZE){
		bytes_read_lt = read( disk, buf, SECTOR_SIZE-bytes_read);
		bytes_read += bytes_read_lt;
		if( bytes_read_lt == 0)
			break;
	}

	close(disk);
	return;

}
#else
void hw_read(void * buf, lba_t lba){
		
}
#endif
