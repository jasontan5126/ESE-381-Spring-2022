/*
 * clk_per_to_clkout.c
 *
 * Created: 2/12/2022 12:04:43 AM
 * Author : jason
 */ 

#include <avr/io.h>


int main(void)
{
    CPU_CCP = CCP_IOREG_gc; //change protected IO register
    CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm;  //enable CLKOUT
	
	while (1) 
    {
    }
}

