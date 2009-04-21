/*******************************************
  * Limitari:
  *  - Directoarele nu pot if mai mari de un cluster ( adica ~100 de intrari)
  *  - Nu poate if deschis mai mult de un director: intre 2 opendir trebuie closedir
  *  - Nu sunt suportate nume lungi
  *  - Nu sunt suportate caractere Japoneze in nume
 ********************************************/

#ifndef _DIR_H_
#define _DIR_H_

#define ATTR_DIR 0x10
#define ATTR_LONGNAME 0x0f

#include <stdint.h>


/***************************************
  * Structura ce reprezinta o intrare in director
  * 
  **************************************/
#define DELETED_ENTRY 0xe5
#define DOT_ENTRY 0x2e
#define LFN_ENTRY 0x0f

struct dirent{
	uint8_t name [8];
	uint8_t ext[3];
	uint8_t attr;
	uint8_t blah1[8];
	uint16_t cluster_high;
	uint8_t blah2[4];
	uint16_t  cluester_low;
	uint32_t size;
}__attribute__((packed));

/**********************************************
 * Structura corespunzatoare unui director/fisier deschis
 *
 **************************************/
struct DIR{
	// 8 x sector
	uint32_t cluster;
	// offsetul in cluster
	uint32_t offset;
};
typedef struct DIR open_file;


/**********************************************
 * Functii de acces la atributele intrarilor in sistemul de fisiere 
 **************************************/
#define DIR_VALID 0x00
#define DIR_INVALID 0x0f
#define DIR_END 0xff

// Functiile intorc:
// 0x00 - intrare valida
// 0x0f - intrare invalida
// 0xff - ultima intrare

uint8_t check( struct dirent * file);

void get_dirent_name(struct dirent * file , char * name_buf );

void is_dir( struct dirent * file, uint8_t * is);

void is_dot( struct dirent * file, uint8_t * is);

void get_dirent_cluster( struct dirent * file, uint32_t * cluster);

void get_dirent_size( struct dirent * file, uint32_t * size);



/**********************************************
 * Functiile standard de acces la directoare 
 *
 **************************************/
// obtine un prim pointer catre directorul radacina
struct DIR * openroot();
// deschide directorul reprezentat de intrarea 'entry' 
struct DIR * opendir ( struct dirent * entry );
// inchide ultimul director deschis
void closedir( );
// intoarce urmatoarea intrare in director
struct dirent * readdir ( struct DIR * dir, void * read_buffer);

// reincepe citirea unui director
void rewinddir ( struct DIR * dir);

// pozitioneaza pointerul de director pe pozitia 'offset'
void seekdir ( struct DIR * dir, uint32_t offset);




#endif

