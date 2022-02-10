/*
 * read_modify_write_sftw_sw0.c
 *
 * Created: 2/3/2022 11:49:03 PM
 * Author : Jason Tan
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
    VPORTD_DIR = 0xFF;   //Configure PORTD pins (PD07 through PD00) as output pins
	
	VPORTB_DIR = 0x08;   //PB3 output for LED0
	PORTB_PIN2CTRL = 0x08;  //Pull up enable for SW0
    
    //Configure PA7 - PA2 as input buffers with internal pull up resistors
    for(uint8_t i = 0; i < 8; i++){
	    *(ptr + i) |= PORT_PULLUPEN_bm;
    }
	
	
	
	uint8_t n = 3;  //Field value starting from 3
	uint8_t field_mask = 0x0F; //Field mask 
	uint8_t field_val; //Read PA3-PA2 and PC1-PC0. 
	VPORTD_OUT = ~((VPORTA_IN & 0xFC) | (VPORTC_IN & 0x03));
		
    while (1) 
    {	
		//Check for if SW0 is press meaning that sends a logic 0
		field_val = ((VPORTC_IN & 0x03 ) | (VPORTA_IN & 0x0C));
		if(!(VPORTB_IN & PIN2_bm)){
			//							 0b1000 0111           0b1111 PA3 PA2 PC1 PC0 -> 1 PA3 PA2 PC1 PC0 000
			VPORTD_OUT = (VPORTD_OUT & ~(field_mask << n)) | (((field_val & field_mask) << n) ^ 0x78);
		}
		
    }
}

