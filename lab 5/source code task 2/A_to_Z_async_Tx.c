/*
 * A_to_Z_async_Tx.c
 *
 * Created: 2/27/2022 2:59:16 PM
 * Author : jason
 */ 

#define F_CPU 4000000
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)))  //Calculation of baud rate from data sheet
#include <avr/io.h>
#include <util/delay.h>

//Header functions
void USART3_sendChar(char c);

char uppercaseLetters = 'A'; 


int main(void)
{
	PORTB.DIR |= PIN0_bm;  //Set PB0 as output (TX pin)
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);   //Taken from data sheet to calculate baud rate
	USART3.CTRLB |= USART_TXEN_bm;  //Enable USART transmitter
	
	while (1)
	{
		while(uppercaseLetters <= 'Z'){
			//Send an uppercase character to the Tera Term (TX pin)
			USART_sw_write(uppercaseLetters);  
			if(uppercaseLetters == 'Z'){
				uppercaseLetters = uppercaseLetters - 26;
				USART_sw_write(0x0A);    //Go to next line after 'Z' is printed
				//Move the cursor back by 26 spaces to the start of next line
				for(int i = 0; i < 26; i++){
					USART_sw_write(0x08); 
				}
			}
			_delay_ms(1);	//Wait in a loop to send character again
			uppercaseLetters++;
		}
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

