#ifndef _FAT_H_
#define _FAT_H_

#include <stdint.h>
#include "hw.h"
#include "dir.h"
#include "file.h"


#define CLUSTER_SIZE 512

typedef uint32_t cluster_t;


// Fiecare functie care pune ceva in buffer si considera ca nu va if modificat de useri
//  scrie in aceasta variabila ce a pus in buffer - util pentru readdir si read
// Daca datele vor if modificate => 0xffffffff; 
extern lba_t cached_sector;

void  advance_pointer(struct file_pointer * dir, uint32_t amount, void * read_buffer);


#endif
