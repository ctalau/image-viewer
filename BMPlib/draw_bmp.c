#include "draw_bmp.h"
#include <stdio.h>
#include "hw.h"
#include "dir.h"
#include "file.h"
#include "lcd.h"


extern unsigned char buffer[SECTOR_SIZE];

void draw_bmp( ){
	struct _bmp_header hd;
	uint8_t i , j;
	char pixel;

	read_f( &hd, sizeof( struct _bmp_header), buffer);
	for( i =0; i < 84; i++){
		for( j =0; j < 8; j++){
			read_f( &pixel, sizeof( char ) , buffer );

			/* Ignoram octetii de padding */
			if( j < 6){
				/* i - coloana , j - linia*/
				LCD_gotoXY(83-i,5-j);
				LCD_raw( &pixel, sizeof( char ));
			}
		}
	}
}
