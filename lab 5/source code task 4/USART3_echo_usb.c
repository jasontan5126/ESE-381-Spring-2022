/*
 * USART3_echo_usb.c
 *
 * Created: 2/27/2022 11:52:14 PM
 * Author : jason
 */ 

#include <avr/io.h>
#define F_CPU 4000000
#include <util/delay.h>

#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)))  //Calculation of baud rate from data sheet

char receivedCharacter;

//All the header functions
uint8_t USART_sw_read();
void USART_sw_write(char);

int main(void)
{
	PORTB.DIR |= PIN0_bm;  //Set PB0 as output (TX pin) and PB1 in input pin (RX pin)
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);   //Taken from data sheet to calculate baud rate
	USART3.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;   //Enable USART transmitter and receiver
	
	
	while (1)
	{
		receivedCharacter = (char)USART_sw_read();
		USART_sw_write(receivedCharacter);	
	}
}


uint8_t USART_sw_read(){
	//Poll to wait for the character to be received from TeraTerm
	while(!(USART3_STATUS & USART_RXCIF_bm)){}
		
	return USART3_RXDATAL;
}

void USART_sw_write(char c){
	//Poll until there is data available to be transmitted or echoed after data is moved to the transmit shift register
	while(!(USART3_STATUS & USART_DREIF_bm)){}
		
	USART3_TXDATAL = c;	//Write the character to be echoed which will output to the TXD pin	
}



