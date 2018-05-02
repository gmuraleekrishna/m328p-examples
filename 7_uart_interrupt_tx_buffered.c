/*
 * 7_uart_interrupt_tx_buffered.c
 *
 *	Objective:  Understand the UART (universal asynchronous receiver and transmitter)
 *				We will use the UART to echo back a received byte using UDRE interrupt. 
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

#define BUF_SIZE	128

unsigned char tx_buf[BUF_SIZE];
int tx_buf_head = 0;
volatile int tx_buf_tail = 0;
volatile int tx_data_size = 0;
volatile first_char_flag = 1;


ISR(USART_UDRE_vect) {
  if((tx_buf_head-tx_buf_tail) != 0) { // when there is data in the ring buffer
    tx_buf[tx_buf_tail];
    tx_buf_tail = (tx_buf_tail+1) % BUF_SIZE;
  }
} else {
  first_char_flag = 1;
  UCSR0B |= (0<<UDRIE0);
 }
}


void put_char_buf(unsigned char ch) {
  /*
    [TASK 1] Implement the code here 
    1. Write a byte to the buffer incrementing the head index
    2. If the head index reaches the end of the buf, then reset to zero
    3. Check if the buf is overrun. We can utilise the data size in the buf
    to detect this and maybe discard the last byte. 
  */
  if(tx_buf_head != tx_buf_tail || first_char_flag) {
    tx_buf[tx_buf_head] = ch;
    first_char_flag = 0;
    tx_buf_head = (tx_buf_head + 1) % BUF_SIZE;
  } else {
    tx_buf_head--;
  }

}

void put_str_buf(unsigned char* str) {
  for (int i=0; i< strlen(str)){
    put_char_buf(str[i]);
  }
  // We've put data in the buffer so turn on the TX-UDRE interrupt
  UCSR0B |= (1<<UDRIE0);
}

void put_char(unsigned char ch) {
  while ( !( UCSR0A & (1<<UDRE0)) );
  UDR0 = ch;
}

void put_str(unsigned char* str) {
  while (*str != 0)
    put_char(*str++);
}

int main(void) {
  int i = 0;
  unsigned char str[128];
  DDRB = (1<<DDB1);
  UBRR0H = (unsigned char)(BAUD_VALUE>>8);
  UBRR0L = (unsigned char)(BAUD_VALUE);
  UCSR0C = (3<<UCSZ00);
  UCSR0B = (1<<TXEN0)|(1<<RXEN0);

  sei();

  while (1) 
    {
      sprintf(str,"[%d] Hello World!\r\n",++i);
      /*
        [TASK 3] Comment out the polling function put)str()
        and then demonstrate the working function - put_str_buf() 
      */
      //put_str_buf(str);
      // put_str(str);
      put_str_buf(str);
      PORTB = ~PORTB;
      _delay_ms(100);
    }
}
