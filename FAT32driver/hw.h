#ifndef __HW_H_
#define __HW_H_

#include <stdint.h>
#define SECTOR_SIZE 512

typedef uint32_t lba_t;
typedef uint32_t sector_t;

// Functie ce trebuie implementata de orice driver 
// folosit de sistemul de fisiere.
// Suportul fizic trebuie sa fie accesat la nivel de LBA
void hw_read( void * buf, lba_t lba);


#endif
