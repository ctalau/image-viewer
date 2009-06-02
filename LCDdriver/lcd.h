#ifndef _NOKIALCD_H_
#define _NOKIALCD_H_

/* Lcd screen size */
#define LCD_X_RES 84
#define LCD_Y_RES 48
#define LCD_CACHE_SIZE ((LCD_X_RES * LCD_Y_RES) / 8)

/* Pinout for LCD */
#define LCD_CLK_PIN 	(1<<PB7)
#define LCD_DATA_PIN 	(1<<PB5)
#define LCD_DC_PIN 		(1<<PB0)
#define LCD_CE_PIN 		(1<<PB1)
#define LCD_RST_PIN 	(1<<PB3)
#define LCD_PORT		PORTB
#define LCD_DDR			DDRB

/* Special Chars */
#define ARROW_RIGHT	ICON(0)
#define ARROW_LEFT 	ICON(1)
#define ARROW_UP 	ICON(2)
#define ARROW_DOWN 	ICON(3)
#define STOP 		ICON(4)
#define PLAY	 	ICON(5)

/* Function for my special characters */
#define	ICON(x)		'z'+1+x


/* Comanda sau date */
typedef enum { LCD_CMD  = 0, LCD_DATA = 1 } LcdCmdData;

/* Macro pt delay delay */
#define lcd_delay() for(volatile int i=-32000;i<32000;i++)

/**
  *	Functiile exportate de driver
  */
#define SELECTED   		1
#define NOT_SELECTED 	0

void LCD_init(void);
void LCD_raw( const char  * splash, uint16_t len );
void LCD_str( char* str,uint8_t sel);
void LCD_clear(void);
void LCD_gotoXY(uint8_t x, uint8_t y);


#define _EXTRA_FEATURES_
#ifdef _EXTRA_FEATURES_
void LCD_contrast(unsigned char contrast);
#endif

#endif



