/*
 * toggle_every_xxx_us.c
 *
 * Created: 2/13/2022 6:25:20 PM
 * Author : jason
 */ 


#define F_CPU 4000000

#include <avr/io.h>
#include <util/delay.h>

#define PULL_UP_VAL 0x08

int main(void)
{
    PORTC_DIR = PIN3_bm;  //Configure PC3 as the output
	
	PORTC_PIN0CTRL = PULL_UP_VAL;    //Enable internal pull up resistor for PC0
	PORTC_PIN1CTRL = PULL_UP_VAL;	//Enable internal pull up resistor for PC1
    
	while (1) 
    {
		PORTC_OUT |= PIN3_bm;   //Set PC3, XOR
		//For two bits to be used for the DIP SWITCH
		//PC0 = 0 and PC1 = 0 for DIP SWITCH
		
		
		if((~(PIN0_bm & PORTC_IN)) && (~(PIN1_bm & PORTC_IN))){
			_delay_loop_1(33);
		}
		//PC0 = 0 and PC1 = 1 for DIP SWITCH
		else if((~(PIN0_bm & PORTC_IN)) && ((PIN1_bm & PORTC_IN))){
			_delay_loop_1(67);
		}
		//PC0 = 1 and PC1 = 0 for DIP SWITCH
		else if(((PIN0_bm & PORTC_IN)) && (~(PIN1_bm & PORTC_IN))){
			_delay_loop_1(137);
		}
		
		
		PORTC_OUT &= ~PIN3_bm;   //Clear PC3
		
		//For two bits to be used for the DIP SWITCH
		//PC0 = 0 and PC1 = 0 for DIP SWITCH
		if((~(PIN0_bm & PORTC_IN)) && (~(PIN1_bm & PORTC_IN))){
			_delay_loop_1(33);
		}
		//PC0 = 0 and PC1 = 1 for DIP SWITCH
		else if((~(PIN0_bm & PORTC_IN)) && ((PIN1_bm & PORTC_IN))){
			_delay_loop_1(67);
		}
		//PC0 = 1 and PC1 = 0 for DIP SWITCH
		else if(((PIN0_bm & PORTC_IN)) && (~(PIN1_bm & PORTC_IN))){
			_delay_loop_1(137);
		}	
		
    }
}

