/*
 * USART3_asynch_transmit.h
 *
 * Created: 4/26/2022 8:06:04 PM
 *  Author: jason
 */ 


#ifndef USART3_ASYNCH_TRANSMIT_H_
#define USART3_ASYNCH_TRANSMIT_H_

#define F_CPU 4000000
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)))  //Calculation of baud rate from data sheet
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

//Header functions
void USART3_sendChar(char c);
void USART3_init (uint16_t, uint8_t, unsigned char);
void USART_sw_write(char);


//A simple function to configure a USART might have a single parameter that 
//specifies the desired baud rate. The function that you must write for this task 
//goes further than that, it allows both baud rate and the frame format to be specified
void USART3_init (uint16_t baud, uint8_t data_bits, unsigned char parity){
	PORTB_DIR |= PIN0_bm;  //To transmit the data
	
	//Specify the baud rate value for the USART3
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(baud);
	
	//Initialize the data bits and the parity bits type
	USART3_CTRLC |= data_bits | parity;
	USART3.CTRLB |= USART_TXEN_bm;  //Enable USART transmitter
}


//To be able to send the string of characters
void USART3_sendString(char* input){
	for(size_t i = 0; i < strlen(input); i++){
		USART_sw_write(input[i]);
	}
}



//Function to be able to transmit characters
//to the TX pin and display on the Tera Term
void USART_sw_write(char c)
{
	//Poll until the  transmit buffer register are empty
	//when they contain data that has not been moved to
	//transmit shift register
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	
	//Load data to transmit shift register and
	//output each of the bits serially to the TXD pin
	USART3.TXDATAL = c;
}



#endif /* USART3_ASYNCH_TRANSMIT_H_ */