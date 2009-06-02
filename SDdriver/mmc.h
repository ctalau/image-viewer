#ifndef _MMC_H_
#define _MMC_H_

/**
  *	SPI pin definitions
  */
#define SPIDI	6	// Port B bit 6 (pin7): data in (data from MMC)
#define SPIDO	5	// Port B bit 5 (pin6): data out (data to MMC)
#define SPICLK	7	// Port B bit 7 (pin8): clock
#define SPICS	4	// Port B bit 4 (pin5: chip select for MMC


#define MMC_GO_IDLE_STATE  	0x00
#define MMC_GET_OP_MODE		0x01
#define MMC_READ_SINGLE		0x11


#define MMC_ENABLE()	PORTB &= ~(1 << SPICS)
#define MMC_DISABLE()	PORTB |= (1 << SPICS);

#define SPI_ENABLE()		SPCR |= (1 << SPE)
#define SPI_DISABLE()		SPCR &= ~(1 << SPE)

void SPI_init(void);
int MMC_init(void);
void MMC_read(uint32_t addr, char * sector);

#endif
