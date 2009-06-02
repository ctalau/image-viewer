#define F_CPU 16000000
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/iom16.h>
#include <uart.h>

#include "mmc.h"



#define SECTOR_SIZE  512
#ifdef _MMC_TEST_
static char  sector[SECTOR_SIZE];
#endif


/**
  * Initializeaza modulul de SPI
  */
void SPI_init(void) {
	// Setam porturile de CLK, DataOut, ChipSelect - iesire, si DataIn - intrare
	DDRB &= ~(1 << SPIDI);
	DDRB |= (1 << SPICLK) | (1 << SPIDO) | (1 << SPICS);

	// SPI Enable, Modul Master, CLK / 128
	SPCR = (1 << MSTR) ;

}

/**
  * Trimitem un caracter prin SPI si citim unul
  */
static char SPI_send(char d) {  // send character over SPI
	char received = 0;

	SPI_ENABLE();
	SPDR = d;
	while(!(SPSR & (1<<SPIF)));
	received = SPDR;
	SPI_DISABLE();
	return (received);
}

/**
  * Trimite o comanda la MMC
  * Formatul este urmatorul: COD_OP, ADDR, CRC
  */
static char MMC_command(char befF, uint32_t Addr, char befH ){
	char ret;
	MMC_ENABLE();

	SPI_send(0xFF);
	SPI_send(0x40 | befF);
	SPI_send((uint8_t)( Addr >> 24));
	SPI_send((uint8_t)( Addr >> 16));
	SPI_send((uint8_t)( Addr >> 8 ));
	SPI_send((uint8_t)( Addr      ));
	SPI_send(befH);
	SPI_send(0xFF);
	ret = SPI_send(0xFF);

	MMC_DISABLE();
	return ret;
}

/**
  * Initializeaza cardul MMC
  */
int MMC_init(void) {
	char i;

	// Activam modul SPI al cardului trimitand 80 de cicluri de ceas caractere dummy
	// cu chipul dezactivat
	MMC_DISABLE();
	for(i=0; i < 10; i++)
		SPI_send(0xFF);
	//PORTB &= ~(1 << SPICS);

	// Trimitem comanda de GO_IDLE_STATE
	if (MMC_command( MMC_GO_IDLE_STATE, 0, 0x95) != 1)
		return 1;

polling:
	// Trimitem comanda de GET_OPERATIONAL_MODE
	if (MMC_command( MMC_GET_OP_MODE,0,0xFF) !=0) goto polling;

	return 0;
}

/**
  *  Citeste un sector de la cardul MMC
  */
void MMC_read(uint32_t addr, char * sector) {
	int i;
	char tmp;
	// Citim 512 octeti
	MMC_command( MMC_READ_SINGLE, addr ,0xFF);

	MMC_ENABLE();

	// Asteptam un 0xFE care sa anunte inceputul transmisiei
	// ATT: typecast (char)0xFE is a must!
	while(SPI_send(0xFF) != (char)0xFE);

	for(i=0; i < SECTOR_SIZE; i++) {
		tmp = SPI_send(0xFF);
		sector[i] = tmp;
	}

	// Trimitem 2 dummy bytes pentru a citi CRC-ul
	SPI_send(0xFF);
	SPI_send(0xFF);
	MMC_DISABLE();
}

#ifdef _MMC_TEST_
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

int main(void) {
	uint32_t i,j;
	uart_init();
	SPI_init();
	MMC_init();


	for( i = 0; i < 1; i++){
		printf("Primesc\r\n");
		MMC_read(SECTOR_SIZE*i, (char *) sector);
		for(j=0; j < SECTOR_SIZE; j++) {
			printf("%02x ",(unsigned char )sector[j]);
			if(j %16 == 0)
				printf("\r\n");
		}
	//_delay_ms(2000);
	}
	printf("512 byteai received\n");

	while (1);
	return 0;
}
#endif
