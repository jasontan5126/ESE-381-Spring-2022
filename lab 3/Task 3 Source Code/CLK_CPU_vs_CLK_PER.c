/*
 * CLK_CPU_vs_CLK_PER.c
 *
 * Created: 2/13/2022 11:12:19 AM
 * Author : jason
 */ 
   
#include <avr/io.h>
#define F_CPU 4000000
#include <util/delay.h>


int main(void)
{
	PORTA_DIR = PIN7_bm;	//Set PA7 as the output pin for the CLK_PER (CLK_OUT)
	PORTB_DIR = PIN3_bm;      //Set PB3 as the output

    CPU_CCP = CCP_IOREG_gc;  //change protected IO register	
	//I guess enable the CLK_MAIN prescaler and set prescaler
	//div to whatever value to divide by what value (4) to see
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV0_bm | CLKCTRL_PEN_bm; 
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

