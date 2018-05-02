/*
 * 6_uart_interrupt_echo.c
 *
 *	Objective:  Understand the UART (universal asynchronous receiver and transmitter)
 *				We will use the UART to echo back a received byte using RXC interrupt. 
 *
 * Created: 2/9/2018 9:32:26 PM
 * Author : Jon Kim
 */ 

#define F_CPU	20000000

#define BAUD_RATE	9600
#define BAUD_VALUE	(F_CPU/16/BAUD_RATE - 1)		// 129 @9600 (Table 24.7)

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(USART_RX_vect)
{
	/*
		[TASK] Implement a ISR code here to echo back a received byte.
	*/
  unsigned volatile char ch = UDR0;
  put_char(ch);
  PORTB = ~PORTB;

}

void put_char(unsigned char ch)
{
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = ch;
}

void put_str(unsigned char* str)
{
	while (*str != 0)
		put_char(*str++);
}


int main(void)
{
	DDRB = (1<<DDB1);
	
	UBRR0H = (unsigned char)(BAUD_VALUE>>8);	
	UBRR0L = (unsigned char)(BAUD_VALUE);
	UCSR0C = (3<<UCSZ00);
	UCSR0B = (1<<TXEN0)|(1<<RXEN0); 
        UCSR0B |= (1<<RXCIE0);
	sei();

	put_str("Serial echoing test. Type any key...\r\n");

    while (1) 
    {
		
		_delay_ms(500);
    }
}
