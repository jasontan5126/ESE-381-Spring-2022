/*
 * asynch_sw_send.c
 *
 * Created: 2/15/2022 8:55:44 AM
 * Author : jason
 */ 

#include <avr/io.h>
#define F_CPU 4000000  
#include <util/delay.h>

#define BAUD_RATE 19200

//header functions that will be in use
void USART_sw_write(char);	

int main(void)
{
	PORTB.DIR = PIN0_bm;													// set PB0 as output.  
	
    while (1) 
    {
		UART_sw_write('z');    //function to write the character.
		_delay_ms(1);     //send with a 1 ms delay
    }
	return 0;
}


void UART_sw_write(char c){
	uint8_t tempData = (uint8_t) c;   // pass in to the tempData variable
	PORTB_OUT &= ~PIN0_bm; // Send the start bit for PB0
	

	//Set the bit times for sending the data
	if(BAUD_RATE == 4800){
		_delay_us(208.3);
	}
	else if(BAUD_RATE == 9600){
		_delay_us(104.2);
	}
	else if(BAUD_RATE == 19200){
		_delay_us(50.1);
	}
	
	
	//Do some kind of a loop to shift right to PB0 to put into that pin output
	for(uint8_t i = 0; i < 8; i++){
		PORTB_OUT = tempData & 0x01;   //Mask to get the LSB to pass to PB0
		tempData >>= 1;			//shift right by 1
		
		//Set the bit times for sending the data
		if(BAUD_RATE == 4800){
			_delay_us(208.3);
		}
		else if(BAUD_RATE == 9600){
			_delay_us(104.2);
		}
		else if(BAUD_RATE == 19200){
			_delay_us(50.1);
		}
		
	}	
	
	PORTB_OUT = PIN0_bm;   //Send the stop bit
	
	//Set the bit times for sending the data
	if(BAUD_RATE == 4800){
		_delay_us(208.3);
	}
	else if(BAUD_RATE == 9600){
		_delay_us(104.2);
	}
	else if(BAUD_RATE == 19200){
		_delay_us(50.1);
	}
}


	
	
	