/************************************
  * Limitari:
  *  - Doar un fisier poate if deschis la un moment dat
  *	 - Fisierele mai mari decat un cluster sunt suportate
  ************************************/

#ifndef _FILE_H_
#define _FILE_H_

#include "dir.h"

struct file_pointer{
	uint32_t cluster;
	uint32_t offset;
};

void open_f(struct dirent * file);
void read_f( void * buf, uint32_t size, void * read_buffer);
void close_f();

/*************************************
 * Intoarce un pointer catre imaginea fisierului din memorie.
 * Limitare: Se mapeaza doar cate 512 bytes si offset e multiplu de 512.
 ****************************************/
// TODO:
void * mmap(struct dirent * file, uint32_t offset);
void munmap();

#endif

