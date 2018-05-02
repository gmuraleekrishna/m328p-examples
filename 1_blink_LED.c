#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 20000000L // or whatever may be your frequency
#endif


int main(void)
{
	DDRB = 0x02;
	
    /* Replace with your application code */
    while (1) 
    {
		PORTB = ~PORTB;
		_delay_ms(500);
	}
}

