#define F_CPU	20000000

#define BAUD_RATE	9600
#define BAUD_VALUE	(F_CPU/16/BAUD_RATE - 1)		// 129 @9600 (Table 24.7)

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>


void put_char(unsigned char ch) {
  while (!(UCSR0A & (1<<UDRE0)));
  UDR0 = ch;
}

void put_str(char* str) {
  while (*str != 0) put_char(*str++);
}

unsigned char get_char(void) {
  while (!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

void get_str(unsigned char* str) {
  //unsigned char data[128];
  unsigned char ch;
  //int i = 0;
  while ((ch = get_char()) != '\r') {
    *str++ = ch;
  }
  *str++ = '\0';
}


int main(void) {
	char str[128], data[128];
  int i = 0;
	DDRB = (1<<DDB1);
	UBRR0H = (unsigned char)(BAUD_VALUE>>8);	
	UBRR0L = (unsigned char)(BAUD_VALUE);
	UCSR0C = (3<<UCSZ00);
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);
  put_str("Type keys followed by [enter]...");
    while (1) {
 
      get_str(data);
      sprintf(str,"[%d][%s]\r\n", ++i, data);
      put_str(str);

      PORTB = ~PORTB;
      //      _delay_ms(500);
    }
}
