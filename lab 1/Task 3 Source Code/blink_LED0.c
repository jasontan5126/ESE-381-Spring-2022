/*
 * blink_LED0.c
 *
 * Created: 2/3/2022 7:30:34 PM
 * Author : Jason Tan
 */ 

#include <avr/io.h>
#define F_CPU 4000000   //Frequency used by delay macros.
#include <util/delay.h>


int main(void)
{
    //PORTB.DIR &= ~PIN3_bm; //Set PB3 to be the input by setting PB3 to 0
	
	//0b0000 1000
	PORTB.DIR |= PIN3_bm;  //Set PB3 to be the output since by default, the 8 bits are all 0x00
	
    while (1) 
    {		
		//PIN3_bm is 0b0000 1000
		PORTB.OUT |= PIN3_bm; //LED is off since PB3 is 1
		_delay_ms(500);	   //Wait 0.5 seconds
	 	PORTB.OUT &= ~PIN3_bm;  //LED is on since PB3 is 0  ~0b1111 0111
		_delay_ms(500);	   //Wait 0.5 seconds
		
    }
}

