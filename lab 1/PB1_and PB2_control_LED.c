/*
 * PB1_and PB2_control_LED.c
 *
 * Created: 2/2/2022 11:07:15 AM
 * Author : Jason Tan
 */ 

#include <avr/io.h>


int main(void)
{
    
	VPORTB_DIR = 0x20;	//Configure PB5 as the output for the LED 
	
	PORTA_PIN2CTRL = 0x08;	//Enable internal pull up resistor for PA2
	PORTA_PIN5CTRL = 0x08;	//Enable internal pull up resistor for PA5
	
	
    while (1) 
    {
		//Check if PA2 and PA5 are both 1
		if((VPORTA_IN & PIN2_bm) && (VPORTA_IN & PIN5_bm)){
			PORTB_OUT |= 0x20;  //To turn off the LED by setting the logic to 1 for the cathode of LED
			//PORTB_OUT &= 0x20; //To turn on the LED by setting the logic to 0 for the cathode of LED
		}
		//Check if PA5 is 1 and PA2 is 0
		else if((~(VPORTA_IN & PIN2_bm)) && (VPORTA_IN & PIN5_bm)){
			PORTB_OUT |= 0x20;  //To turn off the LED by setting the logic to 1 for the cathode of LED
		}
		//Check if PA2 is 0 and PA5 is 1
		else if ((~(VPORTA_IN & PIN2_bm)) && ((VPORTA_IN & PIN5_bm))){
			PORTB_OUT &= ~0x20; //To turn on the LED by setting the logic to 0 for the cathode of LED
		}
		//Check if PA5 is 0 and PA2 is 0
		else if((~(VPORTA_IN & PIN2_bm)) && (~(VPORTA_IN & PIN5_bm))){
			PORTB_OUT |= 0x20;  //To turn off the LED by setting the logic to 1 for the cathode of LED
		}
    }
}

