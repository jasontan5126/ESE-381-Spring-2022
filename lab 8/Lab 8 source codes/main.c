/*
 * DOG_LCD_BasicTest.c
 *
 * Created: 3/19/2022 8:09:13 PM
 * Author : jason
 */ 

#include <avr/io.h>
#include "lcd_dog_AVR128_driver.h"




int main(void) {
	
	//Initialize the buffer of the LCD
	init_lcd_dog();
	
	while(1) {
		
		//Print line 1 message into LCD buffer
		sprintf(dsp_buff1, "Line 1 Message  "); 
		
		//Print line 2 message into LCD buffer 
		sprintf(dsp_buff2, "Line 2 Message  ");
		
		//Print line 3 message into LCD buffer
		sprintf(dsp_buff3, "Line 3 Message  ");
		
		//Update the 3 line messages into the LCD buffer
		update_lcd_dog();
		asm volatile("nop");
	}
}

