#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "draw_bmp.h"

/**
  *	48 bits pe rand => 64 bits (rotunjire) => 8 octeti
  */
struct _bmp_header default_header(){
	struct _bmp_header hd;

	hd.BM[0] = 'B'; hd.BM[1] = 'M';
	hd.size 		= 0x000002DE; //TODO
	hd.pad1 		= 0x0000;
	hd.pad2 		= 0x0000;
	hd.data_off 	= 0x0000003E;
	hd.header_lg 	= 0x00000028;
	hd.width		= 0x00000030;
	hd.height		= 0x00000054;
	hd.no_planes	= 0x0001;
	hd.bit_per_pix	= 0x0001;
	hd.compression 	= 0x00000000;
	hd.data_sz 		= 0x000002A0;
	hd.resX			= 0x00000B13;
	hd.resY			= 0x00000B13;
	hd.palette		= 0x00000002;
	hd.imp			= 0x00000002;
	hd.black		= 0x00FFFFFF;
	hd.white		= 0x00000000;
	return hd;
}

int main(int argc, char ** argv){
	FILE * fout;

	if(argc <2){
		fprintf(stderr,"Usage:\n\t%s poza.bmp\n",argv[0]);
		exit(1);
	}

	fout = fopen(argv[1],"w");

	struct _bmp_header hd = default_header();
	int i;
	uint8_t z = 0;

	fwrite( &hd, sizeof(hd), 1, fout);
	for(i=0; i <  8  * 84; i++){
		fwrite( &z, sizeof(z), 1, fout);
	}




	fclose(fout);

	return 0;
}
