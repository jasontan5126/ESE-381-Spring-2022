/*
 * one_MHz.c
 *
 * Created: 2/7/2022 12:26:01 AM
 * Author : Jason Tan
 */ 

#include <avr/io.h>

#define DISABLE_PRESCALAR 0x00;

int main(void)
{
	PORTA.DIR = PIN7_bm;   //PA7 as the output
	CPU_CCP = CCP_IOREG_gc;		//change protected IO register
	CLKCTRL.MCLKCTRLB = CLKCTRL_PEN_bm | CLKCTRL_PDIV0_bm;    //Enable CLK_MAIN prescalar and divide by 4 for prescalar
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm;		//enable CLKOUT
    while (1) 
    {		
    }
}

