//********************************************************
//**** Functions for Interfacing NOKIA 3310 Display *****
//********************************************************
//Controller:	ATmega32 (Clock: 1 Mhz-internal)
//Compiler:		ImageCraft ICCAVR
//Author:		CC Dharmani, Chennai (India)
//Date:			Sep 2008
//********************************************************

//#include <avr/macros.h>
#include <avr/io.h>

#include "lcd.h"

//global variable for remembering where to start writing the next text string on 3310 LCD
unsigned char char_start;

/*--------------------------------------------------------------------------------------------------
  Name         :  spi_init
  Description  :  Initialising atmega SPI for using with 3310 LCD
  Argument(s)  :  None.
  Return value :  None.
--------------------------------------------------------------------------------------------------*/
//SPI initialize
//clock rate: 250000hz
void spi_init(void)
{
 SPCR = 0x58; //setup SPI
}

/*--------------------------------------------------------------------------------------------------
  Name         :  LCD_init
  Description  :  LCD controller initialization.
  Argument(s)  :  None.
  Return value :  None.
--------------------------------------------------------------------------------------------------*/
void LCD_init ( void )
{
	delay_ms(100);

	CLEAR_SCE_PIN;    //Enable LCD

	CLEAR_RST_PIN;	//reset LCD
    delay_ms(100);
    SET_RST_PIN;

	SET_SCE_PIN;	//disable LCD

    LCD_writeCommand( 0x21 );  // LCD Extended Commands.
    LCD_writeCommand( 0xE0 );  // Set LCD Vop (Contrast).
    LCD_writeCommand( 0x04 );  // Set Temp coefficent.
    LCD_writeCommand( 0x13 );  // LCD bias mode 1:48.
    LCD_writeCommand( 0x20 );  // LCD Standard Commands, Horizontal addressing mode.
    LCD_writeCommand( 0x0c );  // LCD in normal mode.

    LCD_clear();
}

/*--------------------------------------------------------------------------------------------------
  Name         :  LCD_writeCommand
  Description  :  Sends command to display controller.
  Argument(s)  :  command -> command to be sent
  Return value :  None.
--------------------------------------------------------------------------------------------------*/
void LCD_writeCommand ( unsigned char command )
{
    CLEAR_SCE_PIN;	  //enable LCD

	CLEAR_DC_PIN;	  //set LCD in command mode

    //  Send data to display controller.
    SPDR = command;

    //  Wait until Tx register empty.
    while ( !(SPSR & 0x80) );

    SET_SCE_PIN;   	 //disable LCD
}

/*--------------------------------------------------------------------------------------------------
  Name         :  LCD_writeData
  Description  :  Sends Data to display controller.
  Argument(s)  :  Data -> Data to be sent
  Return value :  None.
--------------------------------------------------------------------------------------------------*/
void LCD_writeData ( unsigned char Data )
{
    CLEAR_SCE_PIN;	  //enable LCD

	SET_DC_PIN;	  //set LCD in Data mode

    //  Send data to display controller.
    SPDR = Data;

    //  Wait until Tx register empty.
    while ( !(SPSR & 0x80) );

    SET_SCE_PIN;   	 //disable LCD
}

/*--------------------------------------------------------------------------------------------------
  Name         :  LCD_clear
  Description  :  Clears the display
  Argument(s)  :  None.
  Return value :  None.
--------------------------------------------------------------------------------------------------*/
void LCD_clear ( void )
{
    int i,j;

	LCD_gotoXY (0,0);  	//start with (0,0) position

    for(i=0; i<8; i++)
	  for(j=0; j<90; j++)
	     LCD_writeData( 0x00 );

    LCD_gotoXY (0,0);	//bring the XY position back to (0,0)

}

/*--------------------------------------------------------------------------------------------------
  Name         :  LCD_gotoXY
  Description  :  Sets cursor location to xy location corresponding to basic font size.
  Argument(s)  :  x - range: 0 to 84
  			   	  y -> range: 0 to 5
  Return value :  None.
--------------------------------------------------------------------------------------------------*/
void LCD_gotoXY ( unsigned char x, unsigned char y )
{
    LCD_writeCommand (0x80 | x);   //column
	LCD_writeCommand (0x40 | y);   //row
}

/*--------------------------------------------------------------------------------------------------
  Name         :  LCD_writeChar
  Description  :  Displays a character at current cursor location and increment cursor location.
  Argument(s)  :  ch   -> Character to write.
  Return value :  None.
--------------------------------------------------------------------------------------------------*/
void LCD_writeChar (unsigned char ch)
{
   unsigned char j;

   LCD_writeData(0x00);

   for(j=0; j<5; j++)
     LCD_writeData( smallFont [(ch-32)*5 + j] );

   LCD_writeData( 0x00 );
}

/*--------------------------------------------------------------------------------------------------
  Name         :  LCD_writeString_F
  Description  :  Displays a string stored in FLASH, in small fonts (refer to 3310_routines.h)
  Argument(s)  :  string -> Pointer to ASCII string (stored in FLASH)
  Return value :  None.
--------------------------------------------------------------------------------------------------*/
void LCD_writeString_F ( const unsigned char *string )
{
    while ( *string )
        LCD_writeChar( *string++ );
}


/*--------------------------------------------------------------------------------------------------
  Name         :  delay_ms
  Description  :  1 millisec delay (appx.)
  Argument(s)  :  None.
  Return value :  None.
--------------------------------------------------------------------------------------------------*/
void delay_ms(int miliSec)  //for 1Mhz clock
{
  int i,j;
  for(i=0;i<miliSec;i++)
    for(j=0;j<100;j++)
	{
	  asm("nop");
	  asm("nop");
	}
}

/*--------------------------------------------------------------------------------------------------
  Name         :  LCD_drawBorder
  Description  :  Draws rectangle border on the display
  Argument(s)  :  None
  Return value :  None
--------------------------------------------------------------------------------------------------*/
void LCD_drawBorder (void )
{
  unsigned char i, j;
  for(i=0; i<7; i++)
  {
    LCD_gotoXY (0,i);

	for(j=0; j<84; j++)
	{
	  if(j == 0 || j == 83)
		LCD_writeData (0xff);		// first and last column solid fill to make line
	  else if(i == 0)
	    LCD_writeData (0x08);		// row 0 is having only 5 bits (not 8)
	  else if(i == 6)
	    LCD_writeData (0x04);		// row 6 is having only 3 bits (not 8)
	  else
	    LCD_writeData (0x00);
	}
  }
}	

/*--------------------------------------------------------------------------------------------------
                                         End of file.
--------------------------------------------------------------------------------------------------*/
