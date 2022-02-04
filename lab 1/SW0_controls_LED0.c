/*
 * SW0_controls_LED0.c
 *
 * Created: 1/15/2022 5:05:31 PM
 * Author : jason
 */ 

#include <avr/io.h>


int main(void)
{
    VPORTB_DIR = 0x08;        //PB3 output for LED0
	PORTB_PIN2CTRL = 0x08;	  //PB2 enable pull-up for SW0
    while (1) 
    {
		VPORTB_OUT = VPORTC_IN << 1;	
    }
}

