#ifndef _PARTITIONS_H_
#define _PARTITIONS_H_


#include "utils.h"

/*********************************************
  * Scopul acestui modul e de a calcula LBA-ul de start al partitiei
  * ce contine sistemul de fisiere
  *******************************************/
extern uint32_t partition_lba_start;
#define MBR_LBA 0



#define BOOT_CODE_SIZE 446
#define PARTITION_ENTRY_SIZE 16
#define BOOT_FLAG_SIZE 2
// Structura MBR-ului aflat pe cardul SD
struct MBR{
	BYTE code [BOOT_CODE_SIZE];
	BYTE part1 [PARTITION_ENTRY_SIZE];
	BYTE part2 [PARTITION_ENTRY_SIZE];
	BYTE part3 [PARTITION_ENTRY_SIZE];
	BYTE part4 [PARTITION_ENTRY_SIZE];
	BYTE boot  [BOOT_FLAG_SIZE];
} __attribute__ ((packed));




// Offset-ul campului 'TYPE' in partitie
#define TYPE_OFF 5
// Tipurile de partitie pentru FAT32
#define TYPE_1 0x0c
#define TYPE_2 0x0b
// offsetul campului de start al partitiei
#define START_LBA_OFF 8

/**
  * Structura unei intrari de partitie in MBR
  */ 
struct partition_t{
	BYTE boot_flag;
	// Adresa de start a partitiei in modul CHS
	BYTE C_begin ;
	BYTE H_begin ;
	BYTE S_begin ;
	// Tipul partitiei
	BYTE type;
	// Adresa de sfarsit a partitiei in modul CHS
	BYTE C_end;
	BYTE H_end;
	BYTE S_end;
	uint32_t lba_begin;
	uint32_t no_of_sectors;
}__attribute__ ((packed));


// Determina partitia pe care se afla sistemul de fisiere FAT32
// si initializeaza variabila de globala corespunzatoare
// @param read_buffer Bufferul folosit pentru citirea unui sector de pe card
void  init_partition(void * read_buffer);

#endif
