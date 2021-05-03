#ifndef F_CPU					// if F_CPU was not defined in Project -> Properties
#define F_CPU 16000000UL			// define it now as 16 MHz unsigned long
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



#define BUAD 9600
#define BRC ((F_CPU/16/BUAD) - 1)


void usart_init()
{
	
	UCSRB |= (1 << RXEN) | (1 << TXEN) | (1 << RXCIE) | (1 << TXCIE) | (1 << UDRIE);  // Turn on the transmission and reception circuitry
	UCSRC |= (1 << URSEL) | (1<<USBS) | (1 << UCSZ0) | (1 << UCSZ1);
	// Use 8-bit character sizes
	
	UBRRL = BRC;
	// Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	UBRRH = (BRC >> 8); // Load upper 8-bits of the baud rate value..
	
}
unsigned int RX_GET_DATA()
{
	
	while ((UCSRA & (1 << RXC)) == 0);
	
	return(UDR); 
}
void TX_SEND_DATA(char ch)
{
	UDR = ch;
	while((UCSRA & (1 << TXC)) == 0) {}
	while((UCSRA & (1 << UDRE)) == 0) {}
	//_delay_ms(5);
}
void SEND_TO_UDR(char ch[500])
{
	int i;
	while((UCSRA & (1 << UDRE)) == 0) {}
	for(i = 0; ch[i] ; i++)
	{
		TX_SEND_DATA(ch[i]);
	}
}
int main()
{
	usart_init();
	char global[500] = "Hello";
	while(1)
	{
		SEND_TO_UDR(global);
		_delay_ms(1000);
	}
}