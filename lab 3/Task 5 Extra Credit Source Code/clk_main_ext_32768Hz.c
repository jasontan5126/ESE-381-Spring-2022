/*
 * clk_main_ext_32768Hz.c
 *
 * Created: 2/14/2022 8:40:16 PM
 * Author : Jason Tan
 */ 

#include <avr/io.h>

#define DISABLE_PRESCALAR 0x00

#define EXTERNAL_OSCILLATOR1 0x05   //Configure for external oscillator

#define MAIN_CLOCK_CTRLA 0x82  //configure for MCLKCTRLA


int main(void)
{
    PORTA_DIR = PIN7_bm;        //Configure PA7 as the output
	
	//Enable CLKOUT and clock select bit 2 mask to external crystal 32.768 kHz oscillator
	CPU_CCP = CCP_IOREG_gc;  //change protected IO register
	//External clock on the XTAL32K1 pin
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL1_bm | CLKCTRL_CLKOUT_bm;    // MAIN_CLOCK_CTRLA;  //enable CLKOUT and source for main clock is 32.768kHz external crystal oscillator
    
	CPU_CCP = CCP_IOREG_gc;		//change protected IO register
	CLKCTRL.XOSC32KCTRLA = CLKCTRL_ENABLE_bm | (~CLKCTRL_SEL_bm);   //EXTERNAL_OSCILLATOR1;   //Guess is what enable external crystal oscillator by
	
   
    while (1) 
    {	

    }
}

