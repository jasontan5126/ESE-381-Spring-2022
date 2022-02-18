/*
 * blink_LED0_1sec.c
 *
 * Created: 2/13/2022 9:53:42 AM
 * Author : jason
 */ 

#define F_CPU 4000000
#include <avr/io.h>
#include <util/delay.h>



int main(void)
{
	PORTA_DIR = PIN7_bm;    //PA7 as the output
	PORTB_DIR = PIN3_bm;      //Set PB3 as the output
	CPU_CCP = CCP_IOREG_gc;  //change protected IO register
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm;  //enable CLKOUT
	
    while (1) 
    {
		PORTB_OUT |= PIN3_bm;  
		_delay_ms(500);
		PORTB_OUT &= ~PIN3_bm;
		_delay_ms(500);
    }
}

