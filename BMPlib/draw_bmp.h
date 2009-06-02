#ifndef _DRAW_BMP_H_
#define _DRAW_BMP_H_

#include <stdint.h>
struct _bmp_header{
	uint8_t  	BM[2];		// "BM" - magic number
	uint32_t  	size;		// marimea fisierului
	uint16_t  	pad1;
	uint16_t  	pad2;
	uint32_t	data_off;	// offsetul pixelilor - 0x3E
	uint32_t 	header_lg;	// lungimea headerului din punctul curent - 0x28
	uint32_t 	width;		// latimea in pixeli - 0x30
	uint32_t	height;		// lungimea in pixeli	- 0x54
	uint16_t	no_planes;	//	0x0001
	uint16_t	bit_per_pix;//  0x01
	uint32_t	compression;// 0x00
	uint32_t	data_sz;	// 6 * 84
	uint32_t	resX;		// 0x00
	uint32_t	resY;		// 0x00
	uint32_t	palette;	// 0x00
	uint32_t	imp;		// 0x00
	uint32_t	black;		// 0x00FFFFFF
	uint32_t	white;		// 0x00000000
}__attribute__((packed));


void draw_bmp( );


#endif
