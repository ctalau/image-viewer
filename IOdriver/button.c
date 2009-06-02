#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include "button.h"



void BTN_init(){
	/** Pentru butoane avem nevoie de porturile:
	  *		PD4 , PD5, PC0, PC1
	  *		Configurate ca intrare cu PULL-UP
	  */
	DDRD &= ~(_BV(PD4) | _BV(PD5)) ;
	DDRC  = ~(_BV(PC0) | _BV(PC1)) ;

	PORTD |= _BV(PD4) | _BV(PD5);
	PORTC  = _BV(PC0) | _BV(PC1);
}

uint8_t BTN_wait(){
	uint8_t  pressed = 0;
	_delay_ms(330);
	do{
			pressed = ( ~PIND & ( _BV(PD4) | _BV(PD5) ) ) |
					  ( ~PINC & ( _BV(PC0) | _BV(PC1) ) );
	}while( pressed ==0);
	if(pressed & _BV(PC0))
		return ENTER;
	if(pressed & _BV(PC1))
		return CLOSE;
	if(pressed & _BV(PD4))
		return UP;
	if(pressed & _BV(PD5))
		return DOWN;
	return pressed;
}


