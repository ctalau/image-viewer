#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "fat.h"
#include "volid.h"
#include "partitions.h"
#include "button.h"
#include "lcd.h"
#include "draw_bmp.h"


#define F_CPU 16000000
#include <util/delay.h>

extern unsigned char  buffer[SECTOR_SIZE];
/**
  * Meniul de navigare in fisiere
  */
void printdirs(struct DIR * dir){
	char namebuf [12];
	uint32_t size = 0;
	uint8_t bitmap = 0, selected = 0, cnt = 0, ret_code = 0, is=0;
	struct dirent * crt_dir;

	do{
		/* Afisam continutul directorului curent */
		rewinddir(dir);
		LCD_clear();
		cnt = 0;
		while(1){
			crt_dir = readdir( dir, buffer);
			ret_code = check (crt_dir);
			if(ret_code == DIR_INVALID)
				continue;
			if(ret_code == DIR_END)
				break;

			get_dirent_name(crt_dir, namebuf);
			if(cnt ++ == selected){
				LCD_str( namebuf,SELECTED );
			}else{
				LCD_str( namebuf, NOT_SELECTED);
			}
		}

		/* Asteptam sa selecteze un fisier sau director */
		bitmap = BTN_wait();

		switch(bitmap){
			case UP:
				selected = (selected +1)%cnt;
				continue;
			case DOWN:
				selected = (selected + cnt-1) % cnt;
				continue;
			case ENTER:
				rewinddir(dir);
				selected ++;
				while(1){
					crt_dir = readdir( dir, buffer);
					ret_code = check (crt_dir);
					if( ret_code == DIR_VALID) selected--;
					if( !selected ) break;
				}
				break;
			default:
				continue;

		}

		is_dir(crt_dir, &is);
		if( is){
			dir = opendir(crt_dir);
		}else{
			get_dirent_size(crt_dir, &size);
			open_f(crt_dir);

			/* Curatam ecranul si afisam poza */
			LCD_clear();
			draw_bmp();
			close_f();

			/* Asteptam sa apese butonul de exit */
			while( BTN_wait() != CLOSE);
		}
	}while(1);
	exit(EXIT_SUCCESS);
}



#ifndef TESTING
/**
  *	Cod care initializeaza stiva 'by hand' la sfarsitul memoriei
  */
void stack_init(void) __attribute__ ((naked)) __attribute__ ((section (".init8"))); 
void stack_init(){
	asm("ldi r28,lo8(0x80045F)");
	asm("ldi r29,hi8(0x80045F)");
	asm("out __SP_H__,r29");
	asm("out __SP_L__,r28");
}
#endif



int main(){

#ifndef TESTING

#include "uart.h"
#include "mmc.h"

	// Initializam modulele hardware

	uart_init();
	SPI_init();
	MMC_init();
	BTN_init();
	LCD_init();
#endif
	/* Gaseste prima partitie FAT32 de pe card*/
	init_partition(buffer);

	/**
	  * Gaseste datele despre sistemul de fisiere :
	  * nr de tabele FAT, cluster-ul directorului Root, etc.
	  */
	initFAT32(buffer);
	print_volid(buffer);

	LCD_str( "  ", 0);
	LCD_str( "  ", 0);
	LCD_str( "  Welcome to", 0);
	LCD_str( " MMC Explorer", 0);
	_delay_ms(2500);
	LCD_clear();
	printdirs(openroot());
	return EXIT_SUCCESS;
}


