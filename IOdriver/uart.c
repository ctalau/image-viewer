#include <avr/io.h>
#include <stdio.h>
#include "button.h"


/**
  *  Scrie un caracter pe seriala
  */
static int uart_putchar (char c, FILE * unused) {
  if (c == '\n')							// daca caracterul este "end-of-line" CR_LF
    uart_putchar ('\r', 0);					// se reapeleaza functia cu: CR (pentru a face VTerm-ul fericit!)
  loop_until_bit_is_set (UCSRA, UDRE);		// asteapta pana cand este trimis caracterul anterior
  UDR = c;									// se scrie bufferul cu caracterul dorit - apoi, automat,
  return 0;
}

/**
  * Citeste un caracter de pe seriala
  */
static int uart_getchar (FILE * f){
  char c;
  loop_until_bit_is_set (UCSRA, RXC);	// se asteapta pana exista un caracter nou in bufferul USART-ului
  c = UDR;								// se preia caracterul
  if (c == '\r')						// daca este un CR, se inlocuieste cu CRLF (vezi uart_putchar)
    c = '\n';
  uart_putchar (c, NULL);				// se retrimite caracterul citit inapoi la VTerm pentru efect de echo
  return c;
}

/**
  * Pointerii sunt legatura pentru printf si scanf
  */
FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);	// stdout replacement
FILE mystdin = FDEV_SETUP_STREAM (NULL, uart_getchar, _FDEV_SETUP_READ);	// stdin replacement

/**
  * Functie de initializare a interfetei cu seriala
  */
void uart_init (void){
  UBRRH = 0;											// baudrate-ul este pe 16 biti si daca procesorul merge la 16 MHz
  UBRRL = 0x67;											//    pentru un baudrate de 9600 registrul UBRR va if 0x0067

  UCSRB = _BV (RXEN) | _BV (TXEN);						// UART-ul este activat pentru receptie si transmisie
  UCSRC = _BV (URSEL) | _BV (UCSZ1) | _BV (UCSZ0);		// UART-ul este configurat pentru Rx/Tx de 8 biti
 													 	// implicit USBS este 0   = un sigur bit de STOP
  stdout = &mystdout;
  stdin = &mystdin;
}
#ifdef _USART_TEST_
int main(){
	int i = 0, btn = 0;
	BTN_init();
	uart_init();

	for(;;){
		printf("Intra in bucla\n") ;
		btn = wait_button();
		printf("%d %d\n", btn , i++);
	}
}
#endif





