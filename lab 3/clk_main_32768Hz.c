/*
 * clk_main_32768Hz.c
 *
 * Created: 2/13/2022 7:25:32 PM
 * Author : jason
 */ 

#include <avr/io.h>

#define DISABLE_PRESCALAR 0x00


int main(void)
{
	PORTA_DIR = PIN7_bm;        //Configure PA7 as the output
	CPU_CCP = CCP_IOREG_gc;		//change protected IO register 
    CLKCTRL.MCLKCTRLB = DISABLE_PRESCALAR;   //CLK_MAIN prescalar disabled
	CPU_CCP = CCP_IOREG_gc;  //change protected IO register
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm | CLKCTRL_CLKSEL0_bm;  //Enable CLKOUT and clock select bit 0 mask to 32.768 kHz internal oscillator
	
	
	
    while (1) 
    {
    }
}

