#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>
#include "font.h"
#include "lcd.h"


#ifdef _TEST_LCD_
#include "splash.h"
#endif



/* Function prototypes */
static void LCD_send(unsigned char data, LcdCmdData cd);


/* Performs IO & LCD controller initialization */
void LCD_init(void)
{
    // Pull-up on reset pin
    LCD_PORT |= LCD_RST_PIN;

	// Set output bits on lcd port
	LCD_DDR |= LCD_RST_PIN | LCD_CE_PIN | LCD_DC_PIN | LCD_DATA_PIN | LCD_CLK_PIN;

	// Wait after VCC high for reset (max 30ms)
    _delay_ms(15);

    // Toggle display reset pin
    LCD_PORT &= ~LCD_RST_PIN;
    lcd_delay();
    LCD_PORT |= LCD_RST_PIN;

    // Disable LCD controller
    LCD_PORT |= LCD_CE_PIN;

    LCD_send(0x21, LCD_CMD);  // LCD Extended Commands
    LCD_send(0xC8, LCD_CMD);  // Set LCD Vop(Contrast)
    LCD_send(0x06, LCD_CMD);  // Set Temp coefficent
    LCD_send(0x13, LCD_CMD);  // LCD bias mode 1:48
    LCD_send(0x20, LCD_CMD);  // Standard Commands, Horizontal addressing
    LCD_send(0x0C, LCD_CMD);  // LCD in normal mode

    // Clear lcd
    LCD_clear();
}

/**
  * Sterge ecranul
  */
void LCD_clear(void){
	LCD_gotoXY(0,0);
    for(int i=0;i<LCD_CACHE_SIZE;i++) {
		LCD_send(0x00, LCD_DATA);
    }
}

/**
  *	Deseneaza o imagine pe ecran
  */
void LCD_raw( const char * splash, uint16_t len ){
    for(int i=0; i< len ; i++) {
		LCD_send(splash[i], LCD_DATA);
    }
}

/**
  *	Afisaza un caracter
  */
static int LCD_chr(char chr, uint8_t sel){
    // Caractere de 5 pixeli  + spatiu
    for(unsigned char i=0;i<5;i++) {
		if( sel == SELECTED)
			LCD_send( ~(pgm_read_byte( &font5x7[chr-32][i]) << 1), LCD_DATA);
		else
			LCD_send(  pgm_read_byte( &font5x7[chr-32][i]) << 1, LCD_DATA);
    }

	LCD_send((sel == SELECTED)? 0xFF:0x00, LCD_DATA);
	return 0;
}

#define CHARS_PER_LINE 14
/**
  * Afisaza un string truchiat la CHARS_PER_LINE caractere pe o linie separata
  */
void LCD_str(char *str, uint8_t sel){
	uint8_t i=0;

    while(i++ < CHARS_PER_LINE && *str)
        LCD_chr(*str++,sel);
	for( i-- ; i < CHARS_PER_LINE; i++)
		LCD_chr(' ',sel);
}

/**
  *	Schimba adresa pointerului la pixelul 'addr'
  *  x - numarul coloanei (din 84 posibile
  *	 y - numarul linie ( din 6 posibile)
  */
void LCD_gotoXY(uint8_t x, uint8_t y) {
	LCD_send(0x80 | x , LCD_CMD);
	LCD_send(0x40 | y , LCD_CMD);
}

/* Sends data to display controller */
static void LCD_send(unsigned char data, LcdCmdData cd){
    // Enable display controller (active low)
    LCD_PORT &= ~LCD_CE_PIN;

    // Either command or data
    if(cd == LCD_DATA) {
        LCD_PORT |= LCD_DC_PIN;
    } else {
        LCD_PORT &= ~LCD_DC_PIN;
    }

	for(unsigned char i=0;i<8;i++) {
		// Set the DATA pin value
		if((data>>(7-i)) & 0x01) {
			LCD_PORT |= LCD_DATA_PIN;
		} else {
			LCD_PORT &= ~LCD_DATA_PIN;
		}

		// Toggle the clock
		LCD_PORT |= LCD_CLK_PIN;
		LCD_PORT &= ~LCD_CLK_PIN;
	}

	// Disable display controller
    LCD_PORT |= LCD_CE_PIN;
}

#ifdef _TEST_LCD_
int main(void){
	// Setup LCD
	LCD_init();
	LCD_contrast(0x40);

	while(1){
		LCD_clear();
		LCD_str("fisier 1",NOT_SELECTED);
		LCD_str("fisier 2", SELECTED);
		LCD_str("dir1",NOT_SELECTED);
		_delay_ms(2000);
		LCD_clear();
		LCD_raw((const char *) splash, 6 * 84);
		_delay_ms(2000);
	}
}
#endif



#ifdef _EXTRA_FEATURES_
/* Set display contrast. Note: No change is visible at ambient temperature */
void LCD_contrast(unsigned char contrast)
{
	LCD_send(0x21, LCD_CMD);				// LCD Extended Commands
    LCD_send(0x80 | contrast, LCD_CMD);		// Set LCD Vop(Contrast)
    LCD_send(0x20, LCD_CMD);				// LCD std cmds, hori addr mode
}
#endif


