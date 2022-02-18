/*
 * parallel_in_parallel_out_flat.c
 *
 * Created: 1/28/2022 9:58:44 PM
 * Author : Jason
 *
 */ 

#include <avr/io.h>


int main(void)
{
	//pointer to PIN2CTRL array of pin configuration registers
	uint8_t* ptr = (uint8_t*)&PORTA.PIN2CTRL;
	PORTC_PIN0CTRL = 0x08;  //Enable internal pull up for PC0
	PORTC_PIN1CTRL = 0x08;  //Enable internal pull up for PC1
	
	//DIR is what configure the port pins as inputs or outputs
    VPORTA_DIR = 0x00;	//Configure PORTA pins ( PA7, PA6, PA5, PA4, PA3, PA2) as the inputs
	VPORTC_DIR = 0x00;   //Configure PORTC pins ( PC1, PC0) as the inputs
	VPORTD_DIR = 0xFF;   //Configure PORD pins (PD07 through PD00) as output pins
	
	//Configure PA7 - PA2 as input buffers with internal pull up resistors
	for(uint8_t i = 0; i < 8; i++){
		*(ptr + i) |= PORT_PULLUPEN_bm;
	}
	
	
    while (1) 
    {
		
		VPORTD_OUT = ~((VPORTA_IN & 0xFC) | (VPORTC_IN & 0x03) );  //For the purpose of reading for PA7-PA2 and for PC1-PC0 and negating when 
																   //changing value bit when switch is on and off

    }
}

